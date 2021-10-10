#include "stdafx.h"
#include "floatingmenubutton.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "dbservice.h"
#include "labelbutton.h"


FloatingMenuButton::FloatingMenuButton(QWidget* parent)
	: QWidget(parent)
{
	setWindowTitle("floating_window");
	setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);

	QHBoxLayout* pMainLayout = new QHBoxLayout;

	QIcon icon = QIcon(":/icons/flashnote.png");
	NLabelButton* pLabel = new NLabelButton;
	pLabel->setIcons(MyStyle::dpiScaledSize(QSize(32, 32)), icon, icon, icon);
	connect(pLabel, SIGNAL(clicked()), this, SLOT(onBtnClicked()));

	pMainLayout->addWidget(pLabel);
	pMainLayout->setMargin(0);
	pMainLayout->setContentsMargins(0, 0, 0, 0);

	setLayout(pMainLayout);
}

FloatingMenuButton::~FloatingMenuButton()
{
}

void FloatingMenuButton::SetExtractText(const QString& text)
{
	m_text = text;
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
