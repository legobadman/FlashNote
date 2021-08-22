#include "notecore2.h"
#include "notebase.h"

std::wstring _default_content(NOTE_TYPE type)
{
	if (type == MINDMAP)
	{
		return L"<node text=\"MindMap\"></node>";
	}
	else if (type == SCHEDULE)
	{
		return L"<node text=\"Schedule\"></node>";
	}
	return L"";
}

std::wstring _default_title(NOTE_TYPE type)
{
	if (type == MINDMAP)
	{
		return L"MindMap";
	}
	else if (type == SCHEDULE)
	{
		return L"Schedule";
	}
	return L"";
}

HRESULT CreateNote(NOTE_TYPE type, INote** ppNote)
{
	if (ppNote == NULL)
	{
		return E_FAIL;
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
		return E_FAIL;
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
		return E_FAIL;
	}
	INotebooks* pNotebooks = new NotebooksBase;
	(*ppNotebooks) = pNotebooks;
	(*ppNotebooks)->AddRef();
	return S_OK;
}

HRESULT CreateApplication(INoteApplication** pApp)
{
	if (!pApp)
		return E_FAIL;

	*pApp = new NoteApplication;
	(*pApp)->AddRef();
	return S_OK;
}

HRESULT CreateTrash(ITrash** ppTrash)
{
	if (!ppTrash)
		return E_FAIL;

	*ppTrash = new TrashBase;
	(*ppTrash)->AddRef();
	return S_OK;
}

HRESULT CreateSchedules(ISchedules** ppSchedules)
{
	if (!ppSchedules)
		return E_FAIL;

	*ppSchedules = new SchedulesBase;
	(*ppSchedules)->AddRef();
	return S_OK;
}
