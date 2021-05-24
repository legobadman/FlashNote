#include <boost/shared_ptr.hpp>
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


using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

boost::shared_ptr<TTransport> thrift_socket;
boost::shared_ptr<TTransport> thrift_transport;
boost::shared_ptr<TProtocol> thrift_protocol;


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

	thrift_socket.reset(new TSocket("120.78.150.174", 82));
	thrift_transport.reset(new TBufferedTransport(thrift_socket));
	thrift_protocol.reset(new TBinaryProtocol(thrift_transport));

	m_pClient = new NoteInfoClient(thrift_protocol);
	thrift_transport->open();
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

void RPCService::SynchronizeNote(INotebook* pNotebook, INote* pNote)
{
	BSTR bstrId, bstrTitle, bstrContent;
	pNote->GetId(&bstrId);
	std::wstring id(bstrId, SysStringLen(bstrId));
	std::wstring bookid = AppHelper::GetNotebookId(pNotebook).toStdWString();
	if (id.empty())
	{
		//新建的note，需要先向服务端申请id
		id = RPCService::GetInstance().NewNote(bookid, L"");
		pNote->SetId(SysAllocString(id.c_str()));
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

	bool ret = m_pClient->UpdateNote(converter.to_bytes(id), converter.to_bytes(title), converter.to_bytes(content));
}

std::wstring RPCService::NewNote(std::wstring bookid, std::wstring title)
{
	std::string newnoteid;
	m_pClient->NewNote(newnoteid, _userid, converter.to_bytes(bookid), converter.to_bytes(title));
	return converter.from_bytes(newnoteid);
}

std::wstring RPCService::NewNotebook(std::wstring name)
{
	std::string bookid;
	m_pClient->NewNotebook(bookid, _userid, converter.to_bytes(name));
	return converter.from_bytes(bookid);
}

bool RPCService::RemoveNote(INotebook* pNotebook, INote* pNote)
{
	bool bRet = m_pClient->TrashNote(_userid,
		converter.to_bytes(AppHelper::GetNotebookId(pNotebook).toStdWString()),
		converter.to_bytes(AppHelper::GetNoteId(pNote).toStdWString())
		);
	HRESULT hr = pNotebook->RemoveNote(pNote);
	if (FAILED(hr))
	{
		Q_ASSERT(false);
	}
	return bRet;
}

bool RPCService::RemoveNotebook(INotebook* pNotebook)
{
	std::string bookid = converter.to_bytes(AppHelper::GetNotebookId(pNotebook).toStdWString());
	bool bRet = m_pClient->DeleteNotebook(_userid, bookid);
	if (bRet)
	{
		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);
		HRESULT hr = spNotebooks->DeleteNotebook(pNotebook);
		return (hr == S_OK);
	}
	else
	{
		return false;
	}
}

void RPCService::InitcoreFromRPC(INoteApplication* pApp)
{
	if (!pApp)
		return;

	//创建notebooks
	com_sptr<INotebooks> spNotebooks;
	CreateNotebooks(&spNotebooks);

	coreApp->SetNotebooks(spNotebooks);

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
				CreateNote(NORMAL_NOTE, &spNote);

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

				spNotebook->AddNote(spNote);
			}
		}

		com_sptr<INotebooks> spNotebooks;
		pApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(spNotebook);
	}
}

void RPCService::getnotebooks(std::vector<NOTEBOOK>& vecBooks)
{
	std::vector<Notebook> _vecBooks;
	m_pClient->GetNotebooks(_vecBooks, _userid);
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

			notebook.notes.append(note);
		}
		vecBooks.push_back(notebook);
	}
}