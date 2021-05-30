#include "stdafx.h"
#include "booklistview.h"
#include "note_editwindow.h"
#include "noteseditview.h"
#include "guihelper.h"
#include "MyStyle.h"
#include "LeftSideItemDelegate.h"


MySplitter::MySplitter(QWidget* parent /* = nullptr */)
	: QSplitter(parent)
{
}

void MySplitter::paintEvent(QPaintEvent* event)
{
	QSplitter::paintEvent(event);
}


NotesEditView::NotesEditView(QWidget* parent)
#ifdef SPLITTER_BASE
	: QSplitter(parent)
#else
	: QWidget(parent)
#endif
	, m_type(VIEW_NOTEBOOK)
	, m_pAllNotesView(NULL)
	, m_pTrashView(NULL)
{
	m_pEditView = new NoteEditWindow;
	m_pNoView = new QWidget;

#ifdef SPLITTER_BASE
	setObjectName(QString::fromUtf8("splitter233"));
	setOrientation(Qt::Horizontal);
	setFrameShape(QFrame::VLine);
	setHandleWidth(1);

	QPalette palette;
	palette.setBrush(QPalette::Window, QBrush(QColor(204, 204, 204)));
	setPalette(palette);

	m_pStackedListView = new QStackedWidget(this);

	m_pStackedEdit = new QStackedWidget(this);
	m_pStackedEdit->addWidget(m_pEditView);
	m_pStackedEdit->addWidget(m_pNoView);

	addWidget(m_pStackedListView);
	addWidget(m_pStackedEdit);
#else
	QSplitter* splitter = new QSplitter(this);
	splitter->setObjectName(QString::fromUtf8("splitter233"));
	splitter->setOrientation(Qt::Horizontal);
	splitter->setFrameShape(QFrame::VLine);
	splitter->setHandleWidth(1);

	splitter->addWidget(m_pBookView);
	splitter->addWidget(m_pEditView);
#endif

	m_pNoView->installEventFilter(this);
}

NotesEditView::~NotesEditView()
{
}

#ifndef SPLITTER_BASE
QSize NotesEditView::sizeHint() const
{
	int l = 0;
	int t = 0;

	QSize s;
	s = m_pBookView->sizeHint();
	if (s.isValid())
	{
		l += m_pBookView->width();
		t = qMax(t, s.width());
	}

	s = m_pEditView->sizeHint();
	if (s.isValid())
	{
		l += m_pBookView->width();
		t = qMax(t, s.width());
	}

	return QSize(l, t);
}
#endif

bool NotesEditView::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m_pNoView && event->type() == QEvent::Paint)
	{
		QPaintEvent* e = static_cast<QPaintEvent*>(event);
		QPainter painter(m_pNoView);
		painter.fillRect(e->rect(), QColor(238, 241, 242));
	}
	return QSplitter::eventFilter(watched, event);
}

void NotesEditView::setNotebook(BOOKVIEW_TYPE type, INoteCollection* pNoteCollection)
{
	m_type = type;
	BookListView* pListView = NULL;
	if (VIEW_ALLNOTES == m_type)
	{
		if (m_pAllNotesView == NULL)
		{
			m_pAllNotesView = new BookListView(this);
			m_pAllNotesView->initAllNotes();
			m_pStackedListView->addWidget(m_pAllNotesView);
		}
		pListView = m_pAllNotesView;
	}
	else if (VIEW_NOTEBOOK == m_type)
	{
		com_sptr<INotebook> spNotebook = com_sptr<INotebook>(pNoteCollection);
		QString bookid = AppHelper::GetNotebookId(spNotebook);
		auto iter = m_pNotebookViews.find(bookid);
		if (iter == m_pNotebookViews.end())
		{
			pListView = new BookListView(this);
			pListView->initNoteContainer(m_type, spNotebook);
			spNotebook->addWatcher(pListView);
			m_pNotebookViews.insert(bookid, pListView);
			m_pStackedListView->addWidget(pListView);
		}
		else
		{
			pListView = iter.value();
		}
	}
	else if (VIEW_TRASH == m_type)
	{
		com_sptr<ITrash> spTrash = com_sptr<ITrash>(pNoteCollection);
		if (m_pTrashView == NULL)
		{
			m_pTrashView = new BookListView(this);
			m_pTrashView->initNoteContainer(m_type, spTrash);
			m_pStackedListView->addWidget(m_pTrashView);
			spTrash->addWatcher(m_pTrashView);
		}
		pListView = m_pTrashView;
	}
	else
	{
		Q_ASSERT(false);
	}

	//代替原来的update
	m_pStackedListView->setCurrentWidget(pListView);
	QString noteid = pListView->getCurrentNoteId();
	onShowNotesView(noteid);
}

void NotesEditView::onNoteItemSelected(const QModelIndex& index)
{
	QString noteid = index.data(ItemCoreObjIdRole).toString();
	onShowNotesView(noteid);
}

void NotesEditView::onShowNotesView(QString noteid)
{
	com_sptr<INote> spNote;
	AppHelper::GetNoteById(noteid, &spNote);
	if (spNote == NULL)
	{
		m_pStackedEdit->setCurrentIndex(PAGE_NOEDIT);
	}
	else
	{
		m_pStackedEdit->setCurrentIndex(PAGE_EDITVIEW);

		com_sptr<INotebook> spNotebook;
		BSTR bstrBookid;
		spNote->GetBookId(&bstrBookid);
		std::wstring bookid(bstrBookid);
		AppHelper::GetNotebookById(QString::fromStdWString(bstrBookid), &spNotebook);

		m_pEditView->updateNoteInfo(spNotebook, spNote, VIEW_TRASH != m_type);
	}
}
