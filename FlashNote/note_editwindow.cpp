#include "stdafx.h"
#include "note_editwindow.h"
#include "ui_note_editwindow.h"
#include "moc_note_editwindow.cpp"
#include "MyStyle.h"


NoteEditWindow::NoteEditWindow(QWidget* parent)
    : QWidget(parent)
    , m_pNote(NULL)
{
    init();
    initContent();
}

NoteEditWindow::~NoteEditWindow()
{
}

INote* NoteEditWindow::GetNote()
{
    return m_pNote;
}

bool NoteEditWindow::isTitleEmpty() const
{
    if (!m_pNote)
        return true;
    else
        return m_pNote->GetTitle().empty();
}

void NoteEditWindow::init()
{
    m_ui = new Ui::NoteEditWindow;
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    setAutoFillBackground(true);
    setPalette(palette);

    m_ui->setupUi(this);

    initCustomWidget();
}

void NoteEditWindow::initCustomWidget()
{
	m_ui->font_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(150, 28)));
	m_ui->fontsize_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 28)));

	m_ui->bold->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->bold->setIcon(QIcon(":/icons/16x16/Bold.png"));
	m_ui->bold->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->italic->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->italic->setIcon(QIcon(":/icons/16x16/Italic.png"));
	m_ui->italic->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->underline->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->underline->setIcon(QIcon(":/icons/16x16/Underline.png"));
	m_ui->underline->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->highlight->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->highlight->setIcon(QIcon(":/icons/16x16/highlighted.png"));
	m_ui->highlight->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->fontcolor->setFixedSize(MyStyle::dpiScaledSize(QSize(45, 30)));
	m_ui->fontcolor->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->codeblock->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->codeblock->setIcon(QIcon(":/icons/16x16/codeblock.png"));
	m_ui->codeblock->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->item_symbol->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->item_symbol->setIcon(QIcon(":/icons/16x16/item_symbol.png"));
	m_ui->item_symbol->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->item_id->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->item_id->setIcon(QIcon(":/icons/16x16/item_id.png"));
	m_ui->item_id->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->alignment->setFixedSize(MyStyle::dpiScaledSize(QSize(45, 30)));
	m_ui->alignment->setIcon(QIcon(":/icons/16x16/left_alignment.png"));
	m_ui->alignment->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->attachment->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->attachment->setIcon(QIcon(":/icons/16x16/attachment.png"));
	m_ui->attachment->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->photo->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->photo->setIcon(QIcon(":/icons/16x16/photo.png"));
	m_ui->photo->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
}

void NoteEditWindow::initContent()
{
    m_ui->editTitle->setPlaceholderText(u8"БъЬт");
}