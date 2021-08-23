#ifndef __DBSERVICE_H__
#define __DBSERVICE_H__

#include "notecore2.h"
#include "note_types.h"
#include "CppSQLite3.h"
#include <codecvt>
#include <string>
#include <locale>

class DbService
{
public:
	static DbService& GetInstance();
	void getnotebooks(std::vector<NOTEBOOK>& vecBooks);
	QVector<NOTE> getnotes(const QStringList&);
	NOTE getnote(const QString& noteid);
	void inittrashes(INoteApplication* pApp);
	void initschedules(INoteApplication* pApp);
	void InitcoreFromRPC(INoteApplication* pApp);
	bool SynchronizeNote(INoteApplication* pApp, INotebook* pNotebook, INote* pNote);
	bool SynchronizeNotebook(INotebook* pNotebook);
	bool SynchronizeSchedule(INoteApplication* pApp, INote* pNote);
	bool RemoveNote(INoteApplication* pApp, INoteCollection* pNotebook, INote* pNote);
	bool RemoveSchedule(INoteApplication* pApp, INote* pNote);
	bool DeleteNote(ITrash* pTrash, INote* pNote);
	bool RemoveNotebook(INoteApplication* pApp, INotebook* pNotebook);
	bool RecoverNote(INoteApplication* pApp, ITrash* pSrcNoteColl, INote* pNote);
	bool MoveNotebook(INotebook* pSrcbook, INotebook* pDestbook, INote* pNote);

public:
	void ensureDbCreated();
	void reconstruct();
	bool TrashNote(INoteApplication* pApp, INoteCollection* pNoteColl, INote* pNote);

private:
	DbService();
	DbService(const DbService&) {}
	~DbService();
	void dropAllTables();
	void CreateNoteByNOTE(NOTE note, INote** ppNote);

	const QString m_dbDirName = "database";
	const QString m_assetDirName = "assets";
	const QString m_dbName = "note.db";
	CppSQLite3DB m_db;
};


#endif