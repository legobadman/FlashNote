#include "sql_const.h"
#include <QBuffer>
#include <QFileInfo>
#include <QDir>
#include <QtCore/QStandardPaths>
#include <QCoreApplication>
#include "notecore2.h"
#include "com_sptr.h"
#include "notecoreinit.h"
#include "dbservice.h"
#include "../guimain/pathservice.h"
#include "../guimain/guihelper.h"


DbService& DbService::GetInstance()
{
	static DbService inst;
	return inst;
}

DbService::DbService()
{
	QString dbPath = PathService::instance().GetDbPath();
	QString fullPath = dbPath + "/" + m_dbName;
	if (!QDir(dbPath).exists(m_dbName))
	{
		m_db.open(fullPath.toUtf8());
		if (!m_db.tableExists(TABLE_NAME_NOTE) ||
			!m_db.tableExists(TABLE_NAME_NOTEBOOK) ||
			!m_db.tableExists(TABLE_NAME_TRASH) ||
			!m_db.tableExists(TABLE_NAME_SCHEDULE))
		{
			reconstruct();
		}
	}
	else
	{
		m_db.open(fullPath.toUtf8());
	}
}

DbService::~DbService()
{
	m_db.close();
}

void DbService::dropAllTables()
{
	if (m_db.tableExists(TABLE_NAME_NOTE))
	{
		m_db.execDML(SQL_REMOVE_NOTE);
	}

	if (m_db.tableExists(TABLE_NAME_NOTEBOOK))
	{
		m_db.execDML(SQL_REMOVE_NOTEBOOK);
	}

	if (m_db.tableExists(TABLE_NAME_TRASH))
	{
		m_db.execDML(SQL_REMOVE_TRASH);
	}

	if (m_db.tableExists(TABLE_NAME_SCHEDULE))
	{
		m_db.execDML(SQL_REMOVE_SCHEDULES);
	}
}

void DbService::reconstruct()
{
	dropAllTables();
	ensureDbCreated();
}

void DbService::ensureDbCreated()
{
	if (!m_db.tableExists(TABLE_NAME_NOTE))
	{
		m_db.execDML(SQL_CREATE_NOTE);
	}
	if (!m_db.tableExists(TABLE_NAME_NOTEBOOK))
	{
		m_db.execDML(SQL_CREATE_NOTEBOOK);
	}
	if (!m_db.tableExists(TABLE_NAME_TRASH))
	{
		m_db.execDML(SQL_CREATE_TRASH);
	}
	if (!m_db.tableExists(TABLE_NAME_SCHEDULE))
	{
		m_db.execDML(SQL_CREATE_SCHEDULES);
	}
}

void DbService::getnotebooks(std::vector<NOTEBOOK>& vecBooks)
{
	CppSQLite3Query query = m_db.execQuery("SELECT * FROM NOTEBOOK;");
	while (!query.eof())
	{
		QString id = query.getStringField("ID");
		QString create_time = query.getStringField("create_time");
		QString modify_time = query.getStringField("modify_time");
		QString bookName = QString::fromUtf8(query.getStringField("name"));
		QString notes = QString::fromUtf8(query.getStringField("notes"));
		QStringList list = notes.split('|', Qt::SkipEmptyParts);

		NOTEBOOK notebook;
		notebook.notes = getnotes(list);
		notebook.id = id;
		notebook.name = bookName;

		vecBooks.push_back(notebook);
		query.nextRow();
	}
}

QVector<NOTE> DbService::getnotes(const QStringList& list)
{
	QVector<NOTE> vecNotes;
	for (int i = 0; i < list.length(); i++)
	{
		QString id = list[i];
		vecNotes.append(getnote(id));
	}
	return vecNotes;
}

