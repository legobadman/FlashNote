#ifndef __SEARCHITEM_DELEGATE_H__
#define __SEARCHITEM_DELEGATE_H__

class QAbstractItemView;
class GlobalSearchEditor;

class SearchItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SearchItemDelegate(QObject* parent, GlobalSearchEditor* pEditor);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;

private:
    QAbstractItemView* m_view;
    GlobalSearchEditor* m_pEditor;
};

#endif