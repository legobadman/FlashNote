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
	m_view = qobject_cast<ColorGalleryView*>(parent);
}

QSize ColorItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return MyStyle::dpiScaledSize(QSize(30, 30));
}

void ColorItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	//if (m_view->isHoverIndex(index))
	//{
	//	option->state |= QStyle::State_MouseOver;
	//}
}

void ColorItemDelegate::paintColorTable(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QPainterPath path;

	int r = index.row(), c = index.column();
	if (m_view->isHoverIndex(index))
	{
		//QPainterPath border_path;
		//QRectF rc = QRectF(c * (sTableItemSize + sTableItemMargin), r * (sTableItemSize + sTableItemMargin),
		//	sTableItemSize, sTableItemSize);
		//border_path.addRoundedRect(rc, 1, 1);
		//QColor borderClr(125, 162, 206);
		//QPen pen(borderClr);
		////pen.setWidth(1);
		//painter->setPen(pen);
		//painter->setBrush(Qt::NoBrush);
		//painter->drawPath(border_path);

		//rc.adjust(2, 2, -2, -2);
		//path.addRect(rc);
		//painter->fillPath(path, m_color);

		QRectF rc = QRectF(c * (sTableItemSize + sTableItemMargin), r * (sTableItemSize + sTableItemMargin),
			sTableItemSize, sTableItemSize);
		path.addRect(rc);
		painter->fillPath(path, QColor(255,0,0));
	}
	else
	{
		QRectF rc = QRectF(c * (sTableItemSize + sTableItemMargin), r * (sTableItemSize + sTableItemMargin), 
			sTableItemSize, sTableItemSize);
		path.addRect(rc);
		painter->fillPath(path, m_color);
	}
}

void ColorItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);
	painter->setRenderHint(QPainter::Antialiasing);
	return paintColorTable(painter, opt, index);
}