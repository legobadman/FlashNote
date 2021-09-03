#ifndef __FLOATING_MENUBUTTON_H__
#define __FLOATING_MENUBUTTON_H__

#include "menubutton.h"

class FloatingMenuButton : public QWidget
{
	Q_OBJECT
public:
	FloatingMenuButton(QWidget* parent);
	~FloatingMenuButton();

protected:
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

private:
	ToolButton* m_menubutton;
};


#endif