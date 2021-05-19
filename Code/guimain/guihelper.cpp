#include "stdafx.h"
#include "guihelper.h"


void AppHelper::GetNotebook(int idx, INotebook** ppNotebook)
{
	VARIANT varBook;
	V_VT(&varBook) = VT_I4;
	V_I4(&varBook) = idx;

	HRESULT hr = coreApp->GetNotebook(varBook, ppNotebook);
	if (FAILED(hr) || *ppNotebook == NULL)
	{
		Q_ASSERT(FALSE);
	}
}

void AppHelper::GetNote(INotebook* pNotebook, int idxNote, INote** ppNote)
{
	VARIANT varNote;
	V_VT(&varNote) = VT_I4;
	V_I4(&varNote) = idxNote;

	if (!pNotebook || !ppNote)
	{
		Q_ASSERT(FALSE);
	}

	HRESULT hr = pNotebook->Item(varNote, ppNote);
}

QString AppHelper::GetNotebookName(INotebook* pNotebook)
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