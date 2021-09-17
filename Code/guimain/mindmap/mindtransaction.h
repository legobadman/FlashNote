#ifndef __MIND_TRANSACTION_H__
#define __MIND_TRANSACTION_H__

#include "../transaction/transaction.h"

class MindNode;
class MindMapScene;

class NewChildTransform : public ITransform
{
public:
    NewChildTransform(MindMapScene* pScene, MindNode* parent, bool toRight);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    MindNode* m_parent;
    MindNode* m_child;
    MindMapScene* m_scene;
    bool m_toRight;
};

class RemoveChildTransform : public ITransform
{
public:
    RemoveChildTransform(MindMapScene* pScene, MindNode* parent, MindNode* remove_item);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    int m_remove_idx;
    MindNode* m_parent;
    MindNode* m_child;
    MindMapScene* m_scene;
};

class EditContentTransform : public ITransform
{
public:
    EditContentTransform(MindMapScene* pScene, MindNode* pNode, const QString& orignalText, const QString& newText);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    QString m_orignal;
    QString m_text;
    MindNode* m_pNode;
    MindMapScene* m_scene;
};

//TODO: ¹ØÁª±Ê¼Ç

class DragOutTransform : public ITransform
{
public:
    DragOutTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* parent);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    MindMapScene* m_scene;
    MindNode* m_node;
    MindNode* m_parent;
    int m_remove_idx;
};

class DragInTransform : public ITransform
{
public:
    DragInTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* newParent, int idx, bool toRight);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    MindMapScene* m_scene;
    MindNode* m_node;
    MindNode* m_parent;
    int m_idx;
    bool m_isToRight;
};


class MoveTransform : public ITransform
{
public:
    MoveTransform(MindMapScene* pScene, MindNode* pDragNode, MindNode* oldParent);
    void setNewParent(MindNode* pNewParent);
    bool forward();
    bool backward();
    bool prepare();

private:
    MindMapScene* m_scene;
    MindNode* m_pDragNode;
    MindNode* m_pOldParent;
    MindNode* m_pNewParent;
};


class DocumentTransform : public ITransform
{
public:
    DocumentTransform(MindMapScene* pScene);
    void setNewDocument(const QString& newDocument);
    bool forward();
    bool backward();
    bool prepare() { return true; }

private:
    MindMapScene* m_scene;
    QString m_oldDocument;
    QString m_newDocument;
};

class RAIITransBatch
{
public:
    RAIITransBatch(MindMapScene* scene);
    ~RAIITransBatch();
    void setResult(bool success);

private:
    MindMapScene* m_scene;
    TRANSCATION_PTR m_spTrans;
    shared_ptr<DocumentTransform> m_pTransform;
    int m_id;
    bool m_success;
};

class ManualTransBatch
{
public:
    ManualTransBatch(MindMapScene* scene);
    ~ManualTransBatch();
    void setResult(bool success);
    void startBatch();
    void endBatch();

private:
    MindMapScene* m_scene;
    shared_ptr<DocumentTransform> m_pTransform;
    int m_id;
    bool m_success;
};

#endif