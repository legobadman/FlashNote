#ifndef __NOTE_BASE_H__
#define __NOTE_BASE_H__

#include "framework.h"

class NoteBase : public INote
{
public:
	NoteBase();
	~NoteBase();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);

	HRESULT STDMETHODCALLTYPE GetId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetTitle(OUT BSTR* pbstrName);
	HRESULT STDMETHODCALLTYPE SetTitle(IN BSTR title);
	HRESULT STDMETHODCALLTYPE GetType(NOTE_TYPE* pType);
	HRESULT STDMETHODCALLTYPE SetType(NOTE_TYPE type);
	HRESULT STDMETHODCALLTYPE GetContent(OUT BSTR* pbstrContent);
	HRESULT STDMETHODCALLTYPE SetContent(IN BSTR content);
	HRESULT STDMETHODCALLTYPE GetAbbreText(OUT BSTR* pbstrAbbre);
	HRESULT STDMETHODCALLTYPE SetPlainText(IN BSTR content);
	HRESULT STDMETHODCALLTYPE GetCreateTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetCreateTime(long time);
	HRESULT STDMETHODCALLTYPE GetModifyTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetModifyTime(long time);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	void NotifyThisObj(NotifyOperator ope);

private:
	CComBSTR m_id;
	CComBSTR m_bstrTitle;
	CComBSTR m_bstrContent;
	CComBSTR m_bstrPlainText;
	std::tm m_createtime;
	std::tm m_modifytime;

	std::vector<ICoreNotify*> m_notifies;

	NOTE_TYPE m_type;
	int m_ref;
};

class NotebookBase : public INotebook
{
public:
	NotebookBase();
	~NotebookBase();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);

	HRESULT STDMETHODCALLTYPE GetId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetName(OUT BSTR* pbstrName);
	HRESULT STDMETHODCALLTYPE SetName(IN BSTR bstrName);
	HRESULT STDMETHODCALLTYPE GetCreateTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetCreateTime(long create_time);
	HRESULT STDMETHODCALLTYPE GetModifyTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetModifyTime(long time);
	HRESULT STDMETHODCALLTYPE GetCount(int* pCount);
	HRESULT STDMETHODCALLTYPE Item(VARIANT Index, INote** ppNote);
	HRESULT STDMETHODCALLTYPE AddNote(INote* pNote);
	HRESULT STDMETHODCALLTYPE RemoveNote(INote* pNote);
	HRESULT STDMETHODCALLTYPE GetNoteIdx(INote* pNote, int* pIndex);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	void NotifyThisObj(NotifyOperator ope, INote* pNote);

private:
	CComBSTR m_id;
	CComBSTR m_strName;
	std::tm m_createtime;
	std::tm m_modifytime;
	std::vector<INote*> m_vecNotes;
	//std::vector<ICoreNotify*> m_notifies;
	std::unordered_set<ICoreNotify*> m_notifies;
	int m_ref;
};

class NoteApplication : public INoteApplication
{
public:
	NoteApplication();
	~NoteApplication();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);

	HRESULT STDMETHODCALLTYPE GetNotebook(VARIANT Index, INotebook** ppNotebook);
	HRESULT STDMETHODCALLTYPE AddNotebook(INotebook* pNotebook);
	HRESULT STDMETHODCALLTYPE GetCount(int* pCount);
	HRESULT STDMETHODCALLTYPE GetUserId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetUserId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetNotebookIdx(INotebook* pNote, int* pIdx);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	CComBSTR m_id;
	std::vector<INotebook*> m_vecBooks;
	std::vector<ICoreNotify*> m_notifies;
	int m_ref;
};

#endif
