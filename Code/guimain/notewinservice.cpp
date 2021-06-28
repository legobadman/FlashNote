#include "stdafx.h"
#include "notewinservice.h"
#include "note_types.h"
#include "rpcservice.h"
#include "dbservice.h"
#include "guihelper.h"


NoteWinService::NoteWinService()
	: QObject(NULL)
{
	m_spApp = coreApp;
	initCoreFromRPC();
	initUI();
}

NoteWinService::~NoteWinService()
{
}

void NoteWinService::initUI()
{
	NoteMainWindow* pwtf = new NoteMainWindow(NULL);
	m_mainWindow = QSharedPointer<NoteMainWindow>(pwtf);
	connect(m_mainWindow.data(), SIGNAL(newnote(int)),
		this, SLOT(onNewNote(int)));
	connect(&m_trayIcon, SIGNAL(triggerActivated()),
		this, SLOT(onTrigger()));
	connect(&m_trayIcon, SIGNAL(quickTriggerd()),
		this, SLOT(onQuickApp()));
}

NoteWinService& NoteWinService::GetInstance()
{
	static NoteWinService inst;
	return inst;
}

INoteApplication* NoteWinService::coreApplication()
{
	return m_spApp;
}

void NoteWinService::initCoreFromRPC()
{
#ifdef USE_RPC
	RPCService::GetInstance().InitcoreFromRPC(m_spApp);
#else
	DbService::GetInstance(AppHelper::GetDbPath()).InitcoreFromRPC(m_spApp);
#endif
}

void NoteWinService::startup()
{
	m_trayIcon.show();
	m_mainWindow->showMaximized();
}

void NoteWinService::onQuickApp()
{
	QApplication::quit();
}

void NoteWinService::onTrigger()
{
	m_mainWindow->show();
}
