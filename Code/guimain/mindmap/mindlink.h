#ifndef __MIND_LINK_H__
#define __MIND_LINK_H__

#include <QGraphicsLineItem>

class MindNode;
class MindNode2;

class MindLink : public QGraphicsLineItem
{
public:
	MindLink(MindNode* fromNode, MindNode* toNode);
	~MindLink();

	MindNode* fromNode() const;
	MindNode* toNode() const;

	void setColor(const QColor& color);
	QColor color() const;

	void trackNodes();

private:
	MindNode* myFromNode;
	MindNode* myToNode;
};

class MindLink2 : public QGraphicsPathItem
{
	typedef QGraphicsPathItem _base;
public:
	MindLink2(QGraphicsPathItem* parent = nullptr);
	MindLink2(const QPainterPath& path, QGraphicsItem* parent = nullptr);

	void setUpNodes(MindNode2* parent, MindNode2* child);
	MindNode2* parentNode() const;
	MindNode2* childNode() const;

	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	bool contains(const QPointF& point) const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

	bool isObscuredBy(const QGraphicsItem* item) const override;
	QPainterPath opaqueArea() const override;

protected:

private:
	MindNode2* m_pParent;
	MindNode2* m_pChild;
};

#endif
