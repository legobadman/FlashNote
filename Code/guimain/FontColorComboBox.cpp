#include "stdafx.h"
#include "menubutton.h"
#include "FontColorComboBox.h"
#include "colorgallery.h"
#include "popupwidget.h"
#include "moc_FontColorComboBox.cpp"
#include "MyStyle.h"

//#define USE_POPUP_FLAG


FontColorComboBox::FontColorComboBox(QWidget* parent)
	: MenuButton(parent)
	, m_bUpdated(false)
	, m_pGallery(nullptr)
{
	m_clr = QColor(0, 0, 0);
	
	connect(this, SIGNAL(LButtonPressed()), this, SLOT(popupChildWidget()));
}

FontColorComboBox::~FontColorComboBox()
{
}

void FontColorComboBox::initStyleOption(QStyleOptionComboBox* option) const
{
	if (!option)
		return;

	option->initFrom(this);
	option->editable = false;
	option->frame = false;
	if (hasFocus() && !option->editable)
		option->state |= QStyle::State_Selected;
	option->subControls = QStyle::SC_All;
	option->activeSubControls = QStyle::SC_ComboBoxArrow;
	option->state |= QStyle::State_DownArrow;
	option->state |= QStyle::State_On;
}

void FontColorComboBox::popupChildWidget()
{
#ifndef USE_POPUP_FLAG
	PopupWidget* pPopup = new PopupWidget(this);
	ColorGallery* pColorGallery = new ColorGallery(pPopup);
	connect(pColorGallery, SIGNAL(fontColorChanged(const QColor&)), pPopup, SLOT(hide()));
	connect(pColorGallery, SIGNAL(fontColorChanged(const QColor&)), this, SLOT(onFontColorItemClicked(const QColor&)));
	pPopup->setContentWidget(pColorGallery);

	QPoint pGlobal = mapToGlobal(QPoint(0, 0));
	const int margin = 5;
	setDown(true);
	pPopup->exec(pGlobal.x(), pGlobal.y() + height() + margin, 258, 196);
	setDown(false);
	delete pPopup;
#else
	if (!m_pGallery)
		m_pGallery = new ColorGallery(this);

	QPoint pGlobal = mapToGlobal(QPoint(0, 0));
	const int margin = 5;
	setDown(true);
	m_pGallery->setWindowFlag(Qt::Popup);
	m_pGallery->setGeometry(pGlobal.x(), pGlobal.y() + height() + margin, 258, 196);
	m_pGallery->show();
	setDown(false);
#endif
}

void FontColorComboBox::onFontColorItemClicked(const QColor& newColor)
{
	updateColor(newColor);
	emit colorChanged(newColor);
}

void FontColorComboBox::updateColor(const QColor& color)
{
	if (color != m_clr)
	{
		m_clr = color;
		update();
	}
}

void FontColorComboBox::paintEvent(QPaintEvent* event)
{
	//if (!m_bUpdated)
	{
		updateIcon();
	}
	return MenuButton::paintEvent(event);
}

void FontColorComboBox::updateIcon()
{
	int sz = MyStyle::dpiScaled(24);
	QPixmap newIcon(sz, sz);
	newIcon.fill(Qt::transparent);

	QPainter p(&newIcon);

	QIcon fontClr(":/icons/16x16/fontcolor.png");
	p.drawPixmap(QPoint(6, 5), QIcon(":/icons/16x16/fontcolor.png").pixmap(MyStyle::dpiScaledSize(QSize(16, 16))));

	QPen pen;
	pen.setWidth(3);
	pen.setColor(m_clr);
	p.setPen(pen);
	p.drawLine(6, 21, 22, 21);
	p.end();

	m_icon = QIcon(newIcon);
	m_bUpdated = true;
}