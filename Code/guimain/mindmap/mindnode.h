#ifndef __MIND_NODE_H__
#define __MIND_NODE_H__

#include <QGraphicsItem>
#include <memory>
#include "mindnodebutton.h"
#include "common_types.h"

class MindMapScene;

class RoundedRectItem;

class MindNode : public QGraphicsTextItem
{
	Q_OBJECT
	enum MOUSE_STATE
	{
		MS_UNKNOWN,
		MS_FOCUSOUT,
		MS_SELECTED,
		MS_HOVERED,
	};

	struct UpdateBatch
	{
		UpdateBatch(int* m_count) : pCounter(m_count) { (*pCounter)++; }
		~UpdateBatch() { (*pCounter)--; }
		int* pCounter;
	};

	struct DraggingCache
	{
		DraggingCache() : m_pOrginalParent(NULL), m_idx(-1) {}
		MindNode* m_pOrginalParent;
		int m_idx;
	};

public:
	MindNode(const QString& text, MindNode* parent);
	virtual ~MindNode();
	virtual void setup(MindMapScene* pScene);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	
	//direction: -1  all direction    0: only left     1: only right
	QList<MindNode*> Children(bool excludeHolder = true, int direction = -1) const;
	QRectF childrenOrSelfRect(bool isToRight) const;

	void removeChild(MindNode* pNode);
	void insertChild(MindNode* pNode, int idx);
	MindNode* Parent() const { return m_parent; }
	bool isTopRoot() const { return m_parent == NULL; }
	void setLevel(int nLevel) { m_level = nLevel; }
	int level() const { return m_level; }
	bool isToRight() const { return m_bToRight; }
	void setToRight(bool toRight) { m_bToRight = toRight; }
	void resetAllChildDirection(bool toRight);
	bool isDragging() const { return m_bDragging; }
	virtual bool isHolder() const { return false; }

	EXPAND_STATE leftExpandState() const { return m_left_expand; }
	EXPAND_STATE rightExpandState() const { return m_right_expand; }
	void setLeftExpand(bool bExpanded);
	void setRightExpand(bool bExpanded);

	bool isExpanded(MindNodeButton* pBtn);
	bool isCollapsed(bool bRight) const;
	bool hasDraggingInChildRect(QPointF scenePos, int& dir_idx, bool& toRight);
	void resetPosBeforeDragging();
	QString noteid() const { return m_noteid; }
	void setNoteId(const QString& noteid) { m_noteid = noteid; }

	void setBackground(QColor color) { m_backgroudColor = color; }
	void setFocusoutBorder(QColor color) { m_borderFocusout = color; };
	void setHighlightedBorder(QColor color) { m_highlightedBorder = color; }
	void setSelectedBorder(QColor color) { m_selectedBorder = color; }
	virtual void setPosition(QPointF pos);
	void AddChild(MindNode* pChild);
	virtual void NewChild(bool toRight);	//特指后续的创建而非IO初始化的创建。

	QPainterPath shape() const override;

signals:
	void textChange();
	void dataChanged(bool bEditChanged);
	void expandChanged();
	void nodeCreated(MindNode* pNode);
	void nodeDeleted(MindNode* pNode);
	void nodeDragged(MindNode* pNode);
	void nodeDragging(MindNode* pNode);

public slots:
	void onDocumentContentsChanged(int, int, int);
	virtual void onCreateChildNodeRight();
	void onCreateChildNodeLeft();
	void onCreateSliblingNode();
	virtual void onDeleteNode();
	void onCreateAssociateNote();
	void onEditAssociateNote();
	void onNewNote(const QString&);
	void onLeftExpandBtnToggle();
	void onRightExpandBtnToggle();

public:
	void SetContent(const QString& content);
	QString GetContent() const;
	QRectF boundingRect() const override;
	QRectF hierarchyRect(bool bToRight);
	QRectF childrenRect(bool bToRight) const;

protected:
	virtual void initUIColor();
	void initDirection();

	void createPathItem(const QColor& clr, Qt::PenStyle style);
	void setTextColor(const QColor& clr);
	void setMainThemeColor(const QColor& mainTheme);
	void setColors(const QColor& mainTheme, const QColor& bgClr, const QColor& selectedBdr, const QColor& highlightBlr, const QColor& focusOutBdr);
	int childNodeCount();
	bool hasNoChildren();
	QPointer<QMenu> getMenu();

	virtual void initMenu();
	virtual void resetDecoration();
	bool needShowDecoration() const;
	bool sceneEvent(QEvent* event) override;
	virtual bool sceneEventFilter(QGraphicsItem* watched, QEvent* event);
	void focusOutEvent(QFocusEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	void initSignalSlots(MindMapScene* pScene);
	void initDocFormat(const QString& text);
	int pointSize(int level) const;
	void udpateBorderFormat(const QStyleOptionGraphicsItem* option);
	void initExpandBtns();
	void checkRemoveExpandBtns(bool bToRight);
	qreal _dist(const QPointF& p1, const QPointF& p2);
	void _collaspe(bool toRight);
	bool _hasDraggingInSide(QPointF scenePos, bool toRight, int& dir_idx);

	QString m_content;
    QColor m_textColor;
    QColor m_backgroudColor;
    QColor m_highlightedBorder;
    QColor m_selectedBorder;
    QColor m_borderFocusout;
    QColor m_mainThemeColor;

	MOUSE_STATE m_mouseState;

    QSharedPointer<MindNodeButton> m_pLCollaspBtn;
    QSharedPointer<MindNodeButton> m_pRCollaspBtn;

	QGraphicsPathItem* m_pathItem;
    
    QPointer<QMenu> m_pMenu;
    QList<MindNode*> m_children;

	QString m_noteid;

    QPointF m_initClickScenePos;
    QPointF m_item_event_offset;

	MindNode* m_parent;
	MindMapScene* m_scene;
	DraggingCache m_dragging;

    int m_level;
    int m_borderWidth;
    int m_cornerRadius;
    int m_counter;	//防止绘制重入。
    const int iconSize = 24;

    EXPAND_STATE m_left_expand;
    EXPAND_STATE m_right_expand;

    bool m_bDragging;
    bool m_bHovered;
    bool m_bToRight;	//子节点向右扩展，根节点此属性无效。
};

class RoundedRectItem : public MindNode
{
public:
	RoundedRectItem(QGraphicsItem* parent, const QColor& clr, Qt::PenStyle style);
	QPainterPath shape() const override;
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	bool isHolder() const override { return true; }

private:
	const int m_radius = 5;
	QColor m_color;
};

#endif
