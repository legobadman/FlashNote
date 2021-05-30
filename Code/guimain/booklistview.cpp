#include "stdafx.h"
#include "booklistview.h"
#include "ui_booklistview.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "LeftSideItemDelegate.h"

static const int nContentLimit = 74;


BookListView::BookListView(NotesEditView* parent)
	: QWidget(parent)
	, m_pCustomMenu(NULL)
	, m_type(VIEW_ALLNOTES)
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
	m_ui->more->setIcon(QIcon(":/icons/24x24/more.png"));
	m_ui->more->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(m_ui->listView, SIGNAL(clicked(const QModelIndex&)),
		this, SIGNAL(noteitemselected(const QModelIndex&)));

	m_ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));

	connect(this, SIGNAL(noteitemselected(const QModelIndex&)), 
		m_pNotesView, SLOT(onNoteItemSelected(const QModelIndex&)));

	m_model = new QStandardItemModel(this);
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
		QAction* pDelete = new QAction(u8"删除笔记", m_pCustomMenu);
		pDelete->setData((int)REMOVE_NOTE);
		m_pCustomMenu->addAction(pDelete);
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
	QString noteid = index.data(ItemCoreObjIdRole).toString();
	com_sptr<INote> spNote;
	AppHelper::GetNoteById(noteid, &spNote);

	BSTR bookId;
	spNote->GetBookId(&bookId);

	QString bookid = QString::fromUtf16(reinterpret_cast<ushort*>(bookId));

	if (nIndex == REMOVE_NOTE)
	{
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

ITEM_CONTENT_TYPE BookListView::getItemContentType(INoteCollection* pNotebook)
{
	ITEM_CONTENT_TYPE contentType = ITEM_CONTENT_TYPE::ITEM_UNKNOWN;
	if (com_sptr<INotebook>(pNotebook))
	{
		contentType = ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM;
	}
	else if (com_sptr<ITrash>(pNotebook))
	{
		contentType = ITEM_CONTENT_TYPE::ITEM_TRASHITEM;
	}
	else
	{
		Q_ASSERT(false);
	}
	return contentType;
}

QString BookListView::GetShowContent(INote* pNote)
{
	QString title = AppHelper::GetNoteTitle(pNote);
	QString content = AppHelper::GetNoteContent(pNote);
	QString showContent;

	QTextDocument html;
	html.setHtml(content);
	QString text_abbre = html.toPlainText();
	text_abbre = text_abbre.mid(0, nContentLimit);
	showContent = title + "\n" + text_abbre;
	return showContent;
}

QString BookListView::GetDefaultNoteId(INoteCollection* pNoteCollection)
{
	//TODO: QSettings获取和设置Noteid
	QString noteid;
	if (m_type == VIEW_ALLNOTES)
	{
		
	}
	else if (m_type == VIEW_TRASH)
	{

	}
	else if (m_type == VIEW_NOTEBOOK)
	{

	}

	int n = m_model->rowCount();
	if (n > 0)
	{
		noteid = m_model->index(0,0).data(ItemCoreObjIdRole).toString();
	}
	return noteid;
}

HRESULT BookListView::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (com_sptr<INotebook>(pCoreObj))
	{
		com_sptr<INotebook> spNotebook(pCoreObj);
		return onNotebookNotify(spNotebook, arg);
	}
	else if (com_sptr<ITrash>(pCoreObj))
	{
		com_sptr<ITrash> spTrash(pCoreObj);
		return onTrashNotify(spTrash, arg);
	}
	else if (com_sptr<INote>(pCoreObj))
	{
		return onNoteNotify(pCoreObj, arg);
	}
	else if (com_sptr<INoteApplication>(pCoreObj))
	{
		return E_NOTIMPL;
	}
	return S_OK;
}

HRESULT BookListView::onTrashNotify(ITrash* pCoreObj, NotifyArg arg)
{
	if (m_type != VIEW_TRASH)
	{
		return S_OK;
	}

	Q_ASSERT(pCoreObj);
	com_sptr<INote> spNote(arg.pObj);
	Q_ASSERT(spNote);
	return updateView(arg, spNote);
}

HRESULT BookListView::onNotebookNotify(INotebook* pNotebook, NotifyArg arg)
{
	//先添加到最后的位置
	Q_ASSERT(arg.pObj);
	com_sptr<INote> spNote(arg.pObj);
	Q_ASSERT(spNote);
	return updateView(arg, spNote);
}

