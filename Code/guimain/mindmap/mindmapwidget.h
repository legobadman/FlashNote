#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include "mindmapview.h"
#include "mindnode.h"
#include "mindlink.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();

private slots:
	void addNode();
	MindTextNode* newNode(MindTextNode* pRoot, const QString& text);
	MindTextNode* newProgressNode(MindTextNode* pRoot, const QString& text, float progress);
	void addNode(MindTextNode* pParent, MindTextNode* pChild);
	void addLink();
	void bringToFront();
	void sendToBack();

private:
	typedef QPair<MindNode*, MindNode*> NodePair;

	void createActions();
	void setZValue(int z);
	void setupNode(MindTextNode* node);
	MindNode* selectedNode() const;
	MindLink* selectedLink() const;
	NodePair selectedNodePair() const;
	QRectF arrangeItemPosition(QPoint rootLT, MindTextNode* pItem);
	MindTextNode* _initExample();
	MindTextNode* parseXML(const std::wstring& content);
	MindTextNode* _parse(xml_node<WCHAR>* root, int level);

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
