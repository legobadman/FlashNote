#include "stdafx.h"
#include "screenwidget.h"
#include <QWindow>
#include <QScreen>
#include <QGraphicsSceneEvent>


GrabDraggingRect::GrabDraggingRect(const QRectF& rect, QGraphicsItem* parent)
	: QGraphicsRectItem(rect, parent)
{
}

QRectF GrabDraggingRect::boundingRect() const
{
	QRectF br = QGraphicsRectItem::boundingRect();
	return br;
	return br.adjusted(-50, -50, 50, 50);
}


ScreenGrabRect::ScreenGrabRect(const QPixmap& original, const QRectF& rect, QGraphicsItem* parent)
	: _base(parent)
	, m_originalShot(original)
	, m_rect(rect)
	, m_transform(MOUSE_DONOTHING)
{
	qreal w = m_rect.width();
	qreal h = m_rect.height();
	QPixmap grabImage = QPixmap(QSize(w, h));
	QPainter p2(&grabImage);
	QPointF topLeft = m_rect.topLeft();
	QPixmap slice = m_originalShot.copy(topLeft.x(), topLeft.y(), w, h);
	p2.drawPixmap(0, 0, slice);
	setPixmap(grabImage);
	setPos(topLeft);

	m_borderItem = new QGraphicsRectItem(QRectF(0, 0, w, h), this);
	m_borderItem->setPen(QPen(QColor(21, 152, 255), m_borderWidth));
	m_borderItem->setBrush(Qt::NoBrush);
	m_borderItem->setAcceptHoverEvents(true);
	m_borderItem->installSceneEventFilter(this);

	m_cursor_mapper.insert(make_pair(SCALE_LEFT_TOP, Qt::SizeFDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_LEFT_MID, Qt::SizeHorCursor));
	m_cursor_mapper.insert(make_pair(SCALE_LEFT_BOTTOM, Qt::SizeBDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_MID_TOP, Qt::SizeVerCursor));
	m_cursor_mapper.insert(make_pair(SCALE_MID_BOTTOM, Qt::SizeVerCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_TOP, Qt::SizeBDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_MID, Qt::SizeHorCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_BOTTOM, Qt::SizeFDiagCursor));
	m_cursor_mapper.insert(make_pair(TRANSLATE, Qt::SizeAllCursor));

	QVector<QPointF> pts = {
		QPointF(-3, -3),
		QPointF(-3, h / 2 - 3),
		QPointF(-3, h - 3),

		QPointF(w / 2 - 3, -3),
		QPointF(w / 2 - 3, h - 3),

		QPointF(w - 3, -3),	
		QPointF(w - 3, h / 2 - 3),
		QPointF(w - 3, h - 3)
	};
	m_dragPoints.resize(pts.size());

	for (int i = 0; i < pts.size(); i++)
	{
		m_dragPoints[i] = new GrabDraggingRect(QRectF(pts[i].x(), pts[i].y(), 6, 6), this);
		m_dragPoints[i]->setPen(Qt::NoPen);
		m_dragPoints[i]->setBrush(QColor(21, 152, 255));
		m_dragPoints[i]->installSceneEventFilter(this);
		m_dragPoints[i]->setAcceptHoverEvents(true);
	}

	setFlags(ItemIsMovable);// | ItemIsSelectable /*| ItemSendsGeometryChanges*/);
}

