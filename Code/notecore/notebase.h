#ifndef __NOTE_BASE_H__
#define __NOTE_BASE_H__

class NoteBase : public INote
{
public:
	NoteBase();
	~NoteBase();

	HRESULT GetId(OUT BSTR* pbstrId) const;
	HRESULT SetId(IN BSTR bstrId);
	HRESULT GetTitle(OUT BSTR* pbstrName) const;
	HRESULT SetTitle(IN BSTR title);
	NOTE_TYPE GetType() const;
	HRESULT SetType(NOTE_TYPE type);
	HRESULT GetContent(OUT BSTR* pbstrContent) const;
	HRESULT SetContent(IN BSTR content);
	HRESULT GetAbbreText(OUT BSTR* pbstrAbbre) const;
	HRESULT SetPlainText(IN BSTR content);
	std::tm GetCreateTime() const;
	HRESULT SetCreateTime(std::tm create_time);
	std::tm GetModifyTime() const;
	HRESULT SetModifyTime(std::tm create_time);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	CComBSTR m_id;
	CComBSTR m_bstrTitle;
	CComBSTR m_bstrContent;
	CComBSTR m_bstrPlainText;
	std::tm m_createtime;
	std::tm m_modifytime;

	NOTE_TYPE m_type;
	int m_ref;
};

class NotebookBase : public INotebook
{
public:
	NotebookBase();
	~NotebookBase();

	HRESULT GetId(OUT BSTR* pbstrId) const;
	HRESULT SetId(IN BSTR bstrId);
	HRESULT GetName(OUT BSTR* pbstrName) const;
	HRESULT SetName(IN BSTR bstrName);
	std::tm GetCreateTime() const;
	HRESULT SetCreateTime(std::tm create_time);
	std::tm GetModifyTime() const;
	HRESULT SetModifyTime(std::tm create_time);
	int GetCount() const;
	HRESULT Item(VARIANT Index, INote** ppNote);
	HRESULT AddNote(INote* pNote);
	HRESULT RemoveNote(INote* pNote);
	int GetNoteIdx(INote* pNote);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	CComBSTR m_id;
	CComBSTR m_strName;
	std::tm m_createtime;
	std::tm m_modifytime;
	std::vector<INote*> m_vecNotes;
	int m_ref;
};

class NoteApplication : public INoteApplication
{
public:
	NoteApplication();
	~NoteApplication();
	HRESULT GetNotebook(VARIANT Index, INotebook** ppNotebook);
	HRESULT AddNotebook(INotebook* pNotebook);
	int GetCount() const;
	HRESULT GetUserId(OUT BSTR* pbstrId) const;
	HRESULT SetUserId(IN BSTR bstrId);
	int GetNoteBookIdx(INotebook* pNote);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);

private:
	CComBSTR m_id;
	std::vector<INotebook*> m_vecBooks;
	int m_ref;
};

#endif
