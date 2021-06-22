#include "stdafx.h"
#include "guihelper.h"


void AppHelper::GetNotebook(int idx, INotebook** ppNotebook)
{
	VARIANT varBook;
	V_VT(&varBook) = VT_I4;
	V_I4(&varBook) = idx;

	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);

	HRESULT hr = spNotebooks->Item(varBook, ppNotebook);
	if (FAILED(hr) || *ppNotebook == NULL)
	{
		Q_ASSERT(FALSE);
	}
}

void AppHelper::GetNote(INoteCollection* pNotebook, int idxNote, INote** ppNote)
{
	VARIANT varNote;
	V_VT(&varNote) = VT_I4;
	V_I4(&varNote) = idxNote;

	if (!pNotebook || !ppNote)
	{
		return;
	}
	HRESULT hr = pNotebook->Item(varNote, ppNote);
}

void AppHelper::GetNote(INoteCollection* pNotebook, QString noteid, INote** ppNote)
{
	VARIANT varNote;
	V_VT(&varNote) = VT_BSTR;
	V_BSTR(&varNote) = SysAllocString(noteid.toStdWString().data());

	com_sptr<INote> spNote;
	HRESULT hr = pNotebook->Item(varNote, &spNote);
	if (FAILED(hr))
	{
		VariantClear(&varNote);
		return;
	}

	*ppNote = spNote;
	(*ppNote)->AddRef();

	VariantClear(&varNote);
}

void AppHelper::GetNoteAndBookById(QString noteid, INotebook** ppNotebook, INote** ppNote)
{
	if (!ppNotebook || !ppNote)
		return;

	VARIANT varNote;
	V_VT(&varNote) = VT_BSTR;
	V_BSTR(&varNote) = SysAllocString(noteid.toStdWString().data());

	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);
	int nCount = 0;
	spNotebooks->GetCount(&nCount);

	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;

		VARIANT varIndex;
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = i;

		spNotebooks->Item(varIndex, &spNotebook);

		com_sptr<INote> spNote;
		HRESULT hr = spNotebook->Item(varNote, &spNote);
		if (FAILED(hr))
		{
			continue;
		}

		*ppNotebook = spNotebook;
		(*ppNotebook)->AddRef();
		*ppNote = spNote;
		(*ppNote)->AddRef();
	}

	VariantClear(&varNote);
}

QString AppHelper::GetNotebookName(INoteCollection* pNotebook)
{
	if (!pNotebook)
		return "";

	BSTR bstrName;
	pNotebook->GetName(&bstrName);
	QString bookName = QString::fromUtf16(reinterpret_cast<ushort*>(bstrName));
	return bookName;
}

QString AppHelper::GetNotebookId(INotebook* pNotebook)
{
	if (!pNotebook)
		return "";

	BSTR bstrId;
	pNotebook->GetId(&bstrId);
	QString bookId = QString::fromUtf16(reinterpret_cast<ushort*>(bstrId));
	return bookId;
}

void AppHelper::GetNotebookById(const QString& bookid, INotebook** ppNotebook)
{
	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);

	VARIANT varIndex;
	V_VT(&varIndex) = VT_BSTR;
	V_BSTR(&varIndex) = SysAllocString(bookid.toStdWString().data());

	HRESULT hr = spNotebooks->Item(varIndex, ppNotebook);

	VariantClear(&varIndex);
}

void AppHelper::GetNotebookByNote(INote* pNote, INotebook** ppNotebook)
{
	Q_ASSERT(pNote);

	BSTR bstrBookid;
	pNote->GetBookId(&bstrBookid);

	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);

	VARIANT varIndex;
	V_VT(&varIndex) = VT_BSTR;
	V_BSTR(&varIndex) = bstrBookid;

	HRESULT hr = spNotebooks->Item(varIndex, ppNotebook);
	if (FAILED(hr))
	{
		V_VT(&varIndex) = VT_I4;
		V_I4(&varIndex) = 0;
		//µÚÒ»¸önotebook
		hr = spNotebooks->Item(varIndex, ppNotebook);
	}
}

QString AppHelper::GetNoteId(INote* pNote)
{
	if (!pNote)
		return "";

	BSTR bstrId;
	pNote->GetId(&bstrId);
	QString noteId = QString::fromUtf16(reinterpret_cast<ushort*>(bstrId));
	return noteId;
}

QString AppHelper::GetNoteContent(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	BSTR bstrContent;
	pNote->GetContent(&bstrContent);
	QString content = QString::fromUtf16(reinterpret_cast<ushort*>(bstrContent));
	return content;
}

QString AppHelper::GetNoteAbbre(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	BSTR bstrAbbre;
	pNote->GetAbbreText(&bstrAbbre);
	QString content = QString::fromUtf16(reinterpret_cast<ushort*>(bstrAbbre));
	return content;
}

QString AppHelper::GetNoteTitle(INote* pNote)
{
	if (!pNote)
	{
		return "";
	}
	BSTR bstrContent;
	pNote->GetTitle(&bstrContent);
	QString content = QString::fromUtf16(reinterpret_cast<ushort*>(bstrContent));
	return content;
}

int AppHelper::GetNoteCounts(INoteCollection* pNotebook)
{
	int nCount = 0;
	Q_ASSERT(pNotebook);
	pNotebook->GetCount(&nCount);
	return nCount;
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