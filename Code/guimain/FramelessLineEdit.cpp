#include "stdafx.h"
#include "FramelessLineEdit.h"
#include "moc_FramelessLineEdit.cpp"


FramelessLineEdit::FramelessLineEdit(QWidget* parent)
	: QLineEdit(parent)
{
	setAttribute(Qt::WA_Hover, true);
}

FramelessLineEdit::~FramelessLineEdit()
{
}

void FramelessLineEdit::initStyleOption(QStyleOptionFrame* option) const
{
	QLineEdit::initStyleOption(option);
}

void FramelessLineEdit::paintEvent(QPaintEvent* e)
{
	QLineEdit::paintEvent(e);
}