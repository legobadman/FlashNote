#include "stdafx.h"
#include "ToolButtonStyle.h"
#include "toolbuttonstyleoption.h"
#include "toolbutton.h"
#include "MyStyle.h"
#include "DrawerFunc.h"

namespace {
	QRect calcCenterRect(QRect rc, QSize sz)
	{
		int x = (rc.width() - sz.width()) / 2 + rc.x();
		int y = (rc.height() - sz.height()) / 2 + rc.y();
		return QRect(QPoint(x, y), sz);
	}
	QRect cutRectBottom(QRect& rc, int height)
	{
		QRect r = rc;
		r.setTop(r.bottom() + 1 - height);
		rc.setBottom(rc.bottom() - height);
		return r;
	}
	QRect cutRectRight(QRect& rc, int width)
	{
		QRect r = rc;
		r.setLeft(r.right() + 1 - width);
		rc.setRight(rc.right() - width);
		return r;
	}
	QRect cutRectRight(QRect& rc, QSize sz)
	{
		QRect r = cutRectRight(rc, sz.width());
		return calcCenterRect(r, sz);
	}
	QRect cutRectTop(QRect& rc, int height)
	{
		QRect r = rc;
		r.setBottom(r.top() + height - 1);
		rc.setTop(rc.top() + height);
		return r;
	}
	QSize operator+(QSize sz, const QMargins& m)
	{
		sz.rheight() += m.top() + m.bottom();
		sz.rwidth() += m.left() + m.right();
		return sz;
	}
	QSize operator +=(QSize& sz, const QMargins& m)
	{
		return sz = sz + m;
	}
	QSize horzCombineSize(const QSize& lhs, const QSize& rhs)
	{
		return QSize(lhs.width() + rhs.width(), qMax(lhs.height(), rhs.height()));
	}
	QSize vertCombineSize(const QSize& lhs, const QSize& rhs)
	{
		return QSize(qMax(lhs.width(), rhs.width()), lhs.height() + rhs.height());
	}
}

#define PNT_TO_PIX(p) MyStyle::dpiScaled(p)

#define BUTTON_MARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2))
#define BUTTON_LARGEMARGINS QMargins(PNT_TO_PIX(3),PNT_TO_PIX(1),PNT_TO_PIX(3),PNT_TO_PIX(1))
#define BUTTON_TEXTMARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2))
#define BUTTON_EMBED_ICONQMARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2))
#define BUTTON_ICONMARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2))
#define BUTTON_LARGE_RNICONMARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2))
#define BUTTON_LARGE_ICONMARGINS QMargins(PNT_TO_PIX(3),PNT_TO_PIX(3),PNT_TO_PIX(3),PNT_TO_PIX(3))
#define BUTTON_RNTEXTMARGINS QMargins(PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(2),PNT_TO_PIX(1))
#define BUTTON_ARROWMARGINS QMargins(PNT_TO_PIX(3),PNT_TO_PIX(3),PNT_TO_PIX(2),PNT_TO_PIX(3))
#define BUTTON_ARROW_SIZE QSize(PNT_TO_PIX(5),PNT_TO_PIX(2))
#define BUTTON_ARROWMARGINS QMargins(PNT_TO_PIX(3),PNT_TO_PIX(3),PNT_TO_PIX(2),PNT_TO_PIX(3))

#define BUTTON_SPLITBUTTON_MENUSIZE QSize(PNT_TO_PIX(40), PNT_TO_PIX(32))
#define BUTTON_SPLITBUTTON_ARROWRECT QSize(PNT_TO_PIX(4 + 6), PNT_TO_PIX(2 + 6))

enum {
	KBUTTON_TEXT_LINE_SPACE = 1,
	KBUTTON_RN_TEXT_HSPACE = 0,
	KBUTTON_MIN_TEXT_HEIGHT = 20,
	KRbTab_IconMinus = 2,
};


