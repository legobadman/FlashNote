#include "stdafx.h"
#include "booklistview.h"
#include "note_editwindow.h"
#include "noteseditview.h"
#include "guihelper.h"
#include "MyStyle.h"
#include "LeftSideItemDelegate.h"


NotesEditView::NotesEditView(QWidget* parent)
	: QSplitter(parent)
	, m_type(VIEW_NOTEBOOK)
	, m_pListView(NULL)
	, m_pAllNotesModel(NULL)
	, m_pTrashModel(NULL)
{
	m_pEditView = new NoteEditWindow;
	m_pNoView = new QWidget;

	m_pListView = new BookListView(this);

	setObjectName(QString::fromUtf8("splitter233"));
	setOrientation(Qt::Horizontal);
	setFrameShape(QFrame::VLine);
	setHandleWidth(1);

	QPalette palette;
	palette.setBrush(QPalette::Window, QBrush(QColor(204, 204, 204)));
	setPalette(palette);

	m_pStackedEdit = new QStackedWidget(this);
	m_pStackedEdit->addWidget(m_pEditView);
	m_pStackedEdit->addWidget(m_pNoView);

	addWidget(m_pListView);
	addWidget(m_pStackedEdit);

	m_pNoView->installEventFilter(this);
}

NotesEditView::~NotesEditView()
{
}

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

//临时noteid的获取
QString getCurrentNoteId(INoteCollection* pNoteCollection)
{
	com_sptr<INote> spNote;
	AppHelper::GetNote(pNoteCollection, 0, &spNote);
	return AppHelper::GetNoteId(spNote);
}

void NotesEditView::setNotebook(BOOKVIEW_TYPE type, INoteCollection* pNoteCollection)
{
	m_type = type;
	
	QString noteid;
	QModelIndex index;	//暂时只取model第一个索引作为当前note。
	if (VIEW_ALLNOTES == m_type)
	{
		if (m_pAllNotesModel == NULL)
		{
			m_pAllNotesModel = new AllNotesModel(this);
			m_pAllNotesModel->initAllNotes();
			index = m_pAllNotesModel->index(0, 0);
		}
		m_pListView->resetModel(m_pAllNotesModel, VIEW_ALLNOTES, NULL);
	}
	else if (VIEW_NOTEBOOK == m_type)
	{
		com_sptr<INotebook> spNotebook = com_sptr<INotebook>(pNoteCollection);
		QString bookid = AppHelper::GetNotebookId(spNotebook);

		BookViewModel* pModel = NULL;
		auto iter = m_models.find(bookid);
		if (iter == m_models.end())
		{
			pModel = new BookViewModel(this);
			pModel->initFromCollection(pNoteCollection);
			noteid = getCurrentNoteId(pNoteCollection);
			m_models.insert(bookid, pModel);
		}
		else
		{
			pModel = iter.value();
		}
		index = pModel->index(0, 0);
		m_pListView->resetModel(pModel, VIEW_NOTEBOOK, pNoteCollection);
	}
	else if (VIEW_TRASH == m_type)
	{
		if (m_pTrashModel == NULL)
		{
			m_pTrashModel = new BookViewModel(this);
			m_pTrashModel->initFromCollection(pNoteCollection);
			noteid = getCurrentNoteId(pNoteCollection);
		}
		index = m_pTrashModel->index(0, 0);
		m_pListView->resetModel(m_pTrashModel, VIEW_TRASH, pNoteCollection);
	}
	else
	{
		Q_ASSERT(false);
	}
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	onShowNotesView(spNote);
}

void NotesEditView::onNoteItemSelected(const QModelIndex& index)
{
	QString noteid = index.data(ItemCoreObjIdRole).toString();
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	onShowNotesView(spNote);
}

void NotesEditView::onShowNotesView(INote* pNote)
{
	if (pNote == NULL)
	{
		m_pStackedEdit->setCurrentIndex(PAGE_NOEDIT);
	}
	else
	{
		m_pStackedEdit->setCurrentIndex(PAGE_EDITVIEW);

		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookByNote(pNote, &spNotebook);
		m_pEditView->updateNoteInfo(spNotebook, pNote, VIEW_TRASH != m_type);
	}
}
