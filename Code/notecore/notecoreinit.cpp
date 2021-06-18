#include "notecore.h"
#include "notebase.h"

BSTR _default_content(NOTE_TYPE type)
{
	if (type == MINDMAP)
	{
		return L"<node text=\"思维导图笔记\"></node>";
	}
	else if (type == SCHEDULE)
	{
		return L"<node text=\"进度图\"></node>";
	}
	return L"";
}

BSTR _default_title(NOTE_TYPE type)
{
	if (type == MINDMAP)
	{
		return SysAllocString(L"思维导图笔记");
	}
	else if (type == SCHEDULE)
	{
		return SysAllocString(L"进度图");
	}
	return NULL;
}

HRESULT CreateNote(NOTE_TYPE type, INote** ppNote)
{
	if (ppNote == NULL)
	{
		return E_POINTER;
	}
	INote* pNote = new NoteBase;
	pNote->SetType(type);
	pNote->SetContent(_default_content(type));
	pNote->SetTitle(_default_title(type));
	(*ppNote) = pNote;
	(*ppNote)->AddRef();
	return S_OK;
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

HRESULT CreateNotebooks(INotebooks** ppNotebooks)
{
	if (ppNotebooks == NULL)
	{
		return E_POINTER;
	}
	INotebooks* pNotebooks = new NotebooksBase;
	(*ppNotebooks) = pNotebooks;
	(*ppNotebooks)->AddRef();
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

HRESULT CreateTrash(ITrash** ppTrash)
{
	if (!ppTrash)
		return E_POINTER;

	*ppTrash = new TrashBase;
	(*ppTrash)->AddRef();
	return S_OK;
}

HRESULT CreateSchedules(ISchedules** ppSchedules)
{
	if (!ppSchedules)
		return E_POINTER;

	*ppSchedules = new SchedulesBase;
	(*ppSchedules)->AddRef();
	return S_OK;
}
