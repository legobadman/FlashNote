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
		u8"新建笔记"
	);
	pNewNoteItem->setSelectable(false);
	pNewNoteItem->setEditable(false);
	pNewNoteItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_NEWNOTE));

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"全部笔记"
	);
	pAllNotesItem->setEditable(false);
	pAllNotesItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_ALLNOTE));

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"笔记本"
	);
	pNoteBookItem->setEditable(false);
	pNoteBookItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_NOTEBOOK));

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"全部素材"
	);
	pMaterialItem->setEditable(false);
	pMaterialItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_MATERIAL));

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/socialmediashare.png"),
		u8"碎片信息"
	);
	pFragmentItem->setEditable(false);
	pFragmentItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_SOCIALMEDIA));

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/myspace.png"),
		u8"日记"
	);
	pDiaryItem->setEditable(false);
	pDiaryItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_DIARY));

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_SCHEDULE));

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"草稿"
	);
	pDraftItem->setEditable(false);
	pDraftItem->setData(QVariant::fromValue<LEFT_SIDE_TYPE>(
		LEFT_SIDE_TYPE::ITEM_DRAFT));

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"废纸篓"
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