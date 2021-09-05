#ifndef __FLOATING_MENUBUTTON_H__
#define __FLOATING_MENUBUTTON_H__

#include "menubutton.h"

#define DEBUG_EXTRACT_TEXT

class FloatingMenuButton : public QWidget
{
	Q_OBJECT
public:
	FloatingMenuButton(QWidget* parent);
	~FloatingMenuButton();
	void SetExtractText(const QString& text);

protected:
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;

private:
	QString m_text;
#ifdef DEBUG_EXTRACT_TEXT
	QPushButton* m_pushbutton;
#else
	ToolButton* m_menubutton;
#endif
};


#endif