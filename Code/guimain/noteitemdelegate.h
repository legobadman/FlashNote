#ifndef __NOTEITEMDELEGATE_H__
#define __NOTEITEMDELEGATE_H__

#include "notelistview.h"

class NoteItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	NoteItemDelegate(QAbstractItemView* parent, QLineEdit* pEditor);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;

private:
	QVector<QTextLayout::FormatRange> _getSearchFormatRange(const QString& content) const;

	QAbstractItemView* m_view;
	QLineEdit* m_pEditor;
};

#endif
