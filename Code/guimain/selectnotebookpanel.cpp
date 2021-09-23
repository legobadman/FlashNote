#include "stdafx.h"
#include "selectnotebookpanel.h"
#include "LeftSideItemDelegate.h"
#include "model/notebookmodel.h"
#include "guihelper.h"
#include "MyStyle.h"
#include <QtWidgets/QScrollBar>


BooksListView::BooksListView(QWidget* parent)
	: QListView(parent)
	, m_cachedWidth(-1)
{
}

BooksListView::~BooksListView()
{
}

QSize BooksListView::sizeHint() const
{
	if (model()->rowCount() == 0)
		return QSize(width(), 0);

	int _nItemsToShow = 15;
	int nToShow = _nItemsToShow < model()->rowCount() ? _nItemsToShow : model()->rowCount();
	return QSize(sizeHintForColumn(0), nToShow * sizeHintForRow(0) + 5);
}


/////////////////////////////////////////////////////////////
BookItemDelegate::BookItemDelegate(QObject* parent)
	: QAbstractItemDelegate(parent)	
{
}

void BookItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
	int iconSize = MyStyle::dpiScaled(16);
	int icon_xoffset = MyStyle::dpiScaled(11);
	int icon_yoffset = MyStyle::dpiScaled(5);
	int icon_to_text = MyStyle::dpiScaled(5);

	QColor bgColor, borderColor;
	if (opt.state & QStyle::State_Selected)
	{
		bgColor = QColor(193, 222, 236);
		borderColor = QColor(139, 203, 232);
	}
	else if (opt.state & QStyle::State_MouseOver)
	{
		bgColor = QColor(239, 248, 254);
		borderColor = QColor(195, 229, 245);
	}
	else
	{
		bgColor = QColor(255, 255, 255);
		borderColor = QColor(255, 255, 255);
	}

	const QPointF oldBrushOrigin = painter->brushOrigin();
	painter->setBrushOrigin(opt.rect.topLeft());
	QRect rc = opt.rect.adjusted(MyStyle::dpiScaled(5), 0, MyStyle::dpiScaled(-5), 0);
	QRect rcBackground = rc.adjusted(0, MyStyle::dpiScaled(1), 0, MyStyle::dpiScaled(-1));
	painter->fillRect(rcBackground, bgColor);
	painter->setBrushOrigin(oldBrushOrigin);

	QRect rcBorder = rc.adjusted(0, 0, 0, MyStyle::dpiScaled(-1));
	painter->setPen(borderColor);
	painter->drawRect(rcBorder);

	QString text = index.data().toString();

	icon_yoffset = (opt.rect.height() - iconSize) / 2;

	QIcon icon(":/icons/notebookmenu.png");
	QRect iconRect(opt.rect.x() + icon_xoffset, opt.rect.y() + icon_yoffset, MyStyle::dpiScaled(16), MyStyle::dpiScaled(16));
	
	icon.paint(painter, iconRect, opt.decorationAlignment, QIcon::Normal, QIcon::On);

	QFont font(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
	QFontMetricsF fontMetrics(font);
	int w = fontMetrics.horizontalAdvance(text);
	int h = fontMetrics.height();
	QRect textRect(opt.rect.x() + icon_xoffset + iconSize + icon_to_text, opt.rect.y(), w, opt.rect.height());
	painter->setPen(QColor(0, 0, 0));
	painter->setFont(font);
	painter->drawText(textRect, Qt::AlignVCenter, text);
}

QSize BookItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    static const int iconSize = 16;
	static const int icon_xoffset = 11;
	static const int icon_yoffset = 5;
	static const int icon_to_text = 5;
	static const int text_rightmargin = 20;

	QString text = index.data().toString();
    QFont font(QString::fromUtf16((char16_t*)L"微软雅黑"), 9);
    QFontMetricsF fontMetrics(font);
    int textWidth = fontMetrics.horizontalAdvance(text);
	int w = MyStyle::dpiScaled(icon_xoffset + iconSize + icon_to_text + text_rightmargin) + textWidth;
	int h = MyStyle::dpiScaled(23);
	return QSize(w, h);
}


////////////////////////////////////////////////////////////////////
SelectNotebookPanel::SelectNotebookPanel(QWidget* parent)
	: QWidget(parent)
	, m_pListView(NULL)
{
	m_pListView = new BooksListView(this);
	m_pListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pListView->setContentsMargins(10, 0, 0, 0);
	m_pListView->viewport()->setAttribute(Qt::WA_Hover, true);

	NotebookModel* model = new NotebookModel;
	m_pListView->setModel(model);

	QVBoxLayout* pLayout = new QVBoxLayout;
	pLayout->addWidget(m_pListView);
	pLayout->setMargin(0);
	setLayout(pLayout);

	m_pListView->setItemDelegate(new BookItemDelegate(m_pListView));
	connect(m_pListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onIndexClicked(const QModelIndex&)));
}

SelectNotebookPanel::~SelectNotebookPanel()
{
}

QString SelectNotebookPanel::currentBookId()
{
	QString bookId = m_pListView->currentIndex().data(ItemCoreObjIdRole).toString();
	return bookId;
}

void SelectNotebookPanel::onIndexClicked(const QModelIndex& index)
{
	QString notebookid = index.data(ItemCoreObjIdRole).toString();
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookById(notebookid, &spNotebook);
	emit notebookMoved(spNotebook);
}
