#ifndef __GUID_DEFINE_H__
#define __GUID_DEFINE_H__

#include <combaseapi.h>

template<class T>
const GUID& uuidof();

#define __uuid(cls) uuidof<cls>()

#define DEFINE_INTERFACE_GUID(Interface) \
template<>\
const GUID& uuidof<Interface>()\
{\
	return IID_##Interface;\
}

#endif
