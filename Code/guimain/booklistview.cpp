#include "stdafx.h"
#include "booklistview.h"
#include "ui_booklistview.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "dbservice.h"
#include "LeftSideItemDelegate.h"
#include "noteitemdelegate.h"

static const int nContentLimit = 74;


BookListView::BookListView(NotesEditView* parent)
	: QWidget(parent)
	, m_pCustomMenu(NULL)
	, m_pNotesView(parent)
	, m_model(NULL)
{
	init();
}

void BookListView::init()
{
	m_ui = new Ui::BookListView();
	m_ui->setupUi(this);

	QPalette palette(this->palette());
	palette.setColor(QPalette::Window, QColor(255, 255, 255));
	setAutoFillBackground(true);
	setPalette(palette);

	m_ui->searcheditor->setTextMargins(3, 3, 3, 3);
	m_ui->sort->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->sort->setIcon(QIcon(":/icons/24x24/sort_options.png"));
	m_ui->sort->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->view->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->view->setIcon(QIcon(":/icons/24x24/listview.png"));
	m_ui->view->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->more->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->more->setIcon(QIcon(":/icons/16x16/more.png"));
	m_ui->more->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui->listView->setFrameShape(QFrame::NoFrame);
	m_ui->listView->viewport()->setAttribute(Qt::WA_Hover, true);

	QPalette pal;
	pal.setColor(QPalette::Foreground, QColor(161, 161, 161));
	m_ui->lblNumberNotes->setPalette(pal);

	m_ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui->sort, SIGNAL(clicked()), this, SLOT(onSortBtnClicked()));
	connect(m_ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));
	connect(m_ui->searcheditor, SIGNAL(textChanged(const QString&)), this, SLOT(onSearchTextChanged(const QString&)));

	m_ui->listView->setItemDelegate(new NoteItemDelegate(m_ui->listView, m_ui->searcheditor));
}

BookListView::~BookListView()
{
}

void BookListView::onSortBtnClicked()
{
    if (m_pCustomMenu == NULL)
    {
        m_pCustomMenu = new QMenu(this);
        connect(m_pCustomMenu, SIGNAL(triggered(QAction*)), this, SLOT(MenuActionSlot(QAction*)));
    }
	m_pCustomMenu->clear();

	QAction* pAction = new QAction(u8"修改时间", m_pCustomMenu);
	pAction->setData((int)SORT_MODIFY_TIME);
	pAction->setCheckable(true);
	pAction->setChecked(m_sortOption == SOP_MODIFY_TIME);
	m_pCustomMenu->addAction(pAction);

	pAction = new QAction(u8"创建时间", m_pCustomMenu);
	pAction->setData((int)SORT_CREATE_TIME);
    pAction->setCheckable(true);
    pAction->setChecked(m_sortOption == SOP_CREATE_TIME);
	m_pCustomMenu->addAction(pAction);

	pAction = new QAction(u8"标题", m_pCustomMenu);
	pAction->setData((int)SORT_TITLE);
    pAction->setCheckable(true);
    pAction->setChecked(m_sortOption == SOP_TITLE);
	m_pCustomMenu->addAction(pAction);

	m_pCustomMenu->addSeparator();

	pAction = new QAction(u8"倒序排序", m_pCustomMenu);
	pAction->setData((int)SORT_ASCEND);
	pAction->setCheckable(true);
	pAction->setChecked(!m_bAscent);
	m_pCustomMenu->addAction(pAction);

	m_pCustomMenu->popup(QCursor::pos());
}

void BookListView::onViewBtnClicked()
{

}

void BookListView::onCustomContextMenu(const QPoint& point)
{
	if (m_pCustomMenu == NULL)
	{
		m_pCustomMenu = new QMenu(this);
		connect(m_pCustomMenu, SIGNAL(triggered(QAction*)), this, SLOT(MenuActionSlot(QAction*)));
	}

	m_pCustomMenu->clear();
	QModelIndex index = m_ui->listView->indexAt(point);

	ITEM_CONTENT_TYPE itemType = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	Q_ASSERT(itemType == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM || itemType == ITEM_CONTENT_TYPE::ITEM_TRASHITEM);

	if (itemType == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM)
	{
		QAction* pDelete = new QAction(u8"删除笔记", m_pCustomMenu);
		pDelete->setData((int)REMOVE_NOTE);
		m_pCustomMenu->addAction(pDelete);

		QAction* pOpen = new QAction(u8"打开笔记", m_pCustomMenu);
		pOpen->setData((int)OPEN_NOTE);
		m_pCustomMenu->addAction(pOpen);
	}
	else
	{
		QAction* pDelete = new QAction(u8"彻底删除笔记", m_pCustomMenu);
		pDelete->setData((int)DELETE_NOTE);
		m_pCustomMenu->addAction(pDelete);

		QAction* pRecover = new QAction(u8"恢复笔记", m_pCustomMenu);
		pRecover->setData((int)RECOVER_NOTE);
		m_pCustomMenu->addAction(pRecover);
	}

	m_pCustomMenu->popup(QCursor::pos());
}

