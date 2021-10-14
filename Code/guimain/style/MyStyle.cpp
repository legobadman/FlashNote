#include "stdafx.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "moc_MyStyle.cpp"
#include "listpane.h"
#include "DrawerFunc.h"
#include "ToolButtonStyle.h"
#include "toolbuttonstyleoption.h"
#include "uiapplication.h"
#include <QScreen>

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
        QScreen* screen = AppHelper::uiApp()->primaryScreen();
        qreal dpi = screen->logicalDotsPerInch();
		scale = dpi / 96.0;
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
			static const qreal margin_offset = dpiScaled(1.0);
			if (flags & (State_Sunken | State_On))
			{
				p->setPen(QPen(QColor(26, 112, 185), 1));
				p->setBrush(QColor(202, 224, 243));
				//可能要dpiScaled
				p->drawRect(opt->rect.adjusted(0, 0, -margin_offset, -margin_offset));
			}
			else if (flags & State_MouseOver)
			{
				p->setPen(QPen(QColor(26, 112, 185), 1));
				p->setBrush(QColor(228, 239, 249));
				p->drawRect(opt->rect.adjusted(0, 0, -margin_offset, -margin_offset));
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

void MyStyle::drawNewItemMenu(const QStyleOptionMenuItem* menuitem, QPainter* painter, const QWidget* widget) const
{
	QRect rect = menuitem->rect;
    int x, y, w, h;
    menuitem->rect.getRect(&x, &y, &w, &h);
    int tab = menuitem->tabWidth;
    bool dis = !(menuitem->state & State_Enabled);
    bool checked = menuitem->checkType != QStyleOptionMenuItem::NotCheckable
        ? menuitem->checked : false;
    bool act = menuitem->state & State_Selected;
	const int gutterWidth = 3;
	int checkcol = 36;

    static const int windowsItemVMargin = 4; // menu item ver text margin
    static const int windowsArrowHMargin = 6; // arrow horizontal margin
    static const int windowsRightBorder = 15; // right border on windows

    QRect vCheckRect = visualRect(menuitem->direction, menuitem->rect, QRect(menuitem->rect.x(),
        menuitem->rect.y(), checkcol - (gutterWidth + menuitem->rect.x()), menuitem->rect.height()));

    if (act) {
		//selected color
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(73, 92, 106));
		painter->drawRect(menuitem->rect);
    }

    if (!menuitem->icon.isNull()) {
        QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
        if (act && !dis)
            mode = QIcon::Active;
        QPixmap pixmap;
        if (checked)
            pixmap = menuitem->icon.pixmap(proxy()->pixelMetric(PM_SmallIconSize, menuitem, widget), mode, QIcon::On);
        else
            pixmap = menuitem->icon.pixmap(proxy()->pixelMetric(PM_SmallIconSize, menuitem, widget), mode);
        const int pixw = pixmap.width() / pixmap.devicePixelRatio();
        const int pixh = pixmap.height() / pixmap.devicePixelRatio();
		static const int icon_xoffset = 7;
		QPoint topLeft(menuitem->rect.x() + icon_xoffset, menuitem->rect.y() + (menuitem->rect.height() - pixh) / 2);
        painter->setPen(menuitem->palette.text().color());
        painter->drawPixmap(topLeft, pixmap);
    }

    painter->setPen(menuitem->palette.buttonText().color());

    const QColor textColor = menuitem->palette.text().color();
    if (dis)
        painter->setPen(textColor);

    int xm = 43;
    int xpos = menuitem->rect.x() + xm;
    QRect textRect(xpos, y + windowsItemVMargin, w - xm - windowsRightBorder - tab + 1, h - 2 * windowsItemVMargin);
    QRect vTextRect = visualRect(menuitem->direction, menuitem->rect, textRect);
    QString s = menuitem->text;
    if (!s.isEmpty()) {    // draw text
        painter->save();
        int t = s.indexOf(QLatin1Char('\t'));
        int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
        if (!proxy()->styleHint(SH_UnderlineShortcut, menuitem, widget))
            text_flags |= Qt::TextHideMnemonic;
        text_flags |= Qt::AlignLeft;
        if (t >= 0) {
            QRect vShortcutRect = visualRect(menuitem->direction, menuitem->rect,
                QRect(textRect.topRight(), QPoint(menuitem->rect.right(), textRect.bottom())));
            painter->drawText(vShortcutRect, text_flags, s.mid(t + 1));
            s = s.left(t);
        }
        QFont font = menuitem->font;
        if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
            font.setBold(true);
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(vTextRect, text_flags, s.left(t));
        painter->restore();
    }
}

void MyStyle::drawControl(ControlElement element, const QStyleOption* opt, QPainter* p, const QWidget* w) const
{
	switch (element)
	{
		case CE_ComboBoxLabel:
			break;
		case CE_ShapedFrame:
			break;
		case CE_MenuItem:
		{
			if (w->objectName() == "newnotemenu")
			{
				if (const QStyleOptionMenuItem* menuitem = qstyleoption_cast<const QStyleOptionMenuItem*>(opt))
				{
					if (menuitem->menuItemType == QStyleOptionMenuItem::Normal)
					{
						return drawNewItemMenu(menuitem, p, w);
					}
				}
			}
		}
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
	return base::pixelMetric(m, opt, widget);
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

	QPixmap px = QIcon(":/icons/downarrow.png").pixmap(
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
