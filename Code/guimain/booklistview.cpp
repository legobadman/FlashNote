#include "stdafx.h"
#include "booklistview.h"
#include "ui_booklistview.h"
#include "MyStyle.h"
#include "guihelper.h"


BookListView::BookListView(QWidget* parent)
	: QWidget(parent)
	, m_idxNote(-1)
{
	init();
}

void BookListView::init()
{
	m_ui = new Ui::BookListView();
	m_ui->setupUi(this);

	QPalette palette(this->palette());
	palette.setColor(QPalette::Window, QColor(255, 255, 255));
	setAutoFillBackground(true);
	setPalette(palette);

	m_ui->sort->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->sort->setIcon(QIcon(":/icons/24x24/sort_options.png"));
	m_ui->sort->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->view->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->view->setIcon(QIcon(":/icons/24x24/listview.png"));
	m_ui->view->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	m_ui->more->setFixedSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->more->setIcon(QIcon(":/icons/24x24/more.png"));
	m_ui->more->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));

	connect(m_ui->listView, SIGNAL(clicked(const QModelIndex&)),
		this, SIGNAL(noteitemclicked(const QModelIndex&)));
}

BookListView::~BookListView()
{
}

void BookListView::initNotebook(INotebook* pNotebook, int idxNote)
{
	static const int nContentLimit = 74;

	m_spNotebook = pNotebook;
	m_idxNote = idxNote;

	m_model = new QStandardItemModel(this);
	for (int i = 0; i < m_spNotebook->GetCount(); i++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(m_spNotebook, i, &spNote);
		QString title = AppHelper::GetNoteTitle(spNote);
		QString content = AppHelper::GetNoteContent(spNote);

		if (title.isEmpty())
			title = u8"无标题笔记";

		QTextDocument html;
		html.setHtml(content);
		QString text_abbre = html.toPlainText();
		text_abbre = text_abbre.mid(0, nContentLimit);

		QStandardItem* pNoteItem = new QStandardItem(title + "\n" + text_abbre);
		pNoteItem->setSelectable(true);
		pNoteItem->setEditable(false);

		m_model->appendRow(pNoteItem);
	}

	m_ui->listView->setModel(m_model);
	QModelIndex selectedIndex = m_model->index(idxNote, 0);
	m_ui->listView->setCurrentIndex(selectedIndex);
}
