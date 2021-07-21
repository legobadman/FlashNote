#include "stdafx.h"
#include "mindnode.h"
#include "mindnodebutton.h"

const float MindNodeButton::m_radius = 13.0f;

MindNodeButton::MindNodeButton(MindNode* parent)
	: QGraphicsObject(parent)
	, m_pNode(parent)
{
	Q_ASSERT(m_pNode);
	setAcceptHoverEvents(true);
	setFlags(ItemIsMovable | ItemSendsGeometryChanges);
}

QRectF MindNodeButton::boundingRect() const
{
	return QRectF(0, 0, 2 * m_radius, 2 * m_radius);
}

QPainterPath MindNodeButton::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}

bool MindNodeButton::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		//按钮部分已经在矩形以外了，还需要一些判断。
		show();
		update();
		break;
	case QEvent::GraphicsSceneHoverLeave:
		hide();
		update();
		break;
	case QEvent::GraphicsSceneMouseRelease:
		emit toggled();
		update();
		break;
	}
	return QGraphicsItem::sceneEvent(event);
}

void MindNodeButton::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	static const int line_left_margin = 8;
	int x1 = line_left_margin;
	int x2 = 2 * m_radius - line_left_margin;
	int y1 = line_left_margin;
	int y2 = 2 * m_radius - line_left_margin;
	painter->setRenderHint(QPainter::Antialiasing);

	painter->setPen(QColor(242, 242, 242));
	painter->setBrush(QColor(255, 255, 255));
	painter->drawEllipse(boundingRect());

	QPen pen(QColor(242, 242, 242), 2);
	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);
	painter->drawEllipse(boundingRect());

	if (m_pNode->isExpanded(this))
	{
		pen = QPen(QColor(0, 0, 0), 1);
		painter->setPen(pen);
		painter->drawLine(x1, m_radius, x2, m_radius);
	}
	else
	{
		pen = QPen(QColor(0, 0, 0), 2);
		painter->setPen(pen);
		painter->drawLine(x1, m_radius, x2, m_radius);
		painter->drawLine(m_radius, y1, m_radius, y2);
	}

	painter->restore();
}