#include "stdafx.h"
#include "mindmapwidget.h"
#include <QAction>


MindMapWidget::MindMapWidget(QWidget* parent /* = NULL */)
	: QWidget(parent)
	, m_scene(NULL)
	, m_view(NULL)
	, seqNumber(0)
{
	//m_scene = new QGraphicsScene(0, 0, 400, 600);
	m_scene = new QGraphicsScene;
	//m_scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
	m_view = new MindMapView;
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
	//m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//m_view->setAlignment(Qt::AlignCenter);
	//m_view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->addWidget(m_view);
	setLayout(pMainLayout);

	MindTextNode* pRoot = newNode(NULL, u8"˼ά��ͼ�ʼ�");
	MindTextNode* pChild = newNode(pRoot, u8"�����ڵ�");
	MindTextNode* pChild2 = newNode(pRoot, u8"�����ڵ�");
	MindTextNode* pChild3 = newNode(pRoot, u8"�����ڵ�");
	MindTextNode* pChild4 = newNode(pRoot, u8"�����ڵ�");

	pRoot->insert(0, pChild);
	pRoot->insert(0, pChild2);
	pRoot->insert(0, pChild3);
	pRoot->insert(0, pChild4);

	pChild->insert(0, newNode(pChild, u8"�����ӽڵ�"));
	pChild->insert(0, newNode(pChild, u8"�����ӽڵ�"));

	pChild2->insert(0, newNode(pChild2, u8"�����ӽڵ�"));
	pChild2->insert(0, newNode(pChild2, u8"�����ӽڵ�"));

	//pChild3->insert(0, newNode(pChild3, u8"�����ӽڵ�"));
	//pChild3->insert(0, newNode(pChild3, u8"�����ӽڵ�"));

	pChild4->insert(0, newNode(pChild4, u8"�����ӽڵ�"));
	pChild4->insert(0, newNode(pChild4, u8"�����ӽڵ�"));

	arrangeItemPosition(QPoint(0, 0), pRoot);
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
	MindNode* node = new MindNode(u8"˼ά��ͼ�ʼ�");
	setupNode(node);
}

void MindMapWidget::addNode(MindTextNode* pParent, MindTextNode* pChild)
{
	pParent->insert(0, pChild);
}

MindTextNode* MindMapWidget::newNode(MindTextNode* pRoot, const QString& text)
{
	MindTextNode* node = new MindTextNode(text, pRoot);
	setupNode(node);
	return node;
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

QPoint leftConnectPoint(QRectF boundingRect)
{
	return QPoint(boundingRect.left(), boundingRect.top() + boundingRect.height() / 2);
}


QRectF MindMapWidget::arrangeItemPosition(QPoint rootLT, MindTextNode* pRoot)
{
	//���費�ܻ���
	static int HMargin = 60;
	static int VMargin = 28;
	
	const QList<MindTextNode*>& children = pRoot->children();
	int n = children.size();

	//Ŀǰ��ʱ�������Ӽ��ĸ߶ȱ仯��
	QRectF rootRect = pRoot->boundingRect(); //Ŀǰ��ûָ��λ�ã�ֻ��ȡ��ߡ�
	qreal itemH = pRoot->boundingRect().height();
	qreal itemW = pRoot->boundingRect().width();

	float nn = n;
	
	QPoint childLT;
	if (n > 0)
	{
		//��children���ڵ��γɵ�boundingRect��
		qreal H = 0;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			H += (*it)->boundingRect().height();
			H += VMargin;
		}
		H -= VMargin;
		
		//int childTop = qMax<int>(rootLT.y() + VMargin, (rootLT.y() + itemH / 2.0) - H / 2.0);
		int childTop = rootLT.y();
		childLT = QPoint(rootLT.x() + itemW + HMargin, childTop);
	}

	QRectF boundingRect;
	if (!children.empty())
	{
		QVector<QPoint> vecConnectors;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			QRectF rr = arrangeItemPosition(childLT, *it);
			vecConnectors.push_back(QPoint(rr.left(), rr.top() + rr.height() / 2));
			boundingRect = boundingRect | rr;
			boundingRect.adjust(0, 0, 0, VMargin);
			childLT.setY(boundingRect.bottom());
		}
		boundingRect.adjust(0, 0, 0, -VMargin);

		//��������γɵ�����rExtend��Ȼ��������չ���õ���������boundingrect
		boundingRect.adjust(-HMargin - itemW, 0, 0, 0);
		//ȡboundRect�����м�
		QPointF basePos(boundingRect.left(), (boundingRect.top() + boundingRect.bottom() - itemH) / 2.0);
		pRoot->setPos(basePos);

		QRectF rRoot = pRoot->boundingRect();
		QPointF rootConnector = basePos + QPointF(itemW, itemH / 2.0F);
		//�����˵�ǰ�ڵ㼰�ӽڵ��λ�ã����Ի����ˡ�
		for (int i = 0; i < vecConnectors.size(); i++)
		{
			QPoint childConnector = vecConnectors[i];
			QPainterPath path;
			path.moveTo(rootConnector);

			QPointF c1((rootConnector.x() + childConnector.x()) / 2.0, rootConnector.y());
			QPointF c2((rootConnector.x() + childConnector.x()) / 2.0, childConnector.y());

			path.cubicTo(c1, c2, childConnector);
			QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
			pathItem->setPen(QPen(QColor(0, 181, 72), 3));
			m_scene->addItem(pathItem);
		}
		return boundingRect;
	}
	else
	{
		pRoot->setPos(rootLT);
		boundingRect = QRectF(rootLT.x(), rootLT.y(), pRoot->boundingRect().width(), pRoot->boundingRect().height());
	}
	return boundingRect;
}