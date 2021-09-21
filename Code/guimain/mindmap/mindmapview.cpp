#include "stdafx.h"
#include "mindmapview.h"


MindMapView::MindMapView(QWidget* parent)
	: QGraphicsView(parent)
	, m_dragMove(false)
{
}

void MindMapView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}

void MindMapView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
}

void MindMapView::mouseReleaseEvent(QMouseEvent* event)
{
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