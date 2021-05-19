#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "notetrayicon.h"
#include "LeftSideItemDelegate.h"
#include "NoteMainWindow.h"
#include "note_types.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "newnotewindow.h"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent, Qt::Window)
{
	init();
	//TODO: 本地注册表缓存bookindex。
	int bookidx = 0;
	showNotesInfo(bookidx, getActiveNoteInBook(bookidx));
}

NoteMainWindow::~NoteMainWindow()
{
}

void NoteMainWindow::init()
{
	m_ui = new Ui::NoteMainWindow();
	m_ui->setupUi(this);

	setWindowIcon(QIcon(":/icons/bluewhite.png"));

	m_ui->splitter->setStretchFactor(1, 3);

	connect(m_ui->listpane, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(onLeftTreeClicked(const QModelIndex&)));
	connect(m_ui->listpane, SIGNAL(newnote()), this, SLOT(onNewNote()));
	connect(m_ui->booklist, SIGNAL(noteitemclicked(const QModelIndex&)),
		this, SLOT(onNoteItemClicked(const QModelIndex&)));
}

void NoteMainWindow::initSelected()
{
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(2, 0);
	m_ui->listpane->treeview()->selectionModel()->select(idx, QItemSelectionModel::Select);
	QString wtf = idx.data(Qt::DisplayRole).toString();
	m_ui->editwindow->setBookName(wtf);
}

void NoteMainWindow::onNewNote()
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL);
	pNewNoteWindow->init(getActiveBookIndex());
	pNewNoteWindow->showMaximized();
}

void NoteMainWindow::onShowNote(int idxBook, INote* pNote)
{
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(idxBook, &spNotebook);
	m_ui->editwindow->initWidget(spNotebook, pNote);
}

int NoteMainWindow::getActiveNoteInBook(int bookidx)
{
	//TODO: 以后再缓存活动index
	return 0;
}

void NoteMainWindow::showNotesInfo(int idxNotebook, int idxNote)
{
	//索引到笔记本下拉列表的第idxNotebook项，以及笔记本列表的第idxNote项。
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(1, 0);
	m_ui->listpane->treeview()->expand(idx);

	//最左边项的选择。
	QModelIndex book_idx = idx.child(idxNotebook, 0);
	m_ui->listpane->treeview()->selectionModel()->select(book_idx, QItemSelectionModel::Select);
	
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(idxNotebook, &spNotebook);
	QString bookName = AppHelper::GetNotebookName(spNotebook);

	m_ui->booklist->initNotebook(spNotebook, idxNote);

	com_sptr<INote> spNote;
	AppHelper::GetNote(spNotebook, idxNote, &spNote);
	onShowNote(idxNotebook, spNote);
}

void NoteMainWindow::onLeftTreeClicked(const QModelIndex& index)
{
	QModelIndex root = index.parent();
	if (root.isValid())
	{
		ITEM_CONTENT_TYPE type = root.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
		int bookidx = getActiveBookIndex();
		if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
		{
			showNotesInfo(bookidx, getActiveNoteInBook(bookidx));
		}
	}
}

int NoteMainWindow::getActiveBookIndex()
{
	QModelIndex idx = m_ui->listpane->treeview()->currentIndex();
	QModelIndex parent;

	QModelIndexList idxSelectedItems = m_ui->listpane->treeview()->selectionModel()->selectedRows();
	for (auto iter = idxSelectedItems.constBegin();
		iter != idxSelectedItems.constEnd();
		++iter)
	{
		parent = iter->parent();
	}

	if (parent.isValid() &&
		parent.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>() == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
	{
		int bookidx = idx.row();
		return bookidx;
	}
	return -1;
}

void NoteMainWindow::onNoteItemClicked(const QModelIndex& index)
{
	int idx = index.row();
	com_sptr<INotebook> spNotebook;
	int bookidx = getActiveBookIndex();
	AppHelper::GetNotebook(bookidx, &spNotebook);
	com_sptr<INote> spNote;
	AppHelper::GetNote(spNotebook, idx, &spNote);
	onShowNote(bookidx, spNote);
}

void NoteMainWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	hide();
}