NOTE DbService::getnote(const QString& noteid)
{
	QString sql = QString("SELECT * FROM NOTE WHERE id=\"%1\"").arg(noteid);
	CppSQLite3Query query = m_db.execQuery(sql.toUtf8());
	Q_ASSERT(!query.eof());
	QString title = query.getStringField("title");
	QString content = query.getStringField("content");
	int create_time = query.getIntField("create_time");
	int modify_time = query.getIntField("modify_time");
	int type = query.getIntField("type");

	NOTE note;
	note.id = noteid;
	note.content = content;
	note.title = title;
	switch (type)
	{
	case 0: note.type = NORMAL_NOTE; break;
	case 1: note.type = MINDMAP; break;
	case 2: note.type = SCHEDULE; break;
	default:
		note.type = NORMAL_NOTE; break;
	}
	return note;
}

void DbService::CreateNoteByNOTE(NOTE note, INote** ppNote)
{
	if (!ppNote)
		return;

	com_sptr<INote> spNote;
	CreateNote(note.type, &spNote);

	std::wstring title = note.title.toStdWString();
	spNote->SetTitle(title);

	std::wstring content = note.content.toStdWString();
	spNote->SetContent(content);
	spNote->SetCreateTime(note.create_time.toMSecsSinceEpoch());
	spNote->SetModifyTime(note.modify_time.toMSecsSinceEpoch());
	spNote->SetId(note.id.toStdWString());

	*ppNote = spNote;
	(*ppNote)->AddRef();
}

void DbService::inittrashes(INoteApplication* pApp)
{
	com_sptr<ITrash> spTrash;
	CreateTrash(&spTrash);

	CppSQLite3Query query = m_db.execQuery("SELECT * FROM TRASH;");
	while (!query.eof())
	{
		QString trash_id = query.getStringField("ID");
		QString srcbook_id = query.getStringField("srcbook_id");
		QString note_id = query.getStringField("note_id");
		int trash_time = query.getIntField("trash_time");

		NOTE note = getnote(note_id);

		com_sptr<INote> spNote;
		CreateNoteByNOTE(note, &spNote);

		spNote->SetBookId(srcbook_id.toStdWString());
		spTrash->AddNote(spNote);

		query.nextRow();
	}
	pApp->SetTrash(spTrash);
}

void DbService::initschedules(INoteApplication* pApp)
{
	com_sptr<ISchedules> spSchedules;
	CreateSchedules(&spSchedules);
	CppSQLite3Query query = m_db.execQuery("SELECT * FROM SCHEDULES;");
	while (!query.eof())
	{
		QString note_id = query.getStringField("note_id");
		NOTE note = getnote(note_id);

		com_sptr<INote> spNote;
		CreateNoteByNOTE(note, &spNote);

		spSchedules->AddNote(spNote);
		query.nextRow();
	}
	pApp->SetSchedules(spSchedules);
}

void DbService::InitcoreFromRPC(INoteApplication* pApp)
{
	if (!pApp)
		return;

	//创建notebooks
	com_sptr<INotebooks> spNotebooks;
	CreateNotebooks(&spNotebooks);
	pApp->SetNotebooks(spNotebooks);

	std::vector<NOTEBOOK> vecBooks;
	getnotebooks(vecBooks);

	for (int i = 0; i < vecBooks.size(); i++)
	{
		NOTEBOOK notebook(vecBooks[i]);

		com_sptr<INotebook> spNotebook;
		CreateNotebook(&spNotebook);
		if (spNotebook)
		{
			std::wstring str_ = notebook.name.toStdWString();
			spNotebook->SetName(str_);
			spNotebook->SetCreateTime(notebook.create_time.toMSecsSinceEpoch());
			spNotebook->SetModifyTime(notebook.modify_time.toMSecsSinceEpoch());
			spNotebook->SetId(notebook.id.toStdWString());

			for (int j = 0; j < notebook.notes.size(); j++)
			{
				NOTE note(notebook.notes[j]);

				com_sptr<INote> spNote;
				CreateNoteByNOTE(note, &spNote);
				spNote->SetBookId(notebook.id.toStdWString());
				spNotebook->AddNote(spNote);
			}
		}
		com_sptr<INotebooks> spNotebooks;
		pApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(spNotebook);
	}

	initschedules(pApp);
	inittrashes(pApp);
}