ToolButtonStyle::ToolButtonStyle()
	: m_option(NULL)
	, m_needDrawText(false)
	, m_needDrawIcon(false)
	, m_needDrawRecommendIcon(false)
	, m_needDrawArrow(false)
	, m_bOngmaniStyle(false)
	, m_isRemark(false)
	, m_bArrowAside(false)
{
}

ToolButtonStyle::~ToolButtonStyle()
{

}

bool ToolButtonStyle::isTextUnderIcon() const
{
	return m_option->buttonStyle & ToolButton::ButtonTextUnderIcon;
}

void ToolButtonStyle::setOngmaniStyle(bool value)
{
	m_bOngmaniStyle = value;
}

void ToolButtonStyle::initFromStyleOption(const StyleOptionToolButton* option)
{
	m_option = option;

	bool iconValid = !option->icon.availableSizes().isEmpty();
	m_needDrawIcon = iconValid &&
		(option->text.isEmpty() || option->buttonStyle & ToolButton::ButtonIcon);
	m_needDrawRecommendIcon = option->isRecommend &&
		(option->text.isEmpty() || option->buttonStyle & ToolButton::ButtonIcon);
	m_needDrawText = !option->text.isEmpty() &&
		!option->hideText &&
		(!iconValid || (option->buttonStyle & ToolButton::ButtonText));
	m_needDrawArrow = option->buttonStyle & (ToolButton::ButtonDownArrow | ToolButton::ButtonRightArrow);

	m_bArrowAside = m_bOngmaniStyle;
	m_iconSz = _getIconDrawSize();
	m_textSz = _getTextDrawSize(option->orientation);
	m_arrowSz = _getArrowDrawSize();
	m_themeName = !option->themeName.isEmpty() ? option->themeName : "KToolButton";

	m_isRemark = option->isRemark;
}

bool ToolButtonStyle::isEnabled() const
{
	return m_option->state & QStyle::State_Enabled;
}

bool ToolButtonStyle::isDown() const
{
	return m_option->bDown;
}

bool ToolButtonStyle::isPressed() const
{
	return m_option->state & QStyle::State_Sunken;
}

bool ToolButtonStyle::isChecked() const
{
	return m_option->state & QStyle::State_On;
}

bool ToolButtonStyle::isHover() const
{
	return m_option->state & QStyle::State_MouseOver;
}

bool ToolButtonStyle::isAutoRaise() const
{
	return m_option->state & QStyle::State_AutoRaise;
}

bool ToolButtonStyle::isNoButtonBackGround() const
{
	return m_option->buttonStyle & ToolButton::ButtonNoButtonBg;
}

QColor ToolButtonStyle::textColor() const
{
	if (!isEnabled())
		return DrawerFunc::getDisabledTextColor();

	QColor color = m_option->textColor;
	if (!color.isValid())
	{
		if (m_bOngmaniStyle)
		{
			if (m_option->state & QStyle::State_Sunken || m_option->state & QStyle::State_On)
				color = DrawerFunc::getColor(CLS_TEXT, PROP_DOWN);
			else if (m_option->state & QStyle::State_MouseOver)
				color = DrawerFunc::getColor(CLS_TEXT, PROP_HOVER);
			else
				color = DrawerFunc::getColor(CLS_TEXT, PROP_ENABLE);
		}
		else
		{
			color = DrawerFunc::getColor(CLS_TEXT, PROP_ENABLE);
		}
	}
	if (!color.isValid())
		color = DrawerFunc::getColor(CLS_TEXT, PROP_ENABLE);
	return color;
}

QColor ToolButtonStyle::borderColor() const
{
	QColor color = m_option->borderColor;
	if (!color.isValid())
	{
		if (m_bOngmaniStyle)
		{
			if (m_option->state & QStyle::State_Sunken)
				color = DrawerFunc::getColor(CLS_BORDER, PROP_DOWN);
			else if (m_option->state & QStyle::State_MouseOver)
				color = DrawerFunc::getColor(CLS_BORDER, PROP_HOVER);
			else
				color = DrawerFunc::getColor(CLS_BORDER, PROP_ACTIVE);
		}
		else
			color = DrawerFunc::getColor(CLS_BORDER, PROP_NORMAL);
	}
	return color;
}

