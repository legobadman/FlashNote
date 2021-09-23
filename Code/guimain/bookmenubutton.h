#ifndef __BOOKMENU_BUTTON_H__
#define __BOOKMENU_BUTTON_H__

#include "menubutton.h"

class BookMenuButton : public MenuButton
{
    Q_OBJECT
public:
    BookMenuButton(QWidget* parent = NULL);
    ~BookMenuButton();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event);
};

#endif