#ifndef __UI_APPLICATION_H__
#define __UI_APPLICATION_H__

#include "notetrayicon.h"
#include "NoteMainWindow.h"

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

private:
	void initUI();
	void initCoreFromRPC();

private:
	NoteTrayIcon m_trayIcon;
	com_sptr<INoteApplication> m_spApp;
	QSharedPointer<NoteMainWindow> m_mainWindow;
};


#endif
