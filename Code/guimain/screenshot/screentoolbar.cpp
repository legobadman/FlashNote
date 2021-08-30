#include "stdafx.h"
#include "screentoolbar.h"
#include "MyStyle.h"


ScreenToolBar::ScreenToolBar(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* pLayout = new QHBoxLayout;
	m_pSave = new ToolButton(this);
	m_pFinish = new ToolButton(this);
	m_pClose = new ToolButton(this);

	QPalette palette;
	palette.setBrush(QPalette::Background, QColor(0, 0, 0));
	this->setPalette(palette);
	setStyleSheet("background-color: red");

	static const int sSize = 24;
	static const int sMarginToBar = MyStyle::dpiScaled(5);

	m_pSave->setFixedSize(MyStyle::dpiScaledSize(QSize(sSize, sSize)));
	m_pSave->setIcon(QIcon(":/icons/16x16/download.png"));
	m_pSave->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_pSave->setButtonStyle(ToolButton::ButtonIcon);
	m_pSave->setText("save");

	m_pFinish->setFixedSize(MyStyle::dpiScaledSize(QSize(sSize, sSize)));
	m_pFinish->setIcon(QIcon(":/icons/16x16/finish.png"));
	m_pFinish->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_pFinish->setButtonStyle(ToolButton::ButtonIcon);

	m_pClose->setFixedSize(MyStyle::dpiScaledSize(QSize(sSize, sSize)));
	m_pClose->setIcon(QIcon(":/icons/16x16/close.png"));
	m_pClose->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_pClose->setButtonStyle(ToolButton::ButtonIcon);

	pLayout->setMargin(sMarginToBar);
	pLayout->addWidget(m_pSave);
	pLayout->addWidget(m_pClose);
	pLayout->addWidget(m_pFinish);

	setLayout(pLayout);

	connect(m_pSave, SIGNAL(clicked()), this, SIGNAL(saveTriggered()));
	connect(m_pFinish, SIGNAL(clicked()), this, SIGNAL(finishTriggered()));
	connect(m_pClose, SIGNAL(clicked()), this, SIGNAL(closeTriggered()));
}

ScreenToolBar::~ScreenToolBar()
{

}

void ScreenToolBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QColor(255, 255, 255));
}