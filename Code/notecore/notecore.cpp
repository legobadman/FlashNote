#include "notecore.h"
#include "notebase.h"


HRESULT CreateNote(NOTE_TYPE type, INote** ppNote)
{
	if (ppNote == NULL)
	{
		return E_POINTER;
	}
	if (NORMAL_NOTE == type)
	{
		INote* pNote = new NoteBase;
		(*ppNote) = pNote;
		(*ppNote)->AddRef();
		return S_OK;
	}
	else
	{
		return E_NOTIMPL;
	}
}

HRESULT CreateNotebook(INotebook** ppNotebook)
{
	if (ppNotebook == NULL)
	{
		return E_POINTER;
	}
	INotebook* pNotebook = new NotebookBase;
	(*ppNotebook) = pNotebook;
	(*ppNotebook)->AddRef();
	return S_OK;
}

HRESULT CreateApplication(INoteApplication** pApp)
{
	if (!pApp)
		return E_POINTER;

	*pApp = new NoteApplication;
	(*pApp)->AddRef();
	return S_OK;
}