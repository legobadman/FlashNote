#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"

NoteTrayIcon::NoteTrayIcon(QObject* parent /* = nullptr */)
	: QSystemTrayIcon(parent)
	, m_pWin(NULL)
{
	setIcon(QIcon(":/icons/bluewhite.png"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
}

NoteTrayIcon::~NoteTrayIcon()
{
}

void NoteTrayIcon::setMainWindow(NoteMainWindow* pWin)
{
	m_pWin = pWin;
}


void NoteTrayIcon::onActivated(QSystemTrayIcon::ActivationReason reason)
{
	m_pWin->show();
}
