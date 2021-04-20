#include "stdafx.h"
#include "menubutton.h"
#include "moc_menubutton.cpp"
#include "MyStyle.h"
#include "toolbuttonstyleoption.h"


MenuButton::MenuButton(QWidget* parent)
	: ToolButton(parent)
{
	setButtonStyle(ButtonIcon | ButtonDownArrow);
	setMouseTracking(true);
	connect(this, SIGNAL(popup()), this, SLOT(popupChildWidget()));
}

MenuButton::~MenuButton()
{
}

void MenuButton::popupChildWidget()
{

}

void MenuButton::initStyleOption(StyleOptionToolButton* option) const
{
	ToolButton::initStyleOption(option);
	if (option->rect.width() > 50)
	{
		int j;
		j = 0;
	}
	option->features |= QStyleOptionToolButton::Menu;
	option->triggerAble = true;
}

void MenuButton::paintEvent(QPaintEvent* event)
{
	ToolButton::paintEvent(event);
}

bool MenuButton::event(QEvent* e)
{
	return ToolButton::event(e);
}