void BookListView::MenuActionSlot(QAction* action)
{
	if (action == NULL)
		return;

	MENU_ITEM nIndex = (MENU_ITEM)action->data().toInt();
	QModelIndex index = m_ui->listView->currentIndex();

	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();

	if (nIndex == REMOVE_NOTE)
	{
		std::wstring bookId;
		spNote->GetBookId(bookId);
		QString bookid = QString::fromStdWString(bookId);
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookById(bookid, &spNotebook);
		bool bRet = DbService::GetInstance().RemoveNote(AppHelper::coreApp(), spNotebook, spNote);
	}
	else if (nIndex == RECOVER_NOTE)
	{
		com_sptr<ITrash> pTrash;
		AppHelper::coreApp()->GetTrash(&pTrash);
		bool bRet = DbService::GetInstance().RecoverNote(AppHelper::coreApp(), pTrash, spNote);
	}
	else if (nIndex == DELETE_NOTE)
	{
		com_sptr<ITrash> pTrash;
		AppHelper::coreApp()->GetTrash(&pTrash);
		bool bRet = DbService::GetInstance().DeleteNote(pTrash, spNote);
	}
	else if (nIndex == OPEN_NOTE)
	{
		AppHelper::openNoteInIsoWindow(AppHelper::GetNoteId(spNote));
	}
	else if (nIndex == SORT_CREATE_TIME)
	{
		m_sortOption = SOP_CREATE_TIME;
		_sort();
	}
	else if (nIndex == SORT_MODIFY_TIME)
	{
		m_sortOption = SOP_MODIFY_TIME;
		_sort();
	}
	else if (nIndex == SORT_TITLE)
	{
		m_sortOption = SOP_TITLE;
		_sort();
	}
	else if (nIndex == SORT_ASCEND)
	{
		m_bAscent = !m_bAscent;
		_sort();
	}
}

void BookListView::_sort()
{
	int sortRole = 0;
	if (m_sortOption == SOP_TITLE)
	{
		sortRole = ItemNoteTitle;
	}
	else if (m_sortOption == SOP_CREATE_TIME)
	{
		sortRole = ItemNoteCreateTime;
	}
	else if (m_sortOption == SOP_MODIFY_TIME)
	{
		sortRole = ItemNoteModifyTime;
	}
	m_model->setSortRole(sortRole);
	m_model->sort(0, m_bAscent ? Qt::AscendingOrder : Qt::DescendingOrder);
}

void BookListView::onSearchTextChanged(const QString&)
{
	QString searchContent = m_ui->searcheditor->text();
	emit searchTriggered(searchContent);
}

void BookListView::resetModel(QSortFilterProxyModel* pModel, BOOKVIEW_TYPE type, INoteCollection* pNoteCollection)
{
	m_sortOption = SOP_TITLE;
	m_bAscent = true;

	m_model = pModel;
	m_ui->listView->setModel(pModel);

	connect(this, SIGNAL(searchTriggered(const QString&)), pModel, SLOT(setFilterWildcard(const QString&)));
	connect(m_ui->listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
		m_pNotesView, SLOT(onNoteItemSelected(const QModelIndex&, const QModelIndex&)));

	QModelIndex selectedIndex = pModel->index(0, 0);
	m_ui->listView->setCurrentIndex(selectedIndex);

	QString bookName;
	if (type == VIEW_ALLNOTES)
		bookName = u8"全部笔记";
	else if (type == VIEW_NOTEBOOK || type == VIEW_TRASH)
		bookName = AppHelper::GetNotebookName(pNoteCollection);

	m_ui->lblNotebook->setText(bookName);
	m_ui->lblNumberNotes->setText(QString(u8"%1条笔记").arg(
		QString::number(pModel->rowCount())));
}
