#include "stdafx.h"
#include "noteitemdelegate.h"
#include "MyStyle.h"
#include "LeftSideItemDelegate.h"
#include "guihelper.h"
#include "common_types.h"
#include <QScrollBar>


NoteItemDelegate::NoteItemDelegate(NotesListView* parent)
	: QStyledItemDelegate(parent)
	, m_pListView(parent)
{
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int W = m_pListView->width();
	int scrollWidth = m_pListView->verticalScrollBar()->width();
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

	bool bVisible = m_pListView->verticalScrollBar()->isVisible();
	int scrollWidth = m_pListView->verticalScrollBar()->width();

	QRect itemRect;
	if (bVisible)
		itemRect = opt.rect.adjusted(0, 0, -4, 0);
	else
		itemRect = opt.rect.adjusted(0, 0, -1, 0);

	const NotesListView* noteslist = qobject_cast<const NotesListView*>(option.widget);

	// draw the background
	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		QRect rcBg(itemRect);
		const QPointF oldBrushOrigin = painter->brushOrigin();
		painter->fillRect(rcBg, opt.backgroundBrush);

		//border
		if (opt.state & (QStyle::State_Selected | QStyle::State_MouseOver))
		{
			QColor borderClr;
			if (opt.state & QStyle::State_Selected)
			{
				borderClr = QColor(139, 203, 232);
			}
			else if (opt.state & QStyle::State_MouseOver)
			{
				borderClr = QColor(195, 229, 245);
			}
			painter->setPen(borderClr);
			rcBg.adjust(0, 0, 0, -1);
			painter->drawRect(rcBg);
		}
	}

	// draw the text
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	QString title = AppHelper::GetNoteTitle(spNote);
	QString content = index.data(ItemNoteShowContent).toString();

	//1. draw title
	{
		QFont fontTitle(QString::fromUtf16((char16_t*)L"微软雅黑", 8));
		QFontMetrics fontMetrics(fontTitle);

		int text_xoffset = 9, text_yoffset = 7;
		int w = opt.rect.width() - 2 * text_xoffset;
		int h = fontMetrics.height();
		QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, w, h);

		painter->setPen(QColor(0, 0, 0));

		painter->setFont(fontTitle);
		QPointF pp(textRect.topLeft());
		painter->drawText(textRect, Qt::TextWordWrap, title);
	}

	//2. draw content
	{
		QFont fontContent(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
		QTextLayout textLayout(content, fontContent);
		QFontMetrics fontMetrics(fontContent);
		int fontHeight = fontMetrics.height();

		//计算一行的显示宽度，需要考虑面板的宽度以及有无滚动条，margin。
		int text_xoffset = 9, text_yoffset = 30;
		int lineWidth = itemRect.width() - text_xoffset * 2;

		textLayout.beginLayout();
		int line_limit = 3;
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
		QPointF topLeft(itemRect.x() + text_xoffset, itemRect.y() + text_yoffset);
		textLayout.draw(painter, topLeft);
	}
	painter->restore();
}
