#include "stdafx.h"
#include "toolbutton.h"
#include "moc_toolbutton.cpp"
#include "MyStyle.h"
#include "toolbuttonstyleoption.h"


ToolButton::ToolButton(QWidget* parent)
	: QWidget(parent)
	, m_bDown(false)
	, m_bChecked(false)
	, m_bPressed(false)
	, m_bHideText(false)
	, m_buttonStyle(ButtonIcon)
{
	setMouseTracking(true);
}

ToolButton::~ToolButton()
{
}

QString ToolButton::text() const
{
	return m_text;
}

QIcon ToolButton::icon() const
{
	return m_icon;
}

QSize ToolButton::iconSize() const
{
	return m_iconSize;
}

bool ToolButton::isChecked() const
{
	return m_bChecked;
}

bool ToolButton::isDown() const
{
	return m_bDown;
}

bool ToolButton::isPressed() const
{
	return m_bPressed;
}

int ToolButton::buttonStyle() const
{
	return m_buttonStyle;
}

QSize ToolButton::sizeHint() const
{
	return QWidget::sizeHint();
}

void ToolButton::initStyleOption(StyleOptionToolButton* option) const
{
	if (!option)
		return;

	option->initFrom(this);
	if (isChecked())
		option->state |= QStyle::State_On;
	if (isDown() || isPressed())
		option->state |= QStyle::State_Sunken;
	if (!isChecked() && !isDown() && !isPressed())
		option->state |= QStyle::State_Raised;
	option->state |= QStyle::State_AutoRaise;

	option->icon = icon();
	option->iconSize = iconSize();
	option->text = text();
	option->buttonStyle = buttonStyle();
	option->bDown = isDown();

	if (!(buttonStyle() & (ButtonDownArrow | ButtonRightArrow)))
		option->subControls = QStyle::SC_ToolButton;

	option->hideText = m_bHideText;
}

void ToolButton::setIcon(const QIcon& icon)
{
	m_icon = icon;
}

void ToolButton::setIconSize(const QSize& size)
{
	m_iconSize = size;
}

void ToolButton::showToolTip()
{
	QPoint pt = QCursor::pos();
	QHelpEvent* e = new QHelpEvent(QEvent::ToolTip, mapFromGlobal(pt), pt);
	QApplication::postEvent(this, e);
}

void ToolButton::setChecked(bool bChecked)
{
	if (bChecked == m_bChecked)
		return;
	m_bChecked = bChecked;
	update();
}

void ToolButton::setShortcut(QString text)
{

}

void ToolButton::setDown(bool bDown)
{
	if (bDown == m_bDown)
		return;
	m_bDown = bDown;
	update();
}

void ToolButton::setPressed(bool bPressed)
{
	if (bPressed == m_bPressed)
		return;
	m_bPressed = bPressed;
	update();
}

void ToolButton::setButtonStyle(int style)
{
	m_buttonStyle = style;
}

bool ToolButton::event(QEvent* e)
{
	return QWidget::event(e);
}

void ToolButton::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		emit LButtonPressed();
		setPressed(true);
	}
}

void ToolButton::mouseReleaseEvent(QMouseEvent* e)
{
	setPressed(false);
	if (m_buttonStyle & ButtonCheckable)
	{
		setChecked(!m_bChecked);
	}
	emit clicked();
}

void ToolButton::setCustomTip(QString tip)
{
	m_customTip = tip;
}

QString ToolButton::getCustomTip()
{
	return m_customTip;
}

void ToolButton::setText(const QString& text)
{
	m_text = text;
}

void ToolButton::enterEvent(QEvent* e)
{
	update();
}

void ToolButton::leaveEvent(QEvent* e)
{
	update();
}

void ToolButton::updateIcon()
{
	update();
}

void ToolButton::paintEvent(QPaintEvent* event)
{
	QStylePainter p(this);
	StyleOptionToolButton option;
	initStyleOption(&option);
	p.drawComplexControl(static_cast<QStyle::ComplexControl>(MyStyle::CC_MyToolButton), option);
}