#include "stdafx.h"
#include "MyStyle.h"
#include "moc_MyStyle.cpp"
#include "listpane.h"
#include "DrawerFunc.h"
#include "ToolButtonStyle.h"
#include "toolbuttonstyleoption.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif


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
#ifdef Q_OS_WIN
			HDC hdcScreen = GetDC(0);
			int dpi = GetDeviceCaps(hdcScreen, LOGPIXELSX);
			ReleaseDC(0, hdcScreen);
			scale = dpi / 96.0;
#endif
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
	if (pe == PE_ComboBoxLineEdit)
	{
		QRect r = opt->rect;
		bool hasFocus = opt->state & (State_MouseOver | State_HasFocus);

		p->save();

		p->setRenderHint(QPainter::Antialiasing, true);
		//  ### highdpi painter bug.
		p->translate(0.5, 0.5);

		// Draw Outline
		p->setPen(QPen(QColor(0, 0, 0), 1));

		if (hasFocus) {
			p->setPen(QColor(22, 22, 22));
		}
		else
		{
			p->setPen(QColor(122, 122, 122));
		}

		p->drawRect(r.adjusted(0, 0, -1, -1));

		// Draw inner shadow
		//p->setPen(d->topShadow());
		//p->drawLine(QPoint(r.left() + 2, r.top() + 1), QPoint(r.right() - 2, r.top() + 1));

		p->restore();
	}
	if (pe == PE_ComboBoxDropdownButton)
	{
		if (const QStyleOptionButton* btn = qstyleoption_cast<const QStyleOptionButton*>(opt)) {
			State flags = opt->state;
			if (flags & (State_Sunken | State_On))
			{
				p->setPen(QPen(QColor(26, 112, 185), 1));
				p->setBrush(QColor(202, 224, 243));
				p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
			}
			else if (flags & State_MouseOver)
			{
				p->setPen(QPen(QColor(26, 112, 185), 1));
				p->setBrush(QColor(228, 239, 249));
				p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
			}
		}
		return;
	}
	if (qobject_cast<const NoteItemTreeView*>(widget))
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

void MyStyle::drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
	switch (element)
	{
	case CE_ComboBoxLabel:
		break;
	case CE_ShapedFrame:
		break;
	}
	return base::drawControl(element, opt, p, w);
}

QRect MyStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
	if (cc == MyStyle::CC_MyComboBox && sc == MyStyle::SC_ComboBoxArrow)
	{
		if (const QStyleOptionComboBox* cb = qstyleoption_cast<const QStyleOptionComboBox*>(opt))
		{
			static const int arrowRcWidth = 18;
			const int xpos = cb->rect.x() + cb->rect.width() - dpiScaled(arrowRcWidth);
			//only left to right
			QRect rc(xpos, cb->rect.y(), dpiScaled(arrowRcWidth), cb->rect.height());
			return rc;
		}
	}
	return base::subControlRect(cc, opt, sc, widget);
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

void MyStyle::drawDropdownArrow(QPainter* painter, QRect downArrowRect) const
{
	QRectF arrowRect;
	arrowRect.setWidth(dpiScaled(16));
	arrowRect.setHeight(dpiScaled(16));
	arrowRect.moveTo(downArrowRect.x() + (downArrowRect.width() - arrowRect.width()) / 2.0,
		downArrowRect.y() + (downArrowRect.height() - arrowRect.height()) / 2.0);

	QPointF bottomPoint = QPointF(arrowRect.center().x(), arrowRect.bottom());

	QPixmap px = QIcon(":/icons/16x16/downarrow.png").pixmap(
		MyStyle::dpiScaledSize(QSize(16, 16)));

	painter->drawPixmap(arrowRect.topLeft(), px);
}

void MyStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
	switch (control)
	{
		case CC_MyComboBox:
		{
			//return base::drawComplexControl(CC_ComboBox, option, painter, widget);
			if (const QStyleOptionComboBox* cmb = qstyleoption_cast<const QStyleOptionComboBox*>(option))
			{
				if (cmb->editable)
				{
					QStyleOptionFrame editorOption;
					editorOption.QStyleOption::operator=(*cmb);
					editorOption.rect = option->rect;
					editorOption.state = (cmb->state & (State_Enabled | State_MouseOver | State_HasFocus) | State_KeyboardFocusChange);

					painter->save();
					painter->setRenderHint(QPainter::Antialiasing, true);
					painter->translate(0.5, 0.5);
					painter->setPen(Qt::NoPen);
					painter->setBrush(editorOption.palette.base());
					painter->restore();
					proxy()->drawPrimitive(static_cast<PrimitiveElement>(PE_ComboBoxLineEdit), &editorOption, painter, widget);

					painter->save();

					QStyleOptionComboBox comboBoxCopy = *cmb;
					QRect downArrowRect = proxy()->subControlRect(
						static_cast<QStyle::ComplexControl>(MyStyle::CC_MyComboBox),
						&comboBoxCopy, SC_ComboBoxArrow, widget);
					painter->setClipRect(downArrowRect);

					QStyleOptionButton buttonOption;
					buttonOption.rect = downArrowRect;
					if (cmb->activeSubControls == SC_ComboBoxArrow)
					{
						buttonOption.state = cmb->state;
					}
					proxy()->drawPrimitive(static_cast<PrimitiveElement>(PE_ComboBoxDropdownButton), &buttonOption, painter, widget);

					painter->restore();

					painter->setPen(QPen(QColor(0, 0, 0), 1));
					drawDropdownArrow(painter, downArrowRect);
					return;
				}
				else
				{
					return base::drawComplexControl(control, option, painter, widget);
				}
			}
			break;
		}
		case CC_MyToolButton:
			if (const StyleOptionToolButton* opt = qstyleoption_cast<const StyleOptionToolButton*>(option))
			{
				drawComplexControl_MyToolButton(opt, painter, widget);
				break;
			}
		default:
			return base::drawComplexControl(control, option, painter, widget);
	}
}
