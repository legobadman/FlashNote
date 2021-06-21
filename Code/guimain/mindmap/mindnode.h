#ifndef __MIND_NODE_H__
#define __MIND_NODE_H__

#include <QGraphicsItem>
#include "mindnodebutton.h"

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

public:
	MindNode(const QString& text, MindNode* parent = NULL);
	~MindNode();
	virtual void setup();
	void setHintButton(MindNodeButton* pBtn) {
		m_pBtn = pBtn; m_pBtn->setVisible(false);
	}
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	const QList<MindNode*>& children() const { return m_children; }
	void setParent(MindNode* pNode) { m_parent = pNode; }
	MindNode* Parent() const { return m_parent; }
	void setLevel(float level) { m_level = level; }
	void insert(int i, MindNode* pNode) { m_children.insert(i, pNode); }
	void removeNode(MindNode* pNode);
	void setLevel(int nLevel) { m_level = nLevel; }
	bool isToRight() { return m_bToRight; }
	void setToRight(bool toRight) { m_bToRight = toRight; }
	int level() const { return m_level; }
	void setBackground(QColor color) { m_backgroudColor = color; }
	void setFocusoutBorder(QColor color) { m_borderFocusout = color; };
	void setHighlightedBorder(QColor color) { m_highlightedBorder = color; }
	void setSelectedBorder(QColor color) { m_selectedBorder = color; }

signals:
	void textChange();
	void dataChanged();
	void childNodeCreate(MindNode* parent, bool bRight);
	void silibingNodeCreate(MindNode* pNode);
	void deleteNode(MindNode* pNode);

public slots:
	void onDocumentContentsChanged(int, int, int);
	void onCreateChildNodeRight();
	void onCreateChildNodeLeft();
	void onCreateSliblingNode();
	void onDeleteNode();

public:
	void SetContent(const std::wstring& content);
	std::wstring GetContent() const;
	void append(MindNode* pNode);

protected:
	void init();
	void initDirection();
	bool sceneEvent(QEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	void initDocFormat(const QString& text);
	int pointSize(int level) const;
	void udpateBorderFormat(const QStyleOptionGraphicsItem* option);

protected:
	QString m_content;
	QColor m_textColor;
	QColor m_backgroudColor;
	QColor m_highlightedBorder;
	QColor m_selectedBorder;
	QColor m_borderFocusout;
	int m_level;
	int m_borderWidth;
	int m_cornerRadius;
	int m_counter;	//防止绘制重入。
	MOUSE_STATE m_mouseState;
	bool m_bHovered;
	bool m_bToRight;	//子节点向右扩展。
	MindNode* m_parent;
	MindNodeButton* m_pBtn;
	QList<MindNode*> m_children;
};

#endif
