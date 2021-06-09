#ifndef __MIND_NODE_H__
#define __MIND_NODE_H__

#include <QGraphicsItem>

class MindLink;

class MindNode : public QGraphicsItem
{
	Q_DECLARE_TR_FUNCTIONS(MindNode)
public:
	MindNode();
	~MindNode();
	void setText(const QString& text);
	QString text() const;
	void setTextColor(const QColor& color);
	QColor textColor() const;
	void setOutlineColor(const QColor& color);
	QColor outlineColor() const;
	void setBackgroundColor(const QColor& color);
	QColor backgroundColor() const;

	void addLink(MindLink* link);
	void removeLink(MindLink* link);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
	QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
	QRectF outlineRect() const;
	int roundness(double size) const;

	QSet<MindLink*> myLinks;
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor myOutlineColor;
};

#endif
