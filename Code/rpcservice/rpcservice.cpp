#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

#include "notecore.h"
#include "com_sptr.h"
#include "hello_types.h"
#include "Hello.h"
#include "UserInfo.h"
#include "NoteInfo.h"
#include "rpcservice.h"
#include "notecore.h"
#include "../guimain/guihelper.h"
#include "notecoreinit.h"
#include "thriftclient.h"


RPCService& RPCService::GetInstance()
{
	static RPCService inst;
	return inst;
}

RPCService::RPCService()
	: m_pClient(NULL)
{
	userid = L"609638963d8ba27ccea4e20d";
	_userid = "609638963d8ba27ccea4e20d";

	m_pClient = new ThriftClient;
}

RPCService::~RPCService()
{
	delete m_pClient;
	m_pClient = NULL;
}

bool RPCService::SynchronizeNotebook(INotebook* pNotebook)
{
	BSTR bstrId, bstrName;
	std::wstring bookid = AppHelper::GetNotebookId(pNotebook).toStdWString();
	std::wstring bookName = AppHelper::GetNotebookName(pNotebook).toStdWString();
	if (bookid.empty())
	{
		bookid = NewNotebook(bookName);
		if (bookid.empty())
			return false;

		pNotebook->SetId(SysAllocString(bookid.c_str()));
		
		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(pNotebook);
	}

	//其他更新待续
	return true;
}

void RPCService::SynchronizeNote(INoteApplication* pApp, INotebook* pNotebook, INote* pNote)
{
	BSTR bstrId, bstrTitle, bstrContent;
	pNote->GetId(&bstrId);
	std::wstring id(bstrId, SysStringLen(bstrId));
	std::wstring bookid = AppHelper::GetNotebookId(pNotebook).toStdWString();
	if (id.empty())
	{
		NOTE_TYPE type = NORMAL_NOTE;
		pNote->GetType(&type);
		//新建的note，需要先向服务端申请id
		id = RPCService::GetInstance().NewNote(bookid, L"", type);
		pNote->SetId(SysAllocString(id.c_str()));
		pNote->SetBookId(SysAllocString(bookid.c_str()));
		pNotebook->AddNote(pNote);

		if (id.empty())
		{
			Q_ASSERT(FALSE);
		}
	}

	pNote->GetTitle(&bstrTitle);
	pNote->GetContent(&bstrContent);
	
	std::wstring title(bstrTitle, SysStringLen(bstrTitle));
	std::wstring content(bstrContent, SysStringLen(bstrContent));

	bool ret = m_pClient->client()->UpdateNote(converter.to_bytes(id), converter.to_bytes(title), converter.to_bytes(content));
}

std::wstring RPCService::NewNote(std::wstring bookid, std::wstring title, NOTE_TYPE type)
{
	NoteType::type _type = NoteType::NORMAL_NOTE;
	if (type == NORMAL_NOTE)
		_type = NoteType::NORMAL_NOTE;
	else if (type == MINDMAP)
		_type = NoteType::MINDMAP;

	std::string newnoteid;
	m_pClient->client()->NewNote(newnoteid, _userid, converter.to_bytes(bookid), converter.to_bytes(title), _type);
	return converter.from_bytes(newnoteid);
}

std::wstring RPCService::NewNotebook(std::wstring name)
{
	std::string bookid;
	m_pClient->client()->NewNotebook(bookid, _userid, converter.to_bytes(name));
	return converter.from_bytes(bookid);
}

bool RPCService::RemoveNote(INoteApplication* pApp, INoteCollection* pNoteColl, INote* pNote)
{
	com_sptr<INotebook> pNotebook = pNoteColl;
	bool bRet = m_pClient->client()->TrashNote(_userid,
		converter.to_bytes(AppHelper::GetNotebookId(pNotebook).toStdWString()),
		converter.to_bytes(AppHelper::GetNoteId(pNote).toStdWString())
		);

	com_sptr<ITrash> pTrash;
	pApp->GetTrash(&pTrash);
	pTrash->AddNote(pNote);

	HRESULT hr = pNotebook->RemoveNote(pNote);
	if (FAILED(hr))
	{
		Q_ASSERT(false);
	}
	return bRet;
}

