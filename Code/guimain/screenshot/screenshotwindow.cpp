#include "stdafx.h"
#include "screenshotwindow.h"
#include "screenwidget.h"


ScreenshotWindow::ScreenshotWindow(QWidget* parent /* = nullptr */)
	: QMainWindow(parent, Qt::FramelessWindowHint | Qt::SubWindow)
	, m_toolbar(NULL)
{
	m_view = new ScreenShotWidget(nullptr);
	connect(m_view, SIGNAL(grabFinish()), this, SLOT(onGrabFinish()));
	setCentralWidget(m_view);
	m_toolbar = new ScreenToolBar(this);
	m_toolbar->hide();
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

void ScreenshotWindow::onGrabFinish()
{
	QPixmap retImage;
	QRectF rc = m_view->getGrabImage(retImage);
	qreal top = rc.bottom() + 5;
	qreal left = rc.right() - 100;
	m_toolbar->setGeometry(QRect(left, top, 100, 40));
	m_toolbar->show();
}