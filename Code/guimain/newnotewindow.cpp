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

void NewNoteWindow::init(int bookid)
{
	if (bookid == -1)
	{
		//TODO: 后续应安排default book
		bookid = 0;
	}
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebook(bookid, &spNotebook);
	HRESULT hr = CreateNote(NORMAL_NOTE, &m_pNote);
	m_ui->editwindow->initWidget(spNotebook, m_pNote);
}

void NewNoteWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	deleteLater();
}