HRESULT BookListView::updateView(NotifyArg arg, INote* pNote)
{
	//TODO: noteid不在列表或在列表的异常情况。
	QString noteid = AppHelper::GetNoteId(pNote);

	ITEM_CONTENT_TYPE itemType = ITEM_CONTENT_TYPE::ITEM_UNKNOWN;
	if (m_type == VIEW_ALLNOTES || m_type == VIEW_NOTEBOOK)
		itemType = ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM;
	else
		itemType = ITEM_CONTENT_TYPE::ITEM_TRASHITEM;

	switch (arg.ope)
	{
	case NotifyOperator::Add:
	{
		//监听Notebook::AddWorkbook
		bool bRet = m_model->insertRow(0);

		QModelIndex newItem = m_model->index(0, 0);
		QStandardItem* pItem = m_model->itemFromIndex(newItem);
		QString showContent = GetShowContent(pNote);
		pItem->setText(showContent);
		pItem->setData(QVariant(noteid), ItemCoreObjIdRole);
		pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(itemType), ItemContentTypeRole);
		pItem->setSelectable(true);
		pItem->setEditable(false);

		pNote->addWatcher(this);
		break;
	}
	case NotifyOperator::Delete:
	{
		QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
		if (!indexs.isEmpty())
		{
			Q_ASSERT(indexs.size() == 1);
			QModelIndex index = indexs.at(0);
			m_model->removeRow(index.row());
			QItemSelectionModel* pModel = m_ui->listView->selectionModel();
			QModelIndex newIndex = pModel->currentIndex();
			emit noteitemselected(newIndex);
		}
		if (m_model->rowCount() == 0)
		{
			m_ui->lblNumberNotes->setText(QString(u8"0条笔记"));
		}
		break;
	}
	case NotifyOperator::Update:
	{
		//更新bookName
		QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
		Q_ASSERT(indexs.size() == 1);
		QModelIndex index = indexs.at(0);
		break;
	}
	}
	return S_OK;
}

HRESULT BookListView::onNoteNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	com_sptr<INote> pNote = pCoreObj;
	switch (arg.ope)
	{
		case NotifyOperator::Delete:
		case NotifyOperator::Add:
		{
			//notebook已经做了。
			break;
		}
		case NotifyOperator::Update:
		{
			QString noteid = AppHelper::GetNoteId(pNote);
			QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
			if (!indexs.isEmpty())
			{
				Q_ASSERT(indexs.size() == 1);
				QModelIndex index = indexs.at(0);
				QStandardItem* pItem = m_model->itemFromIndex(index);
				QString showContent = GetShowContent(pNote);
				pItem->setText(showContent);
			}
		}
	}
	return S_OK;
}

void BookListView::initAllNotes()
{
	m_type = VIEW_ALLNOTES;

	//遍历所有笔记本
	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);

	int nCount = 0;
	spNotebooks->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebook(i, &spNotebook);
		spNotebook->addWatcher(this);
		appendNotes(ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM, spNotebook);
	}

	m_model->clear();
	m_model->setColumnCount(1);
	m_ui->listView->setModel(m_model);
	int n = m_model->rowCount();
	if (n == 0)
	{
		return;
	}

	QModelIndex selectedIndex;
	QString activeNoteId = GetDefaultNoteId(NULL);
	QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(activeNoteId));
	if (indexs.empty())
	{
		selectedIndex = m_model->index(0, 0);
	}
	else
	{
		Q_ASSERT(indexs.size() == 1);
		selectedIndex = indexs.at(0);
	}
	m_ui->listView->setCurrentIndex(selectedIndex);
}

void BookListView::appendNotes(ITEM_CONTENT_TYPE itemType, INoteCollection* pNoteCollection)
{
	int n = AppHelper::GetNoteCounts(pNoteCollection);
	for (int i = 0; i < n; i++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(pNoteCollection, i, &spNote);

		spNote->addWatcher(this);

		QString title = AppHelper::GetNoteTitle(spNote);
		QString content = AppHelper::GetNoteContent(spNote);
		QString noteid = AppHelper::GetNoteId(spNote);

		if (title.isEmpty())
			title = u8"无标题笔记";

		QTextDocument html;
		html.setHtml(content);
		QString text_abbre = html.toPlainText();
		text_abbre = text_abbre.mid(0, nContentLimit);

		QStandardItem* pNoteItem = new QStandardItem(title + "\n" + text_abbre);
		pNoteItem->setSelectable(true);
		pNoteItem->setEditable(false);
		pNoteItem->setData(QVariant(noteid), ItemCoreObjIdRole);
		pNoteItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(itemType), ItemContentTypeRole);

		m_model->appendRow(pNoteItem);
	}
}

void BookListView::initNoteContainer(BOOKVIEW_TYPE type, INoteCollection* pNoteCollection)
{
	m_type = type;

	pNoteCollection->addWatcher(this);

	ITEM_CONTENT_TYPE contentType = getItemContentType(pNoteCollection);

	QString bookName = AppHelper::GetNotebookName(pNoteCollection);
	m_ui->lblNotebook->setText(bookName);
	m_ui->lblNumberNotes->setText(QString(u8"%1条笔记").arg(
		QString::number(AppHelper::GetNoteCounts(pNoteCollection))));

	m_model->clear();

	appendNotes(contentType, pNoteCollection);

	int n = AppHelper::GetNoteCounts(pNoteCollection);

	m_model->setColumnCount(1);
	m_ui->listView->setModel(m_model);
	if (n == 0)
	{
		return;
	}

	QModelIndex selectedIndex;
	QString activeNoteId = GetDefaultNoteId(pNoteCollection);
	QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(activeNoteId));
	if (indexs.empty())
	{
		selectedIndex = m_model->index(0, 0);
	}
	else
	{
		Q_ASSERT(indexs.size() == 1);
		selectedIndex = indexs.at(0);
	}
	m_ui->listView->setCurrentIndex(selectedIndex);
}

QString BookListView::getCurrentNoteId()
{
	QString noteid;
	if (m_model->rowCount() == 0)
		return noteid;
	return m_ui->listView->currentIndex().data(ItemCoreObjIdRole).toString();
}
