#ifndef __MIND_NODE_H__
#define __MIND_NODE_H__

#include <QGraphicsItem>
#include "mindnodebutton.h"

class MindTextNode : public QGraphicsTextItem
{
	Q_OBJECT
	enum MOUSE_STATE
	{
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
	MindTextNode(const QString& text, MindTextNode* parent = NULL);
	~MindTextNode();
	void setup();
	void setHintButton(MindNodeButton* pBtn) {
		m_pBtn = pBtn; m_pBtn->setVisible(false);
	}
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	const QList<MindTextNode*>& children() const { return m_children; }
	void setParent(MindTextNode* pNode) { m_parent = pNode; }
	MindTextNode* Parent() const { return m_parent; }
	void setLevel(float level) { m_level = level; }
	void insert(int i, MindTextNode* pNode) { m_children.insert(i, pNode); }
	void setLevel(int nLevel) { m_level = nLevel; }
	int level() const { return m_level; }
	void setBackground(QColor color) { myBackgroundColor = color; }
	void setFocusoutBorder(QColor color) { m_borderFocusout = color; };
	void setHighlightedBorder(QColor color) { m_highlightedBorder = color; }
	void setSelectedBorder(QColor color) { m_selectedBorder = color; }

signals:
	void contentsChange();
	void childNodeCreate(MindTextNode* parent);
	void silibingNodeCreate(MindTextNode* pNode);

public slots:
	void onDocumentContentsChanged(int, int, int);
	void onCreateChildNode();
	void onCreateSliblingNode();

public:
	void SetContent(const std::wstring& content);
	std::wstring GetContent() const;
	void SetProgress(float progress);
	float GetProgress() const;
	bool IsProgress() const;
	void append(MindTextNode* pNode);

protected:
	bool sceneEvent(QEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	void init();
	void initDocFormat(const QString& text);
	int pointSize(int level) const;
	void udpateBorderFormat(const QStyleOptionGraphicsItem* option);

private:
	QString myText;
	QColor myTextColor;
	QColor myBackgroundColor;
	QColor m_highlightedBorder;
	QColor m_selectedBorder;
	QColor m_borderFocusout;
	int m_level;
	int m_borderWidth;
	int m_cornerRadius;
	int m_counter;	//∑¿÷πªÊ÷∆÷ÿ»Î°£
	float m_progress;
	MOUSE_STATE m_mouseState;
	bool m_bHovered;
	bool m_bProgress;
	MindTextNode* m_parent;
	MindNodeButton* m_pBtn;
	QList<MindTextNode*> m_children;
};

#endif
