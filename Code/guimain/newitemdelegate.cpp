#include "stdafx.h"
#include "newitemdelegate.h"
#include "listpane.h"
#include "MyStyle.h"
#include "guihelper.h"


NewItemDelegate::NewItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

QSize NewItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return MyStyle::dpiScaledSize(QSize(NEW_NOTE_MENU_ITEM_WIDTH, NEW_NOTE_MENU_ITEM_HEIGHT));
}

void NewItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	QColor backgroundClr(42, 51, 60);
	QColor selectedClr(QColor(74, 93, 107));
	QColor hoverClr(53, 69, 83);

	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	option->backgroundBrush.setStyle(Qt::SolidPattern);
	option->decorationSize = QSize(28, 28);
	
	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(selectedClr);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		option->backgroundBrush.setColor(hoverClr);
	}
	else
	{
		option->backgroundBrush.setColor(backgroundClr);
	}
	option->font.setPointSize(12);
}

void NewItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
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

	// draw the icon
	QIcon::State state = opt.state& QStyle::State_Open ? QIcon::On : QIcon::Off;

	const int icon_center_xoffset = 22;
	int iconSize = opt.decorationSize.height();
	int textMargin = 8;
	QTextLayout textLayout2(opt.text, opt.font);
	const int maxLineWidth = 8388607; //参照QCommonStylePrivate::viewItemSize
	QSizeF szText = AppHelper::viewItemTextLayout(textLayout2, maxLineWidth);

	int icon_xoffset = icon_center_xoffset - iconSize / 2;
	int icon_yoffset = (opt.rect.height() - iconSize) / 2;
	int text_xoffset = icon_xoffset + iconSize + textMargin;
	int text_yoffset = (opt.rect.height() - szText.height()) / 2;

	QRect iconRect(opt.rect.x() + icon_xoffset, opt.rect.y() + icon_yoffset, iconSize, iconSize);
	QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, szText.width(), szText.height());

	QIcon::Mode mode = (opt.state & (QStyle::State_MouseOver | QStyle::State_Selected)) ? QIcon::Active : QIcon::Normal;
	opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);

	// draw the text
	if (!opt.text.isEmpty())
	{
		QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
			? QPalette::Normal : QPalette::Disabled;
		if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
			cg = QPalette::Inactive;

		if (opt.state & QStyle::State_Selected)
		{
			painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
		}
		else
		{
			painter->setPen(opt.palette.color(cg, QPalette::Text));
		}

		const int textMargin = 2;
		QRect textRect2 = textRect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
		QTextOption textOption;
		textOption.setWrapMode(QTextOption::ManualWrap);
		textOption.setTextDirection(opt.direction);
		textOption.setAlignment(QStyle::visualAlignment(opt.direction, opt.displayAlignment));

		QPointF paintPosition = textRect2.topLeft();

		QTextLayout textLayout(opt.text, opt.font);
		textLayout.setTextOption(textOption);
		AppHelper::viewItemTextLayout(textLayout, textRect.width());
		textLayout.draw(painter, paintPosition);
	}
	painter->restore();
}