QBrush ToolButtonStyle::bgBrush()
{
	QBrush brush = m_option->bgBrush;
	if (brush == Qt::NoBrush)
	{
		if (m_bOngmaniStyle)
		{
			if (m_option->state & QStyle::State_Sunken)
				brush = QBrush(DrawerFunc::getColor(CLS_BACKGROUND, PROP_DOWN));
			else if (m_option->state & QStyle::State_MouseOver)
				brush = QBrush(DrawerFunc::getColor(CLS_BACKGROUND, PROP_HOVER));
			else
				brush = QBrush(DrawerFunc::getColor(CLS_BACKGROUND, PROP_ACTIVE));
		}
		else
			brush = QBrush(DrawerFunc::getColor(CLS_BACKGROUND, PROP_NORMAL));
	}
	return brush;
}

bool ToolButtonStyle::needDrawSperatorLine() const
{
	return !m_bOngmaniStyle && m_option->features & StyleOptionToolButton::Menu &&
		m_option->triggerAble &&
		isEnabled() && (isDown() || isHover() || isChecked());
}

bool ToolButtonStyle::needDrawIcon() const
{
	return m_needDrawIcon;
}

bool ToolButtonStyle::needDrawText() const
{
	return m_needDrawText;
}

bool ToolButtonStyle::needDrawArrow() const
{
	return m_needDrawArrow;
}

bool ToolButtonStyle::needShowHotkey() const
{
	return false;
}

QStringList ToolButtonStyle::getDrawText() const
{
	QStringList sl;
	QString strText = m_option->text;

	if (!isTextUnderIcon())
	{
		strText.remove("\n");
		sl << strText;
	}
	else if (!strText.isNull())
	{
		sl = strText.split('\n');
	}

	return sl;
}

QRect ToolButtonStyle::getHoverRect(const QWidget* widget) const
{
	return m_option->rect - QMargins(1, 1, 1, 1);
}

