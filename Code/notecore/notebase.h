#ifndef __NOTE_BASE_H__
#define __NOTE_BASE_H__

#include "framework.h"
#include "notecontainer.h"

class NoteBase : public INote
{
public:
	NoteBase();
	~NoteBase();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);

	HRESULT STDMETHODCALLTYPE GetId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetBookId(BSTR* pBookId);
	HRESULT STDMETHODCALLTYPE SetBookId(BSTR bookId);
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
	CComBSTR m_bookid;
	CComBSTR m_bstrTitle;
	CComBSTR m_bstrContent;
	CComBSTR m_bstrPlainText;
	std::tm m_createtime;
	std::tm m_modifytime;

	std::vector<ICoreNotify*> m_notifies;

	NOTE_TYPE m_type;
	int m_ref;
};

class NotebookBase : public NoteCollection<INotebook>
{
	typedef NoteCollection<INotebook> _base;
public:
	NotebookBase();
	~NotebookBase();

	HRESULT STDMETHODCALLTYPE GetId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetName(OUT BSTR* pbstrName);
	HRESULT STDMETHODCALLTYPE SetName(IN BSTR bstrName);
	HRESULT STDMETHODCALLTYPE GetCreateTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetCreateTime(long create_time);
	HRESULT STDMETHODCALLTYPE GetModifyTime(long* pTime);
	HRESULT STDMETHODCALLTYPE SetModifyTime(long time);
	HRESULT STDMETHODCALLTYPE AddNote(INote* pNote);
	HRESULT STDMETHODCALLTYPE RemoveNote(INote* pNote);
	HRESULT STDMETHODCALLTYPE GetNoteIdx(INote* pNote, int* pIndex);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

private:
	CComBSTR m_id;
	CComBSTR m_strName;
	std::tm m_createtime;
	std::tm m_modifytime;
};


class Notes : public NoteCollection<INoteCollection>
{
public:
	Notes();
	~Notes();

	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
};


class NotebooksBase : public INotebooks
{
public:
	NotebooksBase();
	~NotebooksBase();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);
	HRESULT STDMETHODCALLTYPE GetCount(/* [out] */ int* pCount);
	HRESULT STDMETHODCALLTYPE Item(VARIANT index, INotebook** ppNote);
	HRESULT STDMETHODCALLTYPE AddNotebook(INotebook* pNotebook);
	HRESULT STDMETHODCALLTYPE DeleteNotebook(INotebook* pNotebook);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	void NotifyThisObj(NotifyOperator ope, INotebook* pNote);

private:
	std::vector<INotebook*> m_vecBooks;
	std::unordered_set<ICoreNotify*> m_notifies;
	int m_ref;
};

class TrashBase : public NoteCollection<ITrash>
{
public:
	TrashBase();
	~TrashBase();

	/* INoteCollection */
	HRESULT STDMETHODCALLTYPE GetName(BSTR* pbstrName);
	/* ITrash */
	HRESULT STDMETHODCALLTYPE DeleteNote(INote* pNote);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
};

class SchedulesBase : public NoteCollection<ISchedules>
{
public:
	SchedulesBase();
	~SchedulesBase();

	HRESULT STDMETHODCALLTYPE GetName(BSTR* pbstrName);
	HRESULT STDMETHODCALLTYPE SetName(IN BSTR bstrName) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE GetId(BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetId(BSTR bstrId);
	
	HRESULT STDMETHODCALLTYPE GetCreateTime(long* pTime) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE SetCreateTime(long create_time) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE GetModifyTime(long* pTime) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE SetModifyTime(long time) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE GetNoteIdx(INote* pNote, int* pIndex) { return E_NOTIMPL; }
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

private:
	CComBSTR m_id;
};

class NoteApplication : public INoteApplication
{
public:
	NoteApplication();
	~NoteApplication();

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify);
	HRESULT STDMETHODCALLTYPE GetNotebooks(INotebooks** ppNotebooks);
	HRESULT STDMETHODCALLTYPE SetNotebooks(INotebooks* pNotebooks);
	HRESULT STDMETHODCALLTYPE GetUserId(OUT BSTR* pbstrId);
	HRESULT STDMETHODCALLTYPE SetUserId(IN BSTR bstrId);
	HRESULT STDMETHODCALLTYPE GetTrash(ITrash** ppTrash);
	HRESULT STDMETHODCALLTYPE SetTrash(ITrash* pTrash);
	HRESULT STDMETHODCALLTYPE GetSchedules(ISchedules** ppSchedules);
	HRESULT STDMETHODCALLTYPE SetSchedules(ISchedules* pSchedules);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	CComBSTR m_id;

	com_sptr<INotebooks> m_spNotebooks;
	com_sptr<ITrash> m_spTrash;
	com_sptr<ISchedules> m_spSchedules;
	std::unordered_set<ICoreNotify*> m_notifies;
	int m_ref;
};

#endif
