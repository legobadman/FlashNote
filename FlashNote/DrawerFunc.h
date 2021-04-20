#ifndef __DRAWER_FUNC_H__
#define __DRAWER_FUNC_H__

enum
{
	ROUND_NULL = 0,
	ROUND_LEFTUP = 1,
	ROUND_RIGHTUP = 1 << 1,
	ROUND_LEFTBUTTOM = 1 << 2,
	ROUND_RIGHTBUTTOM = 1 << 3,
	ROUND_ALL = ROUND_LEFTUP | ROUND_RIGHTUP | ROUND_LEFTBUTTOM | ROUND_RIGHTBUTTOM,
	ROUND_ANGLE = 3,
};

enum DrawClassName
{
	CLS_ARROW,
	CLS_TEXT,
	CLS_BACKGROUND,
	CLS_BORDER,
	CLS_NONE,
};

enum DrawPropName
{
	PROP_NORMAL,
	PROP_ENABLE,
	PROP_DISABLE,
	PROP_DOWN,
	PROP_HOVER,
	PROP_ACTIVE,
};

class DrawerFunc
{
public:
	DrawerFunc() {}
	static QColor getColorFromWidget(const QWidget* w, QStyle::State state, QString propName);
	static QPainterPath getBoundPath2(const QRect& rect, int ROUND);
	static QColor getDisabledTextColor();
	static QColor getColor(DrawClassName className, DrawPropName propName);
	static void drawColorText(QPainter& dc, QColor const& c, QRect const& rc, int flag, QString const& str);
	static void drawDownArrow(QPainter& dc, const QRect& rc, const QPen& c, const QBrush& b);
	static void drawDownArrow(QPainter& dc, const QRect& rc, bool enabled, bool bTriangle = false);
	static void drawRightArrow(QPainter& dc, const QRect& rc, const QPen& c, const QBrush& b);
	static void drawRightArrow(QPainter& dc, const QRect& rc, bool enabled);
};

#endif
