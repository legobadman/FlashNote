#include "stdafx.h"

#include "note_editwindow.h"
#include "rpcservice.h"
#include "dbservice.h"
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
	, m_bTrash(false)
	, m_bEditting(false)
{
    init();
    initContent();
	m_spNotifier.reset(new NoteEditWindowNotifier(this));
	m_timer.setSingleShot(true);
	m_timer.setInterval(3000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(saveNote()));
}

NoteEditWindow::~NoteEditWindow()
{
}

INote* NoteEditWindow::GetNote()
{
    return m_pNote;
}

void NoteEditWindow::updateNoteInfo(INotebook* pNotebook, INote* pNote, bool bTrash)
{
	//要先解除以下内核对象对自己的关联
	if (m_pNote)
		m_pNote->removeWatcher(m_spNotifier);
	if (m_pNotebook)
		m_pNotebook->removeWatcher(m_spNotifier);

	m_pNotebook = pNotebook;
	m_pNote = pNote;

	m_pNotebook->addWatcher(m_spNotifier);
	m_pNote->addWatcher(m_spNotifier);

	m_pNote->GetType(&m_type);
	m_bTrash = bTrash;

	updateBookMenu(m_pNotebook);
	updateEditContent();
	update();
}

void NoteEditWindow::updateEditContent()
{
    QString title = AppHelper::GetNoteTitle(m_pNote);
    QString content = AppHelper::GetNoteContent(m_pNote);
	bool edittable = !m_bTrash;

	m_ui->editTitle->setEnabled(true);
    m_ui->editTitle->blockSignals(true);
    m_ui->editTitle->setText(title);
    m_ui->editTitle->blockSignals(false);
    m_ui->editTitle->setReadOnly(!edittable);

    if (m_type == NORMAL_NOTE)
    {
		m_ui->noramlEditor->setEnabled(true);
        m_ui->noramlEditor->initContent(content, !edittable);
        m_ui->stackedWidget->setCurrentIndex(0);
    }
    else if (m_type == MINDMAP)
    {
		m_ui->mindmapEditor->setEnabled(true);
        m_ui->mindmapEditor->initContent(content, false);
        m_ui->stackedWidget->setCurrentIndex(1);
    }
    else if (m_type == SCHEDULE)
    {
		m_ui->mindmapEditor->setEnabled(true);
        m_ui->mindmapEditor->initContent(content, true);
        m_ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        Q_ASSERT(false);
    }
}

void NoteEditWindow::updateBookMenu(INotebook* pNotebook)
{
	QString bookName = AppHelper::GetNotebookName(pNotebook);
	m_ui->bookmenu->blockSignals(true);
	m_ui->bookmenu->font();
	QFont font("Microsoft YaHei", 9);
	m_ui->bookmenu->setFont(font);

	m_ui->bookmenu->setBookName(bookName);
	m_ui->bookmenu->blockSignals(false);

	const int iconSize = 16;
	m_ui->bookmenu->setIcon(QIcon(":/icons/notebookmenu.png"));
	m_ui->bookmenu->setIconSize(MyStyle::dpiScaledSize(QSize(iconSize, iconSize)));

	m_ui->bookmenu->setCreateContentCallback([this] {
		SelectNotebookPanel* panel = new SelectNotebookPanel;
		connect(panel, SIGNAL(notebookMoved(INotebook*)), m_ui->bookmenu, SIGNAL(popout()));
		connect(panel, SIGNAL(notebookMoved(INotebook*)), this, SLOT(onNotebookMoved(INotebook*)));
		return panel;
	});
}

HRESULT NoteEditWindow::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (m_pNote == pCoreObj)
	{
		if (arg.ope == NotifyOperator::Update && !m_bEditting)
		{
			updateEditContent();
		}
	}
	else if (m_pNotebook == pCoreObj)
	{
		if (arg.ope == NotifyOperator::Delete)
		{
			if (arg.pObj == m_pNote)
			{
				if (m_type == NORMAL_NOTE)
				{
                    m_ui->editTitle->setEnabled(false);
                    m_ui->noramlEditor->setEnabled(false);
				}
			}
		}
	}
	return S_OK;
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
	connect(m_ui->noramlEditor, SIGNAL(textChanged(bool)), this, SLOT(onTextChanged(bool)));
	connect(m_ui->mindmapEditor, SIGNAL(itemContentChanged(bool)), this, SLOT(onMindMapChanged(bool)));
	connect(m_ui->deleteBtn, SIGNAL(clicked()), this, SLOT(trashNote()));
}

void NoteEditWindow::initCustomWidget()
{
	m_ui->editTitle->setFixedHeight(MyStyle::dpiScaled(27));

	m_ui->bookmenu->setIcon(QIcon(":/icons/notebookmenu.png"));
	m_ui->bookmenu->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	
	m_ui->infoBtn->setIcon(QIcon(":/icons/info.png"));
	m_ui->infoBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->infoBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->alarm->setIcon(QIcon(":/icons/alarm.png"));
	m_ui->alarm->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->alarm->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->deleteBtn->setIcon(QIcon(":/icons/trashnote.png"));
	m_ui->deleteBtn->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->deleteBtn->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));

	m_ui->socialmediashare->setIcon(QIcon(":/icons/socialmediashare2.png"));
	m_ui->socialmediashare->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->socialmediashare->setFixedSize(MyStyle::dpiScaledSize(QSize(28, 28)));
}

