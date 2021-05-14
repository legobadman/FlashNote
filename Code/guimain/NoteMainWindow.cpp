#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "NoteMainWindow.h"
#include "notetrayicon.h"
#include "LeftSideItemDelegate.h"
#include "moc_NoteMainWindow.cpp"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	: QMainWindow(parent, Qt::Window)
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
}

void NoteMainWindow::onLeftTreeClicked(const QModelIndex& index)
{
	LEFT_SIDE_TYPE type = index.data(Qt::UserRole + 1).value<LEFT_SIDE_TYPE>();
	if (type == LEFT_SIDE_TYPE::ITEM_NEWNOTE)
	{
		emit newnote();
	}
	else if (type == LEFT_SIDE_TYPE::ITEM_NOTEBOOK)
	{

	}
}

void NoteMainWindow::showNavigationPane(bool show)
{
	if (show)
		m_ui->treeView->show();
	else
		m_ui->treeView->hide();
}

void NoteMainWindow::setApplication(QApplication* pApp)
{
	m_pApp = pApp;
}

void NoteMainWindow::closeEvent(QCloseEvent* event)
{
	//TODO: 外部需要回收窗口
	event->ignore();
	hide();
}
