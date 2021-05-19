#include <boost/shared_ptr.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>
#include "hello_types.h"
#include "Hello.h"
#include "UserInfo.h"
#include "NoteInfo.h"
#include "rpcservice.h"
#include "notecore.h"
#include "../guimain/guihelper.h"


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

void RPCService::RemoveNote(int bookid, INote* pNote)
{

}

void RPCService::InitcoreFromRPC(INoteApplication* pApp)
{
	if (!pApp)
		return;

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
			std::tm create_t;
			create_t.tm_year = notebook.create_time.date().year();
			create_t.tm_mon = notebook.create_time.date().month();
			create_t.tm_mday = notebook.create_time.date().day();
			spNotebook->SetCreateTime(create_t);

			std::tm modify_t;
			modify_t.tm_year = notebook.modify_time.date().year();
			modify_t.tm_mon = notebook.modify_time.date().month();
			modify_t.tm_mday = notebook.modify_time.date().day();
			spNotebook->SetModifyTime(modify_t);
			
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

				create_t.tm_year = note.create_time.date().year();
				create_t.tm_mon = note.create_time.date().month();
				create_t.tm_mday = note.create_time.date().day();
				spNote->SetCreateTime(create_t);

				modify_t.tm_year = note.modify_time.date().year();
				modify_t.tm_mon = note.modify_time.date().month();
				modify_t.tm_mday = note.modify_time.date().day();
				spNote->SetModifyTime(modify_t);

				bstrId = SysAllocString(note.id.toStdWString().c_str());
				spNote->SetId(bstrId);

				spNotebook->AddNote(spNote);
			}
		}
		pApp->AddNotebook(spNotebook);
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