#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include "mindmapview.h"
#include "mindnode.h"
#include "mindmapscene.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();

signals:
	void itemContentChanged(bool);

private:
	QPointer<MindMapScene> m_scene;
	QPointer<MindMapView> m_view;
	MindNode* m_pRoot;
};


#endif
