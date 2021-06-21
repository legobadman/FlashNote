#ifndef __MINDMAP_SCENE_H__
#define __MINDMAP_SCENE_H__

#include <QtWidgets/QGraphicsScene>
#include "mindnode.h"
#include "mindprogressnode.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<WCHAR> XML_NODE;

class MindMapScene : public QGraphicsScene
{
	Q_OBJECT
public:
	MindMapScene(QObject* parent = nullptr);
	~MindMapScene();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();

signals:
	void itemContentChanged();

private slots:
	MindProgressNode* newProgressNode(MindProgressNode* pRoot, const QString& text, float progress);
	void onCreateChildNode(MindNode* pRoot, bool);
	void onCreateSlibingNode(MindNode* pNode);
	void onDeleteNode(MindNode* pNode);
	void onItemTextChanged();

private:
	void onRedrawItems();
	void setupNode(MindNode* node);
	void unsetupNode(MindNode* node);
	void arrangeAllItems();
	QRectF arrangeItemPosition(QPoint rootLT, MindNode* pItem, bool toRight);
	void adjustRightSidePos(MindNode* pRoot, int xoffset, int yoffset);
	MindNode* _initExample();
	MindNode* _initFromFile();
	MindNode* parseXML(const std::wstring& content);
	XML_NODE* _export(MindNode* root, xml_document<WCHAR>& doc);
	MindNode* _parse(xml_node<WCHAR>* root, int level);

private:
	MindNode* m_pRoot;
	QList<QGraphicsPathItem*> m_rightPathItems;	//存起来用于调整位置
	QList<QGraphicsPathItem*> m_pathItems;
	bool m_bSchedule;
};

#endif
