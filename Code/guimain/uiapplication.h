#ifndef __UI_APPLICATION_H__
#define __UI_APPLICATION_H__

#include "notetrayicon.h"
#include "NoteMainWindow.h"
#include "floatingmenubutton.h"

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
	void onTrigger();
	void screenshot();
	void showFloatingWin();
	void hideFloatingWin();
	void installGlobalHook();
	void uninstallGlobalHook();

private:
	void initUI();
	void initCoreFromRPC();

private:
	NoteTrayIcon m_trayIcon;
	com_sptr<INoteApplication> m_spApp;
	QSharedPointer<NoteMainWindow> m_mainWindow;
	FloatingMenuButton* m_pMenuButton;
};


#endif
