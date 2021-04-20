#ifndef __NOTE_BASE_H__
#define __NOTE_BASE_H__

class NoteBase : public INote
{
public:
	NoteBase();
	~NoteBase();
	virtual wstring GetTitle() const;
	virtual HRESULT SetTitle(const wstring& title);
	virtual NOTE_TYPE GetType() const;
	virtual HRESULT SetType(NOTE_TYPE type);

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

private:
	std::wstring m_strTitle;
	std::wstring m_content;
	NOTE_TYPE m_type;
	int m_ref;
};

#endif
