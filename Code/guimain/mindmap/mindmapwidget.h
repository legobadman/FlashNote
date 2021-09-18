#ifndef __MINDMAP_WIDGET_H__
#define __MINDMAP_WIDGET_H__

#include <QtWidgets/QGraphicsScene>
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

signals:
	void itemContentChanged(bool);

protected:
    bool eventFilter(QObject* watched, QEvent* event);

private slots:
	void redo();
	void undo();
	void undoRedoEnable(bool, bool);

private:
	QPointer<MindMapScene> m_scene;
	QPointer<MindMapView> m_view;
	MindNode* m_pRoot;
	NLabelButton* m_undoBtn;
	NLabelButton* m_redoBtn;
    QShortcut* m_undo;
    QShortcut* m_redo;
};


#endif
