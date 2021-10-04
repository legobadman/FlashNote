#ifndef __LINK_NOTE_ITEM_H__
#define __LINK_NOTE_ITEM_H__

#include <QtSvg/QGraphicsSvgItem>

class LinkNoteItem : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    LinkNoteItem(const QString& fileName, QGraphicsItem* parentItem = nullptr);

protected:
    bool sceneEvent(QEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void triggered();
};

#endif
