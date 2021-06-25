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
	QFont font(QString::fromUtf16((char16_t*)L"Î¢ÈíÑÅºÚ"), 9);
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
	QFontComboBox::paintEvent(event);
}