void NoteEditWindow::initContent()
{
    m_ui->editTitle->setPlaceholderText(u8"标题");
}

void NoteEditWindow::trashNote()
{
	if (m_pNote)
	{
		NOTE_TYPE type = UNKNOWN_TYPE;
		m_pNote->GetType(&type);

		DbService& inst = DbService::GetInstance();
		com_sptr<INoteApplication> spApp = AppHelper::coreApp();

		if (m_bTrash)
		{
            com_sptr<ITrash> spTrash;
			spApp->GetTrash(&spTrash);
			inst.DeleteNote(spTrash, m_pNote);
		}
		else
		{
			if (type == SCHEDULE)
			{
				inst.RemoveSchedule(spApp, m_pNote);
			}
			else
			{
                com_sptr<INotebook> spNotebook;
                AppHelper::GetNotebookByNote(m_pNote, &spNotebook);
				inst.RemoveNote(spApp, spNotebook, m_pNote);
			}
		}
	}
}

void NoteEditWindow::saveNote()
{
	RAII_CheckEditting batch(&m_bEditting);
	Q_ASSERT(m_pNote);
	QTextDocument* p = m_ui->noramlEditor->document();
	QString title = m_ui->editTitle->text();
	QString html = p->toHtml();
	QString plainText = p->toPlainText();

	m_pNote->SetTitle(title.toStdWString());
	m_pNote->SetContent(html.toStdWString());
	m_pNote->SetPlainText(plainText.toStdWString());

	com_sptr<INotebook> spNotebook = m_pNotebook;
#ifdef USE_RPC
	bool ret = RPCService::GetInstance().SynchronizeNote(AppHelper::coreApp(), spNotebook, m_pNote);
#else
	bool ret = DbService::GetInstance().SynchronizeNote(AppHelper::coreApp(), spNotebook, m_pNote);
#endif
	if (ret)
	{
		QString noteid = AppHelper::GetNoteId(m_pNote);
		emit noteCommited(noteid);
	}
}

void NoteEditWindow::saveMindMap()
{
	RAII_CheckEditting batch(&m_bEditting);

	QString title = m_ui->editTitle->text();

	m_pNote->SetTitle(title.toStdWString());

	QString wtf = m_ui->mindmapEditor->mindmapXML();
	m_pNote->SetContent(wtf.toStdWString());

	com_sptr<INotebook> spNotebook = m_pNotebook;
#ifdef USE_RPC
	RPCService::GetInstance().SynchronizeNote(AppHelper::coreApp(), spNotebook, m_pNote);
#else
	DbService::GetInstance().SynchronizeNote(AppHelper::coreApp(), spNotebook, m_pNote);
#endif
}

void NoteEditWindow::saveSchedule()
{
	RAII_CheckEditting batch(&m_bEditting);

	QString title = m_ui->editTitle->text();
	m_pNote->SetTitle(title.toStdWString());

	QString wtf = m_ui->mindmapEditor->mindmapXML();
	m_pNote->SetContent(wtf.toStdWString());
	com_sptr<INotebook> spNotebook = m_pNotebook;

	DbService::GetInstance().SynchronizeSchedule(AppHelper::coreApp(), m_pNote);
}

void NoteEditWindow::onNotebookMoved(INotebook* pNewbook)
{
	if (m_pNotebook == pNewbook)
		return;

	if (AppHelper::GetNoteId(m_pNote).isEmpty())
	{
		m_pNotebook = pNewbook;
		updateBookMenu(m_pNotebook);
	}
	else
	{
        bool ret = DbService::GetInstance().MoveNotebook(m_pNotebook, pNewbook, m_pNote);
        if (ret)
        {
            updateBookMenu(pNewbook);
        }
	}
}

void NoteEditWindow::switchtobook(int bookidx)
{
	//TODO
	com_sptr<INotebook> spNewbook;
	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(bookidx, &spNewbook);
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
		onTextChanged(true);
	else if (m_type == MINDMAP || m_type == SCHEDULE)
		onMindMapChanged(true);
}

void NoteEditWindow::onTextChanged(bool delay)
{
	if (delay)
	{
		m_timer.stop();
		m_timer.start();
	}
	else
	{
		saveNote();
	}
}

void NoteEditWindow::onMindMapChanged(bool bEditChange)
{
	if (AppHelper::GetNoteType(m_pNote) == MINDMAP)
	{
		if (bEditChange)
		{
			QTimer::singleShot(2000, this, SLOT(saveMindMap()));
		}
		else
		{
			saveMindMap();
		}
	}
	else if (AppHelper::GetNoteType(m_pNote) == SCHEDULE)
	{
		if (bEditChange)
		{
			QTimer::singleShot(2000, this, SLOT(saveSchedule()));
		}
		else
		{
			saveSchedule();
		}
	}
}