void ToolButtonStyle::getDrawArea(QRect& iconArea, QRect& textArea, QRect& arrowArea) const
{
	bool bVertical = (m_option->orientation == Qt::Vertical) && m_needDrawText && !isTextUnderIcon();
	QSize contentSz = contentSizeHint(m_option->orientation);
	//if (bVertical)
	//	std::swap(contentSz.rwidth(), contentSz.rheight());

	QRect contentRect;
	if (m_option->largeMargins && m_bOngmaniStyle)
	{
		QMargins margin(BUTTON_LARGEMARGINS);
		contentRect = m_option->rect.adjusted(margin.left(), margin.top(), -margin.right(), -margin.bottom());
		contentRect = QRect(contentRect.topLeft(), contentSz);
	}
	else
	{
		contentRect = calcCenterRect(m_option->rect, contentSz);
	}

	if (isTextUnderIcon() && m_option->isTouchScreenMode && m_bOngmaniStyle)
	{
		contentRect = calcCenterRect(m_option->rect, contentSz);
	}

	iconArea = contentRect;
	textArea = contentRect;
	arrowArea = contentRect;

	if (m_option->isSplitButtonLarge)
	{
		//ASSERT(contentRect.size().height() > KBUTTON_SPLITBUTTON_MENUSIZE.height() && contentRect.size().width() > KBUTTON_SPLITBUTTON_MENUSIZE.width());
		QPoint arrowCenter = contentRect.center();
		arrowCenter.setY(arrowCenter.y() + m_iconSz.height() / 2);
		arrowArea.setSize(m_arrowSz);
		arrowArea.moveCenter(arrowCenter);
	}

	QSize textSz = m_textSz;
	QSize arrowSz = m_arrowSz;
	if (isTextUnderIcon())
	{
		if (m_bArrowAside)
		{
			int nAw = (iconArea.width() - m_iconSz.width()) / 2;
			iconArea.adjust(nAw, 0, -nAw, m_iconSz.height() - contentRect.height());
			textArea.setTop(iconArea.bottom() + 1);
			textArea.setBottom(textArea.bottom() - 1);
			if (!m_needDrawArrow)
				return;
			QStringList strList = getDrawText();
			int nCount = strList.count();
			if (nCount > 0)
			{
				const QFontMetrics& fm = m_option->fontMetrics;
				int nHeight = m_option->fontMetrics.height();
				int nWLast = fm.horizontalAdvance(strList.last());
				if (m_option->isSplitButtonLarge)
				{
					arrowArea.moveBottom(contentRect.bottom());
				}
				else
				{
					QPoint pntOff((textArea.width() + nWLast - m_arrowSz.width()) / 2 + 1, MyStyle::dpiScaled(1) + (nCount - 1) * nHeight + (nHeight - m_arrowSz.height()) / 2);
					arrowArea = QRect(QPoint(textArea.topLeft()) + pntOff, m_arrowSz);
				}
			}
		}
		else
		{
			QStringList strList = getDrawText();
			if (strList.count() != 1 && m_needDrawArrow)
			{
				QMargins margin = BUTTON_TEXTMARGINS;
				const QFontMetrics& fm = m_option->fontMetrics;
				int w = fm.horizontalAdvance(strList.back()) + arrowSz.width() + margin.left() + margin.right();
				textSz.setWidth(qMax(textSz.width(), w));
			}
			textArea = cutRectBottom(iconArea, textSz.height());
			if (!m_needDrawArrow)
				return;

			const QFontMetrics& fm = m_option->fontMetrics;
			int nSpace_X = 0;
			int height = m_option->fontMetrics.height();
			int nSpace_Y = height + textArea.y() + height / 2;
			if (strList.count() == 1)
			{
				nSpace_X = textArea.left() + (textArea.width() - arrowSz.width()) / 2;
			}
			else//两行文字时，箭头位于第二行文字后边
			{
				int w = fm.horizontalAdvance(strList.back());
				nSpace_X = textArea.left() + (textArea.width() - w - arrowSz.width()) / 2 + w;
			}
			arrowArea = QRect(nSpace_X, nSpace_Y - BUTTON_ARROW_SIZE.height() / 2, arrowSz.width(), arrowSz.height());
		}
	}
	else
	{
		if (m_needDrawArrow)
		{
			if (m_option->text.isEmpty() && m_option->icon.availableSizes().isEmpty())
				arrowArea = calcCenterRect(iconArea, arrowSz);
			else
				arrowArea = cutRectRight(iconArea, arrowSz);
		}
		if (bVertical)
			textArea = cutRectBottom(iconArea, textSz.height());
		else
			textArea = cutRectRight(iconArea, textSz.width());
	}
}

void ToolButtonStyle::getButtonArea(QRect& btnArea, QRect& popupArea) const
{
	btnArea = m_option->rect;
	popupArea = btnArea;
	if (!m_option->triggerAble)
	{
		btnArea = QRect(0, 0, 0, 0);
		return;
	}
	if (!(m_option->features & QStyleOptionToolButton::Menu))
	{
		popupArea = QRect();
		return;
	}

	QSize contentSz = contentSizeHint(m_option->orientation);
	QRect contentRect = calcCenterRect(btnArea, contentSz);
	if (isTextUnderIcon())
	{
		if (m_bOngmaniStyle)
		{
			btnArea.setHeight(m_iconSz.height() + 1);
			popupArea.setTop(btnArea.bottom());
		}
		else
		{
			QRect r = cutRectTop(contentRect, m_iconSz.height());
			btnArea = cutRectTop(popupArea, r.bottom());
		}
	}
	else
	{
		QRect r = cutRectRight(contentRect, m_arrowSz.width());
		popupArea = cutRectRight(btnArea, btnArea.right() - r.left());
		if (m_bOngmaniStyle)
			popupArea.setLeft(popupArea.left() - 2);
	}
}

