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

	QAction* pOpenAction = pContext->addAction(u8"打开笔记");

#ifdef Q_OS_WIN
	QAction* pHookAction = pContext->addAction(u8"屏幕划取");
	pHookAction->setCheckable(true);
    connect(pHookAction, SIGNAL(triggered(bool)), this, SIGNAL(hookTriggered(bool)));
#endif

	QAction* pQuitAction = pContext->addAction(u8"退出笔记");
	setContextMenu(pContext);

	connect(pOpenAction, SIGNAL(triggered()), this, SIGNAL(triggerActivated()));
	connect(pQuitAction, SIGNAL(triggered()), this, SLOT(onQuickTriggered()));
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
