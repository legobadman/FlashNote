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
#include <QtCore/QLockFile>
#include <QtCore/QProcess>
#include "globalsearcheditor.h"
#include "wke.h"


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

	wkeSetWkeDllPath(L"E:\\FlashNote\\Debug\\bin\\node.dll");
	wkeInitialize();

	UiApplication app(argc, argv);
	QString appDir = QApplication::applicationDirPath();
    QLockFile* lockFile = new QLockFile(appDir + "/" + "flashnote.app.lock");
    if (!lockFile->tryLock(0))
	{
        return 0;
    }
	app.showWidget();
	app.exec();
	wkeFinalize();
	return 0;
#endif
}