QMargins ToolButtonStyle::_getIconMargin() const
{
	if (m_option->bEmbeded)
		return BUTTON_EMBED_ICONQMARGINS;
	else
		return BUTTON_ICONMARGINS;
}

QSize ToolButtonStyle::_getIconDrawSize() const
{
	QSize sz(0, 0);
	if (m_needDrawIcon || m_option->isSplitButtonLarge)
		sz = m_option->iconSize;
	if (m_option->largeMargins)
		sz += m_bOngmaniStyle ? BUTTON_LARGE_RNICONMARGINS : BUTTON_LARGE_ICONMARGINS;
	else
		sz += _getIconMargin();
	return sz;
}

QSize ToolButtonStyle::_getArrowDrawSize() const
{
	if (!m_needDrawArrow)
		return QSize(0, 0);
	if (m_option->buttonStyle & ToolButton::ButtonRightArrow)
	{
		QSize size(BUTTON_ARROW_SIZE.height(), BUTTON_ARROW_SIZE.width());
		return size + BUTTON_ARROWMARGINS;
	}

	QMargins iconMargs;
	iconMargs = BUTTON_ARROWMARGINS;
	return BUTTON_ARROW_SIZE + iconMargs;
}

QSize ToolButtonStyle::_getTextDrawSize(Qt::Orientation o) const
{
	QSize sz(0, 0);
	if (!m_needDrawText)
		return sz;

	QStringList l = getDrawText();

	if (o == Qt::Horizontal)
	{
		const QFontMetrics& fm = m_option->fontMetrics;

		QSize size = fm.size(Qt::TextShowMnemonic, l.front());
		int nWidth = qMax(size.width(), fm.horizontalAdvance(l.back()));
		int nHeight = qMax(fm.height(), size.height());

		if (isTextUnderIcon() && !(m_option->buttonStyle & ToolButton::ButtonSingleLineText))
		{
			int fontHeight = nHeight;
			if (m_bArrowAside)
				nHeight = l.count() * fontHeight + PNT_TO_PIX(KBUTTON_RN_TEXT_HSPACE);

			else // 不管有没有两排字都要留下两排的空间, Rainbow按钮特殊需求。
				nHeight += fontHeight + PNT_TO_PIX(KBUTTON_TEXT_LINE_SPACE);
		}
		sz = QSize(nWidth, nHeight);
	}
	else
	{
		QString str = m_option->text;
		const QFontMetrics& fm = m_option->fontMetrics;
		QRect rc = fm.boundingRect(0, 0, fm.horizontalAdvance(str.left(1)), INT_MAX, Qt::AlignCenter | Qt::TextWordWrap | Qt::TextShowMnemonic, str);
		sz = rc.size();
	}

	sz += m_bOngmaniStyle ? BUTTON_RNTEXTMARGINS : BUTTON_TEXTMARGINS;

	if (sz.height() < PNT_TO_PIX(KBUTTON_MIN_TEXT_HEIGHT))
		sz.rheight() = PNT_TO_PIX(KBUTTON_MIN_TEXT_HEIGHT);

	return sz;
}

