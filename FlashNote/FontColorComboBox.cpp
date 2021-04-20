#include "stdafx.h"
#include "menubutton.h"
#include "FontColorComboBox.h"
#include "moc_FontColorComboBox.cpp"


FontColorComboBox::FontColorComboBox(QWidget* parent)
	: MenuButton(parent)
	, m_bUpdated(false)
{
	m_clr = QColor(0, 0, 0);
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