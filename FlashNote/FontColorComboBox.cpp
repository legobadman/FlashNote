#include "stdafx.h"
#include "menubutton.h"
#include "FontColorComboBox.h"
#include "colorgallery.h"
#include "popupwidget.h"
#include "moc_FontColorComboBox.cpp"

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
	pPopup->setWindowFlag(Qt::Popup);
	ColorGallery* pColorGallery = new ColorGallery(pPopup);
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

void FontColorComboBox::paintEvent(QPaintEvent* event)
{
	if (!m_bUpdated)
	{
		updateIcon();
	}
	return MenuButton::paintEvent(event);
}

void FontColorComboBox::updateIcon()
{
	QPixmap newIcon(24, 24);
	newIcon.fill(Qt::transparent);

	QPainter p(&newIcon);

	QFont font("Calibri Light", 13, QFont::Normal, false);
	p.setFont(font);

	QPen pen;
	pen.setColor(QColor(0, 0, 0));
	p.setPen(pen);
	p.drawText(QRect(9,0,24,24), "a");

	pen.setWidth(3);
	p.setPen(pen);
	p.drawLine(4, 21, 23, 21);
	p.end();

	newIcon.save("wtfwtf.png");
	m_icon = QIcon(newIcon);
	m_bUpdated = true;
}