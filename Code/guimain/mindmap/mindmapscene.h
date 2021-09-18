#ifndef __MINDMAP_SCENE_H__
#define __MINDMAP_SCENE_H__

#include <QtWidgets/QGraphicsScene>
#include "mindnode.h"
#include "mindprogressnode.h"
#include "../transaction/transaction.h"
#include "rapidxml.hpp"

using namespace rapidxml;

typedef xml_node<> XML_NODE;


class TransEndEvent : public QEvent
{
public:
	static const Type TYPE = static_cast<Type>(QEvent::User + 0x10);
	explicit TransEndEvent() : QEvent(TYPE) {}
};


class MindMapScene : public QGraphicsScene
{
	Q_OBJECT
public:
	MindMapScene(QObject* parent = nullptr);
	~MindMapScene();
	void initContent(QString content, bool bSchedule);
	QString mindmapXML();
	MindNode* root() const { return m_pRoot; }
	TranRepository* transRepository() { return m_repo.get(); }
	void undo();
	void redo();
	void reconstruct(const QString& content);
	void refresh(bool bEditChanged = false);
	void startMoveTransaction();
	void emitUndoRedoEnable();

protected:
	bool event(QEvent* event) override;

signals:
	void itemContentChanged(bool bEditChanged);
	void undoRedoEnable(bool, bool);

private slots:
	void onNodeContentsChanged();
	void onNodeStateChanged();
	void onNodeCreated(MindNode* pNode);
	void onNodeDeleted(MindNode* pNode);
	void onNodeDragged(MindNode* pNode);
	void onNodeDragging(MindNode* pNode);

private:
	void onRedrawItems(bool bDrawHolder = false);
	void arrangeAllItems(bool bDrawHolder);
	MindNode* parseXML(const std::string& content);
	XML_NODE* _export(MindNode* root, xml_document<>& doc);
	MindNode* _parse(MindNode* parent, xml_node<>* root, int level);

	MindNode* m_pRoot;
	MindNode* m_pHolder;
	QList<QGraphicsPathItem*> m_pathItems;
	QSet<MindNode*> m_items;
	unique_ptr<TranRepository> m_repo;
	QShortcut* m_undo;
	QShortcut* m_redo;
	QString m_content;
	ManualTransBatch m_moveBatch;
	bool m_bSchedule;
};

#endif
