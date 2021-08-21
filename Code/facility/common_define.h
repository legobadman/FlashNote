#ifndef __COMMON_DEFINE_H__
#define __COMMON_DEFINE_H__

#include <string>
#include <QUuid>

#define interface struct

typedef long HRESULT;

#define S_OK 0
#define E_FAIL -1
#define E_NOTIMPL -2
#define FALSE               0
#define TRUE               1

#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define FAILED(hr)      (((HRESULT)(hr)) < 0)

#endif
