#ifndef __COLOR_ITEM_DELEGATE_H__
#define __COLOR_ITEM_DELEGATE_H__

#include "colortableview.h"

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
	QColor m_color;
	QPointer<ColorTableView> m_view;
};

#endif