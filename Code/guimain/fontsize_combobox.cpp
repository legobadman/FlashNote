#include "stdafx.h"
#include "fontsize_combobox.h"
#include "MyStyle.h"
#include "moc_fontsize_combobox.cpp"

FontComboSizeBox::FontComboSizeBox(QWidget* parent)
	: QComboBox(parent)
{
	setEditable(true);
	QFont font(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
	//TODO: 不知为何指定不了高度。
	//setFixedSize(MyStyle::dpiScaledSize(QSize(60, 25)));
	setFont(font);
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

void FontComboSizeBox::paintEvent(QPaintEvent* e)
{
	QComboBox::paintEvent(e);
}