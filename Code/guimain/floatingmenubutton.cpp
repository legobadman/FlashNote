#include "stdafx.h"
#include "floatingmenubutton.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "dbservice.h"


FloatingMenuButton::FloatingMenuButton(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("floating_window");
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#ifdef DEBUG_EXTRACT_TEXT
	m_pushbutton = new QPushButton(this);
	m_pushbutton->setText("init");
	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->addWidget(m_pushbutton);
	setLayout(pLayout);
#else
    m_menubutton = new ToolButton(this);
    m_menubutton->setIcon(QIcon(":/icons/32x32/floatwin32.png"));
    m_menubutton->setIconSize(MyStyle::dpiScaledSize(QSize(32, 32)));
    m_menubutton->setFixedSize(MyStyle::dpiScaledSize(QSize(32, 32)));
    connect(m_menubutton, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
#endif
}

FloatingMenuButton::~FloatingMenuButton()
{

}

void FloatingMenuButton::SetExtractText(const QString& text)
{
	m_text = text;
#ifdef DEBUG_EXTRACT_TEXT
	m_pushbutton->setText(text);
	update();
#else
	m_menubutton->setText(m_text);
#endif
}

void FloatingMenuButton::onBtnClicked()
{
    hide();
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNotebookByName(u8"临时笔记本", &spNotebook);
	if (spNotebook)
	{
		com_sptr<INote> spNote;
		HRESULT hr = CreateNote(NORMAL_NOTE, &spNote);
		spNote->SetTitle(m_text.toStdWString());
		//TODO: 要转为html作为正文。
		spNotebook->AddNote(spNote);
		//bool ret = DbService::GetInstance().SynchronizeNotebook(spNotebook);
	}
}

void FloatingMenuButton::enterEvent(QEvent* event)
{
}

void FloatingMenuButton::leaveEvent(QEvent* event)
{

}
