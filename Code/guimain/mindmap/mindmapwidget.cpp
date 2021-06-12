#include "stdafx.h"
#include "mindmapwidget.h"
#include <QAction>


MindMapWidget::MindMapWidget(QWidget* parent /* = NULL */)
	: QWidget(parent)
	, m_scene(NULL)
	, m_view(NULL)
	, seqNumber(0)
{
	m_scene = new QGraphicsScene(0, 0, 600, 500);
	m_view = new MindMapView;
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addWidget(m_view);
	setLayout(pMainLayout);

	addNode2();
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::createActions()
{
	m_pAddNode = new QAction(tr("Add &Node"), this);
	connect(m_pAddNode, SIGNAL(triggered()), this, SLOT(addNode()));

	m_pAddLink = new QAction(tr("Add &Link"), this);
	connect(m_pAddLink, SIGNAL(triggered()), this, SLOT(addLink()));
}

void MindMapWidget::setupNode(QGraphicsItem* node)
{
	node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
		80 + (50 * ((seqNumber / 5) % 7))));
	m_scene->addItem(node);
	++seqNumber;

	m_scene->clearSelection();
	node->setSelected(true);
	bringToFront();
}

void MindMapWidget::bringToFront()
{
	++maxZ;
	setZValue(maxZ);
}

void MindMapWidget::sendToBack()
{
	--minZ;
	setZValue(minZ);
}

void MindMapWidget::setZValue(int z)
{
	MindNode* node = selectedNode();
	if (node)
	{
		node->setZValue(z);
	}
}

MindNode* MindMapWidget::selectedNode() const
{
	QList<QGraphicsItem*> items = m_scene->selectedItems();
	if (items.count() == 1) {
		return dynamic_cast<MindNode*>(items.first());
	}
	else {
		return 0;
	}
}

void MindMapWidget::addNode()
{
	MindNode* node = new MindNode(u8"思维导图笔记");
	setupNode(node);
}

void MindMapWidget::addNode2()
{
	MindTextNode* node = new MindTextNode(u8"思维导图笔记");
	setupNode(node);
}

void MindMapWidget::addLink()
{
	NodePair nodes = selectedNodePair();
	if (nodes == NodePair())
		return;

	MindLink* link = new MindLink(nodes.first, nodes.second);
	m_scene->addItem(link);
}

MindMapWidget::NodePair MindMapWidget::selectedNodePair() const
{
	QList<QGraphicsItem*> items = m_scene->selectedItems();
	if (items.count() == 2)
	{
		MindNode* first = dynamic_cast<MindNode*>(items.first());
		MindNode* second = dynamic_cast<MindNode*>(items.last());
		if (first && second)
		{
			return NodePair(first, second);
		}
	}
	return NodePair();
}