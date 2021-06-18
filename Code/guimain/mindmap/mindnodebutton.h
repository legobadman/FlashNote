#ifndef __MINDNODE_BUTTON_H__
#define __MINDNODE_BUTTON_H__

#include <QtWidgets/qgraphicsitem.h>

class MindNodeButton : public QGraphicsItem
{
public:
	MindNodeButton(float r, QGraphicsItem* parent = nullptr);
	QRectF boundingRect() const override;
	QPainterPath shape() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
	float m_radius;
};

#endif
