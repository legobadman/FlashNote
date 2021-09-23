#include "stdafx.h"
#include "bookmenubutton.h"
#include "MyStyle.h"


BookMenuButton::BookMenuButton(QWidget* parent)
    : MenuButton(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

QSize BookMenuButton::sizeHint() const
{
    QSize sz = MenuButton::sizeHint();
    const QFont& font = this->font();
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(text());   //已经算上了dpi。
    const int iconSize = 16, icon_left_margin = 10, icon_text_margin = 4, text_right_margin = 20;
    int w = MyStyle::dpiScaled(iconSize + icon_left_margin + icon_text_margin + text_right_margin) + textWidth;
    int h = MyStyle::dpiScaled(22);
    return QSize(w, h);
}

void BookMenuButton::setBookName(const QString& bookName)
{
    setText(bookName);
    updateGeometry();
}

void BookMenuButton::paintEvent(QPaintEvent* event)
{
    MenuButton::paintEvent(event);
}