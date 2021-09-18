#include "stdafx.h"
#include "labelbutton.h"
#include "MyStyle.h"


NLabelButton::NLabelButton(QWidget* parent)
    : QLabel(parent)
    , m_bHovered(false)
{
}

void NLabelButton::setIcons(const QSize& sz, const QIcon& iconEnable, const QIcon& iconDisable, const QIcon& iconHover)
{
    m_iconSize = sz;
    m_iconEnable = iconEnable;
    m_iconDisable = iconDisable;
    m_iconHover = iconHover;
    setPixmap(m_iconEnable.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
}

void NLabelButton::enterEvent(QEvent* event)
{
    QLabel::enterEvent(event);
    if (isEnabled())
    {
        setPixmap(m_iconHover.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
        //update();
    }
    m_bHovered = true;
}

void NLabelButton::leaveEvent(QEvent* event)
{
    QLabel::leaveEvent(event);
    if (isEnabled())
    {
        setPixmap(m_iconEnable.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
        //update();
    }
    m_bHovered = false;
}

void NLabelButton::mouseReleaseEvent(QMouseEvent* e)
{
    QLabel::mouseReleaseEvent(e);
    emit clicked();
}

void NLabelButton::paintEvent(QPaintEvent* e)
{
    if (isEnabled())
    {
        if (m_bHovered)
            setPixmap(m_iconHover.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
        else
            setPixmap(m_iconEnable.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
    }
    else
    {
        setPixmap(m_iconDisable.pixmap(MyStyle::dpiScaledSize(m_iconSize)));
    }
    QLabel::paintEvent(e);
}