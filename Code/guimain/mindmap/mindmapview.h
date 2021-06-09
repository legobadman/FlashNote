#ifndef __MINDMAP_VIEW_H__
#define __MINDMAP_VIEW_H__

#include <QGraphicsView>

class MindMapView : public QGraphicsView
{
	Q_OBJECT
public:
	MindMapView(QWidget* parent = NULL);
};

#endif
