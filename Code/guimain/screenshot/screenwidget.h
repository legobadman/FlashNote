#ifndef __SCREEN_WIDGET_H__
#define __SCREEN_WIDGET_H__

#include <QGraphicsView>
#include <QGraphicsItem>

class ScreenGrabRect : public QGraphicsPixmapItem
{
	typedef QGraphicsPixmapItem _base;
public:
	ScreenGrabRect(const QPixmap& original, const QRectF& rect, QGraphicsItem* parent = nullptr);
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;

private:
	QPixmap m_originalShot;
	QRectF m_rect;
	QGraphicsRectItem* m_borderItem;
	QGraphicsRectItem* m_dragHolderItem;
	const int m_borderWidth = 2;
};


class ScreenShotWidget : public QGraphicsView
{
	Q_OBJECT
	typedef QGraphicsView _base;
public:
	ScreenShotWidget(QWidget* parent = nullptr);
	~ScreenShotWidget();
	void grab();

private:
	QPixmap m_originalShot;
	QPixmap m_grabImage;
	QPointer<QGraphicsScene> m_scene;
	QGraphicsPixmapItem* m_background;
	QGraphicsPixmapItem* m_grabRegion;
	ScreenGrabRect* m_grabber;
};


#endif