QPainterPath ToolButtonStyle::getHoverPath(const QWidget* widget) const
{
	QPainterPath path;
	if (m_option->features & QStyleOptionToolButton::Menu)
	{
		QRect btnArea, popupArea;
		getButtonArea(btnArea, popupArea);
		QPoint pos = widget->mapFromGlobal(QCursor::pos());
		int round = ROUND_NULL;

		if (popupArea.contains(pos) || isDown() || !m_option->buttonEnabled)
		{
			if (!m_option->triggerAble)
				round = ROUND_ALL;
			else if (isTextUnderIcon())
				round = ROUND_LEFTBUTTOM | ROUND_RIGHTBUTTOM;
			else
				round = ROUND_RIGHTUP | ROUND_RIGHTBUTTOM;

			QRect rc = popupArea.adjusted(0, 0, -1, -1);
			if (isTextUnderIcon() && m_option->triggerAble)
				rc -= QMargins(1, 0, 1, 1);
			else if (m_option->triggerAble)
				rc -= QMargins(0, 1, 1, 1);
			else
				rc -= QMargins(1, 1, 1, 1);

			return DrawerFunc::getBoundPath2(rc, round);
		}
		else
		{
			if (isTextUnderIcon())
				round = ROUND_LEFTUP | ROUND_RIGHTUP;
			else
				round = ROUND_LEFTUP | ROUND_LEFTBUTTOM;
			return DrawerFunc::getBoundPath2(btnArea.adjusted(0, 0, -1, -1) - QMargins(1, 1, 1, 1), round);
		}
	}
	else
	{
		QRect rect = m_option->rect.adjusted(0, 0, -1, -1);
		path = DrawerFunc::getBoundPath2(rect - QMargins(1, 1, 1, 1), m_option->roundCorner);
	}
	return path;
}

QSize ToolButtonStyle::contentSizeHint(Qt::Orientation o) const
{
	QSize iconSz = m_iconSz;
	QSize textSz = m_textSz;
	QSize arrowSz = m_arrowSz;
	QSize sz(0, 0);

	if (isTextUnderIcon())
	{
		QMargins margin = m_bOngmaniStyle ? BUTTON_RNTEXTMARGINS : BUTTON_TEXTMARGINS;
		if (m_bArrowAside)
		{
			QStringList strList = getDrawText();
			if (!strList.isEmpty() && m_needDrawArrow)
			{
				int nWidth = m_option->fontMetrics.horizontalAdvance(strList.last()) + margin.left() + margin.right() + arrowSz.width();
				if (textSz.width() < nWidth)
					textSz.setWidth(nWidth);
			}
			sz = vertCombineSize(iconSz, textSz);
			if (m_option->isSplitButtonLarge)
				sz = vertCombineSize(sz, textSz.isEmpty() ? BUTTON_SPLITBUTTON_MENUSIZE : BUTTON_SPLITBUTTON_ARROWRECT);
		}
		else
		{
			QStringList strList = getDrawText();
			if (!strList.isEmpty() && m_needDrawArrow)
			{
				const QFontMetrics& fm = m_option->fontMetrics;
				int w = fm.horizontalAdvance(strList.back()) + arrowSz.width() + margin.left() + margin.right();
				textSz.setWidth(qMax(textSz.width(), w));
			}
			sz = vertCombineSize(iconSz, textSz);
		}
	}
	else
	{
		QSize szIconAndText = (o == Qt::Horizontal) ? horzCombineSize(iconSz, textSz) : vertCombineSize(iconSz, textSz);
		sz = horzCombineSize(szIconAndText, arrowSz);
	}
	return sz;
}

void ToolButtonStyle::paintBackgrgNormal(QPainter* painter, const QWidget* widget)
{
	QRect rect = m_option->rect.adjusted(0, 0, -1, -1);
	QPainterPath border_path = DrawerFunc::getBoundPath2(rect, m_option->roundCorner);
	QPainterPath hover_path = getHoverPath(widget);

	painter->setPen(borderColor());
	painter->setBrush(Qt::NoBrush);
	painter->drawPath(border_path);

	if (m_option->borderInColor.isValid())
	{
		QRect rc_in = rect.adjusted(1, 1, -1, -1);
		QPainterPath border_in_path = DrawerFunc::getBoundPath2(rc_in, m_option->roundCorner);
		painter->setPen(m_option->borderInColor);
		painter->drawPath(border_in_path);
	}

	painter->setPen(Qt::white);
	painter->setBrush(bgBrush());
	painter->drawPath(hover_path);
}

