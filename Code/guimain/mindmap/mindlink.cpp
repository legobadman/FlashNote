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


//////////////////////////////////////////////////
MindLink2::MindLink2(QGraphicsPathItem* parent)
	: _base(parent)
	, m_pParent(NULL)
	, m_pChild(NULL)
{

}

MindLink2::MindLink2(const QPainterPath& path, QGraphicsItem* parent)
	: _base(path, parent)
	, m_pParent(NULL)
	, m_pChild(NULL)
{

}

void MindLink2::setUpNodes(MindNode2* parent, MindNode2* child)
{
	m_pParent = parent;
	m_pChild = child;
}

MindNode2* MindLink2::parentNode() const
{
	return m_pParent;
}

MindNode2* MindLink2::childNode() const
{
	return m_pChild;
}

QRectF MindLink2::boundingRect() const
{
	QRectF r = _base::boundingRect();
	return r;
}

QPainterPath MindLink2::shape() const
{
	return _base::shape();
}

bool MindLink2::contains(const QPointF& point) const
{
	return _base::contains(point);
}

void MindLink2::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	_base::paint(painter, option, widget);
}

bool MindLink2::isObscuredBy(const QGraphicsItem* item) const
{
	return _base::isObscuredBy(item);
}

QPainterPath MindLink2::opaqueArea() const
{
	return _base::opaqueArea();
}