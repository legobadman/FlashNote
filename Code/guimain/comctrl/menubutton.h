#ifndef __MENUBUTTON_H__
#define __MENUBUTTON_H__

#include "toolbutton.h"
#include "popupwidget.h"

class MenuButton : public ToolButton
{
	Q_OBJECT
public:
	MenuButton(QWidget* parent = NULL);
	~MenuButton();
	void setCreateContentCallback(std::function<QWidget* ()> func);

signals:
	void trigger();
	void popup();
	void popout();
protected slots:
	virtual void popupChildWidget();

protected:
	virtual bool event(QEvent* e);
	void initStyleOption(StyleOptionToolButton* option) const;
	void paintEvent(QPaintEvent* event) override;

protected:
	std::function<QWidget* ()> func_createContentWid;
};

#endif