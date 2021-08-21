#ifndef __GUID_DEFINE_H__
#define __GUID_DEFINE_H__

#include <QUuid>

template<class T>
const QUuid& uuidof();

#define __uuid(cls) uuidof<cls>()

#define DEFINE_INTERFACE_GUID(Interface) \
template<>\
const QUuid& uuidof<Interface>()\
{\
	return IID_##Interface;\
}


#endif
