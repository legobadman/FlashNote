#ifndef __RPC_SERVICE_H__
#define __RPC_SERVICE_H__

#include "note_types.h"
#include "notecore.h"
//#include "NoteInfo.h"
#include <codecvt>
#include <string>
#include <locale>

class NoteInfoClient;
//class TTransport;
//class TProtocol;

class RPCService
{
public:
	static RPCService& GetInstance();
	void getnotebooks(std::vector<NOTEBOOK>& vecBooks);
	void InitcoreFromRPC(INoteApplication* pApp);
	void SynchronizeNote(INotebook* pNotebook, INote* pNote);
	bool SynchronizeNotebook(INotebook* pNotebook);
	bool RemoveNote(INotebook* pNotebook, INote* pNote);
	bool RemoveNotebook(INotebook* pNotebook);
	std::wstring NewNote(std::wstring bookid, std::wstring title);
	std::wstring NewNotebook(std::wstring name);

private:
	RPCService();
	RPCService(const RPCService&) {}

	NoteInfoClient* m_pClient;
	//TTransport* socket2;
	std::string _userid;
	std::wstring userid;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};

#endif
