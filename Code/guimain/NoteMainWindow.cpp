#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "notetrayicon.h"
#include "LeftSideItemDelegate.h"
#include "NoteMainWindow.h"
#include "note_types.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "newnotewindow.h"
#include "addbookdlg.h"
#include "noteseditview.h"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent, Qt::Window)
{
	init();
	//TODO: 本地注册表缓存bookindex。
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
	connect(m_ui->listpane, SIGNAL(newnote(NOTE_TYPE)), this, SLOT(onNewNote(NOTE_TYPE)));
	connect(m_ui->listpane, SIGNAL(addnotebook()), this, SLOT(onAddNotebook()));
}

void NoteMainWindow::initNotesView(int idxNotebook, int idxNote)
{
	//索引到笔记本下拉列表的第idxNotebook项，以及笔记本列表的第idxNote项。
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(1, 0);
	m_ui->listpane->treeview()->expand(idx);

	//最左边项的选择。
	QModelIndex book_idx = idx.child(idxNotebook, 0);
	m_ui->listpane->treeview()->selectionModel()->select(book_idx, QItemSelectionModel::Select);

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(idxNotebook, &spNotebook);

	m_ui->notesview->setNotebook(VIEW_NOTEBOOK, spNotebook);
}

void NoteMainWindow::onNewNote(NOTE_TYPE noteType)
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, noteType);
	pNewNoteWindow->init(getActiveBookId());
	pNewNoteWindow->showMaximized();
}

void NoteMainWindow::onAddNotebook()
{
	AddBookDlg* pDlg = new AddBookDlg(this);
	int ret = pDlg->exec();
	if (ret == QDialog::Accepted)
	{
		QString bookName = pDlg->getName();
		com_sptr<INotebook> spNotebook;
		HRESULT hr = CreateNotebook(&spNotebook);
		if (hr == S_OK)
		{
			BSTR bstrTitle = SysAllocString(bookName.toStdWString().c_str());
			spNotebook->SetName(bstrTitle);
			bool ret = RPCService::GetInstance().SynchronizeNotebook(spNotebook);
			Q_ASSERT(ret);
		}
	}
}

int NoteMainWindow::getActiveNoteInBook(int bookidx)
{
	//TODO: 以后再缓存活动index
	return 0;
}

void NoteMainWindow::onLeftTreeClicked(const QModelIndex& index)
{
	QModelIndex root = index.parent();
	ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM)
	{
		m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
		QString bookid = index.data(ItemCoreObjIdRole).toString();
		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);

		VARIANT varIndex;
		V_VT(&varIndex) = VT_BSTR;
		V_BSTR(&varIndex) = SysAllocString(bookid.toStdWString().data());

		com_sptr<INotebook> spNotebook;
		HRESULT hr = spNotebooks->Item(varIndex, &spNotebook);
		if (hr == S_OK)
		{
			m_ui->notesview->setNotebook(VIEW_NOTEBOOK, spNotebook);
		}
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
	{
		m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTEBOOKS_VIEW);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_TRASH)
	{
		m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
		com_sptr<ITrash> spTrash;
		coreApp->GetTrash(&spTrash);
		m_ui->notesview->setNotebook(VIEW_TRASH, spTrash);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_ALLNOTE)
	{
		m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
		m_ui->notesview->setNotebook(VIEW_ALLNOTES, NULL);
	}
}

QString NoteMainWindow::getActiveBookId()
{
	QModelIndex index = m_ui->listpane->treeview()->currentIndex();

	ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	QString bookId;
	if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM)
	{
		bookId = index.data(ItemCoreObjIdRole).toString();
	}
	else
	{
		com_sptr<INotebooks> spNotebooks;
		coreApp->GetNotebooks(&spNotebooks);
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebook(0, &spNotebook);
		bookId = AppHelper::GetNotebookId(spNotebook);
	}
	return bookId;
}

void NoteMainWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	hide();
}
