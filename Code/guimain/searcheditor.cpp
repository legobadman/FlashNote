#include "stdafx.h"
#include "searcheditor.h"
#include "MyStyle.h"


SearchEditor::SearchEditor(QWidget* parent)
	: QLineEdit(parent)
{
	setPlaceholderText(QString::fromUtf16((char16_t*)L"�����ʼ�"));
	setFixedHeight(MyStyle::dpiScaled(25));
}

SearchEditor::~SearchEditor()
{

}