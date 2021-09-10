#include "stdafx.h"
#include "menubutton.h"
#include "moc_menubutton.cpp"
#include "MyStyle.h"
#include "toolbuttonstyleoption.h"


MenuButton::MenuButton(QWidget* parent)
	: ToolButton(parent)
	, func_createContentWid(NULL)
{
	setButtonStyle(ButtonText | ButtonIcon | ButtonDownArrow);
	setMouseTracking(true);
	connect(this, SIGNAL(clicked()), this, SIGNAL(popup()));
	connect(this, SIGNAL(popup()), this, SLOT(popupChildWidget()));
}

MenuButton::~MenuButton()
{
}

void MenuButton::setCreateContentCallback(std::function<QWidget* ()> func)
{
	func_createContentWid = func;
}

void MenuButton::popupChildWidget()
{
	if (func_createContentWid)
	{
		PopupWidget popup(this);

		connect(this, SIGNAL(popout()), &popup, SIGNAL(aboutToHide()));

		QWidget* pContentWidget = func_createContentWid();
		popup.setContentWidget(pContentWidget);

		QPoint pGlobal = mapToGlobal(QPoint(0, 0));
		const int margin = 5;
		setDown(true);

		int nWidth = 300; pContentWidget->width();
		int nHeight = pContentWidget->height();

		popup.exec(pGlobal.x(), pGlobal.y() + height() + margin, nWidth, nHeight);
		setDown(false);
	}
	
}

void MenuButton::initStyleOption(StyleOptionToolButton* option) const
{
	ToolButton::initStyleOption(option);
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