#include "stdafx.h"
#include "searcheditor.h"
#include "MyStyle.h"


SearchEditor::SearchEditor(QWidget* parent)
	: QLineEdit(parent)
{
	setPlaceholderText(u8"搜索功能暂未开放。");
	setFixedHeight(MyStyle::dpiScaled(25));
}

SearchEditor::~SearchEditor()
{

}
