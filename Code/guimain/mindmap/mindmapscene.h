#ifndef __MINDMAP_SCENE_H__
#define __MINDMAP_SCENE_H__

#include <QtWidgets/QGraphicsScene>
#include "mindnode.h"
#include "mindprogressnode.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<> XML_NODE;

class MindMapScene : public QGraphicsScene
{
	Q_OBJECT
public:
	MindMapScene(QObject* parent = nullptr);
	~MindMapScene();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();
	MindNode* root() const { return m_pRoot; }

signals:
	void itemContentChanged(bool bEditChanged);

private slots:
	void onNodeContentsChanged();
	void onNodeStateChanged();
	void onNodeCreated(MindNode* pNode);
	void onNodeDeleted(MindNode* pNode);
	void onNodeDragged(MindNode* pNode);
	void onNodeDragging(MindNode* pNode);

private:
	void onRedrawItems(bool bDrawHolder = false);
	void arrangeAllItems(bool bDrawHolder);
	MindNode* parseXML(const std::string& content);
	XML_NODE* _export(MindNode* root, xml_document<>& doc);
	MindNode* _parse(MindNode* parent, xml_node<>* root, int level);

private:
	MindNode* m_pRoot;
	MindNode* m_pHolder;
	QList<QGraphicsPathItem*> m_pathItems;
	QSet<MindNode*> m_items;
	bool m_bSchedule;
};

#endif
