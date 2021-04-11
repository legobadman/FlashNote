#include "stdafx.h"
#include "MyStyle.h"
#include "moc_MyStyle.cpp"
#include "listpane.h"

MyStyle::MyStyle()
{

}

MyStyle::~MyStyle()
{

}

void MyStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p,
	const QWidget* widget) const
{
	if (qobject_cast<const NavigationPanel*>(widget))
	{
		if (pe == PE_PanelItemViewRow)
		{
			const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt);
			if (vopt && !vopt->index.isValid())
			{
				QStyleOptionViewItem opt_clone(*vopt);
				opt_clone.features.setFlag(QStyleOptionViewItem::Alternate);

				QBrush brshBackground(QColor(42, 51, 60));
				opt_clone.palette.setBrush(QPalette::All, QPalette::AlternateBase, brshBackground);

				return QProxyStyle::drawPrimitive(pe, &opt_clone, p, widget);
			}
		}
		else if (pe == PE_PanelItemViewItem)
		{
			const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt);
			//写法不是很严谨，以后改一下。
			//if (vopt && vopt->index.row() == 0 && vopt->state & QStyle::State_MouseOver)
			//{
			//	return;
			//}

			bool newStyle = true;
			QAbstractItemView::SelectionBehavior selectionBehavior = QAbstractItemView::SelectRows;
			QAbstractItemView::SelectionMode selectionMode = QAbstractItemView::NoSelection;
			if (const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>(widget)) {
				newStyle = !qobject_cast<const QTableView*>(widget);
				selectionBehavior = view->selectionBehavior();
				selectionMode = view->selectionMode();
			}

			if (newStyle && vopt) {
				bool selected = vopt->state & QStyle::State_Selected;
				const bool hover = selectionMode != QAbstractItemView::NoSelection && (vopt->state & QStyle::State_MouseOver);
				bool active = vopt->state & QStyle::State_Active;

				if (vopt->features & QStyleOptionViewItem::Alternate)
					p->fillRect(vopt->rect, vopt->palette.alternateBase());

				QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled
					? QPalette::Normal : QPalette::Disabled;
				if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
					cg = QPalette::Inactive;

				QRect itemRect = subElementRect(QStyle::SE_ItemViewItemFocusRect, opt, widget).adjusted(-1, 0, 1, 0);
				itemRect.setTop(vopt->rect.top());
				itemRect.setBottom(vopt->rect.bottom());

				QSize sectionSize = itemRect.size();
				if (vopt->showDecorationSelected)
					sectionSize = vopt->rect.size();

				if (selectionBehavior == QAbstractItemView::SelectRows)
					sectionSize.setWidth(vopt->rect.width());
				QPixmap pixmap;

				if (vopt->backgroundBrush.style() != Qt::NoBrush) {
					const QPointF oldBrushOrigin = p->brushOrigin();
					p->setBrushOrigin(vopt->rect.topLeft());
					p->fillRect(vopt->rect, vopt->backgroundBrush);
					p->setBrushOrigin(oldBrushOrigin);
				}
				return;
			}
		}
	}
	return QProxyStyle::drawPrimitive(pe, opt, p, widget);
}

int MyStyle::styleHint(StyleHint sh, const QStyleOption* opt, const QWidget* w, QStyleHintReturn* shret) const
{
	if (QStyle::SH_ItemView_PaintAlternatingRowColorsForEmptyArea == sh)
		return 1;
	return QProxyStyle::styleHint(sh, opt, w, shret);
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

void MyStyle::viewItemDrawText(QPainter* p, const QStyleOptionViewItem* option, const QRect& rect) const
{
	//参照QCommonStylePrivate::viewItemDrawText，做适量讲话
	const QWidget* widget = option->widget;
	const int textMargin = 2;

	QRect textRect = rect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
	const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
	QTextOption textOption;
	textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
	textOption.setTextDirection(option->direction);
	textOption.setAlignment(QStyle::visualAlignment(option->direction, option->displayAlignment));

	QPointF paintPosition;
	const QString newText = option->text;

	QTextLayout textLayout(newText, option->font);
	textLayout.setTextOption(textOption);
	viewItemTextLayout(textLayout, textRect.width());
	textLayout.draw(p, paintPosition);
}

void MyStyle::drawControl(ControlElement element, const QStyleOption* opt, QPainter* painter, const QWidget* widget) const
{
	if (false && element == CE_ItemViewItem && qobject_cast<const NavigationPanel*>(widget))
	{
		painter->save();
		painter->setClipRect(opt->rect);

		const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt);

		QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
		QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
		//if (iconRect.x() == 1)
		//{
		//	iconRect.translate(15, 0);
		//}
		QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);

		// draw the background
		proxy()->drawPrimitive(PE_PanelItemViewItem, opt, painter, widget);

		// draw the check mark
		if (vopt->features & QStyleOptionViewItem::HasCheckIndicator) {
			QStyleOptionViewItem option(*vopt);
			option.rect = checkRect;
			option.state = option.state & ~QStyle::State_HasFocus;

			switch (vopt->checkState) {
			case Qt::Unchecked:
				option.state |= QStyle::State_Off;
				break;
			case Qt::PartiallyChecked:
				option.state |= QStyle::State_NoChange;
				break;
			case Qt::Checked:
				option.state |= QStyle::State_On;
				break;
			}
			proxy()->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &option, painter, widget);
		}

		// draw the icon
		QIcon::Mode mode = QIcon::Normal;
		if (!(vopt->state & QStyle::State_Enabled))
			mode = QIcon::Disabled;
		else if (vopt->state & QStyle::State_Selected)
			mode = QIcon::Selected;
		QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
		vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);

		// draw the text
		if (!vopt->text.isEmpty()) {
			QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled
				? QPalette::Normal : QPalette::Disabled;
			if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
				cg = QPalette::Inactive;

			if (vopt->state & QStyle::State_Selected) {
				painter->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
			}
			else {
				painter->setPen(vopt->palette.color(cg, QPalette::Text));
			}
			if (vopt->state & QStyle::State_Editing) {
				painter->setPen(vopt->palette.color(cg, QPalette::Text));
				painter->drawRect(textRect.adjusted(0, 0, -1, -1));
			}

			viewItemDrawText(painter, vopt, textRect);
		}

		// draw the focus rect
		if (vopt->state & QStyle::State_HasFocus) {
			QStyleOptionFocusRect o;
			o.QStyleOption::operator=(*vopt);
			o.rect = proxy()->subElementRect(SE_ItemViewItemFocusRect, vopt, widget);
			o.state |= QStyle::State_KeyboardFocusChange;
			o.state |= QStyle::State_Item;
			QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled)
				? QPalette::Normal : QPalette::Disabled;
			o.backgroundColor = vopt->palette.color(cg, (vopt->state & QStyle::State_Selected)
				? QPalette::Highlight : QPalette::Window);
			proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, painter, widget);
		}

		painter->restore();
		return;
	}
	return QProxyStyle::drawControl(element, opt, painter, widget);
}