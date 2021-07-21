#ifndef __MINDNODE_BUTTON_H__
#define __MINDNODE_BUTTON_H__

#include <QtWidgets/qgraphicsitem.h>

class MindNode;

class MindNodeButton : public QGraphicsObject
{
	Q_OBJECT
public:
	MindNodeButton(MindNode* parent = nullptr);
	QRectF boundingRect() const override;
	QPainterPath shape() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

	static const float m_radius;

protected:
	bool sceneEvent(QEvent* event) override;

signals:
	void toggled();

private:
	MindNode* m_pNode;
};

#endif
