#include "stdafx.h"
#include "mindtransaction.h"
#include "mindnode.h"
#include "mindprogressnode.h"
#include "mindmapscene.h"


NewChildTransform::NewChildTransform(MindMapScene* pScene, MindNode* parent, bool toRight)
	: m_parent(parent)
	, m_toRight(toRight)
	, m_child(NULL)
	, m_scene(pScene)
{

}

bool NewChildTransform::forward() //forward是否合适成为外部正常调用也使用的方法？
{
	MindNode* pChild = NULL;
	if (m_child == NULL)
	{
        if (qobject_cast<MindProgressNode*>(m_parent))
        {
            m_child = new MindProgressNode(u8"进度节点", m_parent);
        }
        else
        {
            m_child = new MindNode(u8"新增节点", m_parent);
        }
        m_child->setToRight(m_toRight);
        m_child->setLevel(m_parent->level() + 1);
		m_parent->AddChild(m_child);
		m_child->setup(m_scene);
		m_scene->addItem(m_child);
	}
	else
	{
		m_parent->AddChild(m_child);
		m_child->show();
	}
	m_scene->refresh();
	return true;
}

bool NewChildTransform::backward()
{
	if (m_parent && m_child)
	{
		m_parent->removeChild(m_child);
		m_child->hide();
		m_scene->refresh();
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////
RemoveChildTransform::RemoveChildTransform(MindMapScene* pScene, MindNode* parent, MindNode* remove_item)
	: m_parent(parent)
	, m_scene(pScene)
	, m_child(remove_item)
{
	QList<MindNode*> children = m_parent->Children();
	m_remove_idx = children.indexOf(remove_item);
	Q_ASSERT(0 <= m_remove_idx && m_remove_idx < children.length());
}

bool RemoveChildTransform::forward()
{
	if (m_child)
	{
		m_parent->removeChild(m_child);
		m_child->hide();
		m_scene->refresh();
		return true;
	}
	return false;
}

bool RemoveChildTransform::backward()
{
	if (m_child)
	{
		m_parent->insertChild(m_child, m_remove_idx);
        m_child->show();
		m_scene->refresh();
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////
EditContentTransform::EditContentTransform(MindMapScene* pScene, MindNode* pNode, const QString& orignalText, const QString& newText)
	: m_pNode(pNode)
	, m_orignal(orignalText)
	, m_text(newText)
	, m_scene(pScene)
{
}

bool EditContentTransform::forward()
{
	m_pNode->SetContent(m_text);
	m_scene->refresh(true);
	return true;
}

bool EditContentTransform::backward()
{
	m_pNode->SetContent(m_orignal);
	m_scene->refresh(true);
	return true;
}


/////////////////////////////////////////////////////////////////////////
DragOutTransform::DragOutTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* parent)
	: m_scene(pScene)
	, m_node(pDragNode)
	, m_parent(parent)
{
    QList<MindNode*> children = m_parent->Children();
    m_remove_idx = children.indexOf(m_node);
    Q_ASSERT(0 <= m_remove_idx && m_remove_idx < children.length());
}

bool DragOutTransform::forward()
{
	//m_parent->removeChild(m_node);
	//m_node->m_pathItem->hide();
	//m_node->_collaspe(m_node->isToRight());
	//m_node->m_bDragging = true;
	//m_node->setZValue(-1);
	return false;
}

bool DragOutTransform::backward()
{
    m_parent->insertChild(m_node, m_remove_idx);
    m_scene->refresh();
	return false;
}


/////////////////////////////////////////////////////////////////////////
DragInTransform::DragInTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* newParent, int idx, bool toRight)
	: m_scene(pScene)
	, m_node(pDragNode)
	, m_parent(newParent)
	, m_idx(idx)
	, m_isToRight(toRight)
{
}

bool DragInTransform::forward()
{
	m_parent->insertChild(m_node, m_idx);
	m_node->resetAllChildDirection(m_isToRight);
	//m_pHolder->hide();
	//m_scene->refresh();
	return true;
}

bool DragInTransform::backward()
{
	m_parent->removeChild(m_node);
	return true;
}


/////////////////////////////////////////////////////////////////////////
MoveTransform::MoveTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* oldParent)
	: m_scene(pScene)
	, m_pDragNode(pDragNode)
	, m_pOldParent(oldParent)
	, m_pNewParent(NULL)
{



}

void MoveTransform::setNewParent(MindNode* pNewParent)
{
	m_pNewParent = pNewParent;
}

bool MoveTransform::prepare()
{
	return true;
}

bool MoveTransform::forward()
{
	if (m_pNewParent)
	{

	}
	return false;
}

bool MoveTransform::backward()
{
	return false;
}


DocumentTransform::DocumentTransform(MindMapScene* pScene)
	: m_scene(pScene)
{
	m_oldDocument = m_scene->mindmapXML();
}

bool DocumentTransform::forward()
{
	m_scene->reconstruct(m_newDocument);
	return true;
}

bool DocumentTransform::backward()
{
	m_scene->reconstruct(m_oldDocument);
	return true;
}

void DocumentTransform::setNewDocument(const QString& newDocument)
{
	m_newDocument = newDocument;
}


RAIITransBatch::RAIITransBatch(MindMapScene* scene)
	: m_scene(scene)
	, m_success(true)
{
	m_pTransform.reset(new DocumentTransform(scene));
	m_spTrans.reset(new Transaction);
	m_spTrans->SetTransform(m_pTransform);
	m_spTrans->SetState(SS_START);
	m_scene->transRepository()->Add(m_spTrans);
	m_id = m_spTrans->GetId();
}

void RAIITransBatch::setResult(bool success)
{
	m_success = success;
}

RAIITransBatch::~RAIITransBatch()
{
	//要在这里做一次记录
	QString newDocument = m_scene->mindmapXML();
	if (m_success)
    {
        m_pTransform->setNewDocument(newDocument);
        m_scene->transRepository()->Commit(m_id);
    }
	else {
		//TODO: 可能要删除这个事务了。
		m_scene->transRepository()->Rollback(m_id);
	}
}


ManualTransBatch::ManualTransBatch(MindMapScene* scene)
    : m_scene(scene)
    , m_success(false)
	, m_id(-1)
{
}

ManualTransBatch::~ManualTransBatch()
{
}

void ManualTransBatch::startBatch()
{
    m_pTransform.reset(new DocumentTransform(m_scene));
    TRANSCATION_PTR spTrans(new Transaction);
    spTrans->SetTransform(m_pTransform);
    spTrans->SetState(SS_START);
    m_scene->transRepository()->Add(spTrans);
    m_id = spTrans->GetId();
}

void ManualTransBatch::endBatch()
{
	if (m_id != -1)
	{
        if (m_success)
        {
            QString newDocument = m_scene->mindmapXML();

            //失败了就不用setNewDocument了，要看看怎么处理
            m_pTransform->setNewDocument(newDocument);
            m_scene->transRepository()->Commit(m_id);
        }
        else
        {
            m_scene->transRepository()->Rollback(m_id);
        }
	}
}

void ManualTransBatch::setResult(bool success)
{
	m_success = success;
}