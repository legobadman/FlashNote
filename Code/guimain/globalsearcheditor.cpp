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


GlobalSearchEditor::GlobalSearchEditor(QWidget* parent)
	: QWidget(parent)
	, m_pFilterModel(NULL)
	, m_completer(NULL)
{
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	QHBoxLayout* pHLayout = new QHBoxLayout;
	pHLayout->setContentsMargins(10, 6, 10, 6);

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(21, 152, 255));
	setPalette(palette);

    ToolButton* onlyIcon = new ToolButton;
	QLabel* labelIcon = new QLabel;
	QIcon icon(":/icons/32x32/floatwin32.png");
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
	ToolButton* closeBtn = new ToolButton;
	closeBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	closeBtn->setIcon(QIcon(":/icons/16x16/white_close.png"));
	closeBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	closeBtn->setButtonStyle(ToolButton::ButtonIcon);
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
	pPopup->setItemDelegate(new NoteItemDelegate(pPopup, m_pEditor));
	m_pEditor->setCompleter(m_completer);

	setLayout(pHLayout);

	connect(closeBtn, SIGNAL(clicked()), this, SLOT(quit()));
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