#ifndef __SCREEN_WIDGET_H__
#define __SCREEN_WIDGET_H__

#include <QGraphicsView>
#include <QGraphicsItem>

class GrabDraggingRect : public QGraphicsRectItem
{
public:
	GrabDraggingRect(const QRectF& rect, QGraphicsItem* parent = nullptr);

protected:
	QRectF boundingRect() const override;
};

class ScreenGrabRect : public QGraphicsPixmapItem
{
	typedef QGraphicsPixmapItem _base;

	enum MOUSE_TRANSFORM
	{
		MOUSE_DONOTHING,

		SCALE_LEFT_TOP,
		SCALE_LEFT_MID,
		SCALE_LEFT_BOTTOM,

		SCALE_MID_TOP,
		SCALE_MID_BOTTOM,

		SCALE_RIGHT_TOP,
		SCALE_RIGHT_MID,
		SCALE_RIGHT_BOTTOM,

		TRANSLATE,
		OUTSIDE,
	};

public:
	ScreenGrabRect(const QPixmap& original, const QRectF& rect, QGraphicsItem* parent = nullptr);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;

protected:
	bool sceneEventFilter(QGraphicsItem* watched, QEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	MOUSE_TRANSFORM getMouseEventType(QPointF pressPoint);

private:
	QPixmap m_originalShot;
	QRectF m_rect;
	QGraphicsRectItem* m_borderItem;
	QGraphicsRectItem* m_dragHolderItem;
	QVector<GrabDraggingRect*> m_dragPoints;
	map<MOUSE_TRANSFORM, Qt::CursorShape> m_cursor_mapper;
	MOUSE_TRANSFORM m_transform;
	const int m_borderWidth = 1;

	struct {
		qreal old_width;
		qreal old_height;

		QPointF fixed_point;
		qreal fixed_x;	//left
		qreal fixed_y;	//top
	} m_movescale_info;
};


class ScreenShotWidget : public QGraphicsView
{
	Q_OBJECT
	typedef QGraphicsView _base;
public:
	ScreenShotWidget(QWidget* parent = nullptr);
	~ScreenShotWidget();
	void grab();

protected:


private:
	QPixmap m_originalShot;
	QPixmap m_grabImage;
	QPointer<QGraphicsScene> m_scene;
	QGraphicsPixmapItem* m_background;
	QGraphicsPixmapItem* m_grabRegion;
	ScreenGrabRect* m_grabber;
};


#endif