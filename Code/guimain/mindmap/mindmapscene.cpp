#include "stdafx.h"
#include "mindmapscene.h"
#include "rapidxml_print.hpp"
#include "mindnodebutton.h"
#include "mindprogressnode.h"


MindMapScene::MindMapScene(QObject* parent)
	: QGraphicsScene(parent)
	, m_bSchedule(false)
	, m_pRoot(NULL)
{

}

MindMapScene::~MindMapScene()
{
}

void MindMapScene::initContent(QString content, bool bSchedule)
{
	m_bSchedule = bSchedule;
	clear();
	m_pRoot = NULL;		//TODO: 有没有更好的写法？比如gardpointer
	m_pathItems.clear();
	std::wstring wstr = content.toStdWString();
	m_pRoot = parseXML(wstr);
	arrangeItemPosition(QPoint(0, 0), m_pRoot);
	update();
}

QString MindMapScene::mindmapXML()
{
	xml_document<WCHAR> doc;
	XML_NODE* root = _export(m_pRoot, doc);
	doc.append_node(root);

	WCHAR buffer[4096];                      // You are responsible for making the buffer large enough!
	WCHAR* end = print(buffer, doc, 0);
	*end = L'\0';
	return QString::fromUtf16((char16_t*)buffer);
}

MindProgressNode* MindMapScene::newProgressNode(MindProgressNode* pRoot, const QString& text, float progress)
{
	MindProgressNode* node = new MindProgressNode(text, pRoot);
	node->setProgress(progress);
	setupNode(node);
	return node;
}

void MindMapScene::onCreateChildNode(MindNode* pRoot)
{
	MindNode* pChild = NULL;
	if (m_bSchedule)
	{
		MindProgressNode* pRoot_ = qobject_cast<MindProgressNode*>(pRoot);
		Q_ASSERT(pRoot_);
		pChild = new MindProgressNode(u8"进度节点", pRoot_);
	}
	else
	{
		pChild = new MindNode(u8"新增节点", pRoot);
	}
	pRoot->append(pChild);
	pChild->setLevel(pRoot->level() + 1);
	setupNode(pChild);

	onRedrawItems();
}

void MindMapScene::onCreateSlibingNode(MindNode* pNode)
{
	MindNode* parent = pNode->Parent();
	MindNode* pChild = NULL;
	if (m_bSchedule)
	{
		MindProgressNode* pRoot_ = qobject_cast<MindProgressNode*>(parent);
		Q_ASSERT(pRoot_);
		pChild = new MindProgressNode(u8"进度节点", pRoot_);
	}
	else
	{
		pChild = new MindNode(u8"新增节点", parent);
	}

	pChild->setLevel(pNode->level());
	parent->append(pChild);
	setupNode(pChild);

	onRedrawItems();
}

void MindMapScene::onDeleteNode(MindNode* pNode)
{
	pNode->Parent()->removeNode(pNode);
	MindProgressNode* progressNode = qobject_cast<MindProgressNode*>(pNode->Parent());
	if (progressNode)
	{
		progressNode->updateStatus();
	}
	unsetupNode(pNode);
	onRedrawItems();
}

void MindMapScene::onItemTextChanged()
{
	onRedrawItems();
	emit itemContentChanged();
}

void MindMapScene::onRedrawItems()
{
	for (auto it = m_pathItems.begin(); it != m_pathItems.end(); it++)
	{
		removeItem(*it);
	}
	m_pathItems.clear();
	arrangeItemPosition(QPoint(0, 0), m_pRoot);
	clearSelection();
	update();
	emit itemContentChanged();
}

void MindMapScene::setupNode(MindNode* node)
{
	connect(node, SIGNAL(textChange()), this, SLOT(onItemTextChanged()));
	connect(node, SIGNAL(dataChanged()), this, SIGNAL(itemContentChanged()));
	connect(node, SIGNAL(childNodeCreate(MindNode*)), this, SLOT(onCreateChildNode(MindNode*)));
	connect(node, SIGNAL(silibingNodeCreate(MindNode*)), this, SLOT(onCreateSlibingNode(MindNode*)));
	connect(node, SIGNAL(deleteNode(MindNode*)), this, SLOT(onDeleteNode(MindNode*)));
	addItem(node);
	const QList<MindNode*>& children = node->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		setupNode(*it);
	}
	node->setup();
}

void MindMapScene::unsetupNode(MindNode* node)
{
	const QList<MindNode*>& children = node->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		unsetupNode(*it);
	}
	removeItem(node);
}

