#ifndef __LEFTSIDE_ITEM_DELEGATE_H__
#define __LEFTSIDE_ITEM_DELEGATE_H__

enum LEFT_SIDE_TYPE
{
	ITEM_NEWNOTE,
	ITEM_ALLNOTE,
	ITEM_NOTEBOOK,
	ITEM_MATERIAL,
	ITEM_SOCIALMEDIA,
	ITEM_DIARY,
	ITEM_SCHEDULE,
	ITEM_DRAFT,
	ITEM_TRASH,
};

class LeftSideItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	LeftSideItemDelegate(QWidget* parent);
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void drawBackground(QStyleOptionViewItem& opt, QPainter* p, const QWidget* widget) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

#endif