bool DbService::SynchronizeNote(INoteApplication* pApp, INotebook* pNotebook, INote* pNote)
{
	QString title = AppHelper::GetNoteTitle(pNote);
	QString content = AppHelper::GetNoteContent(pNote);
	QString bookid = AppHelper::GetNotebookId(pNotebook);
	QString noteid = AppHelper::GetNoteId(pNote);
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	if (noteid.isEmpty())
	{
		NOTE_TYPE type = NORMAL_NOTE;
		pNote->GetType(&type);
		int _type = (int)type;
		noteid = AppHelper::GenerateGUID();

		QString sql = "INSERT INTO NOTE (ID, create_time, modify_time, type) VALUES (?,?,?,?);";
		CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());

		stmt.bind(1, noteid.toStdString().c_str());	
		stmt.bind(2, localtime.toStdString().c_str());
		stmt.bind(3, localtime.toStdString().c_str());
		stmt.bind(4, _type);

		int nRowsChanged = stmt.execDML();
		Q_ASSERT(nRowsChanged == 1);

		QStringList list = AppHelper::GetNotes(pNotebook);
		list.append(noteid);
		QString notes = list.join("|");

		sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1';").arg(bookid);
		stmt = m_db.compileStatement(sql.toUtf8());
		stmt.bind(1, notes.toStdString().c_str());

		nRowsChanged = stmt.execDML();
		Q_ASSERT(nRowsChanged == 1);

		pNote->SetId(noteid.toStdWString());
		pNote->SetBookId(bookid.toStdWString());
		pNotebook->AddNote(pNote);
	}

	QString sql = QString("UPDATE NOTE SET title=?, content=?, modify_time=? WHERE ID='%1';").arg(noteid);
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());
	stmt.bind(1, title.toUtf8().constData());
	stmt.bind(2, content.toUtf8().constData());
	stmt.bind(3, localtime.toStdString().c_str());

	int nRowsChanged = stmt.execDML();
	return nRowsChanged == 1;
}

bool DbService::SynchronizeSchedule(INoteApplication* pApp, INote* pNote)
{
	QString title = AppHelper::GetNoteTitle(pNote);
	QString content = AppHelper::GetNoteContent(pNote);
	QString noteid = AppHelper::GetNoteId(pNote);
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	if (noteid.isEmpty())
	{
		NOTE_TYPE type = NORMAL_NOTE;
		pNote->GetType(&type);
		int _type = (int)type;
		noteid = AppHelper::GenerateGUID();

		QString sql = "INSERT INTO NOTE (ID, create_time, modify_time, type) VALUES (?,?,?,?);";
		CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());

		stmt.bind(1, noteid.toStdString().c_str());
		stmt.bind(2, localtime.toStdString().c_str());
		stmt.bind(3, localtime.toStdString().c_str());
		stmt.bind(4, _type);

		int nRowsChanged = stmt.execDML();
		Q_ASSERT(nRowsChanged == 1);

		sql = QString("SELECT * FROM SCHEDULES WHERE note_id = '%1'").arg(noteid);
		CppSQLite3Query query = m_db.execQuery(sql.toUtf8());
		if (query.eof())
		{
			sql = "INSERT INTO SCHEDULES (note_id) VALUES (?);";
			stmt = m_db.compileStatement(sql.toUtf8());
			stmt.bind(1, noteid.toStdString().c_str());

			nRowsChanged = stmt.execDML();
			Q_ASSERT(nRowsChanged == 1);
		}

		pNote->SetId(noteid.toStdWString());

		com_sptr<ISchedules> spSchedules;
		pApp->GetSchedules(&spSchedules);
		spSchedules->AddNote(pNote);
	}

	QString sql = QString("UPDATE NOTE SET title=?, content=?, modify_time=? WHERE ID='%1';").arg(noteid);
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());
	stmt.bind(1, title.toUtf8().constData());
	stmt.bind(2, content.toUtf8().constData());
	stmt.bind(3, localtime.toStdString().c_str());

	int nRowsChanged = stmt.execDML();
	return nRowsChanged == 1;
}

