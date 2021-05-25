#include "stdafx.h"
#include "booklistview.h"
#include "ui_booklistview.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "LeftSideItemDelegate.h"

static const int nContentLimit = 74;



BookListView::BookListView(QWidget* parent)
	: QWidget(parent)
	, m_pCustomMenu(NULL)
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

	QModelIndex index = m_ui->listView->indexAt(point);

	m_pCustomMenu->clear();

	QAction* pDelete = new QAction(u8"删除笔记", m_pCustomMenu);
	pDelete->setData((int)DELETE_NOTE);
	m_pCustomMenu->addAction(pDelete);

	m_pCustomMenu->popup(QCursor::pos());
}

void BookListView::MenuActionSlot(QAction* action)
{
	if (action == NULL)
		return;

	MENU_ITEM nIndex = (MENU_ITEM)action->data().toInt();
	if (nIndex == DELETE_NOTE)
	{
		QModelIndex index = m_ui->listView->currentIndex();
		int idxNote = index.row();
		com_sptr<INote> spNote;
		AppHelper::GetNote(m_spNotebook, idxNote, &spNote);
		bool bRet = RPCService::GetInstance().RemoveNote(m_spNotebook, spNote);
	}
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

HRESULT BookListView::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (com_sptr<INotebook>(pCoreObj))
	{
		return onNotebookNotify(pCoreObj, arg);
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

HRESULT BookListView::onNotebookNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	com_sptr<INotebook> pNotebook = pCoreObj;
	if (pNotebook != m_spNotebook)
	{
		//当前页面如果不是接收通知的对象，那就不需要刷新（点击自然会更新）
		return S_OK;
	}

	//先添加到最后的位置
	Q_ASSERT(arg.pObj);
	com_sptr<INote> spNote(arg.pObj);
	Q_ASSERT(spNote);

	QString noteid = AppHelper::GetNoteId(spNote);

	switch (arg.ope)
	{
		case NotifyOperator::Add:
		{
			//监听Notebook::AddWorkbook
			bool bRet = m_model->insertRow(0);
			
			QModelIndex newItem = m_model->index(0, 0);
			QStandardItem* pItem = m_model->itemFromIndex(newItem);
			QString showContent = GetShowContent(spNote);
			pItem->setText(showContent);
			pItem->setData(QVariant(noteid), ItemCoreObjIdRole);
			pItem->setSelectable(true);
			pItem->setEditable(false);

			spNote->addWatcher(this);
			break;
		}
		case NotifyOperator::Delete:
		{
			QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
			if (!indexs.isEmpty())
			{
				Q_ASSERT(indexs.size() == 1);
				QModelIndex index = indexs.at(0);
				m_ui->listView->model()->removeRow(index.row());
				QItemSelectionModel* pModel = m_ui->listView->selectionModel();
				QModelIndex newIndex = pModel->currentIndex();
				emit noteitemselected(newIndex);
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

void BookListView::updateNotebook(INotebook* pNotebook, QString noteid)
{
	if (m_spNotebook == pNotebook)
		return;

	m_spNotebook = pNotebook;
	m_spNotebook->addWatcher(this);

	QString bookName = AppHelper::GetNotebookName(pNotebook);
	m_ui->lblNotebook->setText(bookName);
	m_ui->lblNumberNotes->setText(QString(u8"%1条笔记").arg(
		QString::number(AppHelper::GetNoteCounts(m_spNotebook))));

	m_model = new QStandardItemModel(this);
	int n = AppHelper::GetNoteCounts(m_spNotebook);
	for (int i = 0; i < n; i++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(m_spNotebook, i, &spNote);

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

		m_model->appendRow(pNoteItem);
	}

	m_model->setColumnCount(1);
	m_ui->listView->setModel(m_model);
	if (n == 0)
	{
		return;
	}

	QModelIndex selectedIndex;
	QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
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
