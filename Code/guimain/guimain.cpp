#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include "MyStyle.h"
#include "note_types.h"
#include "CppSQLite3.h"
#include "dbservice.h"
#include "guihelper.h"
#include "uiapplication.h"
#include <QScreen>
#include "globalsearcheditor.h"


//#define TEST_SEARCHER
//#define TEST_WIDGET_WINDOW_PARENT
//#define TEST_SQLITE
//#define TEST_SCREENSHOT

int main(int argc, char *argv[])
{
#ifdef TEST_SEARCHER
	UiApplication app(argc, argv);
	GlobalSearchEditor editor(NULL);
	editor.show();
	return app.exec();

#elif defined(TEST_SQLITE)
	QApplication app(argc, argv);
	QApplication::setStyle(new MyStyle);

	QString guid = AppHelper::GenerateGUID();

	CppSQLite3DB db2;
	db2.open("wtf.db");
	CppSQLite3Query query = db2.execQuery("select datetime('now', 'localtime');");
	const char* field = query.fieldName(0);
	const char* wtf = query.getStringField(field);

	DbService& db = DbService::GetInstance();
	db.reconstruct();

#else
	UiApplication app(argc, argv);
	app.showWidget();
	app.exec();
	return 0;
#endif
}
