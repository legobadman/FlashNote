#include "stdafx.h"
#include "notecompleter.h"
#include "LeftSideItemDelegate.h"
#include "searcheditor.h"


NoteCompleter::NoteCompleter(SearchEditor* pEditor, QObject* parent)
    : QCompleter(parent)
    , m_pEditor(pEditor)
{
}

NoteCompleter::NoteCompleter(SearchEditor* pEditor, QAbstractItemModel* model, QObject* parent)
    : QCompleter(model, parent)
    , m_pEditor(pEditor)
{
}

QString NoteCompleter::pathFromIndex(const QModelIndex& index) const
{
    QAbstractItemModel* sourceModel = this->model();
    if (!sourceModel)
        return QString();
    
    return m_pEditor->text();
}