#include "stdafx.h"

#include "note_editwindow.h"
#include "rpcservice.h"
#include "note_types.h"
#include "guihelper.h"
#include "moc_note_editwindow.cpp"
#include "MyStyle.h"
#include "common_types.h"
#include <QtGui/QClipboard>
#include <QtGui/QTextDocumentWriter>
#include <QMimeData>
#include <QtGui/QFontDatabase>
#include <QInputDialog>
#include <QColorDialog>
#include <QtGui/QTextList>
#include <QtDebug>
#include <QFileDialog>
#include <QtGui/QImageReader>
#include <QSettings>
#include <QBuffer>
#include <QUrl>
#include <QPlainTextEdit>
#include <QMenu>
#include <QDialog>


NoteEditWindow::NoteEditWindow(QWidget* parent)
    : QWidget(parent)
	, m_type(UNKNOWN_TYPE)
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

void NoteEditWindow::updateNoteInfo(INotebook* pNotebook, INote* pNote, bool edittable)
{
	m_pNotebook = pNotebook;
	m_pNote = pNote;

	m_pNote->GetType(&m_type);

	m_bEdittable = edittable;

	QString bookName = AppHelper::GetNotebookName(m_pNotebook);
	m_ui->bookmenu->blockSignals(true);
	m_ui->bookmenu->setText(bookName);
	m_ui->bookmenu->blockSignals(false);

	QString title = AppHelper::GetNoteTitle(m_pNote);
	QString content = AppHelper::GetNoteContent(m_pNote);

	m_ui->editTitle->blockSignals(true);
	m_ui->editTitle->setText(title);
	m_ui->editTitle->blockSignals(false);
	m_ui->editTitle->setReadOnly(!m_bEdittable);

	if (m_type == NORMAL_NOTE)
	{
		m_ui->noramlEditor->initContent(content, !m_bEdittable);
		m_ui->stackedWidget->setCurrentIndex(0);
	}
	else if (m_type == MINDMAP)
	{
		m_ui->mindmapEditor->initContent(content);
		m_ui->stackedWidget->setCurrentIndex(1);
	}
	else
	{
		Q_ASSERT(false);
	}

	update();
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
	initSlots();
}

void NoteEditWindow::setBookName(const QString& name)
{
	m_ui->bookmenu->setText(name);
	update();
}

void NoteEditWindow::initSlots()
{
	connect(m_ui->editTitle, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged()));
	connect(m_ui->noramlEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	connect(m_ui->mindmapEditor, SIGNAL(itemContentChanged()), this, SLOT(onMindMapChanged()));
}

void NoteEditWindow::initCustomWidget()
{
	m_ui->bookmenu->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 22)));
}

void NoteEditWindow::initContent()
{
    m_ui->editTitle->setPlaceholderText(u8"����");
}

void NoteEditWindow::saveNote()
{
	Q_ASSERT(m_pNote);
	QTextDocument* p = m_ui->noramlEditor->document();
	QString title = m_ui->editTitle->text();
	QString html = p->toHtml();
	QString plainText = p->toPlainText();

	BSTR bstrTitle = SysAllocString(title.toStdWString().c_str());
	m_pNote->SetTitle(bstrTitle);
	BSTR bstrContent = SysAllocString(html.toStdWString().c_str());
	m_pNote->SetContent(bstrContent);
	BSTR bstrPlainText = SysAllocString(plainText.toStdWString().c_str());
	m_pNote->SetPlainText(bstrPlainText);

	com_sptr<INotebook> spNotebook = m_pNotebook;
	RPCService::GetInstance().SynchronizeNote(coreApp, spNotebook, m_pNote);
}

void NoteEditWindow::saveMindMap()
{
	QString title = m_ui->editTitle->text();

	BSTR bstrTitle = SysAllocString(title.toStdWString().c_str());
	m_pNote->SetTitle(bstrTitle);

	QString wtf = m_ui->mindmapEditor->mindmapXML();
	BSTR bstrMap = SysAllocString(wtf.toStdWString().c_str());
	m_pNote->SetContent(bstrMap);

	com_sptr<INotebook> spNotebook = m_pNotebook;
	RPCService::GetInstance().SynchronizeNote(coreApp, spNotebook, m_pNote);
}

void NoteEditWindow::switchtobook(int bookidx)
{
	//TODO
	VARIANT newindex;
	V_VT(&newindex) = VT_I4;
	V_I4(&newindex) = bookidx;
	com_sptr<INotebook> spNewbook;
	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(newindex, &spNewbook);
	if (FAILED(hr) || !spNewbook)
	{
		Q_ASSERT(FALSE);
	}

	hr = m_pNotebook->RemoveNote(m_pNote);
	if (FAILED(hr))
		Q_ASSERT(FALSE);
	hr = spNewbook->AddNote(m_pNote);
	if (FAILED(hr))
		Q_ASSERT(FALSE);
	m_pNotebook = spNewbook;
}

void NoteEditWindow::onTitleChanged()
{
	QTimer::singleShot(2000, this, SLOT(saveNote()));
}

void NoteEditWindow::onTextChanged()
{
	QTimer::singleShot(2000, this, SLOT(saveNote()));
}

void NoteEditWindow::onMindMapChanged()
{
	QTimer::singleShot(3000, this, SLOT(saveMindMap()));
}