void ToolButtonStyle::paintBackgrgOngmani(QPainter* painter, const QWidget* widget)
{
	QRect hoverRect = getHoverRect(widget);
	paintOngmaniBtnBackground(painter, hoverRect, m_option);
}

void ToolButtonStyle::paintOngmaniBtnBackground(QPainter* painter,
	const QRect& hoverRect, const StyleOptionToolButton* option)
{
	QRect rect = option->rect.adjusted(0, 0, -1, -1);
	QPainterPath border_path = DrawerFunc::getBoundPath2(rect, option->roundCorner);
	painter->setPen(borderColor());

	if (rect.adjusted(1, 1, 0, 0) == hoverRect)
	{
		painter->setBrush(bgBrush());
	}
	else
	{
		painter->fillRect(hoverRect, bgBrush());
		painter->setBrush(Qt::NoBrush);
	}
	painter->drawPath(border_path);
}

void ToolButtonStyle::drawArrow(QPainter* painter, const QRect& rcArrow, const QPen& c, const QBrush& b)
{
	if (m_option->buttonStyle & ToolButton::ButtonDownArrow)
	{
		DrawerFunc::drawDownArrow(*painter, rcArrow, c, b);
	}
	else if (m_option->buttonStyle & ToolButton::ButtonRightArrow)
	{
		DrawerFunc::drawRightArrow(*painter, rcArrow, c, b);
	}
}

void ToolButtonStyle::drawArrow(QPainter* painter, const QRect& rcArrow, bool enabled)
{
	if (m_option->buttonStyle & ToolButton::ButtonDownArrow)
	{
		DrawerFunc::drawDownArrow(*painter, rcArrow, enabled);
	}
	else if (m_option->buttonStyle & ToolButton::ButtonRightArrow)
	{
		DrawerFunc::drawRightArrow(*painter, rcArrow, enabled);
	}
}

void ToolButtonStyle::drawIcon(QPainter* painter, QRect& rcIcon)
{
	if (m_option->icon.isNull())
		return;

	if (m_option->largeMargins)
		rcIcon -= m_bOngmaniStyle ? BUTTON_LARGE_RNICONMARGINS : BUTTON_LARGE_ICONMARGINS;
	else
		rcIcon -= _getIconMargin();

	QIcon::Mode mode;
	if (!isEnabled() || !m_option->buttonEnabled)
		mode = QIcon::Disabled;
	else if (isPressed() || isChecked())
		mode = QIcon::Selected;
	else if (isHover())
		mode = QIcon::Active;
	else
		mode = QIcon::Normal;

	if (mode != QIcon::Disabled && m_option->icon.availableSizes(mode).isEmpty())
		mode = QIcon::Normal;

	m_option->icon.paint(painter, rcIcon, Qt::AlignCenter, mode);
}

void ToolButtonStyle::drawText(QPainter* painter, QRect& rcText)
{
	rcText -= m_bOngmaniStyle ? BUTTON_RNTEXTMARGINS : BUTTON_TEXTMARGINS;
	QColor text_color = (isEnabled() && m_option->buttonEnabled) ?
		textColor() : DrawerFunc::getDisabledTextColor();

	if (!isTextUnderIcon())
	{
		QStringList sl = getDrawText();
		QString str = sl.isEmpty() ? "" : sl.front();
		if (!needShowHotkey())
			str.replace('&', "&&");
		DrawerFunc::drawColorText(*painter, text_color, rcText,
			Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextShowMnemonic, str);
	}
	else
	{
		QStringList sl = getDrawText();
		QStringList::iterator iter = sl.begin();
		QRect rc = rcText;
		if (m_bArrowAside)
		{
			if (!m_option->isSplitButtonLarge)
			{
				int nOffh = m_option->fontMetrics.height();
				if (int nCount = sl.count())
					nOffh = nCount * nOffh;
				nOffh = (rc.height() - nOffh) / 2;
				rc.adjust(0, nOffh, 0, -nOffh);
			}
		}
		while (iter != sl.end())
		{
			int height = m_option->fontMetrics.height();
			QRect rcTemp = cutRectTop(rc, height);
			//多行文本需要画箭头的时候，需向左移动出箭头宽度的空白，才能保证文字加上箭头后居中。
			if (needDrawArrow() && *iter == sl.last())
			{
				QSize arrowSize = BUTTON_ARROW_SIZE + BUTTON_ARROWMARGINS;
				if (m_option->isSplitButtonLarge == false)
				{
					if (m_bArrowAside)
					{
						int nW = m_option->fontMetrics.horizontalAdvance(*iter);
						int nX = rcText.left() + (rcText.width() - nW - arrowSize.width()) / 2;
						rcTemp.setLeft(nX);
						rcTemp.setWidth(nW);
					}
					else if (sl.count() > 1)
						rcTemp.setRight(rcTemp.right() - arrowSize.width());
				}
			}

			//dc.drawText(rcTemp, Qt::AlignHCenter | Qt::TextShowMnemonic,*iter);

			QString str = *iter;
			if (!needShowHotkey())
				str.replace('&', "&&");

			DrawerFunc::drawColorText(*painter, text_color, rcTemp,
				Qt::AlignHCenter | Qt::TextShowMnemonic, str);
			++iter;
		}
	}
}

