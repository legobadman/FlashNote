#include "stdafx.h"

#include "note_editwindow.h"
#include "rpcservice.h"
#include "note_types.h"
#include "guihelper.h"
#include "moc_note_editwindow.cpp"
#include "MyStyle.h"
#include "common_types.h"
#include "selectnotebookpanel.h"
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

	updateBookMenu(m_pNotebook);

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
		m_ui->mindmapEditor->initContent(content, false);
		m_ui->stackedWidget->setCurrentIndex(1);
	}
	else if (m_type == SCHEDULE)
	{
		m_ui->mindmapEditor->initContent(content, true);
		m_ui->stackedWidget->setCurrentIndex(1);
	}
	else
	{
		Q_ASSERT(false);
	}

	update();
}

void NoteEditWindow::updateBookMenu(INotebook* pNotebook)
{
	QString bookName = AppHelper::GetNotebookName(pNotebook);
	m_ui->bookmenu->blockSignals(true);
	m_ui->bookmenu->font();
	QFont font(QString::fromUtf16((char16_t*)L"΢���ź�"), 9);
	m_ui->bookmenu->setFont(font);
	QFontMetrics fm(font);
	int width = fm.horizontalAdvance(bookName);
	m_ui->bookmenu->setText(bookName);
	m_ui->bookmenu->blockSignals(false);

	const int iconSize = 16;
	m_ui->bookmenu->setIcon(QIcon(":/icons/16x16/notebookmenu.png"));
	m_ui->bookmenu->setIconSize(MyStyle::dpiScaledSize(QSize(iconSize, iconSize)));

	int w = iconSize + 10 + 4 + width + 20;
	m_ui->bookmenu->setFixedSize(MyStyle::dpiScaledSize(QSize(w, 22)));
	m_ui->bookmenu->setCreateContentCallback([this] {
		SelectNotebookPanel* panel = new SelectNotebookPanel;
		connect(panel, SIGNAL(notebookMoved(INotebook*)), m_ui->bookmenu, SIGNAL(popout()));
		connect(panel, SIGNAL(notebookMoved(INotebook*)), this, SLOT(onNotebookMoved(INotebook*)));
		return panel;
	});
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
	m_ui->bookmenu->setIcon(QIcon(":/icons/16x16/notebookmenu.png"));
	m_ui->bookmenu->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->bookmenu->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 22)));
	
	m_ui->infoBtn->setIcon(QIcon(":/icons/16x16/info.png"));
	m_ui->infoBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->infoBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->alarm->setIcon(QIcon(":/icons/16x16/alarm.png"));
	m_ui->alarm->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->alarm->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->moreBtn->setIcon(QIcon(":/icons/16x16/more.png"));
	m_ui->moreBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->moreBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->socialmediashare->setIcon(QIcon(":/icons/16x16/socialmediashare.png"));
	m_ui->socialmediashare->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->socialmediashare->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));
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
	bool ret = RPCService::GetInstance().SynchronizeNote(coreApp, spNotebook, m_pNote);
	if (ret)
	{
		QString noteid = AppHelper::GetNoteId(m_pNote);
		emit noteCommited(noteid);
	}
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

void NoteEditWindow::onNotebookMoved(INotebook* pNewbook)
{
	if (m_pNotebook == pNewbook)
		return;

	updateBookMenu(pNewbook);
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
	if (m_type == NORMAL_NOTE)
		onTextChanged();
	else if (m_type == MINDMAP || m_type == SCHEDULE)
		onMindMapChanged();
}

void NoteEditWindow::onTextChanged()
{
	QTimer::singleShot(2000, this, SLOT(saveNote()));
}

void NoteEditWindow::onMindMapChanged()
{
	QTimer::singleShot(3000, this, SLOT(saveMindMap()));
}
