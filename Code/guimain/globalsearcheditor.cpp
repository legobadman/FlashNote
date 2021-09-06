#include "stdafx.h"
#include "globalsearcheditor.h"
#include "searcheditor.h"
#include "toolbutton.h"
#include "MyStyle.h"


GlobalSearchEditor::GlobalSearchEditor(QWidget* parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	QHBoxLayout* pHLayout = new QHBoxLayout;
	pHLayout->setContentsMargins(10, 2., 10, 2);
	QIcon icon(":/icons/32x32/floatwin32.png");

	QPalette palette;
	palette.setColor(QPalette::Window, QColor(21, 152, 255));
	setPalette(palette);

	ToolButton* onlyIcon = new ToolButton;
	onlyIcon->setFixedSize(MyStyle::dpiScaledSize(QSize(32, 32)));
	onlyIcon->setIcon(QIcon(":/icons/32x32/floatwin32.png"));
	onlyIcon->setIconSize(MyStyle::dpiScaledSize(QSize(32, 32)));
	onlyIcon->setButtonStyle(ToolButton::ButtonIcon);
	pHLayout->addWidget(onlyIcon);

	SearchEditor* pEditor = new SearchEditor;
	pEditor->setFixedWidth(MyStyle::dpiScaled(256));
	pHLayout->addWidget(pEditor);
	
	QVBoxLayout* pVLayout = new QVBoxLayout;
	ToolButton* closeBtn = new ToolButton;
	closeBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	closeBtn->setIcon(QIcon(":/icons/16x16/white_close.png"));
	closeBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	closeBtn->setButtonStyle(ToolButton::ButtonIcon);
	pVLayout->addWidget(closeBtn);

	pHLayout->addLayout(pVLayout);
	
	setLayout(pHLayout);

	connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
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