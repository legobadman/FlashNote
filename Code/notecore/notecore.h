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
	virtual wstring GetTitle() const = 0;
	virtual HRESULT SetTitle(const wstring& title) = 0;
	virtual NOTE_TYPE GetType() const = 0;
	virtual HRESULT SetType(NOTE_TYPE type) = 0;
};

INote* CreateNote(NOTE_TYPE type);

#endif
