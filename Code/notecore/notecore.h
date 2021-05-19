#ifndef __CORE_INTERFACE_H__
#define __CORE_INTERFACE_H__

#include "framework.h"

enum NOTE_TYPE
{
	NORMAL_NOTE,
	UNKNOWN_TYPE,
};


interface INote : public IUnknown
{
	virtual HRESULT GetId(OUT BSTR* pbstrId) const = 0;
	virtual HRESULT SetId(IN BSTR bstrId) = 0;
	virtual HRESULT GetTitle(OUT BSTR* pbstrName) const = 0;
	virtual HRESULT SetTitle(IN BSTR title) = 0;
	virtual NOTE_TYPE GetType() const = 0;
	virtual HRESULT SetType(NOTE_TYPE type) = 0;
	virtual HRESULT GetContent(OUT BSTR* pbstrContent) const = 0;
	virtual HRESULT SetContent(IN BSTR content) = 0;
	virtual HRESULT GetAbbreText(OUT BSTR* pbstrAbbre) const = 0;
	virtual HRESULT SetPlainText(IN BSTR content) = 0;
	virtual std::tm GetCreateTime() const = 0;
	virtual HRESULT SetCreateTime(std::tm create_time) = 0;
	virtual std::tm GetModifyTime() const = 0;
	virtual HRESULT SetModifyTime(std::tm create_time) = 0;
};

interface INotebook : public IUnknown
{
	virtual HRESULT GetId(OUT BSTR* pbstrId) const = 0;
	virtual HRESULT SetId(IN BSTR bstrId) = 0;
	virtual HRESULT GetName(OUT BSTR* pbstrName) const = 0;
	virtual HRESULT SetName(IN BSTR bstrName) = 0;
	virtual std::tm GetCreateTime() const = 0;
	virtual HRESULT SetCreateTime(std::tm create_time) = 0;
	virtual std::tm GetModifyTime() const = 0;
	virtual HRESULT SetModifyTime(std::tm create_time) = 0;
	virtual int GetCount() const = 0;
	virtual HRESULT Item(VARIANT Index, INote** ppNote) = 0;
	virtual HRESULT AddNote(INote* pNote) = 0;
	virtual HRESULT RemoveNote(INote* pNote) = 0;
};

//暂时不区分Notebook的类型，可能不同类型的note组成一个book
interface INoteApplication : public IUnknown
{
	virtual int GetCount() const = 0;
	virtual HRESULT GetNotebook(VARIANT Index, INotebook** ppNotebook) = 0;
	virtual HRESULT AddNotebook(INotebook* pNotebook) = 0;
	virtual HRESULT GetUserId(OUT BSTR* pbstrId) const = 0;
	virtual HRESULT SetUserId(IN BSTR bstrId) = 0;
};

HRESULT CreateNote(NOTE_TYPE type, INote** ppNote);
HRESULT CreateNotebook(INotebook** ppNotebook);
HRESULT CreateApplication(INoteApplication** pApp);

#endif