void ScreenGrabRect::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_transform = getMouseEventType(event->pos());
	QRectF br = boundingRect();
	QPointF scenePos = this->scenePos();
	qreal W = br.width(), H = br.height();
	switch (m_transform)
	{
	case SCALE_RIGHT_BOTTOM:
		m_movescale_info.fixed_point = scenePos;
		break;
	case SCALE_RIGHT_TOP:
		m_movescale_info.fixed_point = QPointF(scenePos.x(), scenePos.y() + H);
		break;
	case SCALE_LEFT_TOP:
		m_movescale_info.fixed_point = QPointF(scenePos.x() + W, scenePos.y() + H);
		break;
	case SCALE_LEFT_BOTTOM:
		m_movescale_info.fixed_point = QPointF(scenePos.x() + W, scenePos.y());
		break;

	case SCALE_MID_TOP:
		m_movescale_info.fixed_x = scenePos.x();
		m_movescale_info.fixed_y = scenePos.y() + H;
		break;

	case SCALE_MID_BOTTOM:
		m_movescale_info.fixed_x = scenePos.x();
		m_movescale_info.fixed_y = scenePos.y();
		break;

	case SCALE_LEFT_MID:
		m_movescale_info.fixed_y = scenePos.y();
		m_movescale_info.fixed_x = scenePos.x() + W;
		break;
	case SCALE_RIGHT_MID:
		m_movescale_info.fixed_y = scenePos.y();
		m_movescale_info.fixed_x = scenePos.x();
		break;

	case TRANSLATE:
		m_movescale_info.old_width = W;
		m_movescale_info.old_height = H;
		break;
	}

	m_movescale_info.old_height = H;
	m_movescale_info.old_width = W;
	_base::mousePressEvent(event);
}

void ScreenGrabRect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	_base::mouseReleaseEvent(event);
}

void ScreenGrabRect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QPointF pt = event->pos();
	if (m_transform == TRANSLATE)
	{
		_base::mouseMoveEvent(event);
		qreal w = m_movescale_info.old_width;
		qreal h = m_movescale_info.old_height;
		QPixmap grabImage = QPixmap(QSize(w, h));
		QPainter p2(&grabImage);
		QPointF topLeft = this->scenePos();
		QPixmap slice = m_originalShot.copy(topLeft.x(), topLeft.y(), w, h);
		p2.drawPixmap(0, 0, slice);
		setPixmap(grabImage);
		update();
	}
	else if (m_transform == OUTSIDE)
	{
		return;
	}
	else
	{
		QPointF scenePos = event->scenePos();
		qreal newWidth = 0, newHeight = 0;
		QPointF newTopLeft;

		switch (m_transform)
		{
			case SCALE_LEFT_TOP:
			case SCALE_RIGHT_TOP:
			case SCALE_LEFT_BOTTOM:
			case SCALE_RIGHT_BOTTOM:
			{
				//fixed_bottomright
				qreal left = min(scenePos.x(), m_movescale_info.fixed_point.x());
				qreal right = max(scenePos.x(), m_movescale_info.fixed_point.x());
				qreal top = min(scenePos.y(), m_movescale_info.fixed_point.y());
				qreal bottom = max(scenePos.y(), m_movescale_info.fixed_point.y());
				newWidth = right - left + 1;
				newHeight = bottom - top + 1;
				newTopLeft = QPointF(left, top);
				break;
			}
			case SCALE_LEFT_MID:
			{
				qreal left = min(scenePos.x(), m_movescale_info.fixed_x);
				qreal right = max(scenePos.x(), m_movescale_info.fixed_x);
				qreal top = m_movescale_info.fixed_y;
				newWidth = right - left + 1;
				newHeight = m_movescale_info.old_height;
				newTopLeft = QPointF(left, top);
				break;
			}
			case SCALE_RIGHT_MID:
			{
				qreal left = min(scenePos.x(), m_movescale_info.fixed_x);
				qreal right = max(scenePos.x(), m_movescale_info.fixed_x);
				qreal top = m_movescale_info.fixed_y;
				newWidth = right - left + 1;
				newHeight = m_movescale_info.old_height;
				newTopLeft = QPointF(left, top);
				break;
			}
		}

		QPixmap grabImage = QPixmap(QSize(newWidth, newHeight));
		QPainter p2(&grabImage);
		QPixmap slice = m_originalShot.copy(newTopLeft.x(), newTopLeft.y(), newWidth, newHeight);
		p2.drawPixmap(0, 0, slice);
		setPixmap(grabImage);
		setPos(newTopLeft);
		m_borderItem->setRect(QRectF(0, 0, newWidth, newHeight));

		QVector<QPointF> pts = {
			QPointF(-3, -3),
			QPointF(-3, newHeight / 2 - 3),
			QPointF(-3, newHeight - 3),

			QPointF(newWidth / 2 - 3, -3),
			QPointF(newWidth / 2 - 3, newHeight - 3),

			QPointF(newWidth - 3, -3),
			QPointF(newWidth - 3, newHeight / 2 - 3),
			QPointF(newWidth - 3, newHeight - 3)
		};
		for (int i = 0; i < m_dragPoints.size(); i++)
		{
			m_dragPoints[i]->setRect(QRectF(pts[i].x(), pts[i].y(), 6, 6));
		}
		update();
	}
}

