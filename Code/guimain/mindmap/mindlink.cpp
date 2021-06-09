#include "stdafx.h"
#include "mindlink.h"
#include "mindnode.h"


MindLink::MindLink(MindNode* fromNode, MindNode* toNode)
	: myFromNode(fromNode)
	, myToNode(toNode)
{
	myFromNode->addLink(this);
	myToNode->addLink(this);

	setFlags(QGraphicsItem::ItemIsSelectable);
	setZValue(-1);

	setColor(Qt::darkRed);
	trackNodes();
}

MindLink::~MindLink()
{
	myFromNode->removeLink(this);
	myToNode->removeLink(this);
}

void MindLink::setColor(const QColor& color)
{
	setPen(QPen(color, 1.0));
}

MindNode* MindLink::fromNode() const
{
	return myFromNode;
}

MindNode* MindLink::toNode() const
{
	return myToNode;
}

QColor MindLink::color() const
{
	return pen().color();
}

void MindLink::trackNodes()
{
	setLine(QLineF(myFromNode->pos(), myToNode->pos()));
}
