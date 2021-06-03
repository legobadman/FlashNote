#ifndef __NOTEITEMDELEGATE_H__
#define __NOTEITEMDELEGATE_H__

class NoteItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	NoteItemDelegate(QWidget* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

#endif
