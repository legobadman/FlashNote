#include "stdafx.h"
#include "mindmapwidget.h"
#include "rapidxml_print.hpp"
#include <QAction>


MindMapWidget::MindMapWidget(QWidget* parent)
	: QWidget(parent)
	, m_scene(NULL)
	, m_view(NULL)
	, m_pRoot(NULL)
{
	m_scene = new MindMapScene;
	m_view = new MindMapView;
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addWidget(m_view);
	setLayout(pMainLayout);

	connect(m_scene, SIGNAL(itemContentChanged()), this, SIGNAL(itemContentChanged()));
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::initContent(QString content)
{
	m_scene->initContent(content);
}

QString MindMapWidget::mindmapXML()
{
	return m_scene->mindmapXML();
}
