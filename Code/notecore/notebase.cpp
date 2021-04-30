#include "framework.h"
#include "notecore.h"
#include "notebase.h"

NoteBase::NoteBase()
	: m_type(NORMAL_NOTE)
	, m_ref(1)
{
}

NoteBase::~NoteBase()
{
}

wstring NoteBase::GetTitle() const 
{
	return m_strTitle;
}

HRESULT NoteBase::SetTitle(const wstring& title)
{
	m_strTitle = title;
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

HRESULT STDMETHODCALLTYPE NoteBase::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	//TODO
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