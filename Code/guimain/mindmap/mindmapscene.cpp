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
	clear();
	m_pRoot = NULL;
}

void MindMapScene::initContent(QString content, bool bSchedule)
{
	m_bSchedule = bSchedule;
	clear();
	m_pRoot = parseXML(content.toStdString());
	arrangeAllItems();
	update();
}

QString MindMapScene::mindmapXML()
{
	xml_document<> doc;
	XML_NODE* root = _export(m_pRoot, doc);
	doc.append_node(root);

	char buffer[40960];              // You are responsible for making the buffer large enough!
	char* end = print(buffer, doc, 0);
	*end = L'\0';
	return QString::fromUtf8(buffer);
}

void MindMapScene::onNodeCreated(MindNode* pChild)
{
	//目前暂时只需重绘
	pChild->setup(this);
	onRedrawItems();
	emit itemContentChanged(false);
}

void MindMapScene::onNodeDeleted(MindNode* pChild)
{
	removeItem(pChild);
	onRedrawItems();
	emit itemContentChanged(false);
}

void MindMapScene::onNodeContentsChanged()
{
	//TODO: 目前规模下看起来没太多问题，以后再优化单个点及子节点的更新
	onRedrawItems();
	emit itemContentChanged(true);
}

void MindMapScene::onNodeStateChanged()
{
	onRedrawItems();
	emit itemContentChanged(false);
}

void MindMapScene::onRedrawItems()
{
	arrangeAllItems();
	clearSelection();
	update();
}

void MindMapScene::arrangeAllItems()
{
	PBOUNDING_NODE pBoundingRoot = construct(m_pRoot, QPoint(0, 0), true, NULL);

	QRectF rBounding = pBoundingRoot->rrect;
	int x = rBounding.left() + pBoundingRoot->pNode->boundingRect().width();
	int y = (rBounding.top() + rBounding.bottom()) / 2;
	QPoint rootRT(x, y);

	construct(m_pRoot, rootRT, false, pBoundingRoot);
	pBoundingRoot->pNode->setPosition(QPointF(0, 0));
	arrangeItemPosition(pBoundingRoot);
	return;
}

void MindMapScene::arrangeItemPosition(PBOUNDING_NODE pBoundingRoot)
{
	QPointF currItemPos = pBoundingRoot->pNode->pos();
	for (auto it = pBoundingRoot->children.begin(); it != pBoundingRoot->children.end(); it++)
	{
		PBOUNDING_NODE pChild = *it;
		QPointF pos = pChild->itemPos(pChild->bRight);
		QPointF parentPos = pBoundingRoot->itemPos(pChild->bRight);
		QPointF offset = pos - parentPos;
		//计算child相对于parent的相对位置。
		pChild->pNode->setPosition(offset);
		arrangeItemPosition(pChild);
	}
}

PBOUNDING_NODE MindMapScene::construct(MindNode* pRoot, QPoint rootLTorRT, const bool toRight, PBOUNDING_NODE pTopRoot)
{
	if (pRoot == NULL)
		return PBOUNDING_NODE();

	//假设不能换行
	static int HMargin = 60;
	static int VMargin = 28;

	PBOUNDING_NODE pBoundingNode;
	if (pRoot->isTopRoot() && pTopRoot)
	{
		pBoundingNode = pTopRoot;
	}
	else
	{
		pBoundingNode.reset(new BoundingNode());
	}
	pBoundingNode->pNode = pRoot;

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

	bool bCollasped = ((toRight && pRoot->rightExpandState() != EXP_EXPAND) ||
		(!toRight && pRoot->leftExpandState() != EXP_EXPAND));
	if (!children.empty() && !bCollasped)
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
			PBOUNDING_NODE pChildNode = construct(pChild, childLTorRT, toRight, NULL);
			pChildNode->bRight = toRight;
			pBoundingNode->children.push_back(pChildNode);
			boundingRect = boundingRect | pChildNode->getRectF();
			boundingRect.adjust(0, 0, 0, VMargin);
			childLTorRT.setY(boundingRect.bottom());
		}
		if (!validItem)
			return pBoundingNode;

		boundingRect.adjust(0, 0, 0, -VMargin);
		if (toRight)
		{
			//获得子树形成的boundingbox，然后向左扩展，得到整棵树的boundingrect
			boundingRect.adjust(-HMargin - itemW, 0, 0, 0);
		}
		else
		{
			boundingRect.adjust(0, 0, HMargin + itemW, 0);	
		}
	}
	else
	{
		if (toRight)
		{
			boundingRect = QRectF(rootLTorRT.x(), rootLTorRT.y(), itemW, itemH);
		}
		else
		{
			boundingRect = QRectF(rootLTorRT.x() - itemW, rootLTorRT.y(), itemW, itemH);
		}
	}
	if (toRight)
	{
		pBoundingNode->rrect = boundingRect;
	}
	else
	{
		pBoundingNode->lrect = boundingRect;
		pBoundingNode->loffset = -itemW;
	}
	pBoundingNode->itemH = itemH;
	return pBoundingNode;
}

