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
	//TODO: ����bstr
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

HRESULT NoteBase::GetBookId(BSTR* pBookId)
{
	if (!pBookId)
		return E_FAIL;
	return m_bookid.CopyTo(pBookId);
}

HRESULT NoteBase::SetBookId(BSTR bookId)
{
	m_bookid.Attach(bookId);
	return S_OK;
}

HRESULT NoteBase::GetTitle(OUT BSTR* pbstrName)
{
	return m_bstrTitle.CopyTo(pbstrName);
}

HRESULT NoteBase::SetTitle(IN BSTR title)
{
	//m_bstrTitle.Detach();
	int n = SysStringByteLen(title);
	m_bstrTitle.AssignBSTR(title);
	NotifyThisObj(NotifyOperator::Update);
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
	NotifyThisObj(NotifyOperator::Update);
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

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INote)
	{
		*ppvObject = static_cast<INote*>(this);
	}

	//*ppvObject = this;
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

void NoteBase::NotifyThisObj(NotifyOperator ope)
{
	for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
	{
		NotifyArg arg;
		arg.ope = ope;
		arg.pObj = this;
		(*it)->onCoreNotify(this, arg);
	}
}


/////////////////////////////////////////////////
NotebookBase::NotebookBase()
{
}

NotebookBase::~NotebookBase()
{
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

HRESULT NotebookBase::AddNote(INote* pNote)
{
	HRESULT hr = _base::AddNote(pNote);
	if (SUCCEEDED(hr))
	{
		pNote->SetBookId(m_id.Copy());
	}
	return hr;
}

HRESULT NotebookBase::RemoveNote(INote* pNote)
{
	HRESULT hr = _base::RemoveNote(pNote);
	if (SUCCEEDED(hr))
	{
		pNote->SetBookId(CComBSTR());
	}
	return hr;
}

HRESULT NotebookBase::GetNoteIdx(INote* pNote, int* pIndex)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (riid == IID_INotebook)
	{
		*ppvObject = static_cast<INotebook*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}


Notes::Notes()
{
}

Notes::~Notes()
{
}

HRESULT Notes::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}


///////////////////////////////////////////////////////////////////

NotebooksBase::NotebooksBase()
	: m_ref(0)
{

}

NotebooksBase::~NotebooksBase()
{

}

