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
{
	m_pBookView = new BookListView;
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

	//m_pAllBookView = new BookListView;
	//m_pStackedListView = new QStackedWidget(this);
	//m_pStackedListView->addWidget(m_pAllBookView);
	//m_pStackedListView->addWidget(m_pBookView);

	m_pStackedEdit = new QStackedWidget(this);
	m_pStackedEdit->addWidget(m_pEditView);
	m_pStackedEdit->addWidget(m_pNoView);

	addWidget(m_pBookView);
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

	connect(m_pBookView, SIGNAL(noteitemselected(const QModelIndex&)),
		this, SLOT(onNoteItemSelected(const QModelIndex&)));
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

void NotesEditView::setNotebook(INoteCollection* pNotebook)
{
	m_pNotebook = pNotebook;
	QString noteid;	//TODO: 注册表实现
	//暂时用book的第一个note
	com_sptr<INote> spNote;
	AppHelper::GetNote(pNotebook, 0, &spNote);
	if (spNote)
	{
		noteid = AppHelper::GetNoteId(spNote);
	}
	onShowNotesView(noteid);
}

void NotesEditView::setAllNotes()
{

}

void NotesEditView::onNoteItemSelected(const QModelIndex& index)
{
	QString noteid = index.data(ItemCoreObjIdRole).toString();
	onShowNotesView(noteid);
}

void NotesEditView::onShowNotesView(QString noteid)
{
	//TODO: 要通过noteid取到note接口指针，从而取得bookid
	//，再取得book接口，而不能缓存	。

	m_pBookView->resetNotebook(m_pNotebook, noteid);

	com_sptr<INote> spNote;
	AppHelper::GetNoteById(noteid, &spNote);
	if (spNote == NULL)
	{
		m_pStackedEdit->setCurrentIndex(PAGE_NOEDIT);
	}
	else
	{
		m_pStackedEdit->setCurrentIndex(PAGE_EDITVIEW);
		m_pEditView->updateNoteInfo(m_pNotebook, spNote);
	}
}
