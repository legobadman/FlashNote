#include "stdafx.h"
#include "LeftSideItemDelegate.h"
#include "searchitemdelegate.h"
#include "globalsearcheditor.h"
#include "MyStyle.h"
#include <QAbstractItemView>
#include <QScrollbar>


SearchItemDelegate::SearchItemDelegate(QObject* parent, GlobalSearchEditor* pEditor)
    : QStyledItemDelegate(parent)
    , m_pEditor(pEditor)
{
    m_view = qobject_cast<QAbstractItemView*>(parent);
}

QSize SearchItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int W = m_view->width();
    int scrollWidth = m_view->verticalScrollBar()->width();
    return MyStyle::dpiScaledSize(QSize(W, 50));
}

void SearchItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    option->backgroundBrush.setStyle(Qt::SolidPattern);
    if (option->state & (QStyle::State_Selected | QStyle::State_MouseOver))
    {
        option->backgroundBrush.setColor(QColor(193, 222, 236));
    }
    else
    {
        option->backgroundBrush.setColor(QColor(255, 255, 255));
    }
}

void SearchItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    painter->save();
    painter->setClipRect(opt.rect);

    QString title = index.data(ItemNoteTitle).toString();
    QString content = index.data(ItemNoteContent).toString();
    QString datetime = index.data(ItemNoteShowDataTime).toString();

    //draw the background
    painter->fillRect(opt.rect, opt.backgroundBrush);

    //draw title
    QFont font("Microsoft YaHei", 11);
    QFontMetrics fontMetrics(font);
    int fontHeight = fontMetrics.height();
    int textWidth = fontMetrics.width(title);
    float width_per_text = (float)textWidth / title.length();

    int title_width = MyStyle::dpiScaled(180), title_height = MyStyle::dpiScaled(opt.rect.height());
    int text_xoffset = 10, text_yoffset = (option.rect.height() - fontHeight) / 2;
    int text_y = opt.rect.y() + text_yoffset;
    QRect titleRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, title_width, title_height);

    painter->setPen(QColor(0, 0, 0));
    painter->setFont(font);

    QString searchText = m_pEditor->getSearchText();
    painter->drawText(titleRect, Qt::TextSingleLine, title);
    painter->restore();
}