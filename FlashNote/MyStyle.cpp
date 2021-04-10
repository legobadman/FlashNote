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
			if (vopt && vopt->index.row() == 0 && vopt->state & QStyle::State_MouseOver)
			{
				return;
			}

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

void MyStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
	return QProxyStyle::drawControl(element, option, painter, widget);
}