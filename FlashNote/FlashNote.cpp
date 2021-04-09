#include "stdafx.h"
#include "NoteMainWindow.h"
#include "MyStyle.h"

int WINAPI WinMain(__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd)
{
	QApplication::addLibraryPath("C:/Qt/Qt-5.15.0/plugins");
	QApplication app(__argc, __argv);

	QApplication::setStyle(new MyStyle);

	NoteMainWindow mainWin(NULL);
	mainWin.show();
	app.exec();
	return 0;
}