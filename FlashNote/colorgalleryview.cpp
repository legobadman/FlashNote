#include "stdafx.h"
#include "ColorGalleryView.h"
#include "coloritemdelegate.h"
#include "MyStyle.h"
#include "moc_colorgalleryview.cpp"


static const int sROW = 5;
static const int sColumn = 10;
static const int sTableItemMargin = 4;
static const int sIconSize = 16;
static const int sTableItemSize = 20;
static const int sViewWidth = sColumn * sTableItemSize + (sColumn - 1) * sTableItemMargin;
static const int sViewHeight = sROW * sTableItemSize + (sROW - 1) * sTableItemMargin;


ColorGalleryView::ColorGalleryView(QWidget* parent)
	: QAbstractItemView(parent)
{
	init();
}

ColorGalleryView::~ColorGalleryView()
{

}

void ColorGalleryView::init()
{
	setMouseTracking(true);
	setFixedSize(QSize(sViewWidth, sViewHeight));
	setFrameShape(QFrame::NoFrame);
	initModel();
}

QRect ColorGalleryView::visualRect(const QModelIndex& index) const
{
	int r = index.row(), c = index.column();
	QRect rc = QRect(c * (sTableItemSize + sTableItemMargin), r * (sTableItemSize + sTableItemMargin),
		sTableItemSize, sTableItemSize);
	return rc;
}

void ColorGalleryView::scrollTo(const QModelIndex& index, ScrollHint hint)
{

}

QModelIndex ColorGalleryView::indexAt(const QPoint& point) const
{
	float colProp = (float)point.x() / sViewWidth;
	int col = colProp * sColumn;
	float rowProp = (float)point.y() / sViewHeight;
	int row = rowProp * sROW;
	return model->index(row, col);
}

QModelIndex ColorGalleryView::moveCursor(CursorAction cursorAction,
	Qt::KeyboardModifiers modifiers)
{
	return QModelIndex();
}

int ColorGalleryView::horizontalOffset() const
{
	return 0;
}

int ColorGalleryView::verticalOffset() const
{
	return 0;
}

bool ColorGalleryView::isIndexHidden(const QModelIndex& index) const
{
	return false;
}

void ColorGalleryView::setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command)
{
}

QRegion ColorGalleryView::visualRegionForSelection(const QItemSelection& selection) const
{
	return QRegion();
}

bool ColorGalleryView::isHoverIndex(const QModelIndex& index)
{
	return index.row() == m_hoverIdx.row() && index.column() == m_hoverIdx.column();
}

void ColorGalleryView::initModel()
{
	model = new QStandardItemModel(sROW, sColumn, this);
	for (int r = 0; r < sROW + 1; r++)
	{
		for (int c = 0; c < sColumn; c++)
		{
			QStandardItem* pColorItem = new QStandardItem;
			model->setItem(r, c, pColorItem);
		}
	}
	setModel(model);
	setItemDelegate(new ColorItemDelegate(this));
}

void ColorGalleryView::enterEvent(QEvent* e)
{
	update();
}

void ColorGalleryView::leaveEvent(QEvent* e)
{
	m_hoverIdx = QModelIndex();
	update();
}

void ColorGalleryView::mouseMoveEvent(QMouseEvent* e)
{
	QPoint p = e->pos();
	m_hoverIdx = indexAt(p);
	int r = m_hoverIdx.row(), c = m_hoverIdx.column();
	//update();
	QAbstractItemView::mouseMoveEvent(e);
}

void ColorGalleryView::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());
	QPainterPath path;

	QRegion rg = e->region();

	//draw the background
	path.addRect(QRectF(0, 0, sViewWidth, sViewHeight));
	p.fillPath(path, Qt::white);

	int xoffset = 7, yoffset = 34;
	for (int r = 0; r < sROW; r++)
	{
		for (int c = 0; c < sColumn; c++)
		{
			QModelIndex idx = model->index(r, c);
			QRect rc = visualRect(idx);
			if (rg.contains(rc))
			{
				QStyleOptionViewItem option;
				itemDelegate(idx)->paint(&p, option, idx);
			}
			else
			{
				int j;
				j = 0;
			}
		}
	}
}