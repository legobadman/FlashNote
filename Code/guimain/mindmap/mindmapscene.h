#ifndef __MINDMAP_SCENE_H__
#define __MINDMAP_SCENE_H__

#include <QtWidgets/QGraphicsScene>
#include "mindnode.h"
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
	MindTextNode* newProgressNode(MindTextNode* pRoot, const QString& text, float progress);
	void onCreateChildNode(MindTextNode* pRoot);
	void onCreateSlibingNode(MindTextNode* pNode);

private:
	void onRedrawItems();
	void setupNode(MindTextNode* node);
	QRectF arrangeItemPosition(QPoint rootLT, MindTextNode* pItem);
	MindTextNode* _initExample();
	MindTextNode* _initFromFile();
	MindTextNode* parseXML(const std::wstring& content);
	XML_NODE* _export(MindTextNode* root, xml_document<WCHAR>& doc);
	MindTextNode* _parse(xml_node<WCHAR>* root, int level);

private:
	MindTextNode* m_pRoot;
	QList<QGraphicsPathItem*> m_pathItems;
	bool m_bSchedule;
};

#endif
