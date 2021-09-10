#include "stdafx.h"
#include "uiapplication.h"
#include "guihelper.h"
#include "newnotewindow.h"
#include <QtCore/QStandardPaths>
#include <QUuid>
#include <QDebug>
#include <QDir>

UiApplication* AppHelper::uiApp()
{
	UiApplication* pApp = qobject_cast<UiApplication*>(QApplication::instance());
	return pApp;
}

INoteApplication* AppHelper::coreApp()
{
	UiApplication* pApp = qobject_cast<UiApplication*>(QApplication::instance());
	return pApp->coreApplication();
}

void AppHelper::GetNotebook(int idx, INotebook** ppNotebook)
{
	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(idx, ppNotebook);
}

void AppHelper::GetNote(INoteCollection* pNotebook, int idxNote, INote** ppNote)
{
	if (!pNotebook || !ppNote)
	{
		return;
	}
	HRESULT hr = pNotebook->Item(idxNote, ppNote);
}

void AppHelper::GetNote(INoteCollection* pNotebook, QString noteid, INote** ppNote)
{
	com_sptr<INote> spNote;
	HRESULT hr = pNotebook->Item(noteid.toStdWString(), &spNote);
	if (FAILED(hr))
	{
		return;
	}
	*ppNote = spNote;
	(*ppNote)->AddRef();
}

void AppHelper::GetNoteAndBookById(QString noteid, INotebook** ppNotebook, INote** ppNote)
{
	if (!ppNotebook || !ppNote)
		return;

	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	int nCount = 0;
	spNotebooks->GetCount(&nCount);

	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(i, &spNotebook);

		com_sptr<INote> spNote;
		HRESULT hr = spNotebook->Item(noteid.toStdWString(), &spNote);
		if (FAILED(hr))
		{
			continue;
		}
		*ppNotebook = spNotebook;
		(*ppNotebook)->AddRef();
		*ppNote = spNote;
		(*ppNote)->AddRef();
	}
}

QString AppHelper::GetNotebookName(INoteCollection* pNotebook)
{
	if (!pNotebook)
		return "";

	std::wstring name;
	pNotebook->GetName(name);
	QString bookName = QString::fromStdWString(name);
	return bookName;
}

QString AppHelper::GetNotebookId(INotebook* pNotebook)
{
	if (!pNotebook)
		return "";

	std::wstring bstrId;
	pNotebook->GetId(bstrId);
	QString bookId = QString::fromStdWString(bstrId);
	return bookId;
}

void AppHelper::GetNotebookById(const QString& bookid, INotebook** ppNotebook)
{
	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(bookid.toStdWString(), ppNotebook);
}

void AppHelper::GetNotebookByNote(INote* pNote, INotebook** ppNotebook)
{
	Q_ASSERT(pNote);

	std::wstring bookId;
	pNote->GetBookId(bookId);
	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(bookId, ppNotebook);
	if (FAILED(hr))
	{
		hr = spNotebooks->Item(0, ppNotebook);
	}
}

void AppHelper::GetNotebookByName(const QString& name, INotebook** ppNotebook)
{
    com_sptr<INotebooks> spNotebooks;
    AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	int nCount = 0;
	spNotebooks->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(i, &spNotebook);
		wstring bookName;
		spNotebook->GetName(bookName);
		if (name.toStdWString() == bookName)
		{
			*ppNotebook = spNotebook;
			(*ppNotebook)->AddRef();
			return;
		}
	}
}

QString AppHelper::GetNoteId(INote* pNote)
{
	if (!pNote)
		return "";

	std::wstring noteid;
	pNote->GetId(noteid);
	QString noteId = QString::fromStdWString(noteid);
	return noteId;
}

QString AppHelper::GetCreateTime(INote* pNote, const QString& dateTimeFormat)
{
	if (!pNote)
		return "";

	long long time;
	pNote->GetCreateTime(&time);
	QDateTime dt = QDateTime::fromMSecsSinceEpoch(time);
	Q_ASSERT(dt.isValid());
	return dt.toString(dateTimeFormat);
}

QString AppHelper::GetModifyTime(INote* pNote, const QString& dateTimeFormat)
{
	if (!pNote)
		return "";

	long long time;
	pNote->GetModifyTime(&time);
	QDateTime dt = QDateTime::fromMSecsSinceEpoch(time);
	Q_ASSERT(dt.isValid());
	return dt.toString(dateTimeFormat);
}

NOTE_TYPE AppHelper::GetNoteType(INote* pNote)
{
	if (!pNote)
		return UNKNOWN_TYPE;

	NOTE_TYPE type = UNKNOWN_TYPE;
	pNote->GetType(&type);
	return type;
}

QStringList AppHelper::GetNotes(INoteCollection* pNoteColl)
{
	QStringList list;
	if (!pNoteColl)
		return list;

	int nCount = 0;
	pNoteColl->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INote> spNote;
		pNoteColl->Item(i, &spNote);
		QString noteid = AppHelper::GetNoteId(spNote);
		list.append(noteid);
	}
	return list;
}

QString AppHelper::GetNoteContent(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	std::wstring bstrContent;
	pNote->GetContent(bstrContent);
	QString content = QString::fromStdWString(bstrContent);
	return content;
}

QString AppHelper::GetNoteAbbre(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	std::wstring bstrAbbre;
	pNote->GetAbbreText(bstrAbbre);
	QString content = QString::fromStdWString(bstrAbbre);
	return content;
}

QString AppHelper::GetNoteTitle(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	std::wstring bstrContent;
	pNote->GetTitle(bstrContent);
	QString content = QString::fromStdWString(bstrContent);
	return content;
}

int AppHelper::GetNoteCounts(INoteCollection* pNotebook)
{
	int nCount = 0;
	Q_ASSERT(pNotebook);
	pNotebook->GetCount(&nCount);
	return nCount;
}

QString AppHelper::GenerateGUID()
{
	QUuid wtf = QUuid::createUuid();
	return wtf.toString();
}

QSizeF AppHelper::viewItemTextLayout(QTextLayout& textLayout, int lineWidth, int maxHeight, int* lastVisibleLine)
{
	if (lastVisibleLine)
		*lastVisibleLine = -1;
	qreal height = 0;
	qreal widthUsed = 0;
	textLayout.beginLayout();
	int i = 0;
	while (true) {
		QTextLine line = textLayout.createLine();
		if (!line.isValid())
			break;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
		// we assume that the height of the next line is the same as the current one
		if (maxHeight > 0 && lastVisibleLine && height + line.height() > maxHeight) {
			const QTextLine nextLine = textLayout.createLine();
			*lastVisibleLine = nextLine.isValid() ? i : -1;
			break;
		}
		++i;
	}
	textLayout.endLayout();
	return QSizeF(widthUsed, height);
}

void AppHelper::openNoteInIsoWindow(const QString& noteid)
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, NORMAL_NOTE);

	com_sptr<INote> spNote;
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNoteAndBookById(noteid, &spNotebook, &spNote);
	QString bookid = AppHelper::GetNotebookId(spNotebook);
	pNewNoteWindow->open(bookid, noteid);
	pNewNoteWindow->setWindowTitle(AppHelper::GetNoteTitle(spNote));
	pNewNoteWindow->show();
}

QColor AppHelper::colorBlue()
{
	return QColor(21, 152, 255);
}