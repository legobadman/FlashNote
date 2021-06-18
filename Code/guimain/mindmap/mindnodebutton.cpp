#include "stdafx.h"
#include "mindnodebutton.h"


MindNodeButton::MindNodeButton(float r, QGraphicsItem* parent)
	: QGraphicsItem(parent)
	, m_radius(r)
{
}

QRectF MindNodeButton::boundingRect() const
{
	return QRectF(0, 0, 2 * m_radius, 2 * m_radius);
}

QPainterPath MindNodeButton::shape() const
{
	return QGraphicsItem::shape();
}

void MindNodeButton::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QColor(242, 242, 242));
	painter->setBrush(QColor(255, 255, 255));
	painter->drawEllipse(boundingRect());

	painter->setPen(QColor(0, 0, 0));
	painter->setBrush(QColor(0, 0, 0));
	painter->drawLine(9, m_radius, 9 + 14, m_radius);

	painter->restore();
}