#ifndef __MINDMAP_VIEW_H__
#define __MINDMAP_VIEW_H__

#include <QGraphicsView>

class MindMapView : public QGraphicsView
{
	Q_OBJECT
public:
	MindMapView(QWidget* parent = NULL);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	QPointF startPos;
	bool m_dragMove;
};

#endif
