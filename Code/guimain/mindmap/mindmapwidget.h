#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QShortcut>
#include "mindmapview.h"
#include "mindnode.h"
#include "mindmapscene.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class NLabelButton;

class MindMapWidget : public QWidget
{
	Q_OBJECT
public:
	MindMapWidget(QWidget* parent = NULL);
	~MindMapWidget();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();
	void gentle_zoom(qreal factor);
    void set_modifiers(Qt::KeyboardModifiers modifiers);

signals:
	void itemContentChanged(bool);
	void zoomed(qreal);

protected:
    bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void redo();
	void undo();
    void zoomIn();
    void zoomOut();
	void undoRedoEnable(bool, bool);
	void resetTransform();

private:
	QPointer<MindMapScene> m_scene;
	QPointer<MindMapView> m_view;
	Qt::KeyboardModifiers _modifiers;
	qreal m_factor;
	const double m_factor_step = 0.25;
	QPointF target_scene_pos, target_viewport_pos, m_startPos;
	MindNode* m_pRoot;
	NLabelButton* m_undoBtn;
	NLabelButton* m_redoBtn;
	NLabelButton* m_zoomout;
	NLabelButton* m_zoomin;
	QLabel* m_zoom_factor;
    QShortcut* m_undo;
    QShortcut* m_redo;
	bool m_dragMove;
};


#endif
