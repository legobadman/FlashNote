#include "stdafx.h"
#include "bookmenubutton.h"

BookMenuButton::BookMenuButton(QWidget* parent)
    : MenuButton(parent)
{
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

BookMenuButton::~BookMenuButton()
{

}

QSize BookMenuButton::sizeHint() const
{
    QSize sz = MenuButton::sizeHint();
    return sz;
    return QSize(100, sz.height());
}

QSize BookMenuButton::minimumSizeHint() const
{
    QSize sz = MenuButton::minimumSizeHint();
    return sz;
}

void BookMenuButton::paintEvent(QPaintEvent* event)
{
    MenuButton::paintEvent(event);
}