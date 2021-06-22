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
	arrangeAllItems();
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

void MindMapScene::onCreateChildNode(MindNode* pRoot, bool bRight)
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
	pChild->setToRight(bRight);
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
	pChild->setToRight(pNode->isToRight());
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
	arrangeAllItems();
	clearSelection();
	update();
	emit itemContentChanged();
}

void MindMapScene::setupNode(MindNode* node)
{
	connect(node, SIGNAL(textChange()), this, SLOT(onItemTextChanged()));
	connect(node, SIGNAL(dataChanged()), this, SIGNAL(itemContentChanged()));
	connect(node, SIGNAL(childNodeCreate(MindNode*, bool)), this, SLOT(onCreateChildNode(MindNode*, bool)));
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

void MindMapScene::adjustAllItemPos(MindNode* pRoot, int xoffset, int yoffset, bool bOnlyRightside)
{
	if (bOnlyRightside)
	{
		// 只调右边的点，不需要动根节点。
		if (pRoot != m_pRoot)
			pRoot->setPos(pRoot->pos() + QPointF(xoffset, yoffset));
	}
	else
	{
		pRoot->setPos(pRoot->pos() + QPointF(xoffset, yoffset));
	}
	const QList<MindNode*>& children = pRoot->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		if (bOnlyRightside)
		{
			if ((*it)->isToRight())
				adjustAllItemPos(*it, xoffset, yoffset, bOnlyRightside);
		}
		else
		{
			adjustAllItemPos(*it, xoffset, yoffset, bOnlyRightside);
		}
	}
}

void MindMapScene::arrangeAllItems()
{
	qreal itemH = m_pRoot->boundingRect().height();
	qreal itemW = m_pRoot->boundingRect().width();

	QRectF rRight = arrangeItemPosition(QPoint(0, 0), m_pRoot, true);
	QPointF oldPos = m_pRoot->pos();
	QRectF rLeft = arrangeItemPosition(QPoint(0, 0), m_pRoot, false);

	QRectF boundingRect;
	if (rRight.isValid() && rLeft.isValid())
	{
		//移动策略：定左移右。
		//计算出右边所有节点的偏移量。
		int xoffset = 0, yoffset = 0;
		QPointF newPos = m_pRoot->pos();
		QPointF offset = newPos - oldPos;
		xoffset = offset.x();
		yoffset = offset.y();

		rRight.moveTo(rRight.left() + xoffset, rRight.top() + yoffset);

		//对右边所有节点进行移动
		adjustAllItemPos(m_pRoot, xoffset, yoffset, true);

		//先用rLeft和rRight拼接成一个整体的boundingbox
		int left = rLeft.left();
		int right = rRight.right();
		int top = min(rLeft.top(), rRight.top());
		int bottom = max(rLeft.bottom(), rRight.bottom());
		boundingRect = QRectF(QPointF(left, top), QPointF(right, bottom));
	}
	else if (rRight.isValid())
	{
		boundingRect = rRight;
	}
	else if (rLeft.isValid())
	{
		boundingRect = rLeft;
	}
	else
	{
		Q_ASSERT(false);
	}

	//调整位置策略：1.根节点在(0,0)。2.boundingbox中心在(0,0)
	int xoffset = 0 - boundingRect.center().x();
	int yoffset = 0 - boundingRect.center().y();
	adjustAllItemPos(m_pRoot, xoffset, yoffset, false);

	appendPaths(m_pRoot);
}

