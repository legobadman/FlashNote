#include "stdafx.h"
#include "fontsize_combobox.h"
#include "moc_fontsize_combobox.cpp"

FontComboSizeBox::FontComboSizeBox(QWidget* parent)
	: QComboBox(parent)
{
	addItem("8");
	addItem("9");
	addItem("10");
	addItem("11");
	addItem("12");
	addItem("14");
	addItem("16");
	addItem("18");
	addItem("24");
	addItem("36");
	addItem("48");
}

FontComboSizeBox::~FontComboSizeBox()
{

}