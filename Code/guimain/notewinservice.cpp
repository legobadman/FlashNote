#include "stdafx.h"
#include "notewinservice.h"

NoteWinService::NoteWinService(QWidget* parent)
	: QWidget(parent)
{
	NoteMainWindow* pwtf = new NoteMainWindow(NULL);
	m_mainWindow = QSharedPointer<NoteMainWindow>(pwtf);
	connect(m_mainWindow.data(), SIGNAL(newnote()), 
		this, SLOT(onNewNote()));
	connect(&m_trayIcon, SIGNAL(triggerActivated()),
		this, SLOT(onTrigger()));
	connect(&m_trayIcon, SIGNAL(quickTriggerd()),
		this, SLOT(onQuickApp()));
}

NoteWinService::~NoteWinService()
{
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

void NoteWinService::onNewNote()
{
	NoteMainWindow* pNewNoteWindow = new NoteMainWindow(NULL);

	pNewNoteWindow->showNavigationPane(false);
	pNewNoteWindow->setWindowTitle(u8"ÐÂ½¨±Ê¼Ç");
	pNewNoteWindow->showMaximized();

	m_subWindows.append(QSharedPointer<NoteMainWindow>(pNewNoteWindow));
}