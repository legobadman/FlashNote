#include "stdafx.h"
#include "mindnode.h"
#include "mindlink.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QLineEdit>


MindNode::MindNode()
{
	myTextColor = Qt::darkGreen;
	myOutlineColor = Qt::darkBlue;
	myBackgroundColor = Qt::white;

	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable);
}

MindNode::~MindNode()
{
	foreach(MindLink * link, myLinks)
		delete link;
}

void MindNode::setText(const QString& text)
{
	prepareGeometryChange();
	myText = text;
	update();
}

QString MindNode::text() const
{
	return myText;
}

void MindNode::setTextColor(const QColor& color)
{
	myTextColor = color;
	update();
}

QColor MindNode::textColor() const
{
	return myTextColor;
}

void MindNode::setOutlineColor(const QColor& color)
{
	myOutlineColor = color;
	update();
}

QColor MindNode::outlineColor() const
{
	return myOutlineColor;
}

void MindNode::setBackgroundColor(const QColor& color)
{
	myBackgroundColor = color;
}

QColor MindNode::backgroundColor() const
{
	return myBackgroundColor;
}

void MindNode::addLink(MindLink* link)
{
	myLinks.insert(link);
}

void MindNode::removeLink(MindLink* link)
{
	myLinks.remove(link);
}

QRectF MindNode::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, Margin, Margin);
}

QPainterPath MindNode::shape() const
{
	QRectF rect = outlineRect();
	QPainterPath path;
	path.addRoundRect(rect, roundness(rect.width()), roundness(rect.height()));
	return path;
}

void MindNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(myOutlineColor);
	if (option->state & QStyle::State_Selected) {
		pen.setStyle(Qt::DotLine);
		pen.setWidth(2);
	}
	painter->setPen(pen);
	painter->setBrush(myBackgroundColor);

	QRectF rect = outlineRect();
	painter->drawRoundRect(rect, roundness(rect.width()), roundness(rect.height()));

	painter->setPen(myTextColor);
	painter->drawText(rect, Qt::AlignCenter, myText);
}

void MindNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QString text = QInputDialog::getText(event->widget(),
					tr("Edit Text"), tr("Enter new text:"),
					QLineEdit::Normal, myText);
	if (!text.isEmpty())
		setText(text);
}

QVariant MindNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		foreach(MindLink * link, myLinks)
			link->trackNodes();
	}
	return QGraphicsItem::itemChange(change, value);
}

QRectF MindNode::outlineRect() const
{
	const int Padding = 8;
	QFontMetricsF metrics = QApplication::fontMetrics();
	QRectF rect = metrics.boundingRect(myText);
	rect.adjust(-Padding, -Padding, Padding, Padding);
	rect.translate(-rect.center());
	return rect;
}

int MindNode::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}
