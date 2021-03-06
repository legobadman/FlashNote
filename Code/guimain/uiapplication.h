#ifndef __UI_APPLICATION_H__
#define __UI_APPLICATION_H__

#include "notetrayicon.h"
#include "NoteMainWindow.h"
#include "globalsearcheditor.h"
#include <QtCore/QTranslator>

class UiApplication : public QApplication
{
	Q_OBJECT
public:
	UiApplication(int& argc, char** argv);
	~UiApplication();

	INoteApplication* coreApplication();
	void showWidget();

public slots:
	void onQuickApp();
	void onHookChecked(bool bChecked);
	void onTrigger();
	void screenshot();
	void showFloatingSearcher();
	void hideFloatingWin();
	void installGlobalHook();
	void uninstallGlobalHook();

private:
	void initUI();
	void initCoreFromRPC();
	void initFont();
#ifdef Q_OS_WIN
	void initFileMapping();
#endif

private:
	NoteTrayIcon m_trayIcon;
	com_sptr<INoteApplication> m_spApp;
	QSharedPointer<NoteMainWindow> m_mainWindow;
	FloatingMenuButton* m_pMenuButton;
	GlobalSearchEditor* m_pSearchEditor;
#ifdef Q_OS_WIN
	HANDLE m_hFileMapT;
	HANDLE m_hNamedEvent;
	HANDLE m_hThread;
#endif
};

#endif
