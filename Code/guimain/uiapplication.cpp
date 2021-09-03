#include "stdafx.h"
#include "uiapplication.h"
#include "MyStyle.h"
#include "dbservice.h"
#include "guihelper.h"
#include "floatingmenubutton.h"
#include "screenshot/screenshotwindow.h"
#include "shortcut/gui/qxtglobalshortcut.h"
#include "notehook.h"


UiApplication::UiApplication(int& argc, char** argv)
	: QApplication(argc, argv)
{
	QApplication::setStyle(new MyStyle);
	CreateApplication(&m_spApp);
	initCoreFromRPC();
	initUI();
}

UiApplication::~UiApplication()
{

}

INoteApplication* UiApplication::coreApplication()
{
	return m_spApp;
}

void UiApplication::initUI()
{
	NoteMainWindow* pwtf = new NoteMainWindow(NULL);
	m_mainWindow = QSharedPointer<NoteMainWindow>(pwtf);
	connect(m_mainWindow.data(), SIGNAL(newnote(int)),
		this, SLOT(onNewNote(int)));
	connect(&m_trayIcon, SIGNAL(triggerActivated()),
		this, SLOT(onTrigger()));
	connect(&m_trayIcon, SIGNAL(quickTriggerd()),
		this, SLOT(onQuickApp()));

	QxtGlobalShortcut* shortcut = new QxtGlobalShortcut(this);
	shortcut->setShortcut(QKeySequence("Alt+Q"));
	connect(shortcut, SIGNAL(activated()), this, SLOT(screenshot()));

	m_pMenuButton = new FloatingMenuButton(NULL);
}

void UiApplication::initCoreFromRPC()
{
	DbService::GetInstance().InitcoreFromRPC(m_spApp);
}

void UiApplication::showWidget()
{
	m_trayIcon.show();
	m_mainWindow->showMaximized();
}

void UiApplication::screenshot()
{
	ScreenShotWidget* pShot = new ScreenShotWidget;
	pShot->showFullScreen();
}

void UiApplication::installGlobalHook()
{
	installHook();
}

void UiApplication::uninstallGlobalHook()
{
	uninstallHook();
}

void UiApplication::showFloatingWin()
{
	m_pMenuButton->show();
}

void UiApplication::hideFloatingWin()
{
	m_pMenuButton->hide();
}

void UiApplication::onQuickApp()
{
	m_mainWindow.clear();
	QApplication::quit();
}

void UiApplication::onTrigger()
{
	m_mainWindow->show();
}
