#include "stdafx.h"
#include "uiapplication.h"
#include "MyStyle.h"
#include "dbservice.h"
#include "guihelper.h"
#include "floatingmenubutton.h"
#include "screenshot/screenshotwindow.h"
#include "shortcut/gui/qxtglobalshortcut.h"
#ifdef Q_OS_WIN
#include "notehook.h"
#endif
#include "globalsearcheditor.h"
#include <QtWidgets/QDesktopWidget>


UiApplication::UiApplication(int& argc, char** argv)
	: QApplication(argc, argv)
#ifdef Q_OS_WIN
	, m_hFileMapT(INVALID_HANDLE_VALUE)
	, m_hNamedEvent(INVALID_HANDLE_VALUE)
#endif
	, m_pSearchEditor(NULL)
{
	QApplication::setStyle(new MyStyle);
    static QTranslator mpQtTr;
	const QString& appdatapath = this->applicationDirPath();
    mpQtTr.load(appdatapath + "/translations/" + "uimain");
    installTranslator(&mpQtTr);
	initFont();
	CreateApplication(&m_spApp);
	initCoreFromRPC();
	initUI();
#ifdef Q_OS_WIN
	initFileMapping();
#endif
}

UiApplication::~UiApplication()
{
#ifdef Q_OS_WIN
	if (m_hFileMapT != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileMapT);
	}
#endif
}

INoteApplication* UiApplication::coreApplication()
{
	return m_spApp;
}

void UiApplication::initUI()
{
	NoteMainWindow* pwtf = new NoteMainWindow(NULL);
	m_mainWindow = QSharedPointer<NoteMainWindow>(pwtf);

	connect(&m_trayIcon, SIGNAL(triggerActivated()), this, SLOT(onTrigger()));
	connect(&m_trayIcon, SIGNAL(quickTriggerd()), this, SLOT(onQuickApp()));
	connect(&m_trayIcon, SIGNAL(hookTriggered(bool)), this, SLOT(onHookChecked(bool)));

	QxtGlobalShortcut* shortcut = new QxtGlobalShortcut(this);
#ifdef Q_OS_WIN
	shortcut->setShortcut(QKeySequence("Alt+Q"));
#else
	shortcut->setShortcut(QKeySequence("Ctrl+Alt+Q"));
#endif
	connect(shortcut, SIGNAL(activated()), this, SLOT(screenshot()));

	QxtGlobalShortcut* selectTray = new QxtGlobalShortcut(this);
#ifdef Q_OS_WIN
	selectTray->setShortcut(QKeySequence("Alt+S"));
#else
	selectTray->setShortcut(QKeySequence("Ctrl+Alt+F"));
#endif
	connect(selectTray, SIGNAL(activated()), this, SLOT(showFloatingSearcher()));
}

void UiApplication::initCoreFromRPC()
{
	DbService::GetInstance().InitcoreFromRPC(m_spApp);
}

void UiApplication::initFont()
{
	int fontId = QFontDatabase::addApplicationFont(":/font/msyh.ttc");
	fontId = QFontDatabase::addApplicationFont(":/font/consola.ttf");
	QString fontStr = QFontDatabase::applicationFontFamilies(fontId).at(0);
	QFont font(fontStr);
}

#ifdef Q_OS_WIN
void UiApplication::initFileMapping()
{
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

void UiApplication::onHookChecked(bool bChecked)
{
	if (bChecked)
		installGlobalHook();
	else
		uninstallGlobalHook();
}

void UiApplication::installGlobalHook()
{
#ifdef Q_OS_WIN
	installHook();
#endif
}

void UiApplication::uninstallGlobalHook()
{
#ifdef Q_OS_WIN
	uninstallHook();
	m_mainWindow->_temp_hide_floatWin();
#endif
}


void UiApplication::showFloatingSearcher()
{
	if (m_pSearchEditor == NULL)
		m_pSearchEditor = new GlobalSearchEditor(NULL);

    QDesktopWidget* desktop = QApplication::desktop();

	QSize sz = m_pSearchEditor->sizeHint();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();

	m_pSearchEditor->move((screenWidth - sz.width()) / 2, (screenHeight - sz.height()) / 2);
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
	m_mainWindow->activateWindow();
}