bool RPCService::DeleteNote(ITrash* pTrash, INote* pNote)
{
	bool bRet = m_pClient->client()->DeleteNote(_userid, converter.to_bytes(AppHelper::GetNoteId(pNote).toStdWString()));
	if (bRet)
	{
		HRESULT hr = pTrash->RemoveNote(pNote);
		Q_ASSERT(hr == S_OK);
	}
	return bRet;
}

bool RPCService::RemoveNotebook(INoteApplication* pApp, INotebook* pNotebook)
{
	std::string bookid = converter.to_bytes(AppHelper::GetNotebookId(pNotebook).toStdWString());
	bool bRet = m_pClient->client()->DeleteNotebook(_userid, bookid);
	if (bRet)
	{
		// 移除notebook下所有note至FreeNotes
		com_sptr<ITrash> spTrash;
		pApp->GetTrash(&spTrash);

		int nCount = 0;
		pNotebook->GetCount(&nCount);
		for (int i = 0; i < nCount; i++)
		{
			com_sptr<INote> spNote;
			AppHelper::GetNote(pNotebook, i, &spNote);
			spNote->SetBookId(L"");
			spTrash->AddNote(spNote);
		}
		pNotebook->Clear();

		com_sptr<INotebooks> spNotebooks;
		pApp->GetNotebooks(&spNotebooks);
		HRESULT hr = spNotebooks->DeleteNotebook(pNotebook);
		return (hr == S_OK);
	}
	else
	{
		return false;
	}
}

bool RPCService::RecoverNote(INoteApplication* pApp, ITrash* pTrash, INote* pNote)
{
	std::string noteid = converter.to_bytes(AppHelper::GetNoteId(pNote).toStdWString());
	bool bRet = m_pClient->client()->RecoverNote(_userid, noteid);
	if (bRet)
	{
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookByNote(pNote, &spNotebook);
		spNotebook->AddNote(pNote);
		pTrash->RemoveNote(pNote);
	}
	return bRet;
}

void RPCService::InitcoreFromRPC(INoteApplication* pApp)
{
	if (!pApp)
		return;

	//创建notebooks
	com_sptr<INotebooks> spNotebooks;
	CreateNotebooks(&spNotebooks);
	pApp->SetNotebooks(spNotebooks);

	std::vector<NOTEBOOK> vecBooks;
	RPCService::GetInstance().getnotebooks(vecBooks);
	pApp->SetUserId(SysAllocString(userid.c_str()));
	for (int i = 0; i < vecBooks.size(); i++)
	{
		NOTEBOOK notebook(vecBooks[i]);
		com_sptr<INotebook> spNotebook;
		CreateNotebook(&spNotebook);
		if (spNotebook)
		{
			std::wstring str_ = notebook.name.toStdWString();
			BSTR bstrName = SysAllocString(str_.c_str());
			spNotebook->SetName(bstrName);

			spNotebook->SetCreateTime(notebook.create_time.toMSecsSinceEpoch());
			spNotebook->SetModifyTime(notebook.modify_time.toMSecsSinceEpoch());
			
			BSTR bstrId = SysAllocString(notebook.id.toStdWString().c_str());
			spNotebook->SetId(bstrId);

			for (int j = 0; j < notebook.notes.size(); j++)
			{
				NOTE note(notebook.notes[j]);

				com_sptr<INote> spNote;
				CreateNote(note.type, &spNote);

				std::wstring title = note.title.toStdWString();
				BSTR bstrTitle = SysAllocString(title.c_str());
				spNote->SetTitle(bstrTitle);

				std::wstring content = note.content.toStdWString();
				BSTR bstrContent = SysAllocString(content.c_str());
				spNote->SetContent(bstrContent);

				spNote->SetCreateTime(note.create_time.toMSecsSinceEpoch());
				spNote->SetModifyTime(note.modify_time.toMSecsSinceEpoch());

				bstrId = SysAllocString(note.id.toStdWString().c_str());
				spNote->SetId(bstrId);

				bstrId = SysAllocString(notebook.id.toStdWString().c_str());
				spNote->SetBookId(bstrId);

				spNotebook->AddNote(spNote);
			}
		}
		com_sptr<INotebooks> spNotebooks;
		pApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(spNotebook);
	}

	inittrashes(pApp);
}

