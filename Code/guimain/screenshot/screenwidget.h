#ifndef __SCREEN_WIDGET_H__
#define __SCREEN_WIDGET_H__

#include <QGraphicsView>
#include <QGraphicsItem>
#include "screentoolbar.h"


class GrabDraggingRect : public QGraphicsRectItem
{
public:
	GrabDraggingRect(const QRectF& rect, QGraphicsItem* parent = nullptr);

protected:
	QRectF boundingRect() const override;
};

class ScreenGrabRect : public QObject
					, public QGraphicsPixmapItem
{
	Q_OBJECT

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
	QRectF getCurrentShot(QPixmap& retImage);

signals:
	void grabStarted();
	void grabFinish();

protected:
	bool sceneEventFilter(QGraphicsItem* watched, QEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	MOUSE_TRANSFORM getMouseEventType(QPointF pressPoint);

private:
	QPixmap m_originalShot;
	QPixmap m_currentShot;
	QRectF m_rect;
	GrabDraggingRect* m_borderItem;
	QVector<QGraphicsRectItem*> m_dragPoints;
	map<MOUSE_TRANSFORM, Qt::CursorShape> m_cursor_mapper;
	MOUSE_TRANSFORM m_transform;
	const int m_borderWidth = 1;
	bool m_choosingRect;	//一开始选择rect，在点击鼠标以后，就设为false

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
	QRectF getGrabImage(QPixmap& retImage);

signals:
	void grabFinish();

protected:
	void keyPressEvent(QKeyEvent* event);
	void paintEvent(QPaintEvent* event);

private slots:
	void onGrabFinish();
	void onGrabStarted();
	void onShotFinished();
	void onSaveShot();

private:
	QPixmap m_originalShot;
	QPixmap m_grabImage;
	QPointer<QGraphicsScene> m_scene;
	QGraphicsPixmapItem* m_background;
	ScreenGrabRect* m_grabber;
	ScreenToolBar* m_toolbar;
};


#endif