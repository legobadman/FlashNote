#include "stdafx.h"
#include "linknoteitem.h"


LinkNoteItem::LinkNoteItem(const QString& fileName, QGraphicsItem* parentItem)
    : QGraphicsSvgItem(fileName, parentItem)
{
    setAcceptHoverEvents(true);
    setFlags(ItemIsMovable | ItemSendsGeometryChanges);
}

bool LinkNoteItem::sceneEvent(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::GraphicsSceneHoverEnter:
        setCursor(QCursor(Qt::PointingHandCursor));
        break;
    case QEvent::GraphicsSceneHoverLeave:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    case QEvent::GraphicsSceneMouseRelease:
        emit triggered();
        break;
    }
    return QGraphicsItem::sceneEvent(event);
}

void LinkNoteItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    //½ûÖ¹ÒÆ¶¯
}