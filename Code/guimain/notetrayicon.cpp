#include "stdafx.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include <QtWidgets/QMenu>


NoteTrayIcon::NoteTrayIcon(QObject* parent /* = nullptr */)
	: QSystemTrayIcon(parent)
	, m_pWin(NULL)
	, m_pApp(NULL)
{
	setIcon(QIcon(":/icons/bluewhite.png"));
	QMenu* pContext = new QMenu(NULL);
	QAction* pQuitAction = pContext->addAction(u8"退出程序");
	setContextMenu(pContext);

	connect(pQuitAction, SIGNAL(triggered()),
		this, SLOT(onQuickTriggered()));
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
	if (reason == Trigger)
		emit triggerActivated();
	else if (reason == Context)
		emit contextActivated();
}

void NoteTrayIcon::onQuickTriggered()
{
	emit quickTriggerd();
}
