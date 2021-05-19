#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notewinservice.h"
#include "notetrayicon.h"
#include "MyStyle.h"
#include "richeditor/mrichtextedit.h"
#include "note_types.h"

INoteApplication* coreApp = NULL;

//#define TEXT_RICH_EDITOR
//#define TEST_WIDGET_WINDOW_PARENT

#ifdef TEST_WIDGET_WINDOW_PARENT
#include "qtexamples/MltpDlgs1.h"
#include "qtexamples/MltpDlgs2.h"
#endif

int WINAPI WinMain(__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd)
{
#ifdef TEXT_RICH_EDITOR
	QApplication app(__argc, __argv);

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
	QApplication a(__argc, __argv);
	MltpDlgs1 w;
	w.setMinimumSize(300, 100);
	w.show();
	return a.exec();

#else
	QApplication::addLibraryPath("C:/Qt/Qt-5.15.0/plugins");
	QApplication app(__argc, __argv);

	QApplication::setStyle(new MyStyle);

	CreateApplication(&coreApp);

	NoteWinService::GetInstance().startup();

	app.exec();
	return 0;
#endif
}