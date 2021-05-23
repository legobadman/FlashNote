#include "framework.h"
#include "notecore.h"
#include "notebase.h"

NoteBase::NoteBase()
	: m_type(NORMAL_NOTE)
	, m_ref(0)
{
}

NoteBase::~NoteBase()
{
	//TODO: Îö¹¹bstr
}

HRESULT NoteBase::GetId(OUT BSTR* pbstrId)
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NoteBase::SetId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT NoteBase::GetTitle(OUT BSTR* pbstrName)
{
	return m_bstrTitle.CopyTo(pbstrName);
}

HRESULT NoteBase::SetTitle(IN BSTR title)
{
	m_bstrTitle.Attach(title);
	return S_OK;
}

HRESULT NoteBase::GetType(NOTE_TYPE* pType)
{
	*pType = m_type;
	return S_OK;
}

HRESULT NoteBase::SetType(NOTE_TYPE type)
{
	m_type = type;
	return S_OK;
}

HRESULT NoteBase::GetContent(OUT BSTR* pbstrContent)
{
	return m_bstrContent.CopyTo(pbstrContent);
}

HRESULT NoteBase::SetContent(IN BSTR content)
{
	m_bstrContent.AssignBSTR(content);
	return S_OK;
}

HRESULT NoteBase::GetAbbreText(OUT BSTR* pbstrAbbre)
{
	if (!pbstrAbbre)
		return E_POINTER;

	std::wstring plainText(m_bstrPlainText, SysStringLen(m_bstrPlainText));
	static const int nContentLimit = 74;

	std::wstring abbreText = plainText.substr(0, nContentLimit);
	*pbstrAbbre = SysAllocStringLen(abbreText.data(), abbreText.size());
	return S_OK;
}

HRESULT NoteBase::SetPlainText(IN BSTR content)
{
	m_bstrPlainText = content;
	return S_OK;
}

HRESULT NoteBase::GetCreateTime(long* pTime)
{
	return E_NOTIMPL;
}

HRESULT NoteBase::SetCreateTime(long create_time)
{
	return E_NOTIMPL;
}

HRESULT NoteBase::GetModifyTime(long* pTime)
{
	return E_NOTIMPL;
}

HRESULT NoteBase::SetModifyTime(long modify_time)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE NoteBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	*ppvObject = this;
	return S_OK;
}

ULONG STDMETHODCALLTYPE NoteBase::AddRef(void)
{
	m_ref++;
	return m_ref;
}

ULONG STDMETHODCALLTYPE NoteBase::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}

HRESULT NoteBase::addWatcher(ICoreNotify* pNotify)
{
	m_notifies.push_back(pNotify);
	return S_OK;
}


/////////////////////////////////////////////////
NotebookBase::NotebookBase()
	: m_ref(0)
{
}

NotebookBase::~NotebookBase()
{
	for (int i = 0; i < (int)m_vecNotes.size(); i++)
	{
		m_vecNotes[i]->Release();
	}
	m_vecNotes.clear();
}

HRESULT NotebookBase::GetId(OUT BSTR* pbstrId)
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NotebookBase::SetId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT NotebookBase::GetName(OUT BSTR* pbstrName)
{
	return m_strName.CopyTo(pbstrName);
}

HRESULT NotebookBase::SetName(IN BSTR bstrName)
{
	m_strName.Attach(bstrName);
	return S_OK;
}

HRESULT NotebookBase::GetCreateTime(long* pTime)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::SetCreateTime(long create_time)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::GetModifyTime(long* pTime)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::SetModifyTime(long modify_time)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::GetCount(int* pCount)
{
	*pCount = m_vecNotes.size();
	return S_OK;
}

