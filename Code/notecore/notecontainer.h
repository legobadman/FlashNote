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

	HRESULT addWatcher(ICoreNotify* pNotify)
	{
		m_notifies.insert(pNotify);
		return S_OK;
	}

	HRESULT GetName(std::wstring&)
	{
		return E_NOTIMPL;
	}

	HRESULT GetCount(int* pCount)
	{
		if (!pCount)
			return E_FAIL;

		*pCount = m_container.size();
		return S_OK;
	}

	HRESULT Item(const wstring& index, INote** ppNote)
	{
		if (!ppNote)
			return E_FAIL;

		if (m_container.find(index) == m_container.end())
		{
			return E_FAIL;
		}
		*ppNote = m_container[index];
		(*ppNote)->AddRef();
		return S_OK;
	}

	HRESULT Item(int index, INote** ppNote)
	{
		if (!ppNote)
			return E_FAIL;

		int idx = index;
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
		return E_FAIL;
	}

	HRESULT AddNote(INote* pNote)
	{
		if (!pNote)
			return E_FAIL;

		std::wstring bstrTrashId;
		pNote->GetId(bstrTrashId);

		pNote->AddRef();
		m_container.insert(std::pair<std::wstring, INote*>(bstrTrashId, pNote));

		NotifyThisObj(Add, pNote);
		return S_OK;
	}

	HRESULT RemoveNote(INote* pNote)
	{
		if (!pNote)
			return E_FAIL;

		std::wstring noteid;
		pNote->GetId(noteid);

		auto it = m_container.find(noteid);
		if (it == m_container.end())
		{
			return E_FAIL;
		}

		NotifyThisObj(Delete, pNote);
		pNote->Release();
		m_container.erase(noteid);
		return S_OK;
	}

	HRESULT Clear(void)
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
	long AddRef(void)
	{
		m_ref++;
		return m_ref;
	}

	long Release(void)
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
			std::wstring noteid;
			pNote->GetId(noteid);
			NotifyArg arg;
			arg.ope = ope;
			arg.pObj = pNote;
			(*it)->onCoreNotify(this, arg);
		}
	}

protected:
	std::unordered_set<ICoreNotify*> m_notifies;
	std::map<std::wstring, INote*> m_container;
	int m_ref;
};

#endif