void RPCService::getnotebooks(std::vector<NOTEBOOK>& vecBooks)
{
	std::vector<Notebook> _vecBooks;
	m_pClient->client()->GetNotebooks(_vecBooks, _userid);
	for (int i = 0; i < _vecBooks.size(); i++)
	{
		NOTEBOOK notebook;
		Notebook _notebook(_vecBooks[i]);
		notebook.name = QString::fromUtf8(_notebook.name.c_str());
		notebook.create_time = QDateTime::fromMSecsSinceEpoch(_notebook.create_time, Qt::UTC);
		notebook.modify_time = QDateTime::fromMSecsSinceEpoch(_notebook.modify_time, Qt::UTC);
		notebook.id = QString::fromUtf8(_notebook.id.c_str());

		for (int j = 0; j < _notebook.notes.size(); j++)
		{
			NOTE note;
			Note _note = _notebook.notes[j];

			note.title = QString::fromUtf8(_note.title.c_str());
			//服务端暂时还没做提取缩略文本。
			note.content = QString::fromUtf8(_note.text_abbre.c_str());
			note.create_time = QDateTime::fromMSecsSinceEpoch(_note.create_time, Qt::UTC);
			note.modify_time = QDateTime::fromMSecsSinceEpoch(_note.modify_time, Qt::UTC);
			note.id = QString::fromUtf8(_note.id.c_str());
			switch (_note.type)
			{
			case NoteType::NORMAL_NOTE:
				note.type = NOTE_TYPE::NORMAL_NOTE;
				break;
			case NoteType::MINDMAP:
				note.type = NOTE_TYPE::MINDMAP;
				break;
			case NoteType::SCHEDULE:
				break;
			}

			notebook.notes.append(note);
		}
		vecBooks.push_back(notebook);
	}
}

void RPCService::inittrashes(INoteApplication* pApp)
{
	//trash涉及到note和notebook的索引，因此用core的note对象来初始化方便些。
	com_sptr<ITrash> spTrash;
	CreateTrash(&spTrash);

	std::vector<Trash> trashes;
	m_pClient->client()->GetTrashes(trashes, _userid);
	for (int i = 0; i < trashes.size(); i++)
	{
		QString trash_id = QString::fromUtf8(trashes[i].trash_id.c_str());
		QString bookid = QString::fromUtf8(trashes[i].notebook.id.c_str());
		QString noteid = QString::fromUtf8(trashes[i].note.id.c_str());
		
		QString note_title = QString::fromUtf8(trashes[i].note.title.c_str());
		QString note_content = QString::fromUtf8(trashes[i].note.text_abbre.c_str());
		QDateTime create_time = QDateTime::fromMSecsSinceEpoch(trashes[i].note.create_time, Qt::UTC);
		QDateTime modify_time = QDateTime::fromMSecsSinceEpoch(trashes[i].note.modify_time, Qt::UTC);
		QDateTime trash_time = QDateTime::fromMSecsSinceEpoch(trashes[i].trash_time, Qt::UTC);
		QString note_id = QString::fromUtf8(trashes[i].note.id.c_str());

		com_sptr<INote> spNote;
		CreateNote(NORMAL_NOTE, &spNote);
		{
			std::wstring title = note_title.toStdWString();
			BSTR bstrTitle = SysAllocString(title.c_str());
			spNote->SetTitle(bstrTitle);

			std::wstring content = note_content.toStdWString();
			BSTR bstrContent = SysAllocString(content.c_str());
			spNote->SetContent(bstrContent);

			spNote->SetCreateTime(create_time.toMSecsSinceEpoch());
			//目前暂时用modifytime作为移除至trash的时间。
			spNote->SetModifyTime(trash_time.toMSecsSinceEpoch());

			BSTR bstrId = SysAllocString(note_id.toStdWString().c_str());
			spNote->SetId(bstrId);

			BSTR bstrBookId = SysAllocString(bookid.toStdWString().c_str());
			spNote->SetBookId(bstrBookId);
		}
		spTrash->AddNote(spNote);
	}
	pApp->SetTrash(spTrash);
}