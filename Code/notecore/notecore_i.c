

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for notecore.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ICoreNotify,0x956003E7,0x3C5A,0x42EB,0xBA,0x19,0xD8,0xEE,0xF5,0x4D,0xAB,0xFF);


MIDL_DEFINE_GUID(IID, IID_INoteCoreObj,0xA13F359A,0x007B,0x472A,0xAC,0x14,0x97,0xFE,0x79,0xC4,0x04,0xFE);


MIDL_DEFINE_GUID(IID, IID_INote,0x8FD93C3C,0x28FD,0x43E0,0x88,0xB5,0x61,0xD1,0xAC,0x4C,0x42,0xF0);


MIDL_DEFINE_GUID(IID, IID_INoteCollection,0x6FC8017C,0xD8CD,0x41D0,0x8C,0x27,0x4C,0x9F,0xB7,0xCC,0x4C,0xA5);


MIDL_DEFINE_GUID(IID, IID_INotebook,0x701090AB,0x4965,0x46F3,0xAE,0xA5,0x36,0xC3,0x0F,0xBD,0xC5,0x7A);


MIDL_DEFINE_GUID(IID, IID_INotebooks,0x279EE392,0x286F,0x4423,0x81,0xB9,0x4D,0xF9,0xE2,0xF4,0x30,0x9E);


MIDL_DEFINE_GUID(IID, IID_INoteApplication,0x9ACA0D9D,0x3CFF,0x4644,0x92,0xE9,0x86,0x3B,0x5F,0xBB,0x3D,0x02);


MIDL_DEFINE_GUID(IID, IID_ITrashRecord,0xBEDCB647,0x62CD,0x4B76,0xA6,0xED,0x33,0xDE,0x73,0xBC,0x2E,0x7A);


MIDL_DEFINE_GUID(IID, IID_ITrash,0x982D1B17,0xC848,0x4701,0xA1,0x12,0x7F,0x03,0xE4,0xD5,0x0C,0x70);


MIDL_DEFINE_GUID(IID, IID_IFreeNotes,0xC97AE266,0x5822,0x4FAE,0x87,0x55,0xDC,0xDE,0x14,0x5D,0x97,0xB6);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