QRectF MindMapScene::arrangeItemPosition(QPoint rootLT, MindNode* pRoot)
{
	if (pRoot == NULL)
		return QRectF();

	//假设不能换行
	static int HMargin = 60;
	static int VMargin = 28;

	const QList<MindNode*>& children = pRoot->children();
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

		//绘制添加按钮。
		float r = 28. / 2;
		MindNodeButton* pBtn = new MindNodeButton(r);
		pBtn->setPos(boundingRect.right(), (boundingRect.top() + boundingRect.bottom() - r) / 2.0);

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
			addItem(pathItem);
			m_pathItems.append(pathItem);
		}
		return boundingRect;
	}
	else
	{
		addItem(pRoot);
		pRoot->setPos(rootLT);
		boundingRect = QRectF(rootLT.x(), rootLT.y(), pRoot->boundingRect().width(), pRoot->boundingRect().height());
	}

	return boundingRect;
}

MindNode* MindMapScene::_initExample()
{
	MindProgressNode* pRoot = newProgressNode(NULL, u8"大米学习计划", 0.1);

	MindProgressNode* pChild = newProgressNode(pRoot, u8"工作回顾", 0.3);
	MindProgressNode* pChild2 = newProgressNode(pRoot, u8"多线程知识", 0.01);
	MindProgressNode* pChild3 = newProgressNode(pRoot, u8"Windows基础", 0.9);
	MindProgressNode* pChild4 = newProgressNode(pRoot, u8"C++基础知识", 0.5);

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

MindNode* MindMapScene::_initFromFile()
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

MindNode* MindMapScene::parseXML(const std::wstring& content)
{
	xml_document<WCHAR> doc;
	doc.parse<0>((LPWSTR)content.c_str());
	xml_node<WCHAR>* root = doc.first_node();
	MindNode* pRoot = _parse(root, 0);
	setupNode(pRoot);
	clearSelection();
	return pRoot;
}

XML_NODE* MindMapScene::_export(MindNode* pRoot, xml_document<WCHAR>& doc)
{
	std::wstring value = pRoot->GetContent();

	XML_NODE* root = doc.allocate_node(node_element, L"node");

	xml_attribute<WCHAR>* attr = doc.allocate_attribute(L"text",
		doc.allocate_string(value.c_str()));
	root->append_attribute(attr);

	const QList<MindNode*>& children = pRoot->children();

	MindProgressNode* pProgress = qobject_cast<MindProgressNode*>(pRoot);
	if (pProgress)
	{
		attr = doc.allocate_attribute(L"is_progress", L"1");
		root->append_attribute(attr);

		if (children.isEmpty())
		{
			float progress = pProgress->progress();
			attr = doc.allocate_attribute(L"progress_value",
				doc.allocate_string(QString::number(progress).toStdWString().c_str()));
			root->append_attribute(attr);

			float hours = pProgress->workHours();
			attr = doc.allocate_attribute(L"working_hours",
				doc.allocate_string(QString::number(hours).toStdWString().c_str()));
			root->append_attribute(attr);
		}
	}
	
	for (auto it = children.begin(); it != children.end(); it++)
	{
		XML_NODE* pChild = _export(*it, doc);
		root->append_node(pChild);
	}

	return root;
}

MindNode* MindMapScene::_parse(xml_node<WCHAR>* root, int level)
{
	MindNode* pRoot = NULL;
	MindProgressNode* pProgress = NULL;
	if (m_bSchedule)
	{
		pProgress = new MindProgressNode("");
		pRoot = pProgress;
	}
	else
	{
		pRoot = new MindNode("");
	}

	pRoot->setLevel(level);
	for (xml_attribute<WCHAR>* attr = root->first_attribute();
		attr;
		attr = attr->next_attribute())
	{
		std::wstring attr_name = attr->name();
		std::wstring value = attr->value();

		if (attr_name == L"text")
		{
			pRoot->SetContent(value);
		}

		if (pProgress)
		{
			if (attr_name == L"progress_value")
			{
				float progress = _wtof(value.c_str());
				pProgress->setProgress(progress);
			}
			if (attr_name == L"working_hours")
			{
				float hours = _wtof(value.c_str());
				pProgress->setWorkhours(hours);
			}
		}
	}

	for (xml_node<WCHAR>* child = root->first_node();
		child != NULL;
		child = child->next_sibling())
	{
		MindNode* pChild = _parse(child, level + 1);
		pRoot->append(pChild);
	}
	return pRoot;
}
