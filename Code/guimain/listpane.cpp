#include "stdafx.h"
#include "listpane.h"
#include "LeftSideItemDelegate.h"
#include "moc_listpane.cpp"


NavigationPanel::NavigationPanel(QWidget* parent)
	: QTreeView(parent)
	, leftsidemodel(NULL)
{
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	setAlternatingRowColors(true);

	leftsidemodel = new	QStandardItemModel(this);

	QStandardItem* pNewNoteItem = new QStandardItem(
		QIcon(":/icons/btn_addnote.png"),
		u8"�½��ʼ�"
	);
	pNewNoteItem->setSelectable(false);
	pNewNoteItem->setEditable(false);
	pNewNoteItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_NEWNOTE));

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"ȫ���ʼ�"
	);
	pAllNotesItem->setEditable(false);
	pAllNotesItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_ALLNOTE));

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"�ʼǱ�"
	);
	pNoteBookItem->setEditable(false);
	pNoteBookItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_NOTEBOOK));

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"ȫ���ز�"
	);
	pMaterialItem->setEditable(false);
	pMaterialItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_MATERIAL));

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/socialmediashare.png"),
		u8"��Ƭ��Ϣ"
	);
	pFragmentItem->setEditable(false);
	pFragmentItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_SOCIALMEDIA));

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/myspace.png"),
		u8"�ռ�"
	);
	pDiaryItem->setEditable(false);
	pDiaryItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_DIARY));

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"���ȱ�"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_SCHEDULE));

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"�ݸ�"
	);
	pDraftItem->setEditable(false);
	pDraftItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_DRAFT));

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"��ֽ¨"
	);
	pTrashItem->setEditable(false);
	pTrashItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_TRASH));

	leftsidemodel->appendRow(pNewNoteItem);
	leftsidemodel->appendRow(pAllNotesItem);
	leftsidemodel->appendRow(pNoteBookItem);
	leftsidemodel->appendRow(pMaterialItem);
	leftsidemodel->appendRow(pFragmentItem);
	leftsidemodel->appendRow(pDiaryItem);
	leftsidemodel->appendRow(pScheduleItem);
	leftsidemodel->appendRow(pDraftItem);
	leftsidemodel->appendRow(pTrashItem);

	setModel(leftsidemodel);
	setItemDelegate(new LeftSideItemDelegate(this));
}

NavigationPanel::~NavigationPanel()
{

}

void NavigationPanel::paintEvent(QPaintEvent* event)
{
	QTreeView::paintEvent(event);
}

void NavigationPanel::resizeEvent(QResizeEvent* event)
{
	QTreeView::resizeEvent(event);
}