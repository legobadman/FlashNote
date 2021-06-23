#ifndef __MIND_PROGRESS_NODE_H__
#define __MIND_PROGRESS_NODE_H__

#include "mindnode.h"

class MindProgressNode : public MindNode
{
	Q_OBJECT
public:
	MindProgressNode(const QString& text, MindProgressNode* parent = NULL);
	~MindProgressNode();
	void setup() override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

	float progress();
	void setProgress(float progress);
	float workHours();
	void setWorkhours(float hours);
	void updateStatus();
	void updateToParent();

public slots:
	void setWorkingHourDlg();
	void markFinish();
	void zeroSchedule();

private:
	void _setProgress(float progress);
	void _setWorkhours(float hours);
	void updateToolTip();
	void updateNodeColor();

protected:
	void initMenu() override;
	void initDecoration() override;
	bool sceneEvent(QEvent* event) override;

private:
	float m_progress;
	float m_workinghours;
};


#endif
