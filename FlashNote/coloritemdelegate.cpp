#include "stdafx.h"
#include "coloritemdelegate.h"
#include "MyStyle.h"
#include "moc_coloritemdelegate.cpp"

#define ROW_GALLERY_VIEW 5
#define COLUMN_GALLERY_VIEW 10

static const int sLeftMargin = MyStyle::dpiScaled(6), sTopMargin = MyStyle::dpiScaled(6), sRightMargin = MyStyle::dpiScaled(6);
static const int sIconSize = 16;
static const int sFirstItemH = 24, sFirstItemW = 99;
static const int sYoffsetFromFirstToTable = 4, sTableItemMargin = 4, sYoffsetFromTableToLast = 4;
static const int sTableItemSize = 20;
static const int sLastItemH = 26, sLastItemW = 108;
static const int sBottomMargin = 6;



ColorItemDelegate::ColorItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

QSize ColorItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return MyStyle::dpiScaledSize(QSize(30, 30));
}

void ColorItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	//static const int sLeftMargin = MyStyle::dpiScaled(6), sTopMargin = MyStyle::dpiScaled(6), sRightMargin = MyStyle::dpiScaled(6);
	//static const int sFirstItemH = 24, sFirstItemW = 99;
	//static const int sYoffsetFromFirstToTable = 4, sColorItemMargin = 4, sYoffsetFromTableToLast = 4;
	//static const int sLastItemH = 26, sLastItemW = 108;
	//static const int sBottomMargin = 6;

	//int r = index.row();
	//if (r == 0)
	//{
	//	QPainterPath border_path;
	//	border_path.addRoundedRect(QRectF(sLeftMargin, sTopMargin, sFirstItemW, sFirstItemH), 2, 2);
	//	QColor borderClr(227, 232, 237);
	//	painter->setPen(borderClr);
	//	painter->setBrush(Qt::NoBrush);
	//	painter->drawPath(border_path);
	//}
}

void ColorItemDelegate::paintFirstItem(QPainter* painter, const QStyleOptionViewItem& option_, const QModelIndex& index) const
{
	QStyleOptionViewItem option = option_;

	QPainterPath border_path;
	border_path.addRoundedRect(QRectF(sLeftMargin, sTopMargin, sFirstItemW, sFirstItemH), 1, 1);
	QColor borderClr(207, 207, 207);
	QPen pen(borderClr);
	//pen.setWidth(1);
	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPath(border_path);

	static const int sLeftMarginToIcon = 6, sTopMarginToIcon = 4;
	QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
	QRect rcIcon = QRect(sLeftMargin + sLeftMarginToIcon, sTopMargin + sTopMarginToIcon, 20, 20);
	icon.paint(painter, rcIcon, Qt::AlignTop | Qt::AlignLeft);

	static const int sTextLeftMargin = sLeftMargin + sLeftMarginToIcon + sIconSize + 4;
	static const int sBorderToTextTopMargin = 3;
	static const int sTextRectWidth = 80, sTextRectHeight = 17;
	QString text(u8"默认颜色(D)");
	pen.setColor(QColor(0, 0, 128));
	painter->setPen(pen);
	QRect rcText = QRect(sTextLeftMargin, sTopMargin + sBorderToTextTopMargin, sTextRectWidth, sTextRectHeight);
	painter->drawText(rcText, text);
}

void ColorItemDelegate::paintColorTable(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QPainterPath path;

	//行数从1开始
	int r = index.row() - 1, c = index.column();
	int yoffset = sTopMargin + sFirstItemH + sYoffsetFromFirstToTable;
	QRectF rc = QRectF(sLeftMargin + c * (sTableItemSize + sTableItemMargin), yoffset + r * (sTableItemSize + sTableItemMargin), sTableItemSize, sTableItemSize);
	path.addRect(rc);
	painter->fillPath(path, m_color);
}

void ColorItemDelegate::paintLastItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QPainterPath border_path;
	int yoffset = sTopMargin + sFirstItemH + sYoffsetFromFirstToTable + ROW_GALLERY_VIEW * sTableItemSize
		+ (ROW_GALLERY_VIEW - 1) * sTableItemMargin + sYoffsetFromTableToLast;
	border_path.addRoundedRect(QRectF(sLeftMargin, yoffset, sLastItemW, sLastItemH), 1, 1);
	QColor borderClr(207, 207, 207);
	QPen pen(borderClr);
	//pen.setWidth(1);
	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPath(border_path);

	static const int sLeftMarginToIcon = 6, sTopMarginToIcon = 5;
	QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
	QRect rcIcon = QRect(sLeftMargin + sLeftMarginToIcon, yoffset + sTopMarginToIcon, 20, 20);
	icon.paint(painter, rcIcon, Qt::AlignTop | Qt::AlignLeft);

	static const int sTextLeftMargin = sLeftMargin + sLeftMarginToIcon + sIconSize + 4;
	static const int sBorderToTextTopMargin = 4;
	static const int sTextRectWidth = 80, sTextRectHeight = 17;
	QString text(u8"更多颜色(M)");
	pen.setColor(QColor(0, 0, 128));
	painter->setPen(pen);
	QRect rcText = QRect(sTextLeftMargin, yoffset + sBorderToTextTopMargin, sTextRectWidth, sTextRectHeight);
	painter->drawText(rcText, text);
}

void ColorItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->setRenderHint(QPainter::Antialiasing);
	if (index.row() == 0)
	{
		return paintFirstItem(painter, option, index);
	}
	else if (index.row() == ROW_GALLERY_VIEW + 1)
	{
		return paintLastItem(painter, option, index);
	}
	else
	{
		return paintColorTable(painter, option, index);
	}
}