QRect ToolButtonStyle::adjustedArrowRect(const QRect& rcArrowDrawArea, const StyleOptionToolButton* option)
{
	return rcArrowDrawArea - BUTTON_ARROWMARGINS;
}

void ToolButtonStyle::paint(QPainter* painter, const QWidget* widget)
{
	bool bHover = isDown() || isHover() || !isAutoRaise();

	//background
	QRect iconArea, textArea, arrowArea;
	getDrawArea(iconArea, textArea, arrowArea);
	if (isEnabled() && (bHover || isChecked()) && !isNoButtonBackGround())
	{
		if (m_bOngmaniStyle && !m_option->borderInColor.isValid())
			paintBackgrgOngmani(painter, widget);
		else
			paintBackgrgNormal(painter, widget);
	}
	else if (m_option->bHasDisabledBound)
	{
		QRect rect = m_option->rect.adjusted(0, 0, -1, -1);
		QPainterPath border_path = DrawerFunc::getBoundPath2(rect, m_option->roundCorner);
		QPainterPath hover_path = getHoverPath(widget);

		painter->setPen(QPen(Qt::gray));
		painter->setBrush(Qt::NoBrush);
		painter->drawPath(border_path);

		if (m_option->borderInColor.isValid())
		{
			QRect rc_in = rect.adjusted(1, 1, -1, -1);
			QPainterPath border_in_path = DrawerFunc::getBoundPath2(rc_in, m_option->roundCorner);
			painter->setPen(m_option->borderInColor);
			painter->drawPath(border_in_path);
		}

		painter->setPen(QPen(Qt::white));
		painter->setBrush(bgBrush());
		painter->drawPath(hover_path);
	}

	if (needDrawSperatorLine())
	{
		painter->setPen(borderColor());
		painter->setBrush(Qt::NoBrush);
		QRect btnArea, popupArea;
		getButtonArea(btnArea, popupArea);
		if (isTextUnderIcon())
			painter->drawLine(btnArea.bottomLeft(), btnArea.bottomRight());
		else
			painter->drawLine(btnArea.topRight(), btnArea.bottomRight());
	}

	if (needDrawIcon())
		drawIcon(painter, iconArea);

	if (needDrawText())
		drawText(painter, textArea);

	if (needDrawArrow())
	{
		if (m_bOngmaniStyle)
		{
			QRect rcA = adjustedArrowRect(arrowArea, m_option);
			QColor color = DrawerFunc::getColor(CLS_ARROW, isEnabled() ? PROP_ENABLE : PROP_DISABLE);
			drawArrow(painter, rcA, color, color);
		}
		else
		{
			drawArrow(painter, arrowArea - BUTTON_ARROWMARGINS, isEnabled());
		}
	}
}