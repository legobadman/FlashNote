#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "NoteMainWindow.h"
#include "LeftSideItemDelegate.h"
#include "moc_NoteMainWindow.cpp"


NoteMainWindow::NoteMainWindow(QWidget* parent)
	:QMainWindow(parent)
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
	
	leftsidemodel = new	QStandardItemModel(m_ui->treeView);

	QStandardItem* pNewNoteItem = new QStandardItem(
		QIcon(":/icons/btn_addnote.png"),
		u8"新建笔记"
	);
	pNewNoteItem->setSelectable(false);
	pNewNoteItem->setEditable(false);
	pNewNoteItem->setData(ITEM_NEWNOTE);

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"全部笔记"
	);
	pAllNotesItem->setEditable(false);
	pAllNotesItem->setData(ITEM_ALLNOTE);

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"笔记本"
	);
	pNoteBookItem->setEditable(false);
	pNoteBookItem->setData(ITEM_NOTEBOOK);

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"全部素材"
	);
	pMaterialItem->setEditable(false);
	pMaterialItem->setData(ITEM_MATERIAL);

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/socialmediashare.png"),
		u8"碎片信息"
	);
	pFragmentItem->setEditable(false);
	pFragmentItem->setData(ITEM_SOCIALMEDIA);

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/myspace.png"),
		u8"日记"
	);
	pDiaryItem->setEditable(false);
	pDiaryItem->setData(ITEM_DIARY);

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(ITEM_SCHEDULE);

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"草稿"
		);
	pDraftItem->setEditable(false);
	pDraftItem->setData(ITEM_DRAFT);

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"废纸篓"
	);
	pTrashItem->setEditable(false);
	pTrashItem->setData(ITEM_TRASH);

	leftsidemodel->appendRow(pNewNoteItem);
	leftsidemodel->appendRow(pAllNotesItem);
	leftsidemodel->appendRow(pNoteBookItem);
	leftsidemodel->appendRow(pMaterialItem);
	leftsidemodel->appendRow(pFragmentItem);
	leftsidemodel->appendRow(pDiaryItem);
	leftsidemodel->appendRow(pScheduleItem);
	leftsidemodel->appendRow(pDraftItem);
	leftsidemodel->appendRow(pTrashItem);

	m_ui->treeView->setModel(leftsidemodel);
	m_ui->treeView->setItemDelegate(new LeftSideItemDelegate(m_ui->treeView));

	m_ui->splitter->setStretchFactor(1, 3);

	showMaximized();
}