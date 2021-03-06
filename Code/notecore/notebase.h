#ifndef __NOTE_BASE_H__
#define __NOTE_BASE_H__

#include "framework.h"
#include "notecontainer.h"
#include "notecore2.h"

class NoteBase : public INote
{
public:
	NoteBase();
	~NoteBase();

	HRESULT addWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT removeWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT GetId(std::wstring& pbstrId);
	HRESULT SetId(const std::wstring& bstrId);
	HRESULT GetBookId(std::wstring& pBookId);
	HRESULT SetBookId(const std::wstring& bookId);
	HRESULT GetTitle(std::wstring& pbstrName);
	HRESULT SetTitle(const std::wstring& title);
	HRESULT GetType(NOTE_TYPE* pType);
	HRESULT SetType(NOTE_TYPE type);
	HRESULT GetContent(std::wstring& pbstrContent);
	HRESULT SetContent(const std::wstring& content);
	HRESULT GetAbbreText(std::wstring& pbstrAbbre);
	HRESULT SetPlainText(const std::wstring& content);
	HRESULT GetCreateTime(long long* pTime);
	HRESULT SetCreateTime(long long time);
	HRESULT GetModifyTime(long long* pTime);
	HRESULT SetModifyTime(long long time);

public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);
	long AddRef(void);
	long Release(void);

private:
	void NotifyThisObj(NotifyOperator ope);

private:
	wstring m_id;
	wstring m_bookid;
	wstring m_bstrTitle;
	wstring m_bstrContent;
	wstring m_bstrPlainText;

	CORE_NOTIFY_SET m_notifies;

	NOTE_TYPE m_type;
	long long modify_time;
	long long create_time;
	int m_ref;
};

class NotebookBase : public NoteCollection<INotebook>
{
	typedef NoteCollection<INotebook> _base;
public:
	NotebookBase();
	~NotebookBase();

	HRESULT GetName(std::wstring& pbstrName);
	HRESULT AddNote(INote* pNote);
	HRESULT RemoveNote(INote* pNote);

	HRESULT GetId(std::wstring& pbstrId);
	HRESULT SetId(const std::wstring& bstrId);
	HRESULT SetName(const std::wstring& bstrName);
	HRESULT GetCreateTime(long long* pTime);
	HRESULT SetCreateTime(long long time);
	HRESULT GetModifyTime(long long* pTime);
	HRESULT SetModifyTime(long long time);
	HRESULT GetNoteIdx(INote* pNote, int* pIndex);

public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);

private:
	wstring m_id;
	wstring m_strName;
	long long modify_time;
	long long create_time;
};

class NotebooksBase : public INotebooks
{
public:
	NotebooksBase();
	~NotebooksBase();

	HRESULT addWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT removeWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT GetCount(int* pCount);
	HRESULT Item(const wstring& index, INotebook** ppNotebook);
	HRESULT Item(int index, INotebook** ppNotebook);
	HRESULT AddNotebook(INotebook* pNotebook);
	HRESULT DeleteNotebook(INotebook* pNotebook);

public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);
	long AddRef(void);
	long Release(void);

private:
	void NotifyThisObj(NotifyOperator ope, INotebook* pNote);

private:
	std::vector<INotebook*> m_vecBooks;
	CORE_NOTIFY_SET m_notifies;
	int m_ref;
};

class TrashBase : public NoteCollection<ITrash>
{
public:
	TrashBase();
	~TrashBase();

	/* INoteCollection */
	HRESULT GetName(std::wstring& name);
	/* ITrash */
	HRESULT DeleteNote(INote* pNote);

public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);
};

class SchedulesBase : public NoteCollection<ISchedules>
{
public:
	SchedulesBase();
	~SchedulesBase();

	HRESULT GetName(std::wstring& name);
	HRESULT SetName(const std::wstring& bstrName) { return E_NOTIMPL; }
	HRESULT GetId(std::wstring& pbstrId);
	HRESULT SetId(const std::wstring& bstrId);
	
	HRESULT GetCreateTime(long long* pTime) { return E_NOTIMPL; }
	HRESULT SetCreateTime(long long create_time) { return E_NOTIMPL; }
	HRESULT GetModifyTime(long long* pTime) { return E_NOTIMPL; }
	HRESULT SetModifyTime(long long time) { return E_NOTIMPL; }

	HRESULT GetNoteIdx(INote* pNote, int* pIndex) { return E_NOTIMPL; }
public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);

private:
	wstring m_id;
};

class NoteApplication : public INoteApplication
{
public:
	NoteApplication();
	~NoteApplication();

	HRESULT addWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT removeWatcher(weak_ptr<ICoreNotify> pNotify);
	HRESULT GetNotebooks(INotebooks** ppNotebooks);
	HRESULT SetNotebooks(INotebooks* pNotebooks);
	HRESULT GetUserId(std::wstring& pbstrId);
	HRESULT SetUserId(const std::wstring& bstrId);
	HRESULT GetTrash(ITrash** ppTrash);
	HRESULT SetTrash(ITrash* pTrash);
	HRESULT GetSchedules(ISchedules** ppSchedules);
	HRESULT SetSchedules(ISchedules* pSchedules);

public:
	HRESULT QueryInterface(QUuid riid, void** ppvObject);
	long AddRef(void);
	long Release(void);

private:
	wstring m_id;
	com_sptr<INotebooks> m_spNotebooks;
	com_sptr<ITrash> m_spTrash;
	com_sptr<ISchedules> m_spSchedules;
	CORE_NOTIFY_SET m_notifies;
	int m_ref;
};

#endif
