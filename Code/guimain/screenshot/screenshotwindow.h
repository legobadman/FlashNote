#ifndef __SCREENSHOT_WINDOW_H__
#define __SCREENSHOT_WINDOW_H__

#include <QLabel>
#include "screenwidget.h"

class ScreenshotWindow : public QMainWindow
{
	typedef QMainWindow base;
	Q_OBJECT
public:
	ScreenshotWindow(QWidget* parent = nullptr);
	~ScreenshotWindow();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QLabel* m_lblImage;
	ScreenWidget* m_screenWidget;
	QPixmap m_screenPixmap;
};

#endif