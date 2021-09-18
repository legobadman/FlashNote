#include "stdafx.h"
#include "mindmapview.h"


MindMapView::MindMapView(QWidget* parent)
	: QGraphicsView(parent)
	, m_dragMove(false)
{
}

void MindMapView::mousePressEvent(QMouseEvent* event)
{
	if (QApplication::keyboardModifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton)
	{
		startPos = event->pos();
		m_dragMove = true;
		setCursor(Qt::OpenHandCursor);
	}
    QGraphicsView::mousePressEvent(event);
}

void MindMapView::mouseMoveEvent(QMouseEvent* event)
{
	if (m_dragMove)
	{
		QPointF delta = startPos - event->pos();
		QTransform transform = this->transform();
		qreal deltaX = delta.x() / transform.m11();
		qreal deltaY = delta.y() / transform.m22();
		setSceneRect(sceneRect().translated(deltaX, deltaY));
		startPos = event->pos();
		setCursor(Qt::OpenHandCursor);
	}
	else
	{
		QGraphicsView::mouseMoveEvent(event);
	}
}

void MindMapView::mouseReleaseEvent(QMouseEvent* event)
{
	m_dragMove = false;
	setCursor(Qt::ArrowCursor);
	QGraphicsView::mouseReleaseEvent(event);
}

void MindMapView::keyPressEvent(QKeyEvent* event)
{
	QGraphicsView::keyPressEvent(event);
}

void MindMapView::keyReleaseEvent(QKeyEvent* event)
{
	QGraphicsView::keyReleaseEvent(event);
}