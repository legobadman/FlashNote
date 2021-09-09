#include "framework.h"
#include "notecore2.h"
#include "notebase.h"
#include "guidconst.h"


NoteBase::NoteBase()
	: m_type(NOTE_TYPE::NORMAL_NOTE)
	, m_ref(0)
	, modify_time(0)
	, create_time(0)
{
}

NoteBase::~NoteBase()
{
	//TODO:
}

HRESULT NoteBase::GetId(std::wstring& pbstrId)
{
	pbstrId = m_id;
	return S_OK;
}

HRESULT NoteBase::SetId(const std::wstring& bstrId)
{
	m_id = bstrId;
	return S_OK;
}

HRESULT NoteBase::GetBookId(std::wstring& pBookId)
{
	pBookId = m_bookid;
	return S_OK;
}

HRESULT NoteBase::SetBookId(const std::wstring& bookId)
{
	m_bookid = bookId;
	return S_OK;
}

HRESULT NoteBase::GetTitle(std::wstring& pbstrName)
{
	pbstrName = m_bstrTitle;
	return S_OK;
}

HRESULT NoteBase::SetTitle(const std::wstring& title)
{
	m_bstrTitle = title;
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

HRESULT NoteBase::GetContent(std::wstring& pbstrContent)
{
	pbstrContent = m_bstrContent;
	return S_OK;
}

HRESULT NoteBase::SetContent(const std::wstring& content)
{
	m_bstrContent = content;
	NotifyThisObj(NotifyOperator::Update);
	return S_OK;
}

HRESULT NoteBase::GetAbbreText(std::wstring& pbstrAbbre)
{
	static const int nContentLimit = 74;
	pbstrAbbre = m_bstrPlainText.substr(0, nContentLimit);
	return S_OK;
}

HRESULT NoteBase::SetPlainText(const std::wstring& content)
{
	m_bstrPlainText = content;
	return S_OK;
}

HRESULT NoteBase::GetCreateTime(long long* pTime)
{
	if (!pTime)
		return E_FAIL;

	*pTime = create_time;
	return S_OK;
}

HRESULT NoteBase::SetCreateTime(long long time)
{
	create_time = time;
	return S_OK;
}

HRESULT NoteBase::GetModifyTime(long long* pTime)
{
	if (!pTime)
		return E_FAIL;

	*pTime = modify_time;
	return S_OK;
}

HRESULT NoteBase::SetModifyTime(long long time)
{
	modify_time = time;
	return S_OK;
}

HRESULT NoteBase::QueryInterface(
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INote)
	{
		*ppvObject = static_cast<INote*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

long NoteBase::AddRef(void)
{
	m_ref++;
	return m_ref;
}

long NoteBase::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}

HRESULT NoteBase::addWatcher(weak_ptr<ICoreNotify> pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NoteBase::removeWatcher(weak_ptr<ICoreNotify> pNotify)
{
	m_notifies.erase(pNotify);
	return S_OK;
}

void NoteBase::NotifyThisObj(NotifyOperator ope)
{
	for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
	{
		NotifyArg arg;
		arg.ope = ope;
		arg.pObj = this;
		if (shared_ptr<ICoreNotify> np = (*it).lock())
		{
			np->onCoreNotify(this, arg);
		}
	}
}


/////////////////////////////////////////////////
NotebookBase::NotebookBase()
	: modify_time(0)
	, create_time(0)
{
}

NotebookBase::~NotebookBase()
{
}

HRESULT NotebookBase::GetId(std::wstring& pbstrId)
{
	pbstrId = m_id;
	return S_OK;
}

HRESULT NotebookBase::SetId(const std::wstring& bstrId)
{
	m_id = bstrId;;
	return S_OK;
}

HRESULT NotebookBase::GetName(std::wstring& pbstrName)
{
	pbstrName = m_strName;
	return S_OK;
}

HRESULT NotebookBase::SetName(const std::wstring& bstrName)
{
	m_strName = bstrName;
	return S_OK;
}

HRESULT NotebookBase::GetCreateTime(long long* pTime)
{
	if (!pTime)
		return E_FAIL;

	*pTime = create_time;
	return S_OK;
}

HRESULT NotebookBase::SetCreateTime(long long time)
{
	create_time = time;
	return S_OK;
}

HRESULT NotebookBase::GetModifyTime(long long* pTime)
{
	if (!pTime)
		return E_FAIL;

	*pTime = modify_time;
	return S_OK;
}

HRESULT NotebookBase::SetModifyTime(long long time)
{
	modify_time = time;
	return S_OK;
}

HRESULT NotebookBase::AddNote(INote* pNote)
{
	HRESULT hr = _base::AddNote(pNote);
	if (SUCCEEDED(hr))
	{
		pNote->SetBookId(m_id);
	}
	return hr;
}

HRESULT NotebookBase::RemoveNote(INote* pNote)
{
	HRESULT hr = _base::RemoveNote(pNote);
	if (SUCCEEDED(hr))
	{
		pNote->SetBookId(L"");
	}
	return hr;
}

HRESULT NotebookBase::GetNoteIdx(INote* pNote, int* pIndex)
{
	return E_NOTIMPL;
}

HRESULT NotebookBase::QueryInterface(
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (QUuid(riid) == IID_INotebook)
	{
		*ppvObject = static_cast<INotebook*>(this);
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

HRESULT NotebooksBase::addWatcher(weak_ptr<ICoreNotify> pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NotebooksBase::removeWatcher(weak_ptr<ICoreNotify> pNotify)
{
    m_notifies.erase(pNotify);
    return S_OK;
}

HRESULT NotebooksBase::GetCount(/* [out] */ int* pCount)
{
	*pCount = m_vecBooks.size();
	return S_OK;
}

HRESULT NotebooksBase::Item(const wstring& index, INotebook** ppNotebook)
{
	if (!ppNotebook)
	{
		return E_FAIL;
	}

	for (auto it = m_vecBooks.begin(); it != m_vecBooks.end(); it++)
	{
		std::wstring _id;
		(*it)->GetId(_id);
		if (index == _id)
		{
			*ppNotebook = *it;
			(*ppNotebook)->AddRef();
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT NotebooksBase::Item(int index, INotebook** ppNotebook)
{
	if (!ppNotebook)
	{
		return E_FAIL;
	}
	if (index >= 0 && index < (int)m_vecBooks.size())
	{
		*ppNotebook = m_vecBooks[index];
		(*ppNotebook)->AddRef();
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

void NotebooksBase::NotifyThisObj(NotifyOperator ope, INotebook* pNotebook)
{
	for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
	{
		std::wstring noteid;
		pNotebook->GetId(noteid);
		NotifyArg arg;
		arg.ope = ope;
		arg.pObj = pNotebook;
        if (shared_ptr<ICoreNotify> np = (*it).lock())
        {
            np->onCoreNotify(this, arg);
        }
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
		return E_FAIL;

	for (auto it = m_vecBooks.begin(); it != m_vecBooks.end(); )
	{
		if (*it == pNotebook)
		{
			m_vecBooks.erase(it);
			pNotebook->Release();
			NotifyThisObj(NotifyOperator::Delete, pNotebook);
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
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INotebooks)
	{
		*ppvObject = static_cast<INotebooks*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

long NotebooksBase::AddRef(void)
{
	m_ref++;
	return m_ref;
}

long NotebooksBase::Release(void)
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
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (QUuid(riid) == IID_ITrash)
	{
		*ppvObject = static_cast<ITrash*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT TrashBase::GetName(std::wstring& name)
{
	name = L"废纸篓";
	return S_OK;
}


//////////////////////////////////////////////////
SchedulesBase::SchedulesBase()
{

}

SchedulesBase::~SchedulesBase()
{

}

HRESULT SchedulesBase::GetName(std::wstring& name)
{
	name = L"进度表";
	return S_OK;
}

HRESULT SchedulesBase::GetId(std::wstring& pbstrId)
{
	pbstrId = m_id;
	return S_OK;
}

HRESULT SchedulesBase::SetId(const std::wstring& bstrId)
{
	m_id = bstrId;
	return S_OK;
}

HRESULT SchedulesBase::QueryInterface(
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INoteCollection)
	{
		*ppvObject = static_cast<INoteCollection*>(this);
	}
	else if (QUuid(riid) == IID_INotebook)
	{
		*ppvObject = static_cast<INotebook*>(this);
	}
	else if (QUuid(riid) == IID_ISchedules)
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
		return E_FAIL;

	*ppTrash = m_spTrash;
	(*ppTrash)->AddRef();
	return S_OK;
}

HRESULT NoteApplication::SetTrash(ITrash* pTrash)
{
	if (!pTrash)
		return E_FAIL;

	m_spTrash = pTrash;
	return S_OK;
}

HRESULT NoteApplication::GetSchedules(ISchedules** ppSchedules)
{
	if (!ppSchedules)
		return E_FAIL;

	*ppSchedules = m_spSchedules;
	(*ppSchedules)->AddRef();
	return S_OK;
}

HRESULT NoteApplication::SetSchedules(ISchedules* pSchedules)
{
	if (!pSchedules)
		return E_FAIL;
	m_spSchedules = pSchedules;
	return S_OK;
}

HRESULT NoteApplication::GetUserId(std::wstring& pbstrId)
{
	pbstrId = m_id;
	return S_OK;
}

HRESULT NoteApplication::SetUserId(const std::wstring& bstrId)
{
	m_id = bstrId;
	return S_OK;
}

HRESULT NoteApplication::addWatcher(weak_ptr<ICoreNotify> pNotify)
{
	m_notifies.insert(pNotify);
	return S_OK;
}

HRESULT NoteApplication::removeWatcher(weak_ptr<ICoreNotify> pNotify)
{
    m_notifies.erase(pNotify);
    return S_OK;
}

HRESULT NoteApplication::QueryInterface(
	QUuid riid,
	void** ppvObject)
{
	if (!ppvObject)
		return E_FAIL;

	if (QUuid(riid) == IID_INoteCoreObj)
	{
		*ppvObject = static_cast<INoteCoreObj*>(this);
	}
	else if (QUuid(riid) == IID_INoteApplication)
	{
		*ppvObject = static_cast<INoteApplication*>(this);
	}
	else
	{
		return E_FAIL;
	}
	return S_OK;
}

long NoteApplication::AddRef(void)
{
	m_ref++;
	return m_ref;
}

long NoteApplication::Release(void)
{
	m_ref--;
	if (m_ref == 0)
	{
		delete this;
	}
	return m_ref;
}
