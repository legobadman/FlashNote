#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include "mindmapview.h"
#include "mindnode.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<WCHAR> XML_NODE;

class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();
	void initContent(QString content);
	QString mindmapXML();

signals:
	void itemContentChanged();

private slots:
	MindTextNode* newNode(MindTextNode* pRoot, const QString& text);
	MindTextNode* newProgressNode(MindTextNode* pRoot, const QString& text, float progress);
	void addNode(MindTextNode* pParent, MindTextNode* pChild);
	void onItemContentChanged();

private:
	void createActions();
	void setupNode(MindTextNode* node);
	QRectF arrangeItemPosition(QPoint rootLT, MindTextNode* pItem);
	MindTextNode* _initExample();
	MindTextNode* _initFromFile();
	MindTextNode* parseXML(const std::wstring& content);
	XML_NODE* _export(MindTextNode* root, xml_document<WCHAR>& doc);
	MindTextNode* _parse(xml_node<WCHAR>* root, int level);

private:
	QGraphicsScene* m_scene;
	MindMapView* m_view;
	MindTextNode* m_pRoot;

	QAction* m_pAddNode;
	QAction* m_pAddLink;
};


#endif
