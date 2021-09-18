#ifndef __LABEL_BUTTON_H__
#define __LABEL_BUTTON_H__


class NLabelButton : public QLabel
{
    Q_OBJECT
public:
    NLabelButton(QWidget* parent = NULL);
    void setIcons(const QSize& sz, const QIcon& iconEnable, const QIcon& iconDisable, const QIcon& iconHover);

protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void paintEvent(QPaintEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

signals:
    void clicked(void);

private:
    QIcon m_iconEnable;
    QIcon m_iconDisable;
    QIcon m_iconHover;
    QSize m_iconSize;
    bool m_bHovered;
};

#endif