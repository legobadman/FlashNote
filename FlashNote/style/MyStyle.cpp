#include "stdafx.h"
#include "MyStyle.h"
#include "moc_MyStyle.cpp"
#include "listpane.h"
#include "DrawerFunc.h"
#include "ToolButtonStyle.h"
#include "toolbuttonstyleoption.h"


MyStyle::MyStyle()
{
}

MyStyle::~MyStyle()
{
}

qreal MyStyle::dpiScaled(qreal value)
{
	static qreal scale = -1;
	if (scale < 0)
	{
		scale = 1.0;
		{
			HDC hdcScreen = GetDC(0);
			int dpi = GetDeviceCaps(hdcScreen, LOGPIXELSX);
			ReleaseDC(0, hdcScreen);
			scale = dpi / 96.0;
		}
	}
	return value * scale;
}

QSize MyStyle::dpiScaledSize(const QSize& value)
{
	return QSize(MyStyle::dpiScaled(value.width()), MyStyle::dpiScaled(value.height()));
}

void MyStyle::drawMyLineEdit(PrimitiveElement pe, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
	QColor clrBorder, clrBackground, clrForeground;

	clrBorder = DrawerFunc::getColorFromWidget(widget, option->state, "border");
	clrBackground = DrawerFunc::getColorFromWidget(widget, option->state, "background");
	clrForeground = DrawerFunc::getColorFromWidget(widget, option->state, "foreground");

	painter->setPen(clrBorder);
	painter->setBrush(clrBackground);
	painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
}

void MyStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* widget) const
{
	if (pe == PE_PanelLineEdit)
	{
		return drawMyLineEdit(pe, opt, p, widget);
	}
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

void MyStyle::drawComplexControl_MyToolButton(const StyleOptionToolButton* option, QPainter* painter, const QWidget* widget) const
{
	ToolButtonStyle style;
	style.setOngmaniStyle(true);
	style.initFromStyleOption(option);
	style.paint(painter, widget);
}

void MyStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
	switch (control)
	{
		case CC_MyToolButton:
			if (const StyleOptionToolButton* opt = qstyleoption_cast<const StyleOptionToolButton*>(option))
			{
				drawComplexControl_MyToolButton(opt, painter, widget);
				break;
			}
		default:
			return QProxyStyle::drawComplexControl(control, option, painter, widget);
	}
}
