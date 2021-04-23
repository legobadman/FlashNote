#include "stdafx.h"
#include "colortableview.h"
#include "coloritemdelegate.h"
#include "MyStyle.h"
#include "moc_colortableview.cpp"


static const int sROW = 5;
static const int sColumn = 10;
static const int sTableItemMargin = 4;
static const int sIconSize = 16;
static const int sTableItemSize = 20;
static const int sBorderSize = 1;
static const int sViewWidth = sColumn * sTableItemSize + (sColumn - 1) * sTableItemMargin;
static const int sViewHeight = sROW * sTableItemSize + (sROW - 1) * sTableItemMargin;

//Color Table
static const char clrTable[sROW][sColumn][8] = {
	{"#D6D6D6", "#5DDAFF", "#81B6FF", "#BA00FF", "#E500FF", "#F14F9A", "#FF7A74", "#FBCD5F", "#F5FF7D", "#96EE7D"},
	{"#A8A8A8", "#1ECCFF", "#5898FF", "#9E00F3", "#D100FF", "#EB0073", "#FF4635", "#FFAF00", "#F3FF38", "#6BE748"},
	{"#797979", "#1AADE0", "#3665EE", "#7600D8", "#A600C4", "#BC0058", "#FF0000", "#FA7A00", "#F7FF00", "#4DCE1D"},
	{"#464646", "#1A90B9", "#2D4FC9", "#4F009A", "#8600A4", "#9C004C", "#E30000", "#DE5700", "#C6C500", "#41AD1C"},
	{"#000000", "#147191", "#1C3387", "#41007D", "#6A0081", "#7B003D", "#AD0000", "#A74500", "#939600", "#328712"}
};


ColorTableView::ColorTableView(QWidget* parent)
	: QAbstractItemView(parent)
{
	init();
}

ColorTableView::~ColorTableView()
{
}

void ColorTableView::init()
{
	setMouseTracking(true);
	setFixedSize(QSize(sViewWidth, sViewHeight));
	setFrameShape(QFrame::NoFrame);
	initModel();
}

QRect ColorTableView::visualRect(const QModelIndex& index) const
{
	int r = index.row(), c = index.column();
	QRect rc = QRect(c * (sTableItemSize + sTableItemMargin), r * (sTableItemSize + sTableItemMargin),
		sTableItemSize, sTableItemSize);
	return rc;
}

void ColorTableView::scrollTo(const QModelIndex& index, ScrollHint hint)
{
}

QModelIndex ColorTableView::indexAt(const QPoint& point) const
{
	float colProp = (float)point.x() / sViewWidth;
	int col = colProp * sColumn;
	float rowProp = (float)point.y() / sViewHeight;
	int row = rowProp * sROW;
	return model->index(row, col);
}

QModelIndex ColorTableView::moveCursor(CursorAction cursorAction,
	Qt::KeyboardModifiers modifiers)
{
	return QModelIndex();
}

int ColorTableView::horizontalOffset() const
{
	return 0;
}

int ColorTableView::verticalOffset() const
{
	return 0;
}

bool ColorTableView::isIndexHidden(const QModelIndex& index) const
{
	return false;
}

void ColorTableView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{
}

QRegion ColorTableView::visualRegionForSelection(const QItemSelection& selection) const
{
	return QRegion();
}

bool ColorTableView::isHoverIndex(const QModelIndex& index) const
{
	return index == m_hoverIdx;
}

void ColorTableView::initModel()
{
	model = new QStandardItemModel(sROW, sColumn, this);
	for (int r = 0; r < sROW + 1; r++)
	{
		for (int c = 0; c < sColumn; c++)
		{
			QStandardItem* pColorItem = new QStandardItem;
			QString clrName(clrTable[r][c]);
			pColorItem->setBackground(QBrush(QColor(clrName)));
			model->setItem(r, c, pColorItem);
		}
	}
	setModel(model);
	setItemDelegate(new ColorItemDelegate(this));
}

void ColorTableView::enterEvent(QEvent* e)
{
	update();
}

void ColorTableView::leaveEvent(QEvent* e)
{
	m_hoverIdx = QModelIndex();
	update();
}

void ColorTableView::mouseMoveEvent(QMouseEvent* e)
{
	m_hoverIdx = indexAt(e->pos());
	QAbstractItemView::mouseMoveEvent(e);
}

void ColorTableView::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());
	QPainterPath path;

	QRegion rg = e->region();

	//draw the background
	path.addRect(QRectF(0, 0, sViewWidth, sViewHeight));
	p.fillPath(path, Qt::white);

	int xoffset = 7, yoffset = 34;
	int hit = 0, cached = 0;
	for (int r = 0; r < sROW; r++)
	{
		for (int c = 0; c < sColumn; c++)
		{
			QModelIndex idx = model->index(r, c);
			QRect rc = visualRect(idx);
			if (rg.contains(rc))
			{
				QStyleOptionViewItem option;
				itemDelegate(idx)->paint(&p, QStyleOptionViewItem(), idx);
				hit++;
			}
			else
			{
				cached++;
			}
		}
	}
}