#include "stdafx.h"
#include "booklistview.h"
#include "note_editwindow.h"
#include "noteseditview.h"
#include "guihelper.h"
#include "MyStyle.h"
#include "dbservice.h"
#include "LeftSideItemDelegate.h"


NotesEditView::NotesEditView(QWidget* parent)
	: QSplitter(parent)
	, m_type(VIEW_NOTEBOOK)
	, m_pListView(NULL)
	, m_pAllNotesModel(NULL)
	, m_pTrashModel(NULL)
{
	m_pEditWindow = new NoteEditWindow;
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
	m_pStackedEdit->addWidget(m_pEditWindow);
	m_pStackedEdit->addWidget(m_pNoView);

	addWidget(m_pListView);
	addWidget(m_pStackedEdit);

	setStretchFactor(1, 3);
	setChildrenCollapsible(false);

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
	m_spNotes = pNoteCollection;
	
	QString noteid;
	QModelIndex index;	//暂时只取model第一个索引作为当前note。
	if (VIEW_ALLNOTES == m_type)
	{
		if (m_pAllNotesModel == NULL)
		{
			m_pAllNotesModel = new AllNotesModel(this);
			m_pAllNotesModel->initAllNotes();
		}
		index = m_pAllNotesModel->index(0, 0);
		m_pCurrFilterModel.reset(new QSortFilterProxyModel(this));
		m_pCurrFilterModel->setSourceModel(m_pAllNotesModel);
		m_pListView->resetModel(m_pCurrFilterModel.get(), VIEW_ALLNOTES, NULL);
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
		m_pCurrFilterModel.reset(new QSortFilterProxyModel(this));
		m_pCurrFilterModel->setSourceModel(pModel);
		m_pListView->resetModel(m_pCurrFilterModel.get(), VIEW_NOTEBOOK, pNoteCollection);
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
		m_pCurrFilterModel.reset(new QSortFilterProxyModel(this));
		m_pCurrFilterModel->setSourceModel(m_pTrashModel);
		m_pListView->resetModel(m_pCurrFilterModel.get(), VIEW_TRASH, pNoteCollection);
	}
	else
	{
		Q_ASSERT(false);
	}
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	onShowNotesView(spNote);
}

QString NotesEditView::currentNoteId()
{
	if (com_sptr<INote> spNote = m_pEditWindow->GetNote())
	{
		return AppHelper::GetNoteId(spNote);
	}
	return "";
}

INote* NotesEditView::currentNote()
{
	return m_pEditWindow->GetNote();
}

INoteCollection* NotesEditView::currentNotes()
{
	com_sptr<INoteCollection> spNotes;
	com_sptr<INote> spNote;
	DbService::GetInstance().RemoveNote(AppHelper::coreApp(), spNotes, spNote);
	return m_spNotes;
}

void NotesEditView::trashNote()
{
	if (m_pStackedEdit->currentWidget() == m_pEditWindow)
	{
		m_pEditWindow->trashNote();
	}
}

void NotesEditView::onNoteItemSelected(const QModelIndex& current, const QModelIndex& previous)
{
	QString noteid = current.data(ItemCoreObjIdRole).toString();
	com_sptr<INote> spNote = current.data(ItemCoreObjRole).value<com_sptr<INote>>();
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
		if (AppHelper::GetNoteType(pNote) == SCHEDULE)
		{
			com_sptr<ISchedules> spSchedules;
			AppHelper::coreApp()->GetSchedules(&spSchedules);
			spNotebook = spSchedules;
		}
		else
		{
			AppHelper::GetNotebookByNote(pNote, &spNotebook);
		}
		m_pEditWindow->updateNoteInfo(spNotebook, pNote, m_type == VIEW_TRASH);
	}
}
