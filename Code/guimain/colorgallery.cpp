#include "stdafx.h"
#include "colorgallery.h"
#include "moc_colorgallery.cpp"
#include "MyStyle.h"


ColorGallery::ColorGallery(QWidget* parent /* = nullptr */)
	: QWidget(parent)
{
	init();
}

ColorGallery::~ColorGallery()
{
}

void ColorGallery::init()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addStretch();

	QHBoxLayout* pHBoxLayout1 = new QHBoxLayout;
	m_btnDefaultClr = new ToolButton(this);
	m_btnDefaultClr->setFixedSize(MyStyle::dpiScaledSize(QSize(97, 24)));
	m_btnDefaultClr->setText(u8"默认颜色(D)");
	m_btnDefaultClr->setButtonStyle(ToolButton::ButtonText | ToolButton::ButtonIcon);
	m_btnDefaultClr->setIcon(QIcon(":/icons/16x16/default_color.png"));
	m_btnDefaultClr->setIconSize(MyStyle::dpiScaledSize(QSize(16,16)));
	
	pHBoxLayout1->addWidget(m_btnDefaultClr);
	pHBoxLayout1->addStretch();
	pMainLayout->addLayout(pHBoxLayout1);

	m_clrTable = new ColorTableView(this);
	pMainLayout->addWidget(m_clrTable);

	QHBoxLayout* pHBoxLayout2 = new QHBoxLayout;
	m_btnMoreClr = new ToolButton(this);
	m_btnMoreClr->setFixedSize(MyStyle::dpiScaledSize(QSize(108, 24)));
	m_btnMoreClr->setText(u8"更多颜色(M)...");
	m_btnMoreClr->setButtonStyle(ToolButton::ButtonText | ToolButton::ButtonIcon);
	m_btnMoreClr->setIcon(QIcon(":/icons/16x16/morecolor.png"));
	m_btnMoreClr->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	pHBoxLayout2->addWidget(m_btnMoreClr);
	pHBoxLayout2->addStretch();
	pMainLayout->addLayout(pHBoxLayout2);
	pMainLayout->addStretch();

	connect(m_btnDefaultClr, SIGNAL(clicked()), this, SLOT(onDefaultColorClicked()));
	connect(m_clrTable, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemActivated(const QModelIndex&)));

	setLayout(pMainLayout);

	setFixedSize(MyStyle::dpiScaledSize(QSize(248, 196)));
}

void ColorGallery::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
}

void ColorGallery::hideEvent(QHideEvent* event)
{
	QWidget::hideEvent(event);
}

QColor ColorGallery::getCurrentColor(const QColor& color)
{
	return m_color;
}

void ColorGallery::onItemActivated(const QModelIndex& index)
{
	QBrush brush = index.data(Qt::BackgroundRole).value<QBrush>();
	QColor clr = brush.color();
	m_color = clr;
	emit fontColorChanged(m_color);
}

void ColorGallery::onDefaultColorClicked()
{
	m_color = QColor(0, 0, 0);
	emit fontColorChanged(m_color);
}
