#ifndef __LIST_PANE__H__
#define __LIST_PANE__H__

class NavigationPanel : public QTreeView
{
	Q_OBJECT
public:
	NavigationPanel(QWidget* parent = nullptr);
	~NavigationPanel();

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	QStandardItemModel* leftsidemodel;
};

#endif
