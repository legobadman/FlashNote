#ifndef __NOTE_CONTAINER_H__
#define __NOTE_CONTAINER_H__

template <class Interface>
class NoteCollection : public Interface
{
public:
	NoteCollection() : m_ref(0) {}
	~NoteCollection()
	{
		Clear();
	}

	HRESULT STDMETHODCALLTYPE addWatcher(ICoreNotify* pNotify)
	{
		m_notifies.insert(pNotify);
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE GetName(BSTR* pbstrName)
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE GetCount(int* pCount)
	{
		if (!pCount)
			return E_POINTER;

		*pCount = m_container.size();
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE Item(VARIANT index, INote** ppNote)
	{
		if (!ppNote)
			return E_POINTER;

		if (V_VT(&index) == VT_BSTR)
		{
			CComBSTR bstrTrashId(V_BSTR(&index));
			if (m_container.find(bstrTrashId) == m_container.end())
			{
				return E_FAIL;
			}
			*ppNote = m_container[bstrTrashId];
			(*ppNote)->AddRef();
			return S_OK;
		}
		else if (V_VT(&index) == VT_I4)
		{
			int idx = V_I4(&index);
			for (auto it = m_container.begin(); it != m_container.end(); it++)
			{
				if (idx == 0)
				{
					*ppNote = it->second;
					(*ppNote)->AddRef();
					return S_OK;
				}
				idx--;
			}
			return E_INVALIDARG;
		}
		else
		{
			return E_INVALIDARG;
		}
	}

	HRESULT STDMETHODCALLTYPE AddNote(INote* pNote)
	{
		if (!pNote)
			return E_FAIL;

		CComBSTR bstrTrashId;
		pNote->GetId(&bstrTrashId);

		pNote->AddRef();
		m_container.insert(std::pair<CComBSTR, INote*>(bstrTrashId, pNote));

		NotifyThisObj(Add, pNote);
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE RemoveNote(INote* pNote)
	{
		if (!pNote)
			return E_FAIL;

		CComBSTR bstrNoteId;
		pNote->GetId(&bstrNoteId);

		auto it = m_container.find(bstrNoteId);
		if (it == m_container.end())
		{
			return E_FAIL;
		}

		NotifyThisObj(Delete, pNote);
		pNote->Release();
		m_container.erase(bstrNoteId);
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE Clear(void)
	{
		//TODO: Notify
		for (auto it = m_container.begin(); it != m_container.end(); it++)
		{
			NotifyThisObj(Delete, it->second);
			it->second->Release();
		}
		m_container.clear();
		return S_OK;
	}

public:
	ULONG STDMETHODCALLTYPE AddRef(void)
	{
		m_ref++;
		return m_ref;
	}

	ULONG STDMETHODCALLTYPE Release(void)
	{
		m_ref--;
		if (m_ref == 0)
		{
			delete this;
		}
		return m_ref;
	}

private:
	void NotifyThisObj(NotifyOperator ope, INote* pNote)
	{
		for (auto it = m_notifies.begin(); it != m_notifies.end(); it++)
		{
			BSTR bstrId;
			pNote->GetId(&bstrId);
			std::wstring noteid(bstrId, SysStringLen(bstrId));
			NotifyArg arg;
			arg.ope = ope;
			arg.pObj = pNote;
			(*it)->onCoreNotify(this, arg);
		}
	}

protected:
	std::unordered_set<ICoreNotify*> m_notifies;
	std::map<CComBSTR, INote*> m_container;
	int m_ref;
};

#endif