QRectF MindMapScene::arrangeItemPosition(QPoint rootLTorRT, MindNode* pRoot, bool toRight)
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

	QPoint childLTorRT;		//如果相左，关注的就是RT
	if (n > 0)
	{
		int childTop = rootLTorRT.y();
		if (toRight)
			childLTorRT = QPoint(rootLTorRT.x() + itemW + HMargin, childTop);
		else
			childLTorRT = QPoint(rootLTorRT.x() - itemW - HMargin, childTop);
	}

	QRectF boundingRect;
	if (!children.empty())
	{
		QVector<QPoint> vecConnectors;
		bool validItem = false;
		for (auto it = children.begin(); it != children.end(); it++)
		{
			MindNode* pChild = *it;
			if (pChild->isToRight() != toRight)
			{
				continue;
			}
			validItem = true;
			QRectF rr = arrangeItemPosition(childLTorRT, pChild, toRight);
			boundingRect = boundingRect | rr;
			boundingRect.adjust(0, 0, 0, VMargin);
			childLTorRT.setY(boundingRect.bottom());
		}
		if (!validItem)
			return boundingRect;

		boundingRect.adjust(0, 0, 0, -VMargin);
	
		QPointF basePos;
		if (toRight)
		{
			//获得子树形成的boundingbox，然后向左扩展，得到整棵树的boundingrect
			boundingRect.adjust(-HMargin - itemW, 0, 0, 0);
			//取boundRect的左中间
			basePos = QPointF(boundingRect.left(), (boundingRect.top() + boundingRect.bottom() - itemH) / 2.0);
		}
		else
		{
			boundingRect.adjust(0, 0, HMargin + itemW, 0);
			//取boundRect的右中间，但需要减去itemW的宽度来设置根的位置。
			basePos = QPointF(boundingRect.right() - itemW, (boundingRect.top() + boundingRect.bottom() - itemH) / 2.0);
		}

		pRoot->setPos(basePos);

		//绘制添加按钮。
		float r = 28. / 2;
		MindNodeButton* pBtn = new MindNodeButton(r);
		pBtn->setPos(boundingRect.right(), (boundingRect.top() + boundingRect.bottom() - r) / 2.0);

		return boundingRect;
	}
	else
	{
		addItem(pRoot);
		QPointF basePos;
		if (toRight)
		{
			basePos = rootLTorRT;
			boundingRect = QRectF(rootLTorRT.x(), rootLTorRT.y(), itemW, itemH);
		}
		else
		{
			basePos = QPointF(rootLTorRT.x() - itemW, rootLTorRT.y());
			boundingRect = QRectF(rootLTorRT.x() - itemW, rootLTorRT.y(), itemW, itemH);
		}
		if (pRoot != m_pRoot)
		{
			pRoot->setPos(basePos);
		}
	}
	return boundingRect;
}

void MindMapScene::appendPaths(MindNode* pRoot)
{
	MindNode* pParent = pRoot->Parent();
	if (pParent != NULL)
	{
		QPainterPath path;

		QPointF pos = pRoot->pos();

		QPointF rootConnector;
		if (pRoot->isToRight())
		{
			rootConnector = pParent->pos() + QPointF(pParent->boundingRect().width(), pParent->boundingRect().height() / 2.0F);
		}
		else
		{
			rootConnector = pParent->pos() + QPointF(0, pParent->boundingRect().height() / 2.0F);
		}

		path.moveTo(rootConnector);

		QPointF childConnector;
		QRectF bbox = pRoot->boundingRect();
		if (pRoot->isToRight())
		{
			childConnector = QPointF(pos.x(), pos.y() + bbox.height() / 2);
		}
		else
		{
			childConnector = QPointF(pos.x() + bbox.width(), pos.y() + bbox.height() / 2);
		}

		QPointF c1((rootConnector.x() + childConnector.x()) / 2.0, rootConnector.y());
		QPointF c2((rootConnector.x() + childConnector.x()) / 2.0, childConnector.y());

		path.cubicTo(c1, c2, childConnector);

		QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
		pathItem->setPen(QPen(QColor(0, 181, 72), 3));
		addItem(pathItem);
		m_pathItems.append(pathItem);
	}
	const QList<MindNode*>& children = pRoot->children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		appendPaths(*it);
	}
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

	//level 1的节点需要记录方向。
	if (pRoot->level() == 1)
	{
		attr = doc.allocate_attribute(L"to_right", 
			doc.allocate_string(QString::number(pRoot->isToRight()).toStdWString().c_str()));
		root->append_attribute(attr);
	}

	if (!pRoot->noteid().isEmpty())
	{
		xml_attribute<WCHAR>* attr = doc.allocate_attribute(L"noteid",
			doc.allocate_string(pRoot->noteid().toStdWString().c_str()));
		root->append_attribute(attr);
	}

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
		if (attr_name == L"to_right")
		{
			bool toRight = _wtoi(value.c_str());
			pRoot->setToRight(toRight);
		}
		if (attr_name == L"noteid")
		{
			pRoot->setNoteId(QString::fromStdWString(value.c_str()));
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
