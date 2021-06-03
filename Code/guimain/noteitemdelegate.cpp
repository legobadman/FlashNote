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

	// draw the background
	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		const QPointF oldBrushOrigin = painter->brushOrigin();
		painter->setBrushOrigin(opt.rect.topLeft());
		painter->fillRect(opt.rect, opt.backgroundBrush);
		painter->setBrushOrigin(oldBrushOrigin);
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

		int text_xoffset = 9, text_yoffset = 17;
		QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, szText.width(), szText.height());

		painter->setPen(QColor(0, 0, 0));

		painter->setFont(fontTitle);
		//textLayout.draw(painter, paintPosition);
		QPointF pp(textRect.topLeft());
		painter->drawText(pp, title);
	}

	//2. draw content
	{
		QFont fontContent(QString::fromUtf16((char16_t*)L"Î¢ÈíÑÅºÚ", 7));
		QTextLayout textLayout2(content, fontContent);
		const int maxLineWidth = 8388607; //²ÎÕÕQCommonStylePrivate::viewItemSize
		QSizeF szText = AppHelper::viewItemTextLayout(textLayout2, maxLineWidth);

		int text_xoffset = 9, text_yoffset = 40;
		QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, szText.width(), szText.height());

		painter->setPen(QColor(102, 102, 102));
		painter->setFont(fontContent);
		painter->drawText(textRect.topLeft(), content);
	}
	painter->restore();
}
