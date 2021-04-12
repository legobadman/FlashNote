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
		const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(opt);
		if (pe == PE_PanelItemViewRow && vopt && !vopt->index.isValid())
		{
			QStyleOptionViewItem opt_clone(*vopt);
			opt_clone.features.setFlag(QStyleOptionViewItem::Alternate);

			QBrush brshBackground(QColor(42, 51, 60));
			opt_clone.palette.setBrush(QPalette::All, QPalette::AlternateBase, brshBackground);

			return QProxyStyle::drawPrimitive(pe, &opt_clone, p, widget);
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

int MyStyle::pixelMetric(PixelMetric m, const QStyleOption* opt, const QWidget* widget) const
{
	return QProxyStyle::pixelMetric(m, opt, widget);
}

QRect MyStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
	return QProxyStyle::subElementRect(element, option, widget);
}