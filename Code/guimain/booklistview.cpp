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

	connect(m_ui->listView, SIGNAL(clicked(const QModelIndex&)),
		this, SIGNAL(noteitemclicked(const QModelIndex&)));

	//m_selectionModel = new QItemSelectionModel;
	//m_ui->listView->setSelectionModel(m_selectionModel);

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

HRESULT BookListView::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	GUID wtf;
	com_sptr<INotebook> pNotebook(pCoreObj);
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

			m_model->insertRow(0);
			QModelIndex newItem = m_model->index(0, 0);
			QStandardItem* pItem = m_model->itemFromIndex(newItem);
			QString title = AppHelper::GetNoteTitle(spNote);
			QString content = AppHelper::GetNoteContent(spNote);
			QString showContent;

			QTextDocument html;
			html.setHtml(content);
			QString text_abbre = html.toPlainText();
			text_abbre = text_abbre.mid(0, nContentLimit);
			showContent = title + "\n" + text_abbre;
			pItem->setText(showContent);
			pItem->setData(QVariant(noteid), ItemCoreObjIdRole);
			pItem->setSelectable(true);
			pItem->setEditable(false);
		}
		break;

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
				emit noteitemclicked(newIndex);
			}
		}
		break;

	case NotifyOperator::Update:
		{
			QModelIndexList indexs = m_model->match(m_model->index(0, 0), ItemCoreObjIdRole, QVariant(noteid));
			Q_ASSERT(indexs.size() == 1);
			QModelIndex index = indexs.at(0);
		}
		break;
	}
	return S_OK;
}

void BookListView::updateNotebook(INotebook* pNotebook, int idxNote)
{
	m_spNotebook = pNotebook;
	m_spNotebook->addWatcher(this);

	QString bookName = AppHelper::GetNotebookName(pNotebook);
	m_ui->lblNotebook->setText(bookName);
	m_ui->lblNumberNotes->setText(QString(u8"%1条笔记").arg(
		QString::number(AppHelper::GetNoteCounts(m_spNotebook))));

	m_model = new QStandardItemModel(this);
	for (int i = 0; i < AppHelper::GetNoteCounts(m_spNotebook); i++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(m_spNotebook, i, &spNote);
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

	m_ui->listView->setModel(m_model);
	QModelIndex selectedIndex = m_model->index(idxNote, 0);
	m_ui->listView->setCurrentIndex(selectedIndex);
}
