#ifndef __SCREEN_WIDGET_H__
#define __SCREEN_WIDGET_H__

class ScreenWidget : public QWidget
{
	Q_OBJECT
public:
	ScreenWidget(QWidget* parent);
	~ScreenWidget();

private:
	QLabel* m_lblImage;
	QPixmap m_origialShot;
};

#endif