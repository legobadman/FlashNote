#include "stdafx.h"
#include "searcheditor.h"
#include "MyStyle.h"


SearchEditor::SearchEditor(QWidget* parent)
	: QLineEdit(parent)
{
	setPlaceholderText(u8"搜索笔记");
	setFixedHeight(MyStyle::dpiScaled(25));
}

SearchEditor::~SearchEditor()
{

}
