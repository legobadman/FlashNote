#include "stdafx.h"
#include "screenshotwindow.h"
#include "screenwidget.h"


ScreenshotWindow::ScreenshotWindow(QWidget* parent /* = nullptr */)
	: QMainWindow(parent, Qt::FramelessWindowHint | Qt::SubWindow)
{
	//m_lblImage = new QLabel(this);
#ifdef USE_WIDGET
	m_screenWidget = new ScreenWidget(nullptr);
	setCentralWidget(m_screenWidget);
#else
	m_view = new ScreenShotWidget(nullptr);
	setCentralWidget(m_view);
#endif
}

ScreenshotWindow::~ScreenshotWindow()
{
}

void ScreenshotWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == 'E')
	{
		this->close();
	}
	base::keyPressEvent(event);
}