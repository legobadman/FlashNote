#ifndef __MIND_LINK_H__
#define __MIND_LINK_H__

#include <QGraphicsLineItem>

class MindNode;

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

#endif
