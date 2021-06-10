#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include "mindmapview.h"
#include "mindnode.h"
#include "mindlink.h"


class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();

private slots:
	void addNode();
	void addNode2();
	void addLink();
	void bringToFront();
	void sendToBack();

private:
	typedef QPair<MindNode*, MindNode*> NodePair;

	void createActions();
	void setZValue(int z);
	void setupNode(QGraphicsItem* node);
	MindNode* selectedNode() const;
	MindLink* selectedLink() const;
	NodePair selectedNodePair() const;

private:
	QGraphicsScene* m_scene;
	MindMapView* m_view;

	QAction* m_pAddNode;
	QAction* m_pAddLink;

	int minZ;
	int maxZ;
	int seqNumber;
};


#endif
