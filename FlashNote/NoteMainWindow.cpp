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
		u8"�½��ʼ�"
	);
	pNewNoteItem->setSelectable(false);
	pNewNoteItem->setEditable(false);
	pNewNoteItem->setData(ITEM_NEWNOTE);

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"ȫ���ʼ�"
	);
	pAllNotesItem->setEditable(false);
	pAllNotesItem->setData(ITEM_ALLNOTE);

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"�ʼǱ�"
	);
	pNoteBookItem->setEditable(false);
	pNoteBookItem->setData(ITEM_NOTEBOOK);

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"ȫ���ز�"
	);
	pMaterialItem->setEditable(false);
	pMaterialItem->setData(ITEM_MATERIAL);

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/socialmediashare.png"),
		u8"��Ƭ��Ϣ"
	);
	pFragmentItem->setEditable(false);
	pFragmentItem->setData(ITEM_SOCIALMEDIA);

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/myspace.png"),
		u8"�ռ�"
	);
	pDiaryItem->setEditable(false);
	pDiaryItem->setData(ITEM_DIARY);

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"���ȱ�"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(ITEM_SCHEDULE);

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"�ݸ�"
		);
	pDraftItem->setEditable(false);
	pDraftItem->setData(ITEM_DRAFT);

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"��ֽ¨"
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