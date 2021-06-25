#ifndef __FONT_ITEM_DELEGATE_H__
#define __FONT_ITEM_DELEGATE_H__

#include <QtWidgets/QAbstractItemDelegate>
#include <QtGui/QFontDatabase>

class FontItemDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
public:
	explicit FontItemDelegate(QObject* parent);

	// painting
	void paint(QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;

	const QIcon truetype;
	const QIcon bitmap;
	QFontDatabase::WritingSystem writingSystem;
};

#endif
