#include "stdafx.h"
#include "screenwidget.h"
#include <QWindow>
#include <QScreen>


ScreenWidget::ScreenWidget(QWidget* parent)
	:QWidget(parent)
{
	m_lblImage = new QLabel(this);
	QScreen* screen = QGuiApplication::primaryScreen();
	if (const QWindow* window = windowHandle())
		screen = window->screen();
	if (!screen)
		return;

	m_origialShot = screen->grabWindow(0);

	QPixmap backgroundShot(m_origialShot.size());
	int alpha = 125;

	QPainter p(&backgroundShot);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawPixmap(0, 0, m_origialShot);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(backgroundShot.rect(), QColor(0, 0, 0, alpha));

	m_lblImage->setPixmap(backgroundShot);
}

ScreenWidget::~ScreenWidget()
{

}