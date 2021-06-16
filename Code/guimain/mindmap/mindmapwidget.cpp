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

	QFile fn("E:\\FlashNote\\Code\\io\\node.xml");
	if (!fn.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QByteArray htmlFile = fn.readAll();
	QString content = QString::fromUtf8(htmlFile);
	std::wstring wstr = content.toStdWString();
	MindTextNode* pRoot = parseXML(wstr);
	arrangeItemPosition(QPoint(0, 0), pRoot);
}

MindMapWidget::~MindMapWidget()
{
}

MindTextNode* MindMapWidget::_initExample()
{
	MindTextNode* pRoot = newProgressNode(NULL, u8"����ѧϰ�ƻ�", 0.1);

	MindTextNode* pChild = newProgressNode(pRoot, u8"�����ع�", 0.3);
	MindTextNode* pChild2 = newProgressNode(pRoot, u8"���߳�֪ʶ", 0.01);
	MindTextNode* pChild3 = newProgressNode(pRoot, u8"Windows����", 0.9);
	MindTextNode* pChild4 = newProgressNode(pRoot, u8"C++����֪ʶ", 0.5);

	pRoot->insert(0, pChild);
	pRoot->insert(0, pChild2);
	pRoot->insert(0, pChild3);
	pRoot->insert(0, pChild4);

	pChild->insert(0, newProgressNode(pChild, u8"bug�ܽ�", 0.4));
	pChild->insert(0, newProgressNode(pChild, u8"���������ܽ�", 0.2));

	pChild2->insert(0, newProgressNode(pChild2, u8"�ź���", 0));
	pChild2->insert(0, newProgressNode(pChild2, u8"mutable", 0));

	pChild4->insert(0, newProgressNode(pChild4, u8"COM���", 0.3));
	pChild4->insert(0, newProgressNode(pChild4, u8"�麯������", 0.6));
	pChild4->insert(0, newProgressNode(pChild4, u8"����ָ��", 0.2));

	return pRoot;
}

MindTextNode* MindMapWidget::parseXML(const std::wstring& content)
{
	xml_document<WCHAR> doc;
	doc.parse<0>((LPWSTR)content.c_str());
	xml_node<WCHAR>* root = doc.first_node();
	MindTextNode* pRoot = _parse(root, 0);
	return pRoot;
}

MindTextNode* MindMapWidget::_parse(xml_node<WCHAR>* root, int level)
{
	MindTextNode* pRoot = new MindTextNode("");
	pRoot->setLevel(level);
	for (xml_attribute<WCHAR>* attr = root->first_attribute();
		attr;
		attr = attr->next_attribute())
	{
		std::wstring attr_name = attr->name();
		std::wstring value = attr->value();
		if (attr_name == L"progress_value")
		{
			float progress = _wtof(value.c_str());
			pRoot->SetProgress(progress);
		}
		if (attr_name == L"text")
		{
			pRoot->SetContent(value);
		}
	}

	setupNode(pRoot);

	for (xml_node<WCHAR>* child = root->first_node();
		child != NULL;
		child = child->next_sibling())
	{
		MindTextNode* pChild = _parse(child, level + 1);
		pRoot->append(pChild);
	}
	return pRoot;
}

void MindMapWidget::createActions()
{
	m_pAddNode = new QAction(tr("Add &Node"), this);
	connect(m_pAddNode, SIGNAL(triggered()), this, SLOT(addNode()));

	m_pAddLink = new QAction(tr("Add &Link"), this);
	connect(m_pAddLink, SIGNAL(triggered()), this, SLOT(addLink()));
}

void MindMapWidget::setupNode(MindTextNode* node)
{
	node->setup();

	m_scene->addItem(node);
	++seqNumber;

	m_scene->clearSelection();
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
	//setupNode(node);
}

void MindMapWidget::addNode(MindTextNode* pParent, MindTextNode* pChild)
{
	pParent->insert(0, pChild);
}

MindTextNode* MindMapWidget::newNode(MindTextNode* pRoot, const QString& text)
{
	MindTextNode* node = new MindTextNode(text, pRoot);
	//setupNode(node);
	return node;
}

MindTextNode* MindMapWidget::newProgressNode(MindTextNode* pRoot, const QString& text, float progress)
{
	MindTextNode* node = new MindTextNode(text, pRoot);
	node->SetProgress(progress);
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