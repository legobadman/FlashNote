#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include "MyStyle.h"
#include "richeditor/mrichtextedit.h"
#include "note_types.h"
#include "CppSQLite3.h"
#include "dbservice.h"
#include "guihelper.h"
#include "uiapplication.h"
#include <QScreen>
#include "screenshot/screenshot.h"


//#define TEST_RICH_EDITOR
//#define TEST_WIDGET_WINDOW_PARENT
//#define TEST_SQLITE
//#define TEST_SCREENSHOT

#ifdef TEST_WIDGET_WINDOW_PARENT
#include "qtexamples/MltpDlgs1.h"
#include "qtexamples/MltpDlgs2.h"
#endif

int main(int argc, char *argv[])
{
#ifdef TEST_SCREENSHOT
	QApplication app(argc, argv);
	Screenshot screenshot;
	screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
	screenshot.show();
	return app.exec();
#elif TEXT_RICH_EDITOR
	QApplication app(argc, argv);

	QDialog* dialog = new QDialog;
	MRichTextEdit* rte = new MRichTextEdit(dialog);
	QGridLayout* gl = new QGridLayout(dialog);
	gl->addWidget(rte, 0, 0, 1, 1);
	dialog->setWindowTitle(QObject::tr("Rich text editor"));
	dialog->setMinimumWidth(400);
	dialog->setMinimumHeight(400);
	dialog->show();

	return app.exec();

#elif defined(TEST_WIDGET_WINDOW_PARENT)
	QApplication a(argc, argv);
	MltpDlgs1 w;
	w.setMinimumSize(300, 100);
	w.show();
	return a.exec();

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
