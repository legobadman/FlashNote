#ifndef __LIST_PANE__H__
#define __LIST_PANE__H__

class NavigationPanel : public QTreeView
{
public:
	NavigationPanel(QWidget* parent = nullptr);
	~NavigationPanel();

protected:
	void paintEvent(QPaintEvent* event) override;
};

#endif
