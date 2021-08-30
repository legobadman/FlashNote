#ifndef __SCREENSHOT_WINDOW_H__
#define __SCREENSHOT_WINDOW_H__

#include <QLabel>
#include "screenwidget.h"
#include "screentoolbar.h"

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

private slots:
	void onGrabFinish();

private:
	ScreenShotWidget* m_view;
	ScreenToolBar* m_toolbar;
};

#endif