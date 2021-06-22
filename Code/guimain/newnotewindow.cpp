#include "stdafx.h"
#include "newnotewindow.h"
#include "ui_newnotewindow.h"
#include "guihelper.h"


NewNoteWindow::NewNoteWindow(QWidget* parent, NOTE_TYPE type)
	: QMainWindow(parent)
	, m_type(type)
{
	m_ui = new Ui::NewNoteWindow();
	m_ui->setupUi(this);
	setWindowIcon(QIcon(":/icons/bluewhite.png"));
	connect(m_ui->editwindow, SIGNAL(noteCommited(const QString&)), this, SIGNAL(noteCommited(const QString&)));
}

NewNoteWindow::~NewNoteWindow()
{
}

void NewNoteWindow::init(QString bookid)
{
	Q_ASSERT(!bookid.isEmpty());

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookById(bookid, &spNotebook);
	HRESULT hr = CreateNote(m_type, &m_pNote);
	m_ui->editwindow->updateNoteInfo(spNotebook, m_pNote, true);
}

void NewNoteWindow::open(QString bookid, QString noteid)
{
	Q_ASSERT(!bookid.isEmpty() && !noteid.isEmpty());

	setWindowTitle(QString(u8"�༭�ʼ�"));

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookById(bookid, &spNotebook);

	com_sptr<INote> spNote;
	AppHelper::GetNote(spNotebook, noteid, &spNote);

	m_ui->editwindow->updateNoteInfo(spNotebook, spNote, true);
}

void NewNoteWindow::initSchedule()
{
	com_sptr<ISchedules> spSchedules;
	coreApp->GetSchedules(&spSchedules);
	HRESULT hr = CreateNote(m_type, &m_pNote);
	com_sptr<INotebook> spNotebook = spSchedules;
	m_ui->editwindow->updateNoteInfo(spNotebook, m_pNote, true);
}

void NewNoteWindow::getNote(INote** ppNote)
{
	if (!ppNote)
		return;
	*ppNote = m_pNote;
	(*ppNote)->AddRef();
}

void NewNoteWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	deleteLater();
}