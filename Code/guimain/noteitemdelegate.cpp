#include "stdafx.h"
#include "noteitemdelegate.h"
#include "MyStyle.h"
#include "LeftSideItemDelegate.h"
#include "guihelper.h"
#include "common_types.h"


NoteItemDelegate::NoteItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return MyStyle::dpiScaledSize(QSize(361, 87));
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

	const QWidget* widget = option.widget;

	// draw the background
	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		QRect rcBg(opt.rect);
		const QPointF oldBrushOrigin = painter->brushOrigin();
		painter->fillRect(rcBg, opt.backgroundBrush);

		painter->setPen(QColor(217, 220, 221));
		painter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());

		//border
		if (opt.state & (QStyle::State_Selected | QStyle::State_MouseOver))
		{
			painter->setPen(QColor(139, 203, 232));
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
		QFont fontTitle(QString::fromUtf16((char16_t*)L"Î¢ÈíÑÅºÚ", 8));
		QTextLayout textLayout2(title, fontTitle);
		const int maxLineWidth = 8388607; //²ÎÕÕQCommonStylePrivate::viewItemSize
		QSizeF szText = AppHelper::viewItemTextLayout(textLayout2, maxLineWidth, 15);
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
		QFont fontContent(QString::fromUtf16((char16_t*)L"Î¢ÈíÑÅºÚ", 7));
		QTextLayout textLayout2(content, fontContent);
		const int maxLineWidth = 8388607; //²ÎÕÕQCommonStylePrivate::viewItemSize
		QSizeF szText = AppHelper::viewItemTextLayout(textLayout2, maxLineWidth);
		QFontMetrics fontMetrics(fontContent);

		int text_xoffset = 9, text_yoffset = 30;
		int w = opt.rect.width() - 2 * text_xoffset;
		int h = fontMetrics.height() * 2;
		QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, w, h);

		painter->setPen(QColor(102, 102, 102));
		painter->setFont(fontContent);
		QRect wtf;
		painter->drawText(textRect, Qt::TextWordWrap, content, &wtf);
	}
	painter->restore();
}
