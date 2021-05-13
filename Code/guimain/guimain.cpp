#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include "MyStyle.h"
#include "richeditor/mrichtextedit.h"

//#define TEXT_RICH_EDITOR


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

#else
	QApplication::addLibraryPath("C:/Qt/Qt-5.15.0/plugins");
	QApplication app(__argc, __argv);

	QApplication::setStyle(new MyStyle);

	NoteMainWindow mainWin(NULL);
	mainWin.show();

	NoteTrayIcon systemTrayIcon;
	systemTrayIcon.setMainWindow(&mainWin);
	systemTrayIcon.show();

	app.exec();
	return 0;
#endif
}