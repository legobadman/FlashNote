#ifndef __MINDMAP_SCENE_H__
#define __MINDMAP_SCENE_H__

#include <QtWidgets/QGraphicsScene>
#include "mindnode.h"
#include "mindprogressnode.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<> XML_NODE;

struct BoundingNode;
typedef std::shared_ptr<BoundingNode> PBOUNDING_NODE;

struct BoundingNode
{
	bool bRight;	//根节点不受限制
	qreal loffset;	//用于计算左边节点的锚点。
	qreal itemH;	//最后归纳节点的高度。
	QRectF lrect;	//如果是根节点，算根节点及左边形成的矩形
	QRectF rrect;	//如果是根节点，算根节点及右边形成的矩形
	MindNode* pNode;
	std::vector<PBOUNDING_NODE> children;
	BoundingNode() : bRight(true), loffset(0), itemH(0) {}
	~BoundingNode() {
		//for (auto it = children.begin(); it != children.end(); it++)
			//(*it).reset();
		children.clear();
	}
	QRectF getRectF()
	{
		return bRight ? rrect : lrect;
	}
	QPointF itemPos(bool bToRight)
	{
		//因为根节点两边都有，所以要引入参数
		if (bToRight)
		{
			QPointF pos(rrect.left(), (rrect.top() + rrect.bottom()) / 2.0 - itemH / 2.0);
			return pos;
		}
		else
		{
			qreal x = lrect.right() + loffset;
			QPointF pos(x, (lrect.top() + lrect.bottom()) / 2.0 - itemH / 2.0);
			return pos;
		}
	}
};

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
	void itemContentChanged();

private slots:
	void onNodeContentsChanged();
	void onNodeCreated(MindNode* pNode);
	void onNodeDeleted(MindNode* pNode);

private:
	void onRedrawItems();
	void arrangeAllItems();
	void arrangeItemPosition(PBOUNDING_NODE pBoundingRoot);
	void adjustAllItemPos(MindNode* pRoot, int xoffset, int yoffset, bool bOnlyRightside);
	void appendPaths(MindNode* pRoot);
	MindNode* parseXML(const std::string& content);
	XML_NODE* _export(MindNode* root, xml_document<>& doc);
	MindNode* _parse(MindNode* parent, xml_node<>* root, int level);
	PBOUNDING_NODE construct(MindNode* pRoot, QPoint rootLTorRT, const bool toRight, PBOUNDING_NODE pTopRoot);

private:
	MindNode* m_pRoot;
	QList<QGraphicsPathItem*> m_pathItems;
	QSet<MindNode*> m_items;
	bool m_bSchedule;
};

#endif
