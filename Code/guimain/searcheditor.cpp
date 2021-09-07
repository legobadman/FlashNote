#include "stdafx.h"
#include "searcheditor.h"
#include "MyStyle.h"


SearchEditor::SearchEditor(QWidget* parent)
	: QLineEdit(parent)
{
	setPlaceholderText(u8"搜索笔记");
}

SearchEditor::~SearchEditor()
{

}

void SearchEditor::paintEvent(QPaintEvent* event)
{
	QLineEdit::paintEvent(event);
}
