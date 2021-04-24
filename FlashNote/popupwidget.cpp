#include "stdafx.h"
#include "popupwidget.h"
#include "moc_popupwidget.cpp"


PopupWidget::PopupWidget(QWidget* parent)
	: QWidget(parent)
	, m_pContentWidget(nullptr)
{
	setMouseTracking(true);
    setWindowFlag(Qt::Popup);
}

PopupWidget::~PopupWidget()
{
}

void PopupWidget::setContentWidget(QWidget* contentWidget)
{
	m_pContentWidget = contentWidget;
}

void PopupWidget::hideEvent(QHideEvent* event)
{
	emit aboutToHide();
}

void PopupWidget::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	emit aboutToHide();
}

void PopupWidget::exec(int top, int left, int width, int height)
{
	if (!m_pContentWidget)
		return;

	this->setGeometry(top, left, width, height);
	//m_pContentWidget->setWindowFlag(Qt::Popup);
	QEventLoop eventLoop;
	show();
	connect(this, SIGNAL(aboutToHide()), &eventLoop, SLOT(quit()));
	eventLoop.exec();
}