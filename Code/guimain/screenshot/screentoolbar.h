#ifndef __SCREEN_TOOLBAR_H__
#define __SCREEN_TOOLBAR_H__

#include "toolbutton.h"

class ScreenToolBar : public QWidget
{
	Q_OBJECT
public:
	ScreenToolBar(QWidget* parent);
	~ScreenToolBar();

protected:
	void paintEvent(QPaintEvent* event);

signals:
	void closeTriggered();
	void finishTriggered();
	void saveTriggered();

private:
	ToolButton* m_pSave;
	ToolButton* m_pFinish;
	ToolButton* m_pClose;
};

#endif
