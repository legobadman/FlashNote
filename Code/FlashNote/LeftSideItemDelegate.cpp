#include "stdafx.h"
#include "LeftSideItemDelegate.h"
#include "moc_LeftSideItemDelegate.cpp"


LeftSideItemDelegate::LeftSideItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

void LeftSideItemDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model,
	const QModelIndex& index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
}

QSize LeftSideItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int w = ((QWidget*)parent())->width();
	if (index.row() == 0)
		return QSize(w, 60);
	else
		return QSize(w, 36);
}

void LeftSideItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);

	bool bNewNote = index.row() == 0;
	QColor backgroundClr(42, 51, 60);
	QColor selectedClr(QColor(74, 93, 107));

	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	option->backgroundBrush.setStyle(Qt::SolidPattern);
	option->showDecorationSelected = false;
	if (bNewNote)
	{
		option->decorationSize = QSize(28, 28);
	}
	else
	{
		option->decorationSize = QSize(16, 16);
	}

	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(selectedClr);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		if (bNewNote)
		{
			option->backgroundBrush.setColor(backgroundClr);
			option->palette.setColor(QPalette::All, QPalette::Text, QColor(255, 255, 255));
		}
		else
		{
			option->backgroundBrush.setColor(QColor(53, 69, 83));
		}
	}
	else
	{
		option->backgroundBrush.setColor(backgroundClr);
	}
	if (bNewNote)
		option->font.setPointSize(13);
	else
		option->font.setPointSize(12);
}

static QSizeF viewItemTextLayout(QTextLayout& textLayout, int lineWidth, int maxHeight = -1, int* lastVisibleLine = nullptr)
{
	if (lastVisibleLine)
		*lastVisibleLine = -1;
	qreal height = 0;
	qreal widthUsed = 0;
	textLayout.beginLayout();
	int i = 0;
	while (true) {
		QTextLine line = textLayout.createLine();
		if (!line.isValid())
			break;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
		// we assume that the height of the next line is the same as the current one
		if (maxHeight > 0 && lastVisibleLine && height + line.height() > maxHeight) {
			const QTextLine nextLine = textLayout.createLine();
			*lastVisibleLine = nextLine.isValid() ? i : -1;
			break;
		}
		++i;
	}
	textLayout.endLayout();
	return QSizeF(widthUsed, height);
}

void LeftSideItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	//return QStyledItemDelegate::paint(painter, option, index);
	Q_ASSERT(index.isValid());

	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	int roleData = index.data(Qt::UserRole + 1).toInt();
	LEFT_SIDE_TYPE type = LEFT_SIDE_TYPE::ITEM_UNKNOWN;
	if (roleData >= ITEM_NEWNOTE && roleData < ITEM_UNKNOWN)
		type = (LEFT_SIDE_TYPE)roleData;

	const QWidget* widget = option.widget;

	painter->save();
	painter->setClipRect(opt.rect);

	const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>(widget);

	QPalette palette = opt.palette;
	palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
	opt.palette = palette;

	const int icon_center_xoffset = 30;

	int iconSize = opt.decorationSize.height();
	int textMargin = (type == ITEM_NEWNOTE) ? 8 : 5;
	QTextLayout textLayout2(opt.text, opt.font);
	const int maxLineWidth = 8388607; //²ÎÕÕQCommonStylePrivate::viewItemSize
	QSizeF szText = viewItemTextLayout(textLayout2, maxLineWidth);

	int icon_xoffset = icon_center_xoffset - iconSize / 2;
	int icon_yoffset = (opt.rect.height() - iconSize) / 2;
	int text_xoffset = icon_xoffset + iconSize + textMargin;
	int text_yoffset = (opt.rect.height() - szText.height()) / 2;

	QRect iconRect(opt.rect.x() + icon_xoffset, opt.rect.y() + icon_yoffset, iconSize, iconSize);
	QRect textRect(opt.rect.x() + text_xoffset, opt.rect.y() + text_yoffset, szText.width(), szText.height());

	// draw the background
	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		const QPointF oldBrushOrigin = painter->brushOrigin();
		painter->setBrushOrigin(opt.rect.topLeft());
		painter->fillRect(opt.rect, opt.backgroundBrush);
		painter->setBrushOrigin(oldBrushOrigin);
	}

	// draw the icon
	QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
	opt.icon.paint(painter, iconRect, opt.decorationAlignment, QIcon::Normal, state);

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
		viewItemTextLayout(textLayout, textRect.width());
		textLayout.draw(painter, paintPosition);
	}
	painter->restore();
}