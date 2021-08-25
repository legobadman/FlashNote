#ifndef __NOTECORE2_H__
#define __NOTECORE2_H__

#include "common_define.h"
#include "iunknown.h"
#include "framework.h"

interface INoteCoreObj;

enum NOTE_TYPE
{
	NORMAL_NOTE = 0,
	MINDMAP = (NORMAL_NOTE + 1),
	SCHEDULE = (MINDMAP + 1),
	UNKNOWN_TYPE = (SCHEDULE + 1)
};

enum NotifyOperator
{
	Add = 0,
	Delete,
	Update,
	Other
};

struct NotifyArg
{
	enum NotifyOperator ope;
	int idx;
	wchar_t* wid;
	INoteCoreObj* pObj;
};

interface ICoreNotify : public IUnknown
{
	virtual HRESULT onCoreNotify(INoteCoreObj * pCoreObj, NotifyArg arg) = 0;
};

interface INoteCoreObj : public IUnknown
{
	virtual HRESULT addWatcher(ICoreNotify * pNotify) = 0;
};

interface INote: public INoteCoreObj
{
	virtual HRESULT GetId(std::wstring& pbstrId) = 0;
	virtual HRESULT SetId(const std::wstring& bstrId) = 0;
	virtual HRESULT GetBookId(std::wstring& pBookId) = 0;
	virtual HRESULT SetBookId(const std::wstring& bookId) = 0;
	virtual HRESULT GetTitle(std::wstring& pbstrName) = 0;
	virtual HRESULT SetTitle(const std::wstring& title) = 0;
	virtual HRESULT GetType(NOTE_TYPE* pType) = 0;
	virtual HRESULT SetType(NOTE_TYPE type) = 0;
	virtual HRESULT GetContent(std::wstring& pbstrContent) = 0;
	virtual HRESULT SetContent(const std::wstring& content) = 0;
	virtual HRESULT GetAbbreText(std::wstring& pbstrAbbre) = 0;
	virtual HRESULT SetPlainText(const std::wstring& content) = 0;
	virtual HRESULT GetCreateTime(long long* pTime) = 0;
	virtual HRESULT SetCreateTime(long long time) = 0;
	virtual HRESULT GetModifyTime(long long* pTime) = 0;
	virtual HRESULT SetModifyTime(long long time) = 0;
};

interface INoteCollection : public INoteCoreObj
{
public:
	virtual HRESULT GetName(std::wstring& pbstrName) = 0;
	virtual HRESULT GetCount(int* pCount) = 0;
	virtual HRESULT Item(const wstring& index, INote** ppNote) = 0;
	virtual HRESULT Item(int index, INote** ppNote) = 0;
	virtual HRESULT AddNote(INote* pNote) = 0;
	virtual HRESULT RemoveNote(INote* pNote) = 0;
	virtual HRESULT Clear(void) = 0;
};

interface INotebook : public INoteCollection
{
public:
	virtual HRESULT GetId(std::wstring& pbstrId) = 0;
	virtual HRESULT SetId(const std::wstring& bstrId) = 0;
	virtual HRESULT SetName(const std::wstring& bstrName) = 0;
	virtual HRESULT GetCreateTime(long long* pTime) = 0;
	virtual HRESULT SetCreateTime(long long time) = 0;
	virtual HRESULT GetModifyTime(long long* pTime) = 0;
	virtual HRESULT SetModifyTime(long long time) = 0;
	virtual HRESULT GetNoteIdx(INote* pNote, int* pIndex) = 0;
};

interface INotebooks: public INoteCoreObj
{
public:
	virtual HRESULT GetCount(int* pCount) = 0;
	virtual HRESULT Item(const wstring& index, INotebook** ppNotebook) = 0;
	virtual HRESULT Item(int index, INotebook** ppNotebook) = 0;
	virtual HRESULT AddNotebook(INotebook* pNotebook) = 0;
	virtual HRESULT DeleteNotebook(INotebook* pNotebook) = 0;
};

interface ISchedules : public INotebook
{
public:
};

interface ITrash : public INoteCollection
{
public:
	virtual HRESULT DeleteNote(INote * pNote) = 0;
};

interface INoteApplication: public INoteCoreObj
{
public:
	virtual HRESULT GetNotebooks(INotebooks * *ppNotebooks) = 0;
	virtual HRESULT SetNotebooks(INotebooks* pNotebooks) = 0;
	virtual HRESULT GetTrash(ITrash** ppTrash) = 0;
	virtual HRESULT SetTrash(ITrash* pTrash) = 0;
	virtual HRESULT GetSchedules(ISchedules** ppSchedules) = 0;
	virtual HRESULT SetSchedules(ISchedules* pSchedules) = 0;
	virtual HRESULT GetUserId(std::wstring& pbstrId) = 0;
	virtual HRESULT SetUserId(const std::wstring& bstrId) = 0;
};

#endif