#ifndef __SCREENSHOT_WINDOW_H__
#define __SCREENSHOT_WINDOW_H__

#include <QLabel>
#include "screenwidget.h"

//#define USE_WIDGET

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
#ifdef USE_WIDGET
	ScreenWidget* m_screenWidget;
#else
	ScreenShotWidget* m_view;
#endif
	QPixmap m_screenPixmap;
};

#endif