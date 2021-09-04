#include "stdafx.h"
#include "floatingmenubutton.h"
#include "MyStyle.h"


FloatingMenuButton::FloatingMenuButton(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("floating_window");
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	m_menubutton = new ToolButton(this);
	m_menubutton->setIcon(QIcon(":/icons/24x24/floatwin.png"));
	m_menubutton->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_menubutton->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	connect(m_menubutton, SIGNAL(clicked()), this, SLOT(hide()));
}

FloatingMenuButton::~FloatingMenuButton()
{

}

bool FloatingMenuButton::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	if (message)
	{
		MSG* msg = reinterpret_cast<MSG*>(message);
		if (msg->message == WM_COPYDATA)
		{
			COPYDATASTRUCT* data = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);
			POINT* pGloal = reinterpret_cast<POINT*>(data->lpData);
			//取出剪贴板数据
			this->setGeometry(QRect(pGloal->x, pGloal->y, 24, 24));
			this->show();
		}
	}
	return QWidget::nativeEvent(eventType, message, result);
}