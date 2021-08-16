#ifndef __NOTEITEMDELEGATE_H__
#define __NOTEITEMDELEGATE_H__

#include "notelistview.h"

class NoteItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	NoteItemDelegate(NotesListView* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;

private:
	NotesListView* m_pListView;
};

#endif
