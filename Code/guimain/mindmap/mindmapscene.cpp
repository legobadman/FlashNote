#include "stdafx.h"
#include "mindmapscene.h"
#include "rapidxml_print.hpp"
#include "mindnodebutton.h"
#include "mindprogressnode.h"
#include <QDebug>


MindMapScene::MindMapScene(QObject* parent)
	: QGraphicsScene(parent)
	, m_bSchedule(false)
	, m_pRoot(NULL)
	, m_pHolder(NULL)
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
	m_pHolder = new RoundedRectItem(NULL);
	m_pHolder->hide();
	addItem(m_pHolder);
	m_pRoot = parseXML(content.toStdString());
	arrangeAllItems(false);
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
	//Ŀǰ��ʱֻ���ػ�
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
	//TODO: Ŀǰ��ģ�¿�����û̫�����⣬�Ժ����Ż������㼰�ӽڵ�ĸ���
	onRedrawItems();
	emit itemContentChanged(true);
}

void MindMapScene::onNodeStateChanged()
{
	onRedrawItems();
	emit itemContentChanged(false);
}

void MindMapScene::onRedrawItems(bool bDrawHolder)
{
	arrangeAllItems(bDrawHolder);
	clearSelection();
	update();
}

void MindMapScene::arrangeAllItems(bool bDrawHolder)
{
	std::stack<MindNode*> sck, pre;
	sck.push(m_pRoot);
	while (!sck.empty())
	{
		MindNode* pRoot = sck.top();
		sck.pop();
		foreach(MindNode * pChild, pRoot->Children(!bDrawHolder))
		{
			sck.push(pChild);
		}
		pre.push(pRoot);
	}

	int globalY = 0;
	const int HMargin = 60, VMargin = 28;
	while (!pre.empty())
	{
		MindNode* pRoot = pre.top();
		
		//�۲��ӽڵ�ĸ߶ȣ���y�Ļ�����Ϊ��ǰ�ڵ�
		QList<MindNode*> children = pRoot->Children(!bDrawHolder);
		int W = pRoot->boundingRect().width();
		int n = children.size();

		QList<MindNode*> leftChildren, rightChildren;
		qreal totalRightH = 0, totalLeftH = 0;
		for (int i = 0; i < n; i++)
		{
			MindNode* pChild = children[i];
			if (pChild->isToRight())
			{
				QRectF wholeBoundingRect = pChild->childrenOrSelfRect(true);
				totalRightH += wholeBoundingRect.height();
				totalRightH += VMargin;
				rightChildren.append(pChild);
			}
			else
			{
				QRectF wholeBoundingRect = pChild->childrenOrSelfRect(false);
				totalLeftH += wholeBoundingRect.height();
				totalLeftH += VMargin;
				leftChildren.append(pChild);
			}
		}
		totalRightH -= VMargin;
		totalLeftH -= VMargin;

		if (!pRoot->isCollapsed(false) && !leftChildren.empty())
		{
			//�ٱ���ÿһ���ӿ�
			qreal startY = -totalLeftH / 2. + pRoot->boundingRect().height() / 2.;
			for (int i = 0; i < leftChildren.size(); i++)
			{
				MindNode* pChild = leftChildren[i];
				QRectF br = pChild->childrenOrSelfRect(false);
				qreal y = startY + br.height() / 2 - pChild->boundingRect().height() / 2.;
				qreal x = -pChild->boundingRect().width() - HMargin;
				pChild->setPosition(QPointF(x, y));
				startY += br.height() + VMargin;
			}
		}

		if (!pRoot->isCollapsed(true) && !rightChildren.empty())
		{
			//�ٱ���ÿһ���ӿ�
			qreal startY = -totalRightH / 2. + pRoot->boundingRect().height() / 2.;
			for (int i = 0; i < rightChildren.size(); i++)
			{
				MindNode* pChild = rightChildren[i];
				QRectF br = pChild->childrenOrSelfRect(true);
				qreal y = startY + br.height() / 2 - pChild->boundingRect().height() / 2.;
				qreal x = W + HMargin;
				pChild->setPosition(QPointF(x, y));
				startY += br.height() + VMargin;
			}
		}
		pre.pop();
	}
	
	m_pRoot->setPosition(QPointF(0, 0));
}

void MindMapScene::onNodeDragging(MindNode* pDraggingNode)
{
	int idx = -1;
	QPointF scenePos = pDraggingNode->scenePos();

	MindNode* pNewHolderParent = NULL;
	int holderIdx = 0;
	bool toRight = false;

	std::stack<MindNode*> sck, post;
	sck.push(m_pRoot);
	while (!sck.empty())
	{
		MindNode* pRoot = sck.top();
		sck.pop();

		bool bFinded = pRoot->hasDraggingInChildRect(scenePos, holderIdx, toRight);
		if (bFinded)
		{
			pNewHolderParent = pRoot;
			break;
		}
		else
		{
			holderIdx = -1;
			toRight = false;
			pNewHolderParent = NULL;
		}
		foreach(MindNode * pChild, pRoot->Children())
		{
			if (!pChild->isHolder())
				sck.push(pChild);
		}
	}

	//�����ͬλ���Ƿ���holder�ˣ���������λ��ûɶ�ɱ���
	if (pNewHolderParent == NULL || holderIdx < 0 ||
		holderIdx > pNewHolderParent->Children().length())
	{
		m_pHolder->hide();
	}
	else
	{
		m_pHolder->setToRight(toRight);
		m_pHolder->setVisible(true);
		if (m_pHolder->Parent())
			m_pHolder->Parent()->removeChild(m_pHolder);
		pNewHolderParent->insertChild(m_pHolder, holderIdx);
	}
	onRedrawItems(true);
}

void MindMapScene::onNodeDragged(MindNode* pNode)
{
	if (m_pHolder && m_pHolder->Parent())
	{
		MindNode* parent = m_pHolder->Parent();
		//TODO������Ҫ���Ƿ��жϣ�����������ASSERT���̵���ȷ�ԡ�
		int idx = parent->Children(false).indexOf(m_pHolder);
		parent->removeChild(m_pHolder);
		parent->insertChild(pNode, idx);
		pNode->setToRight(m_pHolder->isToRight());
		m_pHolder->hide();
		onRedrawItems();
		//emit itemContentChanged(false);
	}
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

	//level 1�Ľڵ���Ҫ��¼����
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

	const QList<MindNode*>& children = pRoot->Children();

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
		//�����ǰpChild�ķ����۵�����ôpChild��Ϊ���ء�
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
