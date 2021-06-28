#include "sql_const.h"
#include <QBuffer>
#include <QFileInfo>
#include <QCoreApplication>
#include "notecore.h"
#include "com_sptr.h"
#include "notecoreinit.h"
#include "dbservice.h"
#include "../guimain/guihelper.h"


DbService& DbService::GetInstance(QString dbPath)
{
	static DbService inst(dbPath);
	return inst;
}

DbService::DbService(QString dbPath)
{
	userid = L"609638963d8ba27ccea4e20d";
	_userid = "609638963d8ba27ccea4e20d";
	m_dbPath = dbPath;
	QString fullPath = m_dbPath + "/" + m_dbName;
	m_db.open(fullPath.toUtf8());
	//reconstruct();
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
		QString modify_time = query.getIntField("modify_time");
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
	BSTR bstrTitle = SysAllocString(title.c_str());
	spNote->SetTitle(bstrTitle);

	std::wstring content = note.content.toStdWString();
	BSTR bstrContent = SysAllocString(content.c_str());
	spNote->SetContent(bstrContent);

	spNote->SetCreateTime(note.create_time.toMSecsSinceEpoch());
	spNote->SetModifyTime(note.modify_time.toMSecsSinceEpoch());

	BSTR bstrId = SysAllocString(note.id.toStdWString().c_str());
	spNote->SetId(bstrId);

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

		BSTR bstrBookId = SysAllocString(srcbook_id.toStdWString().c_str());
		spNote->SetBookId(bstrBookId);
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
				CreateNoteByNOTE(note, &spNote);

				bstrId = SysAllocString(notebook.id.toStdWString().c_str());
				spNote->SetBookId(bstrId);

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

		pNote->SetId(SysAllocString(noteid.toStdWString().c_str()));
		pNote->SetBookId(SysAllocString(bookid.toStdWString().c_str()));
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

		sql = "INSERT INTO SCHEDULES (note_id) VALUES (?);";
		stmt = m_db.compileStatement(sql.toUtf8());
		stmt.bind(1, noteid.toStdString().c_str());

		nRowsChanged = stmt.execDML();
		Q_ASSERT(nRowsChanged == 1);

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

		pNotebook->SetId(SysAllocString(bookid.toStdWString().c_str()));

		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);
		spNotebooks->AddNotebook(pNotebook);
	}
	//其他更新待续
	return true;
}

bool DbService::RemoveNote(INoteApplication* pApp, INoteCollection* pNoteColl, INote* pNote)
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

	QStringList list = AppHelper::GetNotes(pNotebook);
	Q_ASSERT(list.contains(noteid));
	list.removeAll(noteid);
	QString notes = list.join("|");

	sql = QString("UPDATE NOTEBOOK SET notes = ? WHERE ID = '%1'").arg(bookid);
	stmt = m_db.compileStatement(sql.toUtf8());	
	stmt.bind(1, notes.toUtf8().constData());

	rowsChanged = stmt.execDML();
	Q_ASSERT(rowsChanged == 1);

	com_sptr<ITrash> pTrash;
	pApp->GetTrash(&pTrash);
	pTrash->AddNote(pNote);
	HRESULT hr = pNotebook->RemoveNote(pNote);
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
		VARIANT varIndex;
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = i;

		com_sptr<INote> spNote;
		pNotebook->Item(varIndex, &spNote);

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

bool DbService::RecoverNote(INoteApplication* pApp, ITrash* pSrcNoteColl, INote* pNote)
{
	QString localtime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString noteid = AppHelper::GetNoteId(pNote);

	QString sql = QString("SELECT * FROM TRASH WHERE note_id = '%1'").arg(noteid);
	CppSQLite3Query query = m_db.execQuery(sql.toUtf8());
	Q_ASSERT(!query.eof());

	QString trashId = query.getStringField("ID");
	QString bookid = query.getStringField("srcbook_id");

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
	pSrcNoteColl->RemoveNote(pNote);

	return true;
}