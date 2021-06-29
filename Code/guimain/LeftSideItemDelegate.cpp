#include "stdafx.h"
#include "LeftSideItemDelegate.h"
#include "moc_LeftSideItemDelegate.cpp"
#include "MyStyle.h"
#include "listpane.h"
#include "guihelper.h"


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
	ITEM_WIDGET_TYPE widgetType = index.data(ItemWidgetTypeRole).value<ITEM_WIDGET_TYPE>();
	if (widgetType == ITEM_WIDGET_TYPE::ITEM_TOPLEVEL)
		return MyStyle::dpiScaledSize(QSize(w, 36));
	else if (widgetType == ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL)
		return MyStyle::dpiScaledSize(QSize(w, 24));
}

void LeftSideItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);

	ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	ITEM_WIDGET_TYPE widgetType = index.data(ItemWidgetTypeRole).value<ITEM_WIDGET_TYPE>();
	QColor backgroundClr(42, 51, 60);
	QColor selectedClr(QColor(74, 93, 107));

	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	option->backgroundBrush.setStyle(Qt::SolidPattern);
	option->showDecorationSelected = false;

	option->decorationSize = QSize(16, 16);

	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(selectedClr);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		option->backgroundBrush.setColor(QColor(53, 69, 83));
	}
	else
	{
		option->backgroundBrush.setColor(backgroundClr);
	}
	if (widgetType == ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL)
		option->font.setPointSize(10);
	else
		option->font.setPointSize(12);
}

void LeftSideItemDelegate::drawExpandArrow(QPainter* painter, const QStyleOptionViewItem& option) const
{
	QTreeView* treeview = (QTreeView*)parent();
	const QModelIndex& index = option.index;

	bool bExpanded = treeview->isExpanded(index);
	QPoint basePt = option.rect.topLeft();

	qreal leftmargin = 10, height = 36, bottommargin = 13, leg = 8, base_side = 10;

	QPainterPath path;
	if (bExpanded)
	{
		/*
			------------------------------------

		  ←	leftmargin →  ⊿            leg
						  🠕
					 bottommargin
						  ↓
			------------------------------------
		*/
		bottommargin = 12;

		QPoint lb, rb, rt;
		lb.setX(leftmargin);
		lb.setY(height - bottommargin);
		lb += basePt;

		rb.setX(leftmargin + leg);
		rb.setY(height - bottommargin);
		rb += basePt;

		rt.setX(leftmargin + leg);
		rt.setY(height - bottommargin - leg);
		rt += basePt;

		path.moveTo(lb);
		path.lineTo(rb);
		path.lineTo(rt);
		path.lineTo(lb);

		painter->setPen(Qt::NoPen);
		painter->fillPath(path, QBrush(QColor(212, 220, 226)));

	}
	else
	{
		/*
			------------------------------------
			←	leftmargin →  ▷         base_side:(左边的“底边”), height;
							  🠕
						 bottommargin
							  ↓
			------------------------------------
		*/
		QPointF lt, lb, rp;

		lb.setX(leftmargin);
		lb.setY(height - bottommargin);
		lb += basePt;

		lt.setX(leftmargin);
		lt.setY(height - bottommargin - base_side);
		lt += basePt;

		qreal yyy = (lb.y() + lt.y());
		rp.setY(yyy / 2.0);
		rp.setX(leftmargin + 5.0);	//height

		path.moveTo(lb);
		path.lineTo(rp);
		path.lineTo(lt);
		path.lineTo(lb);

		painter->setPen(QPen(QColor(212, 220, 226)));
		painter->drawPath(path);
	}
}

void LeftSideItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	Q_ASSERT(index.isValid());

	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	ITEM_WIDGET_TYPE widgetType = index.data(ItemWidgetTypeRole).value<ITEM_WIDGET_TYPE>();

	NoteItemTreeView* pTreeview = qobject_cast<NoteItemTreeView*>(parent());

	painter->save();

	const QWidget* widget = option.widget;

	painter->setClipRect(opt.rect);

	const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>(widget);

	QPalette palette = opt.palette;
	palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
	opt.palette = palette;

	int icon_center_xoffset = 32;
	if (ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL == widgetType)
	{
		icon_center_xoffset += 5;
	}

	int iconSize = opt.decorationSize.height();
	int textMargin = 5;
	QTextLayout textLayout2(opt.text, opt.font);
	const int maxLineWidth = 8388607; //参照QCommonStylePrivate::viewItemSize
	QSizeF szText = AppHelper::viewItemTextLayout(textLayout2, maxLineWidth);

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

	//TODO: 展开项在收缩时也能呈现被选中的状态。

	//展开收缩箭头。
	bool isExpandable = (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK || type == ITEM_CONTENT_TYPE::ITEM_SCHEDULE);
	if (isExpandable)
	{
		if (index.model()->hasChildren(index))
			drawExpandArrow(painter, opt);
	}

	// draw the icon
	QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
	opt.icon.paint(painter, iconRect, opt.decorationAlignment, QIcon::Normal, state);

	// 添加按钮
	if (isExpandable && (opt.state & QStyle::State_MouseOver))
	{
		QIcon icon;

		if (pTreeview->GetHoverObj() == MOUSE_IN_ADD)
			icon.addFile(":/icons/16x16/add_hover.png");
		else
			icon.addFile(":/icons/16x16/add_normal.png");

		iconSize = 16;
		int icon_offset = 10;

		QRect addiconRect(opt.rect.width() - icon_offset - iconSize, 
			opt.rect.y() + icon_offset, iconSize, iconSize);

		if (opt.state & QStyle::State_MouseOver)
			icon.paint(painter, addiconRect, opt.decorationAlignment, QIcon::Normal, state);
	}

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