#include "stdafx.h"
#include "noteitemdelegate.h"
#include "MyStyle.h"
#include "LeftSideItemDelegate.h"
#include "guihelper.h"
#include "common_types.h"
#include <QScrollBar>


NoteItemDelegate::NoteItemDelegate(QAbstractItemView* parent, QLineEdit* pEditor)
	: QStyledItemDelegate(parent)
	, m_view(parent)
	, m_pEditor(pEditor)
{
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int W = m_view->width();
	int scrollWidth = m_view->verticalScrollBar()->width();
	return MyStyle::dpiScaledSize(QSize(W - scrollWidth, 100));
}

void NoteItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);

	option->backgroundBrush.setStyle(Qt::SolidPattern);
	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(QColor(193, 222, 236));
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		option->backgroundBrush.setColor(QColor(239, 248, 254));
	}
	else
	{
		option->backgroundBrush.setColor(QColor(255, 255, 255));
	}
	option->font.setPointSize(9);
}

void NoteItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	painter->save();
	painter->setClipRect(opt.rect);

	bool bVisible = m_view->verticalScrollBar()->isVisible();
	int scrollWidth = m_view->verticalScrollBar()->width();

	QRect itemRect;
	if (bVisible)
		itemRect = opt.rect.adjusted(0, 0, -4, 0);
	else
		itemRect = opt.rect.adjusted(0, 0, -1, 0);

	// draw the background
	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		QRect rcBg(itemRect);
		const QPointF oldBrushOrigin = painter->brushOrigin();
		painter->fillRect(rcBg, opt.backgroundBrush);

		//border
		bool hoverSelected = opt.state & (QStyle::State_Selected | QStyle::State_MouseOver);
		QColor borderClr;
		if (hoverSelected)
		{
			if (opt.state & QStyle::State_Selected)
			{
				borderClr = QColor(139, 203, 232);
			}
			else if (opt.state & QStyle::State_MouseOver)
			{
				borderClr = QColor(195, 229, 245);
			}
			painter->setPen(borderClr);
			painter->drawRect(rcBg);
		}
		else
		{
			borderClr = QColor(235, 235, 235);
		}
		
		painter->setPen(borderClr);
		painter->drawLine(itemRect.bottomLeft(), itemRect.bottomRight());
	}

	// draw the text
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	QString title = AppHelper::GetNoteTitle(spNote);
	QString content = index.data(ItemNoteContent).toString();
	QString datetime = index.data(ItemNoteShowDataTime).toString();

	//1. draw title
	{
		QFont fontTitle("Microsoft YaHei", 10);
		if (title.length() > 25)
		{
			title = title.mid(0, 25);
			title.append("...");
		}

		QTextLayout textLayout(title, fontTitle);
		QFontMetrics fontMetrics(fontTitle);

		int text_xoffset = 9, text_yoffset = 7;
		int w = opt.rect.width() - 2 * text_xoffset;
		int h = fontMetrics.height();
		QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, w, h);

		textLayout.beginLayout();
		QTextLine line = textLayout.createLine();
		int lineWidth = itemRect.width() - text_xoffset * 2;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, 0));
		textLayout.endLayout();

        QVector<QTextLayout::FormatRange> selections = _getSearchFormatRange(title);

		painter->setPen(QColor(0, 0, 0));
        textLayout.draw(painter, textRect.topLeft(), selections);
	}

	//2. draw content
	{
		QFont fontContent(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
		QTextLayout textLayout(content, fontContent);
		QFontMetrics fontMetrics(fontContent);
		int fontHeight = fontMetrics.height();

		int text_xoffset = 9, text_yoffset = 30;
		int lineWidth = itemRect.width() - text_xoffset * 2;

		textLayout.beginLayout();
		int line_limit = 2;
		for (int i = 0; i < line_limit; i++)
		{
			QTextLine line = textLayout.createLine();
			if (!line.isValid())
				break;
			line.setLineWidth(lineWidth);
			line.setPosition(QPointF(0, fontHeight * i));
		}
		textLayout.endLayout();

		painter->setPen(QColor(102, 102, 102));

		QVector<QTextLayout::FormatRange> selections = _getSearchFormatRange(content);
		QPointF topLeft(itemRect.x() + text_xoffset, itemRect.y() + text_yoffset);
		textLayout.draw(painter, topLeft, selections);
	}

	//3. draw modify_time
	{
        QFont fontDate(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
        QFontMetrics fontMetrics(fontDate);
        int fontHeight = fontMetrics.height();

		int text_xoffset = 9, text_yoffset = fontHeight * 4 + 5;
		QSize sz = MyStyle::dpiScaledSize(QSize(80, fontHeight));
		QPoint topLeft(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset);
		QRect dateRect(topLeft, sz);
        painter->setPen(QColor(21, 152, 255));
		painter->setFont(fontDate);
        painter->drawText(dateRect, Qt::TextWordWrap, datetime);
	}

	painter->restore();
}

QVector<QTextLayout::FormatRange> NoteItemDelegate::_getSearchFormatRange(const QString& content) const
{
    QVector<QTextLayout::FormatRange> selections;
    QString searchText = m_pEditor->text();
    int idx = 0;
    while (true)
    {
        idx = content.indexOf(searchText, idx, Qt::CaseInsensitive);
        if (idx == -1)
            break;
        QTextLayout::FormatRange frg;
        frg.start = idx;
        frg.length = searchText.length();
        frg.format.setForeground(QColor(255, 0, 0));
        selections.push_back(frg);
        idx++;
    }
	return selections;
}
