#include "stdafx.h"
#include "DrawerFunc.h"
#include "FramelessLineEdit.h"
#include "searcheditor.h"
#include "note_editwindow.h"


QColor DrawerFunc::getColorFromWidget(const QWidget* w, QStyle::State state, QString propName)
{
	if (qobject_cast<const FramelessLineEdit*>(w))
	{
		if (state & QStyle::State_MouseOver || state & QStyle::State_HasFocus)
		{
			if (propName == "border")
			{
				return QColor(204, 204, 204);
			}
			if (propName == "background")
			{
				return QColor(255, 255, 255);
			}
		}
		if (propName == "foreground")
		{
			const NoteEditWindow* pWin = qobject_cast<const NoteEditWindow*>(w->parent());
			if (pWin)
			{

			}
		}
	}
	else if (qobject_cast<const SearchEditor*>(w))
	{
		if (propName == "border")
		{
			return QColor(204, 204, 204);
		}
		return QColor(255, 255, 255);
	}
	return QColor(255, 255, 255);
}

QPainterPath DrawerFunc::getBoundPath2(const QRect& rect, int ROUND)
{
	QPainterPath path;
	if (ROUND == ROUND_NULL)
	{
		path.addRect(rect);
		return path;
	}

	int nLeft = rect.left();
	int nTop = rect.top();
	int nRight = rect.right() + 1;
	int nBottom = rect.bottom() + 1;

	if (ROUND & ROUND_LEFTUP)
	{
		path.moveTo(nLeft, nTop + 1);
		path.lineTo(nLeft + 1, nTop + 1);
		path.lineTo(nLeft + 1, nTop);
	}
	else
	{
		path.moveTo(nLeft, nTop);
	}

	if (ROUND & ROUND_RIGHTUP)
	{
		path.lineTo(nRight - 1, nTop);
		path.lineTo(nRight - 1, nTop + 1);
		path.lineTo(nRight, nTop + 1);
	}
	else
	{
		path.lineTo(nRight, nTop);
	}

	if (ROUND & ROUND_RIGHTBUTTOM)
	{
		path.lineTo(nRight, nBottom - 1);
		path.lineTo(nRight - 1, nBottom - 1);
		path.lineTo(nRight - 1, nBottom);
	}
	else
	{
		path.lineTo(nRight, nBottom);
	}

	if (ROUND & ROUND_LEFTBUTTOM)
	{
		path.lineTo(nLeft + 1, nBottom);
		path.lineTo(nLeft + 1, nBottom - 1);
		path.lineTo(nLeft, nBottom - 1);
	}
	else
	{
		path.lineTo(nLeft, nBottom);
	}

	path.closeSubpath();
	return path;
}

QColor DrawerFunc::getDisabledTextColor()
{
	static QColor gray(Qt::gray);
	return gray;
}

void DrawerFunc::drawColorText(QPainter& dc, QColor const& c, QRect const& rc, int flag, QString const& str)
{
	dc.save();
	dc.setPen(c);
	dc.drawText(rc, flag, str);
	dc.restore();
}

void DrawerFunc::drawDownArrow(QPainter& dc, const QRect& rc, const QPen& c, const QBrush& b)
{
	dc.save();

	if (Qt::SolidPattern == b.style() && c.color() == b.color())
	{
		dc.setPen(c);
		dc.setBrush(Qt::NoBrush);

		QVector<QPoint> pts;
		for (int y = 0, row = 0; y <= rc.height(); ++y, ++row)
		{
			for (int x = row; x <= rc.width() - row; ++x)
				pts.append(QPoint(rc.x() + x, rc.y() + y));
		}
		dc.drawPoints(pts.data(), pts.count());
	}
	else
	{
		dc.setPen(c);
		dc.setBrush(b);

		QPolygonF ps;
		ps.push_back(QPointF(rc.x(), rc.y()));
		ps.push_back(QPointF(rc.x() + rc.width(), rc.y()));
		ps.push_back(QPointF(rc.x() + rc.width() / 2, rc.y() + rc.height()));
		dc.drawPolygon(ps);
	}

	dc.restore();
}