MindNode* MindMapScene::parseXML(const std::string& content)
{
	xml_document<> doc;
	doc.parse<0>(const_cast<char*>(content.c_str()));
	xml_node<>* root = doc.first_node();
	MindNode* pRoot = _parse(NULL, root, 0);
	addItem(pRoot);
	clearSelection();
	return pRoot;
}

XML_NODE* MindMapScene::_export(MindNode* pRoot, xml_document<>& doc)
{
	std::string value = pRoot->GetContent().toStdString();

	XML_NODE* root = doc.allocate_node(node_element, "node");

	xml_attribute<>* attr = doc.allocate_attribute("text",
		doc.allocate_string(value.c_str()));
	root->append_attribute(attr);

	EXPAND_STATE leftState = pRoot->leftExpandState();
	if (leftState != EXP_NODEFINE)
	{
		if (leftState == EXP_EXPAND)
			attr = doc.allocate_attribute("left_expand", "1");
		else
			attr = doc.allocate_attribute("left_expand", "0");
		root->append_attribute(attr);
	}
	
	EXPAND_STATE rightState = pRoot->rightExpandState();
	if (rightState != EXP_NODEFINE)
	{
		if (rightState == EXP_EXPAND)
			attr = doc.allocate_attribute("right_expand", "1");
		else
			attr = doc.allocate_attribute("right_expand", "0");
		root->append_attribute(attr);
	}

	//level 1的节点需要记录方向。
	if (pRoot->level() == 1)
	{
		attr = doc.allocate_attribute("to_right", 
			doc.allocate_string(QString::number(pRoot->isToRight()).toStdString().c_str()));
		root->append_attribute(attr);
	}

	if (!pRoot->noteid().isEmpty())
	{
		xml_attribute<>* attr = doc.allocate_attribute("noteid",
			doc.allocate_string(pRoot->noteid().toStdString().c_str()));
		root->append_attribute(attr);
	}

	const QList<MindNode*>& children = pRoot->children();

	MindProgressNode* pProgress = qobject_cast<MindProgressNode*>(pRoot);
	if (pProgress)
	{
		attr = doc.allocate_attribute("is_progress", "1");
		root->append_attribute(attr);

		if (children.isEmpty())
		{
			float progress = pProgress->progress();
			attr = doc.allocate_attribute("progress_value",
				doc.allocate_string(QString::number(progress).toStdString().c_str()));
			root->append_attribute(attr);

			float hours = pProgress->workHours();
			attr = doc.allocate_attribute("working_hours",
				doc.allocate_string(QString::number(hours).toStdString().c_str()));
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

MindNode* MindMapScene::_parse(MindNode* parent, xml_node<>* root, int level)
{
	MindNode* pRoot = NULL;
	MindProgressNode* pProgress = NULL;
	if (m_bSchedule)
	{
		pProgress = new MindProgressNode("", parent);
		pRoot = pProgress;
	}
	else
	{
		pRoot = new MindNode("", parent);
	}
	if (parent)
		parent->AddChild(pRoot);

	pRoot->setLevel(level);

	for (xml_attribute<>* attr = root->first_attribute();
		attr;
		attr = attr->next_attribute())
	{
		std::string attr_name = attr->name();
		std::string value = attr->value();

		//DEBUG
		if (value == "fifa")
		{
			int j;
			j = 0;
		}
		if (attr_name == "text")
		{
			pRoot->SetContent(QString::fromUtf8(value.c_str()));
		}
		if (attr_name == "to_right")
		{
			bool toRight = std::stoi(value.c_str());
			pRoot->setToRight(toRight);
		}
		if (attr_name == "left_expand")
		{
			bool expand = std::stoi(value.c_str());
			pRoot->setLeftExpand(expand);
		}
		if (attr_name == "right_expand")
		{
			bool expand = std::stoi(value.c_str());
			pRoot->setRightExpand(expand);
		}
		if (attr_name == "noteid")
		{
			pRoot->setNoteId(QString::fromStdString(value.c_str()));
		}

		if (pProgress)
		{
			if (attr_name == "progress_value")
			{
				float progress = std::stof(value.c_str());
				pProgress->setProgress(progress);
			}
			if (attr_name == "working_hours")
			{
				float hours = std::stof(value.c_str());
				pProgress->setWorkhours(hours);
			}
		}
	}

	for (xml_node<>* child = root->first_node();
		child != NULL;
		child = child->next_sibling())
	{
		MindNode* pChild = _parse(pRoot, child, level + 1);
		//如果当前pChild的方向被折叠，那么pChild设为隐藏。
		if (pChild->isToRight())
		{
			bool bVisible = (pRoot->rightExpandState() == EXP_EXPAND);
			pChild->setVisible(bVisible);
		}
		else
		{
			bool bVisible = (pRoot->leftExpandState() == EXP_EXPAND);
			pChild->setVisible(bVisible);
		}
	}

	pRoot->setup(this);
	return pRoot;
}
