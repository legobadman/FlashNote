#include "stdafx.h"
#include "font_combobox.h"
#include "moc_font_combobox.cpp"

FontComboBox::FontComboBox(QWidget* parent)
	: QFontComboBox(parent)
{

}

FontComboBox::~FontComboBox()
{

}

void FontComboBox::init()
{

}

void FontComboBox::paintEvent(QPaintEvent* event)
{
	QFontComboBox::paintEvent(event);
}