void _getArrowPenAndBrush(bool enabled, QPen* c, QBrush* b)
{
	QString colorname = QString("arrow-%1").arg((enabled) ? "enable" : "disable");
	QColor color(255, 255, 255);// DrawerFunc::getColorFromTheme("arrow", colorname);
	*b = QBrush(color);
	*c = QPen(color);
}

void DrawerFunc::drawDownArrow(QPainter& dc, const QRect& rc, bool enabled, bool bTriangle)
{
	if (!enabled || rc.width() != 4 || rc.height() != 2)
	{
		QPen   c;
		QBrush b;
		_getArrowPenAndBrush(enabled, &c, &b);
		drawDownArrow(dc, rc, c, b);
		return;
	}

	int arrowPixels = 9;
	QVector<QColor> colors;
	for (int i = 0; i < arrowPixels; ++i)
	{
		colors.push_back(QColor(255, 255, 255));
	}

	dc.save();

	for (int y = 0, row = 0, coloridx = 0; y <= rc.height(); ++y, ++row)
	{
		for (int x = row; x <= rc.width() - row; ++x)
		{
			QPoint p = QPoint(rc.x() + x, rc.y() + y);
			QColor c = colors[coloridx++];

			dc.setPen(c);
			dc.setBrush(c);
			dc.drawPoint(p);
		}
	}

	dc.restore();
}

void DrawerFunc::drawRightArrow(QPainter& dc, const QRect& rc, const QPen& c, const QBrush& b)
{
	dc.save();

	if (Qt::SolidPattern == b.style() && c.color() == b.color())
	{
		dc.setPen(c);
		dc.setBrush(Qt::NoBrush);

		QVector<QPoint> pts;
		for (int x = 0, col = 0; x <= rc.width(); ++x, ++col)
		{
			for (int y = col; y <= rc.height() - col; ++y)
				pts.append(QPoint(rc.x() + x, rc.y() + y));
		}
		dc.drawPoints(pts.data(), pts.count());
	}
	else
	{
		dc.setPen(c);
		dc.setBrush(b);

		QPolygonF ps;
		ps.push_back(QPointF(rc.x(), rc.y()));
		ps.push_back(QPointF(rc.x(), rc.y() + rc.height()));
		ps.push_back(QPointF(rc.x() + rc.width(), rc.y() + rc.height() / 2));
		dc.drawPolygon(ps);
	}

	dc.restore();
}

QColor DrawerFunc::getColor(DrawClassName className, DrawPropName propName)
{
	QColor clr;
	switch (className)
	{
	case CLS_ARROW:
	{
		clr = QColor(0, 0, 0);
		break;
	}
	case CLS_TEXT:
	{
		clr = QColor(0, 0, 0);
		break;
	}
	case CLS_BORDER:
	{
		switch (propName)
		{
		case PROP_ACTIVE:
		case PROP_DOWN:
		case PROP_HOVER: clr = QColor(139, 202, 231); break;
		default:
			clr = QColor(255, 255, 255);
			break;
		}
		break;
	}
	case CLS_BACKGROUND:
	{
		switch (propName)
		{
		//case PROP_NORMAL:
		//case PROP_ENABLE: clr = QColor(0, 0, 0); break;
		//case PROP_DISABLE:clr = QColor(204, 204, 204); break;
		case PROP_DOWN:	clr = QColor(201, 230, 244); break;
		case PROP_HOVER:  clr = QColor(227,241,249); break;
		case PROP_ACTIVE:clr = QColor(201, 230, 244); break;
		default:
			clr = QColor(255, 0, 0); break;
		}
		break;
	}
	case CLS_NONE:
		break;
	}
	return clr;
}

void DrawerFunc::drawRightArrow(QPainter& dc, const QRect& rc, bool enabled)
{
	QPen   c;
	QBrush b;

	_getArrowPenAndBrush(enabled, &c, &b);
	drawRightArrow(dc, rc, c, b);
}