bool DbService::SynchronizeNotebook(INotebook* pNotebook)
{
	//format like 2021-06-27 23:24:50
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	if (AppHelper::GetNotebookId(pNotebook).isEmpty())
	{
		QString bookid = AppHelper::GenerateGUID();
		QString bookName = AppHelper::GetNotebookName(pNotebook);

		QString sql = "INSERT INTO NOTEBOOK (ID, create_time, modify_time, name) VALUES (?,?,?,?);";
		CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());

		stmt.bind(1, bookid.toUtf8().constData());
		stmt.bind(2, localtime.toStdString().c_str());
		stmt.bind(3, localtime.toStdString().c_str());
		stmt.bind(4, bookName.toUtf8().constData());

		int nRowsChanged = stmt.execDML();
		Q_ASSERT(nRowsChanged == 1);

		pNotebook->SetId(bookid.toStdWString());

		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(pNotebook);
	}
	//其他更新待续
	return true;
}

bool DbService::TrashNote(INoteApplication* pApp, INoteCollection* pNoteColl, INote* pNote)
{
	com_sptr<INotebook> pNotebook = pNoteColl;
	QString noteid = AppHelper::GetNoteId(pNote);
	QString bookid = AppHelper::GetNotebookId(pNotebook);
	QString trashId = AppHelper::GenerateGUID();
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

	QString sql("INSERT INTO TRASH (ID, srcbook_id, note_id, trash_time) VALUES (?,?,?,?);");
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());
	stmt.bind(1, trashId.toStdString().c_str());
	stmt.bind(2, bookid.toStdString().c_str());
	stmt.bind(3, noteid.toStdString().c_str());
	stmt.bind(4, localtime.toStdString().c_str());

	int rowsChanged = stmt.execDML();
	Q_ASSERT(rowsChanged == 1);

	com_sptr<ITrash> pTrash;
	pApp->GetTrash(&pTrash);
	pTrash->AddNote(pNote);

	return true;
}

bool DbService::RemoveNote(INoteApplication* pApp, INoteCollection* pNoteColl, INote* pNote)
{
	com_sptr<INotebook> pNotebook = pNoteColl;
	QString noteid = AppHelper::GetNoteId(pNote);
	QString bookid = AppHelper::GetNotebookId(pNotebook);

	TrashNote(pApp, pNoteColl, pNote);

	QStringList list = AppHelper::GetNotes(pNotebook);
	Q_ASSERT(list.contains(noteid));
	list.removeAll(noteid);
	QString notes = list.join("|");

	QString sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1'").arg(bookid);
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());	
	stmt.bind(1, notes.toUtf8().constData());

	int rowsChanged = stmt.execDML();
	Q_ASSERT(rowsChanged == 1);

	HRESULT hr = pNotebook->RemoveNote(pNote);
	if (FAILED(hr))
	{
		Q_ASSERT(false);
	}
	return true;
}

bool DbService::RemoveSchedule(INoteApplication* pApp, INote* pNote)
{
	com_sptr<ISchedules> spSchedules;
	pApp->GetSchedules(&spSchedules);
	TrashNote(pApp, spSchedules, pNote);

	QString noteid = AppHelper::GetNoteId(pNote);
	QString sql = QString("DELETE FROM SCHEDULES WHERE note_id = '%1'").arg(noteid);
	int ret = m_db.execDML(sql.toUtf8());
	Q_ASSERT(ret == 1);

	HRESULT hr = spSchedules->RemoveNote(pNote);
	if (FAILED(hr))
	{
		Q_ASSERT(false);
	}
	return true;
}

