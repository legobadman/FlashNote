#include "stdafx.h"
#include "booklistview.h"
#include "ui_booklistview.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "LeftSideItemDelegate.h"
#include "noteitemdelegate.h"

static const int nContentLimit = 74;


BookListView::BookListView(NotesEditView* parent)
	: QWidget(parent)
	, m_pCustomMenu(NULL)
	, m_pNotesView(parent)
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

	QPalette pal;
	pal.setColor(QPalette::Foreground, QColor(161, 161, 161));
	m_ui->lblNumberNotes->setPalette(pal);

	connect(m_ui->listView, SIGNAL(clicked(const QModelIndex&)),
		this, SIGNAL(noteitemselected(const QModelIndex&)));

	m_ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));

	connect(this, SIGNAL(noteitemselected(const QModelIndex&)), 
		m_pNotesView, SLOT(onNoteItemSelected(const QModelIndex&)));

	m_ui->listView->setItemDelegate(new NoteItemDelegate(m_ui->listView));
}

BookListView::~BookListView()
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
		QAction* pDelete = new QAction(u8"É¾³ý±Ê¼Ç", m_pCustomMenu);
		pDelete->setData((int)REMOVE_NOTE);
		m_pCustomMenu->addAction(pDelete);
	}
	else
	{
		QAction* pDelete = new QAction(u8"³¹µ×É¾³ý±Ê¼Ç", m_pCustomMenu);
		pDelete->setData((int)DELETE_NOTE);
		m_pCustomMenu->addAction(pDelete);

		QAction* pRecover = new QAction(u8"»Ö¸´±Ê¼Ç", m_pCustomMenu);
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
		BSTR bookId;
		spNote->GetBookId(&bookId);
		QString bookid = QString::fromUtf16(reinterpret_cast<ushort*>(bookId));
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookById(bookid, &spNotebook);
		bool bRet = RPCService::GetInstance().RemoveNote(coreApp, spNotebook, spNote);
	}
	else if (nIndex == RECOVER_NOTE)
	{
		com_sptr<ITrash> pTrash;
		coreApp->GetTrash(&pTrash);
		bool bRet = RPCService::GetInstance().RecoverNote(coreApp, pTrash, spNote);
	}
	else if (nIndex == DELETE_NOTE)
	{
		com_sptr<ITrash> pTrash;
		coreApp->GetTrash(&pTrash);
		bool bRet = RPCService::GetInstance().DeleteNote(pTrash, spNote);
	}
}

void BookListView::resetModel(BookViewModel* pModel, BOOKVIEW_TYPE type, INoteCollection* pNoteCollection)
{
	m_ui->listView->setModel(pModel);
	QModelIndex selectedIndex = pModel->index(0, 0);
	m_ui->listView->setCurrentIndex(selectedIndex);

	QString bookName;
	if (type == VIEW_ALLNOTES)
		bookName = u8"È«²¿±Ê¼Ç";
	else if (type == VIEW_NOTEBOOK || type == VIEW_TRASH)
		bookName = AppHelper::GetNotebookName(pNoteCollection);

	m_ui->lblNotebook->setText(bookName);
	m_ui->lblNumberNotes->setText(QString(u8"%1Ìõ±Ê¼Ç").arg(
		QString::number(pModel->rowCount())));
}
