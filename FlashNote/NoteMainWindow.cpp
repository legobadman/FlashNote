#include "stdafx.h"
#include "ui_NoteMainWindow.h"
#include "NoteMainWindow.h"
#include "moc_NoteMainWindow.cpp"


LeftSideItemDelegate::LeftSideItemDelegate(QObject* parent)
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
	//QStyleOptionViewItem opt = option;
	//initStyleOption(&opt, index);

	QStyledItemDelegate::paint(painter, option, index);
}

QSize LeftSideItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(276, 36);
	return QStyledItemDelegate::sizeHint(option, index);
}

void LeftSideItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(QColor(74, 93, 107));
		option->backgroundBrush.setStyle(Qt::SolidPattern);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		option->backgroundBrush.setColor(QColor(53, 69, 83));
		option->backgroundBrush.setStyle(Qt::SolidPattern);
	}
	else
	{
		option->backgroundBrush.setColor(QColor(42, 51, 60));
		option->backgroundBrush.setStyle(Qt::SolidPattern);
	}
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

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"全部笔记"
	);

	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"笔记本"
	);

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"全部素材"
	);

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/fragments.png"),
		u8"碎片信息"
	);

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/diarys.png"),
		u8"日记"
	);

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"废纸篓"
	);

	leftsidemodel->appendRow(pNewNoteItem);
	leftsidemodel->appendRow(pAllNotesItem);
	leftsidemodel->appendRow(pNoteBookItem);
	leftsidemodel->appendRow(pMaterialItem);
	leftsidemodel->appendRow(pFragmentItem);
	leftsidemodel->appendRow(pDiaryItem);
	leftsidemodel->appendRow(pScheduleItem);
	leftsidemodel->appendRow(pTrashItem);

	m_ui->treeView->setModel(leftsidemodel);
	m_ui->treeView->setItemDelegate(new LeftSideItemDelegate(m_ui->treeView));
}