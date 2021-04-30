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
static const int sImageSize = 18;
static const int sBorderSize = 1;
static const int sTableItemSize = 20;
static const int sLastItemH = 26, sLastItemW = 108;
static const int sBottomMargin = 6;


ColorItemDelegate::ColorItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
	m_view = qobject_cast<ColorTableView*>(parent);
}

QSize ColorItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return MyStyle::dpiScaledSize(QSize(30, 30));
}

void ColorItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
}

void ColorItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int r = index.row(), c = index.column();
	int baseX = c * (sImageSize + 2 * sBorderSize + sTableItemMargin);
	int baseY = r * (sImageSize + 2 * sBorderSize + sTableItemMargin);
	QBrush brush = index.data(Qt::BackgroundRole).value<QBrush>();
	QColor clr = brush.color();

	//TODO: 研究为什么灰色border会是2像素，并且边角缺口？(已解决：抗锯齿的设定)
	QRectF borderRect(baseX, baseY, sImageSize + 2 * sBorderSize - 1, sImageSize + 2 * sBorderSize - 1);
	QPen pen;
	pen.setWidth(1);

	bool bHover = m_view->isHoverIndex(index);
	pen.setColor(bHover ? QColor(77, 130, 174) : QColor(197, 197, 197));
	painter->setPen(pen);

	painter->drawRect(borderRect);

	int colorItemSize = bHover ? 16 : 18;
	int border_to_color = bHover ? 1 : 0;

	QRectF rcColor = QRectF(baseX + border_to_color + sBorderSize,
		baseY + border_to_color + sBorderSize,
		colorItemSize,
		colorItemSize);

	QPainterPath content_path;
	content_path.addRect(rcColor);
	painter->fillPath(content_path, clr);
}