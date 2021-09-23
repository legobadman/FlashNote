#ifndef __BOOKMENU_BUTTON_H__
#define __BOOKMENU_BUTTON_H__

#include "menubutton.h"

class BookMenuButton : public MenuButton
{
    Q_OBJECT
public:
    BookMenuButton(QWidget* parent = NULL);
    QSize sizeHint() const override;
    void setBookName(const QString& bookName);

protected:
    void paintEvent(QPaintEvent* event);
};

#endif