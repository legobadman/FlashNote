#include "stdafx.h"
#include "SeperateLine.h"

SeperateHLine::SeperateHLine(QWidget* parent /* = nullptr */)
	: QFrame(parent)
{
	setFrameShape(QFrame::HLine);
	setFrameShadow(QFrame::Sunken);
	QPalette palette(this->palette());
	palette.setColor(QPalette::Dark, QColor(204, 204, 204));
	palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
	setPalette(palette);
}

void SeperateHLine::paintEvent(QPaintEvent* e)
{
	QFrame::paintEvent(e);
}


SeperateVLine::SeperateVLine(QWidget* parent /* = nullptr */)
	: QFrame(parent)
{
	setFrameShape(QFrame::VLine);
	setFrameShadow(QFrame::Plain);
	QPalette palette(this->palette());
	palette.setColor(QPalette::Dark, QColor(204, 204, 204));
	palette.setColor(QPalette::WindowText, QColor(204, 204, 204));
	setPalette(palette);
}

void SeperateVLine::paintEvent(QPaintEvent* e)
{
	QFrame::paintEvent(e);
}