HRESULT NotebooksBase::addWatcher(ICoreNotify* pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NotebooksBase::GetCount(/* [out] */ int* pCount)
{
	*pCount = m_vecBooks.size();
	return S_OK;
}

HRESULT NotebooksBase::Item(VARIANT index, INotebook** ppNotebook)
{
	if (!ppNotebook)
	{
		return E_POINTER;
	}
	if (V_VT(&index) == VT_I4)
	{
		int nIndex = V_I4(&index);
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
	else if (V_VT(&index) == VT_BSTR)
	{
		BSTR bstrId = V_BSTR(&index);
		for (auto it = m_vecBooks.begin(); it != m_vecBooks.end(); it++)
		{
			BSTR _id;
			(*it)->GetId(&_id);
			if (0 == wcscmp(bstrId, _id))
			{
				*ppNotebook = *it;
				(*ppNotebook)->AddRef();
				return S_OK;
			}
		}
		return E_FAIL;
	}
	return E_NOTIMPL;
}

void NotebooksBase::NotifyThisObj(NotifyOperator ope, INotebook* pNotebook)
{
	for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
	{
		BSTR bstrId;
		pNotebook->GetId(&bstrId);
		std::wstring noteid(bstrId, SysStringLen(bstrId));
		NotifyArg arg;
		arg.ope = ope;
		arg.pObj = pNotebook;
		(*it)->onCoreNotify(this, arg);
	}
}

HRESULT NotebooksBase::AddNotebook(INotebook* pNotebook)
{
	if (!pNotebook)
		return E_NOTIMPL;

	pNotebook->AddRef();
	m_vecBooks.push_back(pNotebook);

	NotifyThisObj(NotifyOperator::Add, pNotebook);
	return S_OK;
}

HRESULT NotebooksBase::DeleteNotebook(INotebook* pNotebook)
{
	if (!pNotebook)
		return E_POINTER;

	for (auto it = m_vecBooks.begin(); it != m_vecBooks.end(); )
	{
		if (*it == pNotebook)
		{
			m_vecBooks.erase(it);
			pNotebook->Release();
			NotifyThisObj(Delete, pNotebook);
			return S_OK;
		}
		else
		{
			it++;
		}
	}
	return E_FAIL;


	return E_NOTIMPL;
}

HRESULT NotebooksBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INotebooks)
	{
		*ppvObject = static_cast<INotebooks*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

ULONG NotebooksBase::AddRef(void)
{
	m_ref++;
	return m_ref;
}

ULONG NotebooksBase::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}


//////////////////////////////////////////////////////
TrashBase::TrashBase()
{

}

TrashBase::~TrashBase()
{
}

HRESULT TrashBase::DeleteNote(INote* pNote)
{
	return E_NOTIMPL;
}

HRESULT TrashBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (riid == IID_ITrash)
	{
		*ppvObject = static_cast<ITrash*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT TrashBase::GetName(BSTR* pbstrName)
{
	if (!pbstrName)
		return E_POINTER;

	*pbstrName = SysAllocString(L"��ֽ¨");
	return S_OK;
}


//////////////////////////////////////////////////
SchedulesBase::SchedulesBase()
{

}

SchedulesBase::~SchedulesBase()
{

}

HRESULT SchedulesBase::GetName(BSTR* pbstrName)
{
	if (!pbstrName)
		return E_POINTER;

	*pbstrName = SysAllocString(L"���ȱ�");
	return S_OK;
}

HRESULT SchedulesBase::GetId(BSTR* pbstrId)
{
	return m_id.CopyTo(pbstrId);
}

HRESULT SchedulesBase::SetId(BSTR bstrId)
{
	m_id.Attach(bstrId);
	return S_OK;
}

HRESULT SchedulesBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (riid == IID_INotebook)
	{
		*ppvObject = static_cast<INotebook*>(this);
	}
	else if (riid == IID_ISchedules)
	{
		*ppvObject = static_cast<ISchedules*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

//////////////////////////////////////////////////
NoteApplication::NoteApplication()
	: m_ref(0)
{
}

NoteApplication::~NoteApplication()
{
}

HRESULT NoteApplication::GetNotebooks(INotebooks** ppNotebooks)
{
	if (!ppNotebooks)
		return E_FAIL;

	*ppNotebooks = m_spNotebooks;
	(*ppNotebooks)->AddRef();
	return S_OK;
}

HRESULT NoteApplication::SetNotebooks(INotebooks* pNotebooks)
{
	m_spNotebooks = pNotebooks;
	return S_OK;
}

HRESULT NoteApplication::GetTrash(ITrash** ppTrash)
{
	if (!ppTrash)
		return E_POINTER;

	*ppTrash = m_spTrash;
	(*ppTrash)->AddRef();
	return S_OK;
}

HRESULT NoteApplication::SetTrash(ITrash* pTrash)
{
	if (!pTrash)
		return E_POINTER;

	m_spTrash = pTrash;
	return S_OK;
}

HRESULT NoteApplication::GetSchedules(ISchedules** ppSchedules)
{
	if (!ppSchedules)
		return E_POINTER;

	*ppSchedules = m_spSchedules;
	(*ppSchedules)->AddRef();
	return S_OK;
}

HRESULT NoteApplication::SetSchedules(ISchedules* pSchedules)
{
	if (!pSchedules)
		return E_POINTER;
	m_spSchedules = pSchedules;
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

HRESULT NoteApplication::addWatcher(ICoreNotify* pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NoteApplication::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (riid == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (riid == IID_INoteApplication)
	{
		*ppvObject = static_cast<INoteApplication*>(this);
	}
	else
	{
		return E_FAIL;
	}
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