#ifndef __RPC_SERVICE_H__
#define __RPC_SERVICE_H__

#include "note_types.h"
#include "notecore.h"
#include <codecvt>
#include <string>
#include <locale>

class ThriftClient;

class RPCService
{
public:
	static RPCService& GetInstance();
	void getnotebooks(std::vector<NOTEBOOK>& vecBooks);
	void inittrashes(INoteApplication* pApp);
	void InitcoreFromRPC(INoteApplication* pApp);
	void SynchronizeNote(INoteApplication* pApp, INotebook* pNotebook, INote* pNote);
	bool SynchronizeNotebook(INotebook* pNotebook);
	bool RemoveNote(INoteApplication* pApp, INoteCollection* pNotebook, INote* pNote);
	bool DeleteNote(ITrash* pTrash, INote* pNote);
	bool RemoveNotebook(INoteApplication* pApp, INotebook* pNotebook);
	bool RecoverNote(INoteApplication* pApp, ITrash* pSrcNoteColl, INote* pNote);
	std::wstring NewNote(std::wstring bookid, std::wstring title, NOTE_TYPE type);
	std::wstring NewNotebook(std::wstring name);

private:
	RPCService();
	RPCService(const RPCService&) {}
	~RPCService();

	ThriftClient* m_pClient;

	std::string _userid;
	std::wstring userid;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
};

#endif
