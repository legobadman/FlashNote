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

HRESULT NoteBase::GetId(OUT BSTR* pbstrId) const
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NoteBase::SetId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT NoteBase::GetTitle(OUT BSTR* pbstrName) const
{
	return m_bstrTitle.CopyTo(pbstrName);
}

HRESULT NoteBase::SetTitle(IN BSTR title)
{
	m_bstrTitle.Attach(title);
	return S_OK;
}

NOTE_TYPE NoteBase::GetType() const
{
	return m_type;
}

HRESULT NoteBase::SetType(NOTE_TYPE type)
{
	m_type = type;
	return S_OK;
}

HRESULT NoteBase::GetContent(OUT BSTR* pbstrContent) const
{
	return m_bstrContent.CopyTo(pbstrContent);
}

HRESULT NoteBase::SetContent(IN BSTR content)
{
	m_bstrContent.AssignBSTR(content);
	return S_OK;
}

HRESULT NoteBase::GetAbbreText(OUT BSTR* pbstrAbbre) const
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

std::tm NoteBase::GetCreateTime() const
{
	return m_createtime;
}

HRESULT NoteBase::SetCreateTime(std::tm create_time)
{
	m_createtime = create_time;
	return S_OK;
}

std::tm NoteBase::GetModifyTime() const
{
	return m_modifytime;
}

HRESULT NoteBase::SetModifyTime(std::tm modify_time)
{
	m_modifytime = modify_time;
	return S_OK;
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


/////////////////////////////////////////////////
NotebookBase::NotebookBase()
	: m_ref(0)
{
}

NotebookBase::~NotebookBase()
{
	for (int i = 0; i < m_vecNotes.size(); i++)
	{
		m_vecNotes[i]->Release();
	}
	m_vecNotes.clear();
}

HRESULT NotebookBase::GetId(OUT BSTR* pbstrId) const
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NotebookBase::SetId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT NotebookBase::GetName(OUT BSTR* pbstrName) const
{
	return m_strName.CopyTo(pbstrName);
}

HRESULT NotebookBase::SetName(IN BSTR bstrName)
{
	m_strName.Attach(bstrName);
	return S_OK;
}

std::tm NotebookBase::GetCreateTime() const
{
	return m_createtime;
}

HRESULT NotebookBase::SetCreateTime(std::tm create_time)
{
	m_createtime = create_time;
	return S_OK;
}

std::tm NotebookBase::GetModifyTime() const
{
	return m_modifytime;
}

HRESULT NotebookBase::SetModifyTime(std::tm modify_time)
{
	m_modifytime = modify_time;
	return S_OK;
}

int NotebookBase::GetCount() const
{
	return m_vecNotes.size();
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
		if (nIndex >= 0 && nIndex < m_vecNotes.size())
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
	return E_NOTIMPL;
}

HRESULT NotebookBase::AddNote(INote* pNote)
{
	if (!pNote)
		return E_NOTIMPL;

	pNote->AddRef();
	m_vecNotes.push_back(pNote);
	return S_OK;
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
			return S_OK;
		}
	}
	return E_FAIL;
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
		if (nIndex >= 0 && nIndex < m_vecBooks.size())
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

int NoteApplication::GetCount() const
{
	return m_vecBooks.size();
}

HRESULT NoteApplication::GetUserId(OUT BSTR* pbstrId) const
{
	return m_id.CopyTo(pbstrId);
}

HRESULT NoteApplication::SetUserId(IN BSTR bstrId)
{
	m_id.Attach(bstrId);
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