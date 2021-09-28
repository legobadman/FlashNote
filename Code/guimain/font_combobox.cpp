#include "stdafx.h"
#include "font_combobox.h"
#include "fontitemdelegate.h"
#include "moc_font_combobox.cpp"
#include "MyStyle.h"


FontComboBox::FontComboBox(QWidget* parent)
	: QFontComboBox(parent)
{
	init();
}

FontComboBox::~FontComboBox()
{
}

void FontComboBox::init()
{
	QFont font("Microsoft YaHei", 9);
	setFont(font);
	setFixedHeight(MyStyle::dpiScaled(25));
	setMaxVisibleItems(20);
	setItemDelegate(new FontItemDelegate(this));
}

QSize FontComboBox::sizeHint() const
{
	return MyStyle::dpiScaledSize(QSize(50, 25));
}

void FontComboBox::paintEvent(QPaintEvent* event)
{
	QStylePainter painter(this);
	painter.setPen(palette().color(QPalette::Text));

	// draw the combobox frame, focusrect and selected etc.
	QStyleOptionComboBox opt;
	initStyleOption(&opt);
	painter.drawComplexControl(static_cast<QStyle::ComplexControl>(MyStyle::CC_MyComboBox), opt);

	if (currentIndex() < 0)
		opt.palette.setBrush(QPalette::ButtonText, opt.palette.brush(QPalette::ButtonText).color().lighter());

	// draw the icon and text
	painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}
