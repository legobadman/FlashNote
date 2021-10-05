#ifndef __FLOATING_MENUBUTTON_H__
#define __FLOATING_MENUBUTTON_H__

#include "menubutton.h"

class FloatingMenuButton : public QWidget
{
	Q_OBJECT
public:
	FloatingMenuButton(QWidget* parent);
	~FloatingMenuButton();
	void SetExtractText(const QString& text);

protected:
	void enterEvent(QEvent* event) override;
	void leaveEvent(QEvent* event) override;

private slots:
	void onBtnClicked();

private:
	QString m_text;
	ToolButton* m_menubutton;
};


#endif