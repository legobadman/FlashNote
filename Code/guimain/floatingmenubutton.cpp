#include "stdafx.h"
#include "floatingmenubutton.h"
#include "MyStyle.h"


FloatingMenuButton::FloatingMenuButton(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("floating_window");
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#ifdef DEBUG_EXTRACT_TEXT
	m_pushbutton = new QPushButton(this);
	m_pushbutton->setText("init");
	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->addWidget(m_pushbutton);
	setLayout(pLayout);
#else
    m_menubutton = new ToolButton(this);
    m_menubutton->setIcon(QIcon(":/icons/24x24/floatwin.png"));
    m_menubutton->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));
    m_menubutton->setFixedSize(MyStyle::dpiScaledSize(QSize(124, 24)));
    connect(m_menubutton, SIGNAL(clicked()), this, SLOT(hide()));
#endif
}

FloatingMenuButton::~FloatingMenuButton()
{

}

void FloatingMenuButton::SetExtractText(const QString& text)
{
	m_text = text;
#ifdef DEBUG_EXTRACT_TEXT
	m_pushbutton->setText(text);
	update();
#else
	m_menubutton->setText(m_text);
#endif
}

void FloatingMenuButton::enterEvent(QEvent* event)
{
}

void FloatingMenuButton::leaveEvent(QEvent* event)
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