#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "NoteMainWindow.h"
#include "moc_NoteMainWindow.cpp"


LeftSideItemDelegate::LeftSideItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

void LeftSideItemDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model,
	const QModelIndex& index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
}

void LeftSideItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	QStyledItemDelegate::paint(painter, option, index);
}

QSize LeftSideItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int w = ((QWidget*)parent())->width();
	if (index.row() == 0)
		return QSize(w, 60);
	else
		return QSize(w, 36);
}

void LeftSideItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	
	bool bNewNote = index.row() == 0;
	QColor backgroundClr(42, 51, 60);
	QColor selectedClr(QColor(74, 93, 107));

	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	option->backgroundBrush.setStyle(Qt::SolidPattern);

	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(selectedClr);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		if (bNewNote)
		{
			option->backgroundBrush.setColor(backgroundClr);
			option->palette.setColor(QPalette::All, QPalette::Text, QColor(255, 255, 255));
		}
		else
		{
			option->backgroundBrush.setColor(QColor(53, 69, 83));
		}
	}
	else
	{
		option->backgroundBrush.setColor(backgroundClr);
	}
	if (bNewNote)
		option->font.setPointSize(13);
	else
		option->font.setPointSize(12);
}



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

	setWindowIcon(QIcon(":/icons/flash.png"));
	
	leftsidemodel = new	QStandardItemModel(m_ui->treeView);

	QStandardItem* pNewNoteItem = new QStandardItem(
		QIcon(":/icons/add_note.png"),
		u8"新建笔记"
	);

	pNewNoteItem->setSelectable(false);
	pNewNoteItem->setEditable(false);

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"全部笔记"
	);
	pAllNotesItem->setEditable(false);

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"笔记本"
	);
	pNoteBookItem->setEditable(false);

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"全部素材"
	);
	pMaterialItem->setEditable(false);

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/fragments.png"),
		u8"碎片信息"
	);
	pFragmentItem->setEditable(false);

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/diarys.png"),
		u8"日记"
	);
	pDiaryItem->setEditable(false);

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);
	pScheduleItem->setEditable(false);

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"草稿"
		);
	pDraftItem->setEditable(false);

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"废纸篓"
	);
	pTrashItem->setEditable(false);

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

	this->showMaximized();
}