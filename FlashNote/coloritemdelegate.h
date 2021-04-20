#ifndef __COLOR_ITEM_DELEGATE_H__
#define __COLOR_ITEM_DELEGATE_H__

class ColorItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ColorItemDelegate(QWidget* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;

private:
	void paintFirstItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void paintColorTable(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void paintLastItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QColor m_color;
};

#endif