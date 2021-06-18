#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include "mindmapview.h"
#include "mindnode.h"
#include "mindmapscene.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<WCHAR> XML_NODE;

class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();

signals:
	void itemContentChanged();

private:
	MindMapScene* m_scene;
	MindMapView* m_view;
	MindTextNode* m_pRoot;

	QAction* m_pAddNode;
	QAction* m_pAddLink;
};


#endif
