#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "notetrayicon.h"
#include "LeftSideItemDelegate.h"
#include "NoteMainWindow.h"
#include "note_types.h"
#include "guihelper.h"
#include "rpcservice.h"
#include "dbservice.h"
#include "newnotewindow.h"
#include "addbookdlg.h"
#include "noteseditview.h"
#include "floatingmenubutton.h"
#include "notehook.h"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent, Qt::Window)
	, m_pMenuButton(NULL)
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

	m_pMenuButton = new FloatingMenuButton(NULL);
}

void NoteMainWindow::initNotesView(int idxNotebook, int idxNote)
{
	//索引到笔记本下拉列表的第idxNotebook项，以及笔记本列表的第idxNote项。
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(1, 0);
	m_ui->listpane->treeview()->expand(idx);

	idx = m_ui->listpane->treeview()->model()->index(2, 0);
	m_ui->listpane->treeview()->expand(idx);

	//最左边项的选择。
	QModelIndex book_idx = idx.child(idxNotebook, 0);
	m_ui->listpane->treeview()->selectionModel()->select(book_idx, QItemSelectionModel::Select);

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(idxNotebook, &spNotebook);
	if (!spNotebook)
	{
		idx = m_ui->listpane->treeview()->model()->index(0, 0);
		m_ui->listpane->treeview()->selectionModel()->select(idx, QItemSelectionModel::Select);

		m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
		m_ui->notesview->setNotebook(VIEW_ALLNOTES, NULL);
	}
	else
	{
		m_ui->notesview->setNotebook(VIEW_NOTEBOOK, spNotebook);
	}
}

void NoteMainWindow::_temp_hide_floatWin()
{
	if (m_pMenuButton)
		m_pMenuButton->hide();
}

void NoteMainWindow::onNewNote(NOTE_TYPE noteType)
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, noteType);
	if (noteType == SCHEDULE)
	{
		pNewNoteWindow->initSchedule();
	}
	else
	{
		QString bookId = getActiveBookId();
		if (bookId.isEmpty())
		{
			return;
		}
		pNewNoteWindow->init(bookId);
	}
	pNewNoteWindow->show();
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
			spNotebook->SetName(bookName.toStdWString());
#ifdef USE_RPC
			bool ret = RPCService::GetInstance().SynchronizeNotebook(spNotebook);
#else
			bool ret = DbService::GetInstance().SynchronizeNotebook(spNotebook);
#endif
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
		m_ui->stackedWidget2->setCurrentWidget(m_ui->notesview);
		QString bookid = index.data(ItemCoreObjIdRole).toString();
		com_sptr<INotebooks> spNotebooks;

		AppHelper::coreApp()->GetNotebooks(&spNotebooks);

		com_sptr<INotebook> spNotebook;
		HRESULT hr = spNotebooks->Item(bookid.toStdWString(), &spNotebook);
		if (hr == S_OK)
		{
			m_ui->notesview->setNotebook(VIEW_NOTEBOOK, spNotebook);
		}
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->booksview);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_TRASH)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->notesview);
		com_sptr<ITrash> spTrash;
		AppHelper::coreApp()->GetTrash(&spTrash);
		m_ui->notesview->setNotebook(VIEW_TRASH, spTrash);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_ALLNOTE)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->notesview);
		m_ui->notesview->setNotebook(VIEW_ALLNOTES, NULL);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULE)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->schedulesview);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULEITEM)
	{
		com_sptr<ISchedules> spSchedules;
		AppHelper::coreApp()->GetSchedules(&spSchedules);
		com_sptr<INotebook> spNotebook(spSchedules);

		QString noteid = index.data(ItemCoreObjIdRole).toString();
		com_sptr<INote> spNote;
		spNotebook->Item(noteid.toStdWString(), &spNote);

		m_ui->scheduleeditor->updateNoteInfo(spNotebook, spNote, true);
		m_ui->stackedWidget2->setCurrentWidget(m_ui->scheduleeditor);
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
		AppHelper::coreApp()->GetNotebooks(&spNotebooks);
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

bool NoteMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	if (message)
	{
		MSG* msg = reinterpret_cast<MSG*>(message);
		if (msg->message == WM_COPYDATA)
		{
			COPYDATASTRUCT* data = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);
			EXTRACT_INFO info = *reinterpret_cast<EXTRACT_INFO*>(data->lpData);
			//if (GetCurrentProcessId() == info.dwProcessId)
			//{
   //             return QWidget::nativeEvent(eventType, message, result);
			//}
			QString extractText = QString::fromStdWString(info.text);
			m_pMenuButton->SetExtractText(extractText);
			if (extractText.isEmpty())
			{
				m_pMenuButton->hide();
			}
			else
			{
				m_pMenuButton->setGeometry(QRect(info.p.x + 20, info.p.y + 20, 32, 32));
				m_pMenuButton->show();
			}
		}
	}
	return QWidget::nativeEvent(eventType, message, result);
}