#ifndef __MIND_NODE_H__
#define __MIND_NODE_H__

#include <QGraphicsItem>

class MindLink;

class MindNode : public QGraphicsItem
{
	typedef QGraphicsItem _base;
	Q_DECLARE_TR_FUNCTIONS(MindNode)
public:
	MindNode(const QString& text);
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
	int pointSize(int level) const;

	QSet<MindLink*> myLinks;
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor myOutlineColor;
	int m_level;
};

class MindTextNode : public QGraphicsTextItem
{
	Q_DECLARE_TR_FUNCTIONS(MindTextNode)
public:
	MindTextNode(const QString& text);
	~MindTextNode();
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	const QList<MindTextNode*>& children() const { return m_children; }
	void insert(int i, MindTextNode* pNode) { m_children.insert(i, pNode); }

protected:
	bool sceneEvent(QEvent* event) override;

private:
	void init();
	void initDocFormat(const QString& text);
	int pointSize(int level) const;
	void udpateBorderFormat(const QStyleOptionGraphicsItem* option);

private:
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor myOutlineColor;
	int m_level;
	int m_borderWidth;
	int m_cornerRadius;
	bool m_bHovered;
	QList<MindTextNode*> m_children;
};

#endif
