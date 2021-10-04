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
#include "uiapplication.h"
#include "selectnotebookdlg.h"

#ifdef Q_OS_WIN
#include "notehook.h"
#endif

NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent, Qt::Window)
	, m_pMenuButton(NULL)
{
	init();
	initMenuSignal();
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

	connect(m_ui->listpane, SIGNAL(currentChanged(const QModelIndex&)),
		this, SLOT(onLeftTreeClicked(const QModelIndex&)));
	connect(m_ui->listpane, SIGNAL(newnote(NOTE_TYPE)), this, SLOT(onNewNote(NOTE_TYPE)));
	connect(m_ui->listpane, SIGNAL(addnotebook()), this, SLOT(onAddNotebook()));

	m_pMenuButton = new FloatingMenuButton(NULL);
}

void NoteMainWindow::initNotesView(int idxNotebook, int idxNote)
{
	//暂时不做缓存索引。
	idxNotebook = -1;
	//索引到笔记本下拉列表的第idxNotebook项，以及笔记本列表的第idxNote项。
	QModelIndex idx = m_ui->listpane->treeview()->model()->index(1, 0);
	m_ui->listpane->treeview()->expand(idx);

	idx = m_ui->listpane->treeview()->model()->index(2, 0);
	m_ui->listpane->treeview()->expand(idx);

	idx = m_ui->listpane->treeview()->model()->index(0, 0);
	m_ui->listpane->treeview()->selectionModel()->select(idx, QItemSelectionModel::Select);

	m_ui->stackedWidget2->setCurrentIndex(CONTENT_MAIN_VIEW::NOTES_VIEW);
	m_ui->notesview->setNotebook(VIEW_ALLNOTES, NULL);
}

void NoteMainWindow::initMenuSignal()
{
	connect(m_ui->action_NewBook, &QAction::triggered, [=]() { onAddNotebook(); });
	connect(m_ui->action_NewNote, &QAction::triggered, [=]() { onNewNote(NORMAL_NOTE); });
	connect(m_ui->action_NewMindmap, &QAction::triggered, [=]() { onNewNote(MINDMAP); });
	connect(m_ui->action_NewSchedule, &QAction::triggered, [=]() { onNewNote(SCHEDULE); });
	connect(m_ui->action_Search, &QAction::triggered, [=]() { AppHelper::uiApp()->showFloatingSearcher(); });
	connect(m_ui->action_OpenNewWindow, &QAction::triggered, [=]() { 
			AppHelper::openNoteInIsoWindow(AppHelper::GetNoteId(GetCurrentActiveNote()));
		});
	connect(m_ui->action_DeleteNote, &QAction::triggered, [=]() {
		if (m_ui->stackedWidget2->currentWidget() == m_ui->notesview)
		{
			m_ui->notesview->trashNote();
		}
		else if (m_ui->stackedWidget2->currentWidget() == m_ui->scheduleeditor)
		{
            m_ui->scheduleeditor->trashNote();
        }
	});
	connect(m_ui->action_MoveNote, &QAction::triggered, [=]() {
		com_sptr<INote> spNote = GetCurrentActiveNote();
		if (spNote)
		{
			NOTE_TYPE type = UNKNOWN_TYPE;
			spNote->GetType(&type);
			if (type != SCHEDULE && type != UNKNOWN_TYPE)
			{
				SelectNotebookDlg dlg;
                dlg.setWindowTitle(tr("Move Note"));
				dlg.setButtonText(tr("&Move(M)"), tr("&Cancel(C)"));
				if (QDialog::Accepted == dlg.exec())
				{
					QString srcBookId;
					wstring bookId;
					spNote->GetBookId(bookId);
					srcBookId = QString::fromStdWString(bookId);
					QString destBookId = dlg.getBookId();

					com_sptr<INotebook> spSrcbook, spDestbook;
					AppHelper::GetNotebookById(srcBookId, &spSrcbook);
					AppHelper::GetNotebookById(destBookId, &spDestbook);

					DbService::GetInstance().MoveNotebook(spSrcbook, spDestbook, spNote);
                }
			}
		}
	});
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

INote* NoteMainWindow::GetCurrentActiveNote()
{
    if (m_ui->stackedWidget2->currentWidget() == m_ui->notesview)
    {
        return m_ui->notesview->currentNote();
    }
    else if (m_ui->stackedWidget2->currentWidget() == m_ui->scheduleeditor)
    {
        return m_ui->scheduleeditor->GetNote();
    }
	else
	{
		return NULL;
	}
}

INoteCollection* NoteMainWindow::GetCurrentNoteCollection()
{
    if (m_ui->stackedWidget2->currentWidget() == m_ui->notesview)
    {
		if (com_sptr<INote> spNote = m_ui->notesview->currentNote())
		{
            std::wstring bookId;
            spNote->GetBookId(bookId);
            QString bookid = QString::fromStdWString(bookId);
            com_sptr<INotebook> spNotebook;
            AppHelper::GetNotebookById(bookid, &spNotebook);
            return spNotebook;
		}
		return NULL;
    }
    else if (m_ui->stackedWidget2->currentWidget() == m_ui->scheduleeditor)
    {
		com_sptr<ISchedules> spNoteColl;
		AppHelper::coreApp()->GetSchedules(&spNoteColl);
		return spNoteColl;
    }
    else
    {
        return NULL;
    }
}

void NoteMainWindow::_enableNoteMenu(bool enable)
{
    m_ui->action_OpenNewWindow->setEnabled(enable);
    m_ui->action_MoveNote->setEnabled(enable);
    m_ui->action_DeleteNote->setEnabled(enable);
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
		_enableNoteMenu(true);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->booksview);
		_enableNoteMenu(false);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_TRASH)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->notesview);
		com_sptr<ITrash> spTrash;
		AppHelper::coreApp()->GetTrash(&spTrash);
		m_ui->notesview->setNotebook(VIEW_TRASH, spTrash);
		_enableNoteMenu(true);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_ALLNOTE)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->notesview);
		m_ui->notesview->setNotebook(VIEW_ALLNOTES, NULL);
		_enableNoteMenu(true);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULE)
	{
		m_ui->stackedWidget2->setCurrentWidget(m_ui->schedulesview);
		_enableNoteMenu(false);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULEITEM)
	{
		com_sptr<ISchedules> spSchedules;
		AppHelper::coreApp()->GetSchedules(&spSchedules);
		com_sptr<INotebook> spNotebook(spSchedules);

		QString noteid = index.data(ItemCoreObjIdRole).toString();
		com_sptr<INote> spNote;
		spNotebook->Item(noteid.toStdWString(), &spNote);

		m_ui->scheduleeditor->updateNoteInfo(spNotebook, spNote, false);
		m_ui->stackedWidget2->setCurrentWidget(m_ui->scheduleeditor);
		_enableNoteMenu(true);
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_MATERIAL)
	{
		_enableNoteMenu(false);
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
#ifdef Q_OS_WIN
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
#endif
	return QWidget::nativeEvent(eventType, message, result);
}