ScreenGrabRect::MOUSE_TRANSFORM ScreenGrabRect::getMouseEventType(QPointF pressPoint)
{
	qreal xp = pressPoint.x(), yp = pressPoint.y();
	qreal h = boundingRect().height(), w = boundingRect().width();
	if (xp < -3)
	{
		return OUTSIDE;
	}
	else if (xp >= -3 && xp <= 6)
	{
		if (yp < -3)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 && yp <= 6)
		{
			return SCALE_LEFT_TOP;
		}
		else if (yp >= h / 2 - 3 && yp <= h / 2 + 3)
		{
			return SCALE_LEFT_MID;
		}
		else if (yp >= h - 3 && yp <= h + 3)
		{
			return SCALE_LEFT_BOTTOM;
		}
		else if (yp > h + 3)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp >= w / 2 - 3 && xp <= w / 2 + 3)
	{
		if (yp < -3)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 && yp <= 6)
		{
			return SCALE_MID_TOP;
		}
		else if (yp >= h - 3 && yp <= h + 3)
		{
			return SCALE_MID_BOTTOM;
		}
		else if (yp > h + 3)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp >= w - 3 && xp <= w + 3)
	{
		if (yp < -3)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 && yp <= 6)
		{
			return SCALE_RIGHT_TOP;
		}
		else if (yp >= h / 2 - 3 && yp <= h / 2 + 3)
		{
			return SCALE_RIGHT_MID;
		}
		else if (yp >= h - 3 && yp <= h + 3)
		{
			return SCALE_RIGHT_BOTTOM;
		}
		else if (yp > h + 3)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp > w + 3)
	{
		return OUTSIDE;
	}
	else
	{
		return TRANSLATE;
	}
}

QRectF ScreenGrabRect::boundingRect() const
{
	return _base::boundingRect();
}

bool ScreenGrabRect::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
	if (event->type() == QEvent::GraphicsSceneHoverEnter || event->type() == QEvent::GraphicsSceneHoverMove)
	{
		QGraphicsSceneHoverEvent* e = static_cast<QGraphicsSceneHoverEvent*>(event);
		MOUSE_TRANSFORM mouse_type = getMouseEventType(e->pos());
		setCursor(m_cursor_mapper[mouse_type]);
	}
	else if (event->type() == QEvent::GraphicsSceneHoverLeave)
	{
		this->setCursor(Qt::ArrowCursor);
	}
	return _base::sceneEventFilter(watched, event);
}

void ScreenGrabRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QStyleOptionGraphicsItem myOption(*option);
	myOption.state &= ~QStyle::State_Selected;
	_base::paint(painter, &myOption, widget);
}


/////////////////////////////////////////////////
ScreenShotWidget::ScreenShotWidget(QWidget* parent)
	: _base(parent)
{
	m_scene = new QGraphicsScene;
	m_background = new QGraphicsPixmapItem;
	setScene(m_scene);
	m_scene->addItem(m_background);
	setFrameShape(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	grab();
}

ScreenShotWidget::~ScreenShotWidget()
{
}

void ScreenShotWidget::grab()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	if (const QWindow* window = windowHandle())
		screen = window->screen();
	if (!screen)
		return;

	m_originalShot = screen->grabWindow(0);

	QPixmap backgroundShot(m_originalShot.size());
	int alpha = 125;
	QPainter p(&backgroundShot);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawPixmap(0, 0, m_originalShot);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(backgroundShot.rect(), QColor(0, 0, 0, alpha));
	m_background->setPixmap(backgroundShot);

	QPoint topLeft(500, 250);
	QPoint bottomRight(1000, 550);
	QRectF rc(topLeft, bottomRight);
	m_grabber = new ScreenGrabRect(m_originalShot, rc, m_background);
}