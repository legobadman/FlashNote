#ifndef __NOTE_COMPLETER_H__
#define __NOTE_COMPLETER_H__

#include <QCompleter>

class SearchEditor;

class NoteCompleter : public QCompleter
{
    Q_OBJECT
public:
    NoteCompleter(SearchEditor* pEditor, QObject* parent = nullptr);
    NoteCompleter(SearchEditor* pEditor, QAbstractItemModel* model, QObject* parent = nullptr);
    QString pathFromIndex(const QModelIndex& index) const override;

private:
    SearchEditor* m_pEditor;
};


#endif
