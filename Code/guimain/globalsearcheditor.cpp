#include "stdafx.h"
#include "bookviewmodel.h"
#include "searcheditor.h"
#include "toolbutton.h"
#include "MyStyle.h"
#include "popupwidget.h"
#include "globalsearcheditor.h"
#include "notecompleter.h"
#include "searchitemdelegate.h"
#include "noteitemdelegate.h"
#include "guihelper.h"
#include "labelbutton.h"


GlobalSearchEditor::GlobalSearchEditor(QWidget* parent)
	: QWidget(parent)
	, m_pFilterModel(NULL)
	, m_completer(NULL)
{
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	QHBoxLayout* pHLayout = new QHBoxLayout;
	pHLayout->setContentsMargins(10, 6, 10, 6);

	QPalette palette;
	palette.setColor(QPalette::Window, AppHelper::colorBlue());
	setPalette(palette);

    ToolButton* onlyIcon = new ToolButton;
	QLabel* labelIcon = new QLabel;
	QIcon icon(":/icons/flashnote.png");
	labelIcon->setPixmap(icon.pixmap(MyStyle::dpiScaledSize(QSize(32, 32))));
    pHLayout->addWidget(labelIcon);

    m_pEditor = new SearchEditor;
	m_pEditor->setTextMargins(5, 5, 5, 5);
    m_pEditor->setFixedWidth(MyStyle::dpiScaled(460));
    m_pEditor->installEventFilter(this);
    QFont font("Microsoft YaHei", 11);
    m_pEditor->setFont(font);
    pHLayout->addWidget(m_pEditor);
	
	QVBoxLayout* pVLayout = new QVBoxLayout;
	NLabelButton* closeBtn = new NLabelButton;
	closeBtn->setIcons(QSize(16, 16), QIcon(":/icons/white_close.png"), QIcon(":/icons/white_close.png"), QIcon(":/icons/white_close_hover.png"));
	pVLayout->addWidget(closeBtn);

	pHLayout->addLayout(pVLayout);

	m_model = new QuickSearchModel(this);
	m_pFilterModel = new QSortFilterProxyModel(this);
	m_pFilterModel->setSourceModel(m_model);

	m_completer = new NoteCompleter(m_pEditor, m_pFilterModel, this);
	m_completer->setCaseSensitivity(Qt::CaseInsensitive);
	m_completer->setFilterMode(Qt::MatchContains);
	m_completer->setCompletionRole(ItemNoteAllContent);
	m_completer->setCompletionMode(QCompleter::PopupCompletion);
	m_completer->setPopupPosPolicy(true);

	QAbstractItemView* pPopup = m_completer->popup();
    disconnect(pPopup, SIGNAL(clicked(QModelIndex)), m_completer, SLOT(_q_complete(QModelIndex)));
	pPopup->viewport()->setAttribute(Qt::WA_Hover, true);
	pPopup->setItemDelegate(new NoteItemDelegate(pPopup, m_pEditor));
	pPopup->installEventFilter(this);
	m_pEditor->setCompleter(m_completer);

	setLayout(pHLayout);

	connect(closeBtn, SIGNAL(clicked()), this, SLOT(quit()));
	connect(m_completer, SIGNAL(activated(const QModelIndex&)), this, SLOT(onItemActivated(const QModelIndex&)));
	connect(pPopup, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemActivated(const QModelIndex&)));
}

GlobalSearchEditor::~GlobalSearchEditor()
{
}

void GlobalSearchEditor::mousePressEvent(QMouseEvent* event)
{
	m_origin_pos = event->pos();
}

void GlobalSearchEditor::mouseMoveEvent(QMouseEvent* event)
{
	QPoint delta = event->globalPos() - m_origin_pos;
	move(delta);
}

void GlobalSearchEditor::quit()
{
	close();
}

void GlobalSearchEditor::onItemActivated(const QModelIndex& index)
{
	com_sptr<INote> spNote = index.data(ItemCoreObjRole).value<com_sptr<INote>>();
	AppHelper::openNoteInIsoWindow(AppHelper::GetNoteId(spNote));
	hide();
}
