#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include "LeftSideItemDelegate.h"
#include "moc_NoteMainWindow.cpp"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	init();
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

	connect(m_ui->treeView, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(onLeftTreeClicked(const QModelIndex&)));

	showMaximized();
}

void NoteMainWindow::onLeftTreeClicked(const QModelIndex& index)
{
	LEFT_SIDE_TYPE type = index.data(Qt::UserRole + 1).value<LEFT_SIDE_TYPE>();
	if (type == LEFT_SIDE_TYPE::ITEM_NEWNOTE)
	{
		NoteEditWindow* pEditWin = new NoteEditWindow;
		pEditWin->show();
	}
	else if (type == LEFT_SIDE_TYPE::ITEM_NOTEBOOK)
	{

	}
}

void NoteMainWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	hide();
}


