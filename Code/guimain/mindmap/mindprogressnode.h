#ifndef __MIND_PROGRESS_NODE_H__
#define __MIND_PROGRESS_NODE_H__

#include "mindnode.h"

class MindProgressNode : public MindNode
{
	Q_OBJECT
public:
	MindProgressNode(const QString& text, MindNode* parent);
	~MindProgressNode();
	void setup(MindMapScene*) override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	float progress();
	void setProgress(float progress);
	float workHours();
	void setWorkhours(float hours);
	void updateStatus();
	void updateToParent();
	void setPosition(QPointF pos) override;

public slots:
	void setWorkingHourDlg();
	void markFinish();
	void zeroSchedule();
	virtual void onDeleteNode() override;

private:
	void _setProgress(float progress);
	void _setWorkhours(float hours);
	void updateToolTip();
	void updateNodeColor();
	void adjustIndicator();
	void updateTipIcon();

protected:
	void initMenu() override;
	void initUIColor() override;
	void resetDecoration() override;
	bool sceneEvent(QEvent* event) override;

private:
	float m_progress;
	float m_workinghours;
	QGraphicsPixmapItem* m_tipItem;
};


#endif