HRESULT NotebookBase::Item(VARIANT Index, INote** ppNote)
{
	if (!ppNote)
	{
		return E_POINTER;
	}
	if (V_VT(&Index) == VT_I4)
	{
		int nIndex = V_I4(&Index);
		if (nIndex >= 0 && nIndex < (int)m_vecNotes.size())
		{
			*ppNote = m_vecNotes[nIndex];
			(*ppNote)->AddRef();
			return S_OK;
		}
		else
		{
			return E_INVALIDARG;
		}
	}
	else if (V_VT(&Index) == VT_BSTR)
	{
		BSTR bstrId = V_BSTR(&Index);
		for (auto it = m_vecNotes.begin(); it != m_vecNotes.end(); it++)
		{
			BSTR _id;
			(*it)->GetId(&_id);
			if (0 == wcscmp(bstrId, _id))
			{
				*ppNote = *it;
				(*ppNote)->AddRef();
				return S_OK;
			}
		}
		return E_FAIL;
	}
	return E_NOTIMPL;
}

HRESULT NotebookBase::AddNote(INote* pNote)
{
	if (!pNote)
		return E_NOTIMPL;

	pNote->AddRef();
	m_vecNotes.push_back(pNote);
	NotifyThisObj(Add, pNote);
	return S_OK;
}

void NotebookBase::NotifyThisObj(NotifyOperator ope, INote* pNote)
{
	for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
	{
		BSTR bstrId;
		pNote->GetId(&bstrId);
		std::wstring noteid(bstrId, SysStringLen(bstrId));
		NotifyArg arg;
		arg.ope = ope;
		arg.pObj = pNote;
		(*it)->onCoreNotify(this, arg);
	}
}

HRESULT NotebookBase::RemoveNote(INote* pNote)
{
	if (!pNote)
		return E_POINTER;

	for (auto it = m_vecNotes.begin(); it != m_vecNotes.end(); )
	{
		if (*it == pNote)
		{
			m_vecNotes.erase(it);
			pNote->Release();
			NotifyThisObj(Delete, pNote);
			return S_OK;
		}
		else
		{
			it++;
		}
	}
	return E_FAIL;
}

HRESULT NotebookBase::GetNoteIdx(INote* pNote, int* pIndex)
{
	for (int i = 0; i < (int)m_vecNotes.size(); i++)
	{
		if (m_vecNotes[i] == pNote)
		{
			*pIndex = i;
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT NotebookBase::addWatcher(ICoreNotify* pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NotebookBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	*ppvObject = this;
	return S_OK;
}

ULONG NotebookBase::AddRef(void)
{
	m_ref++;
	return m_ref;
}

ULONG NotebookBase::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}


//////////////////////////////////////////////////
NoteApplication::NoteApplication()
	: m_ref(0)
{
}

NoteApplication::~NoteApplication()
{
}

HRESULT NoteApplication::GetNotebook(VARIANT Index, INotebook** ppNotebook)
{
	if (!ppNotebook)
	{
		return E_POINTER;
	}
	if (V_VT(&Index) == VT_I4)
	{
		int nIndex = V_I4(&Index);
		if (nIndex >= 0 && nIndex < (int)m_vecBooks.size())
		{
			*ppNotebook = m_vecBooks[nIndex];
			(*ppNotebook)->AddRef();
			return S_OK;
		}
		else
		{
			return E_INVALIDARG;
		}
	}
	return E_NOTIMPL;
}

HRESULT NoteApplication::AddNotebook(INotebook* pNotebook)
{
	if (!pNotebook)
		return E_NOTIMPL;

	pNotebook->AddRef();
	m_vecBooks.push_back(pNotebook);
	return S_OK;
}

HRESULT NoteApplication::GetCount(int* pCount)
{
	*pCount = m_vecBooks.size();
	return S_OK;
}

HRESULT NoteApplication::GetUserId(OUT BSTR* pbstrId)
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NoteApplication::SetUserId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT NoteApplication::GetNotebookIdx(INotebook* pNote, int* pIdx)
{
	for (int i = 0; i < (int)m_vecBooks.size(); i++)
	{
		if (m_vecBooks[i] == pNote)
		{
			*pIdx = i;
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT NoteApplication::addWatcher(ICoreNotify* pNotify)
{
	m_notifies.push_back(pNotify);
	return S_OK;
}

HRESULT NoteApplication::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	*ppvObject = this;
	return S_OK;
}

ULONG NoteApplication::AddRef(void)
{
	m_ref++;
	return m_ref;
}

ULONG NoteApplication::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}