#ifndef __NOTE_WIN_SERVICE_H__
#define __NOTE_WIN_SERVICE_H__

#include <QObject>

#include "NoteMainWindow.h"
#include "newnotewindow.h"
#include "notetrayicon.h"

class NoteWinService : public QObject
{
	Q_OBJECT
public:
	static NoteWinService& GetInstance();
	~NoteWinService();
	void startup();
	INoteApplication* coreApplication();
	void initUI();
	void initCoreFromRPC();

public slots:
	void onQuickApp();
	void onTrigger();

private:
	NoteWinService();
	NoteWinService(const NoteWinService&) {}

private:
	QSharedPointer<NoteMainWindow> m_mainWindow;
	NoteTrayIcon m_trayIcon;
	com_sptr<INoteApplication> m_spApp;
};

#endif