bool DbService::DeleteNote(ITrash* pTrash, INote* pNote)
{
	QString noteid = AppHelper::GetNoteId(pNote);
	QString sql = QString("DELETE FROM TRASH WHERE note_id='%1';").arg(noteid);
	int ret = m_db.execDML(sql.toUtf8());
	Q_ASSERT(ret == 1);

	sql = QString("DELETE FROM NOTE WHERE ID = '%1';").arg(noteid);
	ret = m_db.execDML(sql.toUtf8());
	Q_ASSERT(ret == 1);

	HRESULT hr = pTrash->RemoveNote(pNote);
	Q_ASSERT(hr == S_OK);
	return true;
}

bool DbService::RemoveNotebook(INoteApplication* pApp, INotebook* pNotebook)
{
	com_sptr<ITrash> spTrash;
	pApp->GetTrash(&spTrash);

	QString bookid = AppHelper::GetNotebookId(pNotebook);
	int nCount = 0;
	pNotebook->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INote> spNote;
		pNotebook->Item(i, &spNote);
		RemoveNote(pApp, pNotebook, spNote);
		spNote->SetBookId(NULL);
	}

	int rowsChanged = m_db.execDML(QString("DELETE FROM NOTEBOOK WHERE ID = '%1';").arg(bookid).toUtf8());
	Q_ASSERT(rowsChanged == 1);

	com_sptr<INotebooks> spNotebooks;
	pApp->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->DeleteNotebook(pNotebook);
	return (hr == S_OK);
}

bool DbService::MoveNotebook(INotebook* pSrcbook, INotebook* pDestbook, INote* pNote)
{
	//Remove from srcbook
	QString srcbook_id = AppHelper::GetNotebookId(pSrcbook);
	QString destbook_id = AppHelper::GetNotebookId(pDestbook);
	QString noteid = AppHelper::GetNoteId(pNote);

	QStringList list = AppHelper::GetNotes(pSrcbook);
	list.removeAll(noteid);
	QString sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1'").arg(srcbook_id);
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());
	QString notes = list.join("|");
	stmt.bind(1, notes.toUtf8().constData());
	int ret = stmt.execDML();
	Q_ASSERT(ret == 1);

	HRESULT hr = pSrcbook->RemoveNote(pNote);
	Q_ASSERT(hr == S_OK);

	list = AppHelper::GetNotes(pDestbook);
	list.append(noteid);
	sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1'").arg(destbook_id);
	stmt = m_db.compileStatement(sql.toUtf8());
	notes = list.join("|");
	stmt.bind(1, notes.toUtf8().constData());
	ret = stmt.execDML();
	Q_ASSERT(ret == 1);

	hr = pDestbook->AddNote(pNote);
	Q_ASSERT(hr == S_OK);

	return true;
}

bool DbService::RecoverNote(INoteApplication* pApp, ITrash* pSrcNoteColl, INote* pNote)
{
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString noteid = AppHelper::GetNoteId(pNote);

	QString sql = QString("SELECT * FROM TRASH WHERE note_id = '%1'").arg(noteid);
	CppSQLite3Query query = m_db.execQuery(sql.toUtf8());
	Q_ASSERT(!query.eof());

	QString trashId = query.getStringField("ID");
	QString bookid = query.getStringField("srcbook_id");

	sql = QString("DELETE FROM TRASH WHERE note_id = '%1'").arg(noteid);
	int rowsChanged = m_db.execDML(sql.toUtf8());
	Q_ASSERT(rowsChanged == 1);

	pSrcNoteColl->RemoveNote(pNote);

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookById(bookid, &spNotebook);
	if (!spNotebook)
	{
		return false;
	}

	sql = QString("SELECT * FROM NOTEBOOK WHERE ID = '%1'").arg(bookid);
	query = m_db.execQuery(sql.toUtf8());
	Q_ASSERT(!query.eof());

	QStringList list = AppHelper::GetNotes(spNotebook);
	list.append(noteid);

	sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1'").arg(bookid);
	CppSQLite3Statement stmt = m_db.compileStatement(sql.toUtf8());

	QString notes = list.join('|');
	stmt.bind(1, notes.toUtf8().constData());

	int nRowsChanged = stmt.execDML();
	Q_ASSERT(nRowsChanged == 1);

	spNotebook->AddNote(pNote);

	return true;
}
