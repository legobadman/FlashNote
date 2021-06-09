#ifndef __NEWITEM_DELEGATE_H__
#define __NEWITEM_DELEGATE_H__

class NewItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	NewItemDelegate(QWidget* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

#endif
