#include "stdafx.h"
#include "mindmapwidget.h"
#include "rapidxml_print.hpp"
#include <QAction>
#include <QShortcut>


MindMapWidget::MindMapWidget(QWidget* parent)
	: QWidget(parent)
	, m_scene(new MindMapScene)
	, m_view(new MindMapView)
	, m_pRoot(NULL)
{
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_view->setFrameShape(QFrame::HLine);
	m_view->setFrameShadow(QFrame::Sunken);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addWidget(m_view);
	setLayout(pMainLayout);

	connect(m_scene, SIGNAL(itemContentChanged(bool)), this, SIGNAL(itemContentChanged(bool)));

    m_undo = new QShortcut(this);
	m_undo->setKey(QKeySequence("Ctrl+Z"));
    m_redo = new QShortcut(this);
	m_redo->setKey(QKeySequence("Ctrl+Y"));

	connect(m_undo, SIGNAL(activated()), this, SLOT(undo()));
	connect(m_redo, SIGNAL(activated()), this, SLOT(redo()));
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::initContent(QString content, bool bSchedule)
{
	m_scene->initContent(content, bSchedule);
}

QString MindMapWidget::mindmapXML()
{
	return m_scene->mindmapXML();
}

void MindMapWidget::redo()
{
	m_scene->redo();
}

void MindMapWidget::undo()
{
	m_scene->undo();
}
