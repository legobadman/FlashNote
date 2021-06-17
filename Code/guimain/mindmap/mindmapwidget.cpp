#include "stdafx.h"
#include "mindmapwidget.h"
#include "rapidxml_print.hpp"
#include <QAction>


MindMapWidget::MindMapWidget(QWidget* parent /* = NULL */)
	: QWidget(parent)
	, m_scene(NULL)
	, m_view(NULL)
	, m_pRoot(NULL)
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
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::initContent(QString content)
{
	std::wstring wstr = content.toStdWString();
	m_pRoot = parseXML(wstr);
	arrangeItemPosition(QPoint(0, 0), m_pRoot);
}

QString MindMapWidget::mindmapXML()
{
	xml_document<WCHAR> doc;
	XML_NODE* root = _export(m_pRoot, doc);
	doc.append_node(root);

	WCHAR buffer[4096];                      // You are responsible for making the buffer large enough!
	WCHAR* end = print(buffer, doc, 0);
	*end = L'\0';
	return QString::fromUtf16((char16_t*)buffer);
}

XML_NODE* MindMapWidget::_export(MindTextNode* pRoot, xml_document<WCHAR>& doc)
{
	std::wstring value = pRoot->GetContent();

	XML_NODE* root = doc.allocate_node(node_element, L"node");

	xml_attribute<WCHAR>* attr = doc.allocate_attribute(L"text", 
		doc.allocate_string(value.c_str()));
	root->append_attribute(attr);

	if (pRoot->IsProgress())
	{
		attr = doc.allocate_attribute(L"progress_value",
			doc.allocate_string(QString::number(pRoot->GetProgress()).toStdWString().c_str()));
		root->append_attribute(attr);
	}

	const QList<MindTextNode*>& children = pRoot->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		XML_NODE* pChild = _export(*it, doc);
		root->append_node(pChild);
	}

	return root;
}

MindTextNode* MindMapWidget::_initFromFile()
{
	QFile fn("E:\\FlashNote\\Code\\io\\node.xml");
	if (!fn.open(QIODevice::ReadOnly | QIODevice::Text))
		return NULL;

	QByteArray htmlFile = fn.readAll();
	QString content = QString::fromUtf8(htmlFile);
	std::wstring wstr = content.toStdWString();
	m_pRoot = parseXML(wstr);
	arrangeItemPosition(QPoint(0, 0), m_pRoot);
}

MindTextNode* MindMapWidget::_initExample()
{
	MindTextNode* pRoot = newProgressNode(NULL, u8"大米学习计划", 0.1);

	MindTextNode* pChild = newProgressNode(pRoot, u8"工作回顾", 0.3);
	MindTextNode* pChild2 = newProgressNode(pRoot, u8"多线程知识", 0.01);
	MindTextNode* pChild3 = newProgressNode(pRoot, u8"Windows基础", 0.9);
	MindTextNode* pChild4 = newProgressNode(pRoot, u8"C++基础知识", 0.5);

	pRoot->insert(0, pChild);
	pRoot->insert(0, pChild2);
	pRoot->insert(0, pChild3);
	pRoot->insert(0, pChild4);

	pChild->insert(0, newProgressNode(pChild, u8"bug总结", 0.4));
	pChild->insert(0, newProgressNode(pChild, u8"开发功能总结", 0.2));

	pChild2->insert(0, newProgressNode(pChild2, u8"信号量", 0));
	pChild2->insert(0, newProgressNode(pChild2, u8"mutable", 0));

	pChild4->insert(0, newProgressNode(pChild4, u8"COM组件", 0.3));
	pChild4->insert(0, newProgressNode(pChild4, u8"虚函数机制", 0.6));
	pChild4->insert(0, newProgressNode(pChild4, u8"智能指针", 0.2));

	return pRoot;
}

MindTextNode* MindMapWidget::parseXML(const std::wstring& content)
{
	xml_document<WCHAR> doc;
	doc.parse<0>((LPWSTR)content.c_str());
	xml_node<WCHAR>* root = doc.first_node();
	MindTextNode* pRoot = _parse(root, 0);
	setupNode(pRoot);
	m_scene->clearSelection();
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
	//connect(m_pAddNode, SIGNAL(triggered()), this, SLOT(addNode()));

	m_pAddLink = new QAction(tr("Add &Link"), this);
	//connect(m_pAddLink, SIGNAL(triggered()), this, SLOT(addLink()));
}

void MindMapWidget::setupNode(MindTextNode* node)
{
	node->setup();
	connect(node, SIGNAL(contentsChange()), this, SIGNAL(itemContentChanged()));
	m_scene->addItem(node);
	const QList<MindTextNode*>& children = node->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		setupNode(*it);
	}
}

void MindMapWidget::addNode(MindTextNode* pParent, MindTextNode* pChild)
{
	pParent->insert(0, pChild);
}

void MindMapWidget::onItemContentChanged()
{
	//QTimer::singleShot(3000, this, SLOT(saveNote()));
}

MindTextNode* MindMapWidget::newNode(MindTextNode* pRoot, const QString& text)
{
	MindTextNode* node = new MindTextNode(text, pRoot);
	return node;
}

MindTextNode* MindMapWidget::newProgressNode(MindTextNode* pRoot, const QString& text, float progress)
{
	MindTextNode* node = new MindTextNode(text, pRoot);
	node->SetProgress(progress);
	setupNode(node);
	return node;
}

QPoint leftConnectPoint(QRectF boundingRect)
{
	return QPoint(boundingRect.left(), boundingRect.top() + boundingRect.height() / 2);
}

QRectF MindMapWidget::arrangeItemPosition(QPoint rootLT, MindTextNode* pRoot)
{
	//假设不能换行
	static int HMargin = 60;
	static int VMargin = 28;
	
	const QList<MindTextNode*>& children = pRoot->children();
	int n = children.size();

	//目前暂时不考虑子级的高度变化。
	QRectF rootRect = pRoot->boundingRect(); //目前还没指定位置，只能取宽高。
	qreal itemH = pRoot->boundingRect().height();
	qreal itemW = pRoot->boundingRect().width();

	float nn = n;
	
	QPoint childLT;
	if (n > 0)
	{
		//从children根节点形成的boundingRect中
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

		//获得子树形成的区域rExtend，然后向左扩展，得到整棵树的boundingrect
		boundingRect.adjust(-HMargin - itemW, 0, 0, 0);
		//取boundRect的左中间
		QPointF basePos(boundingRect.left(), (boundingRect.top() + boundingRect.bottom() - itemH) / 2.0);
		pRoot->setPos(basePos);

		QRectF rRoot = pRoot->boundingRect();
		QPointF rootConnector = basePos + QPointF(itemW, itemH / 2.0F);
		//锁定了当前节点及子节点的位置，可以画线了。
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