#ifndef __MENUBUTTON_H__
#define __MENUBUTTON_H__

#include "toolbutton.h"

class MenuButton : public ToolButton
{
	Q_OBJECT
public:
	MenuButton(QWidget* parent = NULL);
	~MenuButton();

signals:
	void trigger();
	void popup();
protected slots:
	virtual void popupChildWidget();

protected:
	virtual bool event(QEvent* e);
	void initStyleOption(StyleOptionToolButton* option) const;
	void paintEvent(QPaintEvent* event) override;
};

#endif