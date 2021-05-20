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
	//TODO: ����ע�����bookindex��
	int bookidx = 0;
	initNotesView(bookidx, getActiveNoteInBook(bookidx));
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
}

void NoteMainWindow::initNotesView(int idxNotebook, int idxNote)
{
	//�������ʼǱ������б�ĵ�idxNotebook��Լ��ʼǱ��б�ĵ�idxNote�
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(1, 0);
	m_ui->listpane->treeview()->expand(idx);

	//��������ѡ��
	QModelIndex book_idx = idx.child(idxNotebook, 0);
	m_ui->listpane->treeview()->selectionModel()->select(book_idx, QItemSelectionModel::Select);

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(idxNotebook, &spNotebook);

	m_ui->notesview->setNotebook(spNotebook);
}

void NoteMainWindow::onNewNote()
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL);
	pNewNoteWindow->init(getActiveBookIndex());
	pNewNoteWindow->showMaximized();
}

int NoteMainWindow::getActiveNoteInBook(int bookidx)
{
	//TODO: �Ժ��ٻ���index
	return 0;
}

void NoteMainWindow::onLeftTreeClicked(const QModelIndex& index)
{
	QModelIndex root = index.parent();
	if (root.isValid())
	{
		ITEM_CONTENT_TYPE type = root.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
		if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
		{
			m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
			int bookidx = getActiveBookIndex();
			com_sptr<INotebook> spNotebook;
			AppHelper::GetNotebook(bookidx, &spNotebook);
			m_ui->notesview->setNotebook(spNotebook);
		}
	}
	else
	{
		ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
		if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
		{
			m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTEBOOKS_VIEW);
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

void NoteMainWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	hide();
}
