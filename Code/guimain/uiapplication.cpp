#include "stdafx.h"
#include "uiapplication.h"
#include "MyStyle.h"
#include "dbservice.h"
#include "guihelper.h"
#include "floatingmenubutton.h"
#include "screenshot/screenshotwindow.h"
#include "shortcut/gui/qxtglobalshortcut.h"
#include "notehook.h"
#include "globalsearcheditor.h"


UiApplication::UiApplication(int& argc, char** argv)
	: QApplication(argc, argv)
#ifdef Q_OS_WIN
	, m_hFileMapT(INVALID_HANDLE_VALUE)
	, m_hNamedEvent(INVALID_HANDLE_VALUE)
#endif
	, m_pSearchEditor(NULL)
{
	QApplication::setStyle(new MyStyle);
	CreateApplication(&m_spApp);
	initCoreFromRPC();
	initUI();
	initFileMapping();
}

UiApplication::~UiApplication()
{
	if (m_hFileMapT != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileMapT);
	}
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

	QxtGlobalShortcut* selectTray = new QxtGlobalShortcut(this);
	selectTray->setShortcut(QKeySequence("Alt+S"));
	connect(selectTray, SIGNAL(activated()), this, SLOT(showFloatingSearcher()));
}

void UiApplication::initCoreFromRPC()
{
	DbService::GetInstance().InitcoreFromRPC(m_spApp);
}

#ifdef Q_OS_WIN
void UiApplication::initFileMapping()
{
	//创建内存映射文件。
	m_hFileMapT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, 4 * 1024, TEXT("FlashMousePosition"));
	m_hNamedEvent = CreateEventW(NULL, FALSE, FALSE, L"FlashMouseEvnet");
}
#endif

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
	m_mainWindow->_temp_hide_floatWin();
}

void UiApplication::showFloatingSearcher()
{
	//读取共享文件上的值
	if (m_pSearchEditor == NULL)
		m_pSearchEditor = new GlobalSearchEditor(NULL);
	m_pSearchEditor->show();
}

void UiApplication::hideFloatingWin()
{
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
