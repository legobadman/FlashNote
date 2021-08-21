#ifndef __IUNKNOWN_H__
#define __IUNKNOWN_H__

#include "common_define.h"
#include <QUuid>

interface IUnknown
{
	virtual HRESULT QueryInterface(GUID riid, void** ppvObject) = 0;
	virtual long AddRef(void) = 0;
	virtual long Release(void) = 0;
};

#endif
