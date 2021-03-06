#ifndef __COM_SPTR_H__
#define __COM_SPTR_H__

#include "../notecore/guiddefine.h"
#include "iunknown.h"

template <class T>
class com_sptr
{
public:
	com_sptr() : m_ptr(NULL) {}

	com_sptr(T* ptr) : m_ptr(ptr)
	{
		m_ptr->AddRef();
	}

	com_sptr(IUnknown* pObj)
		: m_ptr(NULL)
	{
		if (pObj)
			pObj->AddRef();
		if (m_ptr)
			m_ptr->Release();
		pObj->QueryInterface(__uuid(T), (void**)&m_ptr);
	}

	com_sptr(const T* ptr) : m_ptr(ptr) {}

	com_sptr(const com_sptr& sptr)
	{
		m_ptr = sptr.m_ptr;
		m_ptr->AddRef();
	}

	~com_sptr()
	{
		if (m_ptr)
			m_ptr->Release();
	}

	void clear() {
		m_ptr->Release();
	}

	void assign(T* ptr) {
		m_ptr = ptr;
	}

	void attach(T* ptr) {
		m_ptr = ptr;
		m_ptr->AddRef();
	}

	T* detach() {
		T* ptr = m_ptr;
		m_ptr->Release();
		m_ptr = NULL;
		return ptr;
	}

	T** operator&() // The assert on operator& usually indicates a bug.
	{
		return &m_ptr;
	}

	T* operator->() const
	{
		return m_ptr;
	}

	T* operator=(T* p)
	{
		if (p)
			p->AddRef();
		if (m_ptr)
			m_ptr->Release();
		return m_ptr = p;
	}

	//T* operator=(IUnknown* p)
	//{
	//	
	//}

	T* operator=(const com_sptr<T>& p)
	{
		if (p.m_ptr)
			p.m_ptr->AddRef();
		if (m_ptr)
			m_ptr->Release();
		return m_ptr = p.m_ptr;
	}

	bool operator!() const
	{
		return m_ptr == NULL;
	}
	bool operator==(T* p) const
	{
		return m_ptr == p;
	}
	bool operator!=(T* p) const
	{
		return m_ptr != p;
	}
	operator T* () const
	{
		return m_ptr;
	}

private:
	T* m_ptr;
};



#endif

