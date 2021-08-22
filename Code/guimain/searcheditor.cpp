#include "stdafx.h"
#include "searcheditor.h"
#include "MyStyle.h"


SearchEditor::SearchEditor(QWidget* parent)
	: QLineEdit(parent)
{
	setPlaceholderText(QString::fromUtf16((char16_t*)L"微软雅黑"));
	setFixedHeight(MyStyle::dpiScaled(25));
}

SearchEditor::~SearchEditor()
{

}
