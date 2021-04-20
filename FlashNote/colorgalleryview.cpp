#include "stdafx.h"
#include "ColorGalleryView.h"
#include "coloritemdelegate.h"
#include "MyStyle.h"
#include "moc_colorgalleryview.cpp"

#define ITEM_SIZE 20
#define ROW_GALLERY_VIEW 5
#define COLUMN_GALLERY_VIEW 10
#define VIEW_MARGIN 4
#define VIEW_WIDTH MyStyle::dpiScaled(248)
#define VIEW_HEIGHT MyStyle::dpiScaled(186)


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
	setFixedSize(MyStyle::dpiScaledSize(QSize(VIEW_WIDTH, VIEW_HEIGHT)));
	initModel();
}

QRect ColorGalleryView::visualRect(const QModelIndex& index) const
{
	return QRect();
}

void ColorGalleryView::scrollTo(const QModelIndex& index, ScrollHint hint)
{

}

QModelIndex ColorGalleryView::indexAt(const QPoint& point) const
{
	return QModelIndex();
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

void ColorGalleryView::initModel()
{
	model = new QStandardItemModel(ROW_GALLERY_VIEW, COLUMN_GALLERY_VIEW, this);

	QStandardItem* pDefaultColor = new QStandardItem;
	pDefaultColor->setIcon(QIcon(":/icons/16x16/default_color.png"));
	pDefaultColor->setText(u8"默认颜色(D)");

	model->setItem(0, 0, pDefaultColor);
	for (int r = 1; r < ROW_GALLERY_VIEW + 1; r++)
	{
		for (int c = 0; c < COLUMN_GALLERY_VIEW; c++)
		{
			QStandardItem* pColorItem = new QStandardItem;
			model->setItem(r, c, pColorItem);
		}
	}

	QStandardItem* pMoreColor = new QStandardItem;
	pMoreColor->setIcon(QIcon(":/icons/16x16/morecolor.png"));
	pMoreColor->setText(u8"更多颜色(M)");
	model->setItem(ROW_GALLERY_VIEW + 1, 0, pMoreColor);

	setModel(model);
	setItemDelegate(new ColorItemDelegate(this));
}

void ColorGalleryView::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());
	QPainterPath path;

	//draw the background
	path.addRect(QRectF(0, 0, VIEW_WIDTH, VIEW_HEIGHT));
	p.fillPath(path, Qt::white);

	//default color item
	QModelIndex idx = model->index(0, 0);

	QStyleOptionViewItem optionItem;
	optionItem.state = QStyle::State_Enabled;
	optionItem.rect = QRect(8, 8, 96, 25);
	optionItem.type = QStyleOption::SO_ToolButton;
	optionItem.decorationAlignment = Qt::AlignCenter;
	optionItem.decorationPosition = QStyleOptionViewItem::Left;
	optionItem.decorationSize = MyStyle::dpiScaledSize(QSize(16, 16));
	optionItem.icon = idx.data(Qt::DecorationRole).value<QIcon>();
	optionItem.text = idx.data(Qt::DisplayRole).toString();

	itemDelegate(idx)->paint(&p, QStyleOptionViewItem(), idx);

	int xoffset = 7, yoffset = 34;
	for (int r = 0; r < ROW_GALLERY_VIEW; r++)
	{
		for (int c = 0; c < COLUMN_GALLERY_VIEW; c++)
		{
			idx = model->index(r + 1, c);
			QStyleOptionViewItem option;
			itemDelegate(idx)->paint(&p, option, idx);
		}
	}

	//more color item
	idx = model->index(ROW_GALLERY_VIEW + 1, 0);
	optionItem.rect = QRect(7, 156, 108, 25);
	optionItem.icon = idx.data(Qt::DecorationRole).value<QIcon>();
	optionItem.text = idx.data(Qt::DisplayRole).toString();
	itemDelegate(idx)->paint(&p, QStyleOptionViewItem(), idx);
}