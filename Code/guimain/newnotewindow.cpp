#include "stdafx.h"
#include "newnotewindow.h"
#include "ui_newnotewindow.h"
#include "guihelper.h"


NewNoteWindow::NewNoteWindow(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui = new Ui::NewNoteWindow();
	m_ui->setupUi(this);
	setWindowIcon(QIcon(":/icons/bluewhite.png"));
}

NewNoteWindow::~NewNoteWindow()
{
}

void NewNoteWindow::init(QString bookid)
{
	Q_ASSERT(!bookid.isEmpty());

	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookById(bookid, &spNotebook);
	HRESULT hr = CreateNote(NORMAL_NOTE, &m_pNote);
	m_ui->editwindow->updateNoteInfo(spNotebook, m_pNote, true);
}

void NewNoteWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	deleteLater();
}