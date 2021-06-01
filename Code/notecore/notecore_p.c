

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "notecore.h"

#define TYPE_FORMAT_STRING_SIZE   1215                              
#define PROC_FORMAT_STRING_SIZE   1459                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _notecore_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } notecore_MIDL_TYPE_FORMAT_STRING;

typedef struct _notecore_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } notecore_MIDL_PROC_FORMAT_STRING;

typedef struct _notecore_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } notecore_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const notecore_MIDL_TYPE_FORMAT_STRING notecore__MIDL_TypeFormatString;
extern const notecore_MIDL_PROC_FORMAT_STRING notecore__MIDL_ProcFormatString;
extern const notecore_MIDL_EXPR_FORMAT_STRING notecore__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICoreNotify_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICoreNotify_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INoteCoreObj_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INoteCoreObj_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INote_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INote_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INoteCollection_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INoteCollection_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INotebook_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INotebook_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INotebooks_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INotebooks_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INoteApplication_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INoteApplication_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITrash_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITrash_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const notecore_MIDL_PROC_FORMAT_STRING notecore__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure onCoreNotify */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCoreObj */

/* 24 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter arg */

/* 30 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	NdrFcShort( 0x14 ),	/* Type Offset=20 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure addWatcher */

/* 42 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x3 ),	/* 3 */
/* 50 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 52 */	NdrFcShort( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 58 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNotify */

/* 66 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 70 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 72 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetName */


	/* Procedure GetId */

/* 78 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 80 */	NdrFcLong( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x4 ),	/* 4 */
/* 86 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 94 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 96 */	NdrFcShort( 0x1 ),	/* 1 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrName */


	/* Parameter pbstrId */

/* 102 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 104 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 106 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */


	/* Return value */

/* 108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 110 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetId */

/* 114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x5 ),	/* 5 */
/* 122 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 128 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 130 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 134 */	NdrFcShort( 0x1 ),	/* 1 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrId */

/* 138 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 140 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 142 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 144 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 146 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetBookId */

/* 150 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 152 */	NdrFcLong( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x6 ),	/* 6 */
/* 158 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0x8 ),	/* 8 */
/* 164 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 166 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 168 */	NdrFcShort( 0x1 ),	/* 1 */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBookId */

/* 174 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 176 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 178 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 180 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 182 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBookId */

/* 186 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 188 */	NdrFcLong( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x7 ),	/* 7 */
/* 194 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 200 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 202 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x1 ),	/* 1 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bookId */

/* 210 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 212 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 214 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 216 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 218 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetUserId */


	/* Procedure GetTitle */

/* 222 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 224 */	NdrFcLong( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 230 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 236 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 238 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 240 */	NdrFcShort( 0x1 ),	/* 1 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrId */


	/* Parameter pbstrName */

/* 246 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 248 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 250 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */


	/* Return value */

/* 252 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 254 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetUserId */


	/* Procedure SetTitle */

/* 258 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 260 */	NdrFcLong( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x9 ),	/* 9 */
/* 266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 272 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 274 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x1 ),	/* 1 */
/* 280 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrId */


	/* Parameter title */

/* 282 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 284 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 286 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */


	/* Return value */

/* 288 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 290 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetType */

/* 294 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0xa ),	/* 10 */
/* 302 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0x22 ),	/* 34 */
/* 308 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 310 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pType */

/* 318 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 320 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 322 */	NdrFcShort( 0x70 ),	/* Type Offset=112 */

	/* Return value */

/* 324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 326 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetType */

/* 330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0xb ),	/* 11 */
/* 338 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 340 */	NdrFcShort( 0x6 ),	/* 6 */
/* 342 */	NdrFcShort( 0x8 ),	/* 8 */
/* 344 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 346 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 356 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 358 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 362 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetContent */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0xc ),	/* 12 */
/* 374 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 380 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 382 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 384 */	NdrFcShort( 0x1 ),	/* 1 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrContent */

/* 390 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 392 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 394 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 398 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetContent */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0xd ),	/* 13 */
/* 410 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 418 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 422 */	NdrFcShort( 0x1 ),	/* 1 */
/* 424 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter content */

/* 426 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 428 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 430 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 432 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 434 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAbbreText */

/* 438 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 440 */	NdrFcLong( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0xe ),	/* 14 */
/* 446 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x8 ),	/* 8 */
/* 452 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 454 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 456 */	NdrFcShort( 0x1 ),	/* 1 */
/* 458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 460 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrAbbre */

/* 462 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 464 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 466 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 468 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 470 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetPlainText */

/* 474 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 476 */	NdrFcLong( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0xf ),	/* 15 */
/* 482 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 488 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 490 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	NdrFcShort( 0x1 ),	/* 1 */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter content */

/* 498 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 500 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 502 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 504 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 506 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCreateTime */

/* 510 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 512 */	NdrFcLong( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x10 ),	/* 16 */
/* 518 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x24 ),	/* 36 */
/* 524 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 526 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTime */

/* 534 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 536 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 540 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 542 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCreateTime */

/* 546 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 548 */	NdrFcLong( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x11 ),	/* 17 */
/* 554 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 556 */	NdrFcShort( 0x8 ),	/* 8 */
/* 558 */	NdrFcShort( 0x8 ),	/* 8 */
/* 560 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 562 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 564 */	NdrFcShort( 0x0 ),	/* 0 */
/* 566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter time */

/* 570 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 572 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 578 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetModifyTime */

/* 582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x12 ),	/* 18 */
/* 590 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x24 ),	/* 36 */
/* 596 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 598 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTime */

/* 606 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 608 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 612 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 614 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 616 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModifyTime */

/* 618 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 620 */	NdrFcLong( 0x0 ),	/* 0 */
/* 624 */	NdrFcShort( 0x13 ),	/* 19 */
/* 626 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 628 */	NdrFcShort( 0x8 ),	/* 8 */
/* 630 */	NdrFcShort( 0x8 ),	/* 8 */
/* 632 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 634 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter time */

/* 642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 644 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 650 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCount */

/* 654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x5 ),	/* 5 */
/* 662 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x24 ),	/* 36 */
/* 668 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 670 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCount */

/* 678 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 680 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 686 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x6 ),	/* 6 */
/* 698 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 704 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 706 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x1 ),	/* 1 */
/* 712 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 714 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 716 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 718 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Parameter ppNote */

/* 720 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 722 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 724 */	NdrFcShort( 0x466 ),	/* Type Offset=1126 */

	/* Return value */

/* 726 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 728 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 730 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddNote */

/* 732 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 734 */	NdrFcLong( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0x7 ),	/* 7 */
/* 740 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x8 ),	/* 8 */
/* 746 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 748 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNote */

/* 756 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 758 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 760 */	NdrFcShort( 0x46a ),	/* Type Offset=1130 */

	/* Return value */

/* 762 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 764 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveNote */

/* 768 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 770 */	NdrFcLong( 0x0 ),	/* 0 */
/* 774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 776 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x8 ),	/* 8 */
/* 782 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 784 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNote */

/* 792 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 794 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 796 */	NdrFcShort( 0x46a ),	/* Type Offset=1130 */

	/* Return value */

/* 798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 800 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Clear */

/* 804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 810 */	NdrFcShort( 0x9 ),	/* 9 */
/* 812 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 818 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 820 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 828 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 830 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 832 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetId */

/* 834 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 836 */	NdrFcLong( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0xa ),	/* 10 */
/* 842 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 850 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 852 */	NdrFcShort( 0x1 ),	/* 1 */
/* 854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrId */

/* 858 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 860 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 862 */	NdrFcShort( 0x58 ),	/* Type Offset=88 */

	/* Return value */

/* 864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 866 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetId */

/* 870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0xb ),	/* 11 */
/* 878 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 884 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 886 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0x1 ),	/* 1 */
/* 892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrId */

/* 894 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 898 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 900 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 902 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetName */

/* 906 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 908 */	NdrFcLong( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0xc ),	/* 12 */
/* 914 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 920 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 922 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 926 */	NdrFcShort( 0x1 ),	/* 1 */
/* 928 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrName */

/* 930 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 932 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 934 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 936 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 938 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 940 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCreateTime */

/* 942 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 944 */	NdrFcLong( 0x0 ),	/* 0 */
/* 948 */	NdrFcShort( 0xd ),	/* 13 */
/* 950 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 954 */	NdrFcShort( 0x24 ),	/* 36 */
/* 956 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 958 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 960 */	NdrFcShort( 0x0 ),	/* 0 */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTime */

/* 966 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 968 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 972 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 974 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetCreateTime */

/* 978 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0xe ),	/* 14 */
/* 986 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 988 */	NdrFcShort( 0x8 ),	/* 8 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 994 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter time */

/* 1002 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1004 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1008 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1010 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1012 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetModifyTime */

/* 1014 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1016 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0xf ),	/* 15 */
/* 1022 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1028 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1030 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1032 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1034 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTime */

/* 1038 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1040 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1044 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1046 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1048 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetModifyTime */

/* 1050 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1052 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1056 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1058 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1060 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1062 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1064 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1066 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1072 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter time */

/* 1074 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1076 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1080 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1082 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNoteIdx */

/* 1086 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1094 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1098 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1100 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1102 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNote */

/* 1110 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1112 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1114 */	NdrFcShort( 0x46a ),	/* Type Offset=1130 */

	/* Parameter pIndex */

/* 1116 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1118 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1122 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCount */

/* 1128 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1130 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1134 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1136 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1140 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1142 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1144 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1150 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCount */

/* 1152 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1154 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1156 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1158 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1160 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Item */

/* 1164 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1166 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1170 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1172 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1176 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1178 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1180 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1186 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1188 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1190 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1192 */	NdrFcShort( 0x45c ),	/* Type Offset=1116 */

	/* Parameter ppNote */

/* 1194 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1196 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1198 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Return value */

/* 1200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1202 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddNotebook */

/* 1206 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1208 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1212 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1214 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1220 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1222 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1228 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNotebook */

/* 1230 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1232 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1234 */	NdrFcShort( 0x480 ),	/* Type Offset=1152 */

	/* Return value */

/* 1236 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1238 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteNotebook */

/* 1242 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1244 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1248 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1250 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1252 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1254 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1256 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1258 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1264 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNotebook */

/* 1266 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1268 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1270 */	NdrFcShort( 0x480 ),	/* Type Offset=1152 */

	/* Return value */

/* 1272 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1274 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetNotebooks */

/* 1278 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1280 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1284 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1286 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1292 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1294 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1300 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppNotebooks */

/* 1302 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1304 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1306 */	NdrFcShort( 0x492 ),	/* Type Offset=1170 */

	/* Return value */

/* 1308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1310 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetNotebooks */

/* 1314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0x5 ),	/* 5 */
/* 1322 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1330 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNotebooks */

/* 1338 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1342 */	NdrFcShort( 0x496 ),	/* Type Offset=1174 */

	/* Return value */

/* 1344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1346 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetTrash */

/* 1350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1358 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1364 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1366 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1372 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppTrash */

/* 1374 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1376 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1378 */	NdrFcShort( 0x4a8 ),	/* Type Offset=1192 */

	/* Return value */

/* 1380 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1382 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1384 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTrash */

/* 1386 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1388 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1392 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1394 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1400 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1402 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTrash */

/* 1410 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1412 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1414 */	NdrFcShort( 0x4ac ),	/* Type Offset=1196 */

	/* Return value */

/* 1416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1418 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeleteNote */

/* 1422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1428 */	NdrFcShort( 0xa ),	/* 10 */
/* 1430 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1436 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1438 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pNote */

/* 1446 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1448 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1450 */	NdrFcShort( 0x46a ),	/* Type Offset=1130 */

	/* Return value */

/* 1452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1454 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const notecore_MIDL_TYPE_FORMAT_STRING notecore__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0xa13f359a ),	/* -1589693030 */
/*  8 */	NdrFcShort( 0x7b ),	/* 123 */
/* 10 */	NdrFcShort( 0x472a ),	/* 18218 */
/* 12 */	0xac,		/* 172 */
			0x14,		/* 20 */
/* 14 */	0x97,		/* 151 */
			0xfe,		/* 254 */
/* 16 */	0x79,		/* 121 */
			0xc4,		/* 196 */
/* 18 */	0x4,		/* 4 */
			0xfe,		/* 254 */
/* 20 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 22 */	NdrFcShort( 0x10 ),	/* 16 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0xa ),	/* Offset= 10 (36) */
/* 28 */	0xd,		/* FC_ENUM16 */
			0x8,		/* FC_LONG */
/* 30 */	0x36,		/* FC_POINTER */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 32 */	0x0,		/* 0 */
			NdrFcShort( 0xffe1 ),	/* Offset= -31 (2) */
			0x5b,		/* FC_END */
/* 36 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 38 */	0x5,		/* FC_WCHAR */
			0x5c,		/* FC_PAD */
/* 40 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 42 */	NdrFcLong( 0x956003e7 ),	/* -1788869657 */
/* 46 */	NdrFcShort( 0x3c5a ),	/* 15450 */
/* 48 */	NdrFcShort( 0x42eb ),	/* 17131 */
/* 50 */	0xba,		/* 186 */
			0x19,		/* 25 */
/* 52 */	0xd8,		/* 216 */
			0xee,		/* 238 */
/* 54 */	0xf5,		/* 245 */
			0x4d,		/* 77 */
/* 56 */	0xab,		/* 171 */
			0xff,		/* 255 */
/* 58 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 60 */	NdrFcShort( 0x1c ),	/* Offset= 28 (88) */
/* 62 */	
			0x13, 0x0,	/* FC_OP */
/* 64 */	NdrFcShort( 0xe ),	/* Offset= 14 (78) */
/* 66 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 68 */	NdrFcShort( 0x2 ),	/* 2 */
/* 70 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 72 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 74 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 76 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 78 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 80 */	NdrFcShort( 0x8 ),	/* 8 */
/* 82 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (66) */
/* 84 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 86 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 88 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x4 ),	/* 4 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0xffde ),	/* Offset= -34 (62) */
/* 98 */	
			0x12, 0x0,	/* FC_UP */
/* 100 */	NdrFcShort( 0xffea ),	/* Offset= -22 (78) */
/* 102 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x4 ),	/* 4 */
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (98) */
/* 112 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 114 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/* 116 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 118 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 120 */	
			0x12, 0x0,	/* FC_UP */
/* 122 */	NdrFcShort( 0x3ce ),	/* Offset= 974 (1096) */
/* 124 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 126 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 128 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 130 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 132 */	NdrFcShort( 0x2 ),	/* Offset= 2 (134) */
/* 134 */	NdrFcShort( 0x10 ),	/* 16 */
/* 136 */	NdrFcShort( 0x2f ),	/* 47 */
/* 138 */	NdrFcLong( 0x14 ),	/* 20 */
/* 142 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 144 */	NdrFcLong( 0x3 ),	/* 3 */
/* 148 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 150 */	NdrFcLong( 0x11 ),	/* 17 */
/* 154 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 156 */	NdrFcLong( 0x2 ),	/* 2 */
/* 160 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 162 */	NdrFcLong( 0x4 ),	/* 4 */
/* 166 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 168 */	NdrFcLong( 0x5 ),	/* 5 */
/* 172 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 174 */	NdrFcLong( 0xb ),	/* 11 */
/* 178 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 180 */	NdrFcLong( 0xa ),	/* 10 */
/* 184 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 186 */	NdrFcLong( 0x6 ),	/* 6 */
/* 190 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (422) */
/* 192 */	NdrFcLong( 0x7 ),	/* 7 */
/* 196 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 198 */	NdrFcLong( 0x8 ),	/* 8 */
/* 202 */	NdrFcShort( 0xff98 ),	/* Offset= -104 (98) */
/* 204 */	NdrFcLong( 0xd ),	/* 13 */
/* 208 */	NdrFcShort( 0xdc ),	/* Offset= 220 (428) */
/* 210 */	NdrFcLong( 0x9 ),	/* 9 */
/* 214 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (446) */
/* 216 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 220 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (464) */
/* 222 */	NdrFcLong( 0x24 ),	/* 36 */
/* 226 */	NdrFcShort( 0x31c ),	/* Offset= 796 (1022) */
/* 228 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 232 */	NdrFcShort( 0x316 ),	/* Offset= 790 (1022) */
/* 234 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 238 */	NdrFcShort( 0x314 ),	/* Offset= 788 (1026) */
/* 240 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 244 */	NdrFcShort( 0x312 ),	/* Offset= 786 (1030) */
/* 246 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 250 */	NdrFcShort( 0x310 ),	/* Offset= 784 (1034) */
/* 252 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 256 */	NdrFcShort( 0x30e ),	/* Offset= 782 (1038) */
/* 258 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 262 */	NdrFcShort( 0x30c ),	/* Offset= 780 (1042) */
/* 264 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 268 */	NdrFcShort( 0x30a ),	/* Offset= 778 (1046) */
/* 270 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 274 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1030) */
/* 276 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 280 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1034) */
/* 282 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 286 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (1050) */
/* 288 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 292 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1046) */
/* 294 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 298 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1054) */
/* 300 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 304 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1058) */
/* 306 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 310 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1062) */
/* 312 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 316 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1066) */
/* 318 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 322 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1070) */
/* 324 */	NdrFcLong( 0x10 ),	/* 16 */
/* 328 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 330 */	NdrFcLong( 0x12 ),	/* 18 */
/* 334 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 336 */	NdrFcLong( 0x13 ),	/* 19 */
/* 340 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 342 */	NdrFcLong( 0x15 ),	/* 21 */
/* 346 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 348 */	NdrFcLong( 0x16 ),	/* 22 */
/* 352 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 354 */	NdrFcLong( 0x17 ),	/* 23 */
/* 358 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 360 */	NdrFcLong( 0xe ),	/* 14 */
/* 364 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1078) */
/* 366 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 370 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1088) */
/* 372 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 376 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1092) */
/* 378 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 382 */	NdrFcShort( 0x288 ),	/* Offset= 648 (1030) */
/* 384 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 388 */	NdrFcShort( 0x286 ),	/* Offset= 646 (1034) */
/* 390 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 394 */	NdrFcShort( 0x284 ),	/* Offset= 644 (1038) */
/* 396 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 400 */	NdrFcShort( 0x27a ),	/* Offset= 634 (1034) */
/* 402 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 406 */	NdrFcShort( 0x274 ),	/* Offset= 628 (1034) */
/* 408 */	NdrFcLong( 0x0 ),	/* 0 */
/* 412 */	NdrFcShort( 0x0 ),	/* Offset= 0 (412) */
/* 414 */	NdrFcLong( 0x1 ),	/* 1 */
/* 418 */	NdrFcShort( 0x0 ),	/* Offset= 0 (418) */
/* 420 */	NdrFcShort( 0xffff ),	/* Offset= -1 (419) */
/* 422 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 424 */	NdrFcShort( 0x8 ),	/* 8 */
/* 426 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 428 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 430 */	NdrFcLong( 0x0 ),	/* 0 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 440 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 442 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 444 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 446 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 448 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 458 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 460 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 462 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 464 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 466 */	NdrFcShort( 0x2 ),	/* Offset= 2 (468) */
/* 468 */	
			0x12, 0x0,	/* FC_UP */
/* 470 */	NdrFcShort( 0x216 ),	/* Offset= 534 (1004) */
/* 472 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 474 */	NdrFcShort( 0x18 ),	/* 24 */
/* 476 */	NdrFcShort( 0xa ),	/* 10 */
/* 478 */	NdrFcLong( 0x8 ),	/* 8 */
/* 482 */	NdrFcShort( 0x5a ),	/* Offset= 90 (572) */
/* 484 */	NdrFcLong( 0xd ),	/* 13 */
/* 488 */	NdrFcShort( 0x7e ),	/* Offset= 126 (614) */
/* 490 */	NdrFcLong( 0x9 ),	/* 9 */
/* 494 */	NdrFcShort( 0x9e ),	/* Offset= 158 (652) */
/* 496 */	NdrFcLong( 0xc ),	/* 12 */
/* 500 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (700) */
/* 502 */	NdrFcLong( 0x24 ),	/* 36 */
/* 506 */	NdrFcShort( 0x124 ),	/* Offset= 292 (798) */
/* 508 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 512 */	NdrFcShort( 0x140 ),	/* Offset= 320 (832) */
/* 514 */	NdrFcLong( 0x10 ),	/* 16 */
/* 518 */	NdrFcShort( 0x15a ),	/* Offset= 346 (864) */
/* 520 */	NdrFcLong( 0x2 ),	/* 2 */
/* 524 */	NdrFcShort( 0x174 ),	/* Offset= 372 (896) */
/* 526 */	NdrFcLong( 0x3 ),	/* 3 */
/* 530 */	NdrFcShort( 0x18e ),	/* Offset= 398 (928) */
/* 532 */	NdrFcLong( 0x14 ),	/* 20 */
/* 536 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (960) */
/* 538 */	NdrFcShort( 0xffff ),	/* Offset= -1 (537) */
/* 540 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 542 */	NdrFcShort( 0x4 ),	/* 4 */
/* 544 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 550 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 552 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 554 */	NdrFcShort( 0x4 ),	/* 4 */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x1 ),	/* 1 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 564 */	0x12, 0x0,	/* FC_UP */
/* 566 */	NdrFcShort( 0xfe18 ),	/* Offset= -488 (78) */
/* 568 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 570 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 572 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 576 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 578 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 580 */	NdrFcShort( 0x4 ),	/* 4 */
/* 582 */	NdrFcShort( 0x4 ),	/* 4 */
/* 584 */	0x11, 0x0,	/* FC_RP */
/* 586 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (540) */
/* 588 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 590 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 592 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 596 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 602 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 606 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 608 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 610 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (428) */
/* 612 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 614 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 616 */	NdrFcShort( 0x8 ),	/* 8 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x6 ),	/* Offset= 6 (626) */
/* 622 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 624 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 626 */	
			0x11, 0x0,	/* FC_RP */
/* 628 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (592) */
/* 630 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 634 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 640 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 644 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 646 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 648 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (446) */
/* 650 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 652 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 658 */	NdrFcShort( 0x6 ),	/* Offset= 6 (664) */
/* 660 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 662 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 664 */	
			0x11, 0x0,	/* FC_RP */
/* 666 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (630) */
/* 668 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 670 */	NdrFcShort( 0x4 ),	/* 4 */
/* 672 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 678 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 680 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 682 */	NdrFcShort( 0x4 ),	/* 4 */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x1 ),	/* 1 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	0x12, 0x0,	/* FC_UP */
/* 694 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1096) */
/* 696 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 698 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 700 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x6 ),	/* Offset= 6 (712) */
/* 708 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 710 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 712 */	
			0x11, 0x0,	/* FC_RP */
/* 714 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (668) */
/* 716 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 718 */	NdrFcLong( 0x2f ),	/* 47 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 728 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 730 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 732 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 734 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 736 */	NdrFcShort( 0x1 ),	/* 1 */
/* 738 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 740 */	NdrFcShort( 0x4 ),	/* 4 */
/* 742 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 744 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 746 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 748 */	NdrFcShort( 0x10 ),	/* 16 */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0xa ),	/* Offset= 10 (762) */
/* 754 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 756 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 758 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (716) */
/* 760 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 762 */	
			0x12, 0x0,	/* FC_UP */
/* 764 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (734) */
/* 766 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 768 */	NdrFcShort( 0x4 ),	/* 4 */
/* 770 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 774 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 776 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 778 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 780 */	NdrFcShort( 0x4 ),	/* 4 */
/* 782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 784 */	NdrFcShort( 0x1 ),	/* 1 */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	0x12, 0x0,	/* FC_UP */
/* 792 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (746) */
/* 794 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 796 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 798 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 800 */	NdrFcShort( 0x8 ),	/* 8 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x6 ),	/* Offset= 6 (810) */
/* 806 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 808 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 810 */	
			0x11, 0x0,	/* FC_RP */
/* 812 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (766) */
/* 814 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 818 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 820 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 822 */	NdrFcShort( 0x10 ),	/* 16 */
/* 824 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 826 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 828 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (814) */
			0x5b,		/* FC_END */
/* 832 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x18 ),	/* 24 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0xa ),	/* Offset= 10 (848) */
/* 840 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 842 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 844 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (820) */
/* 846 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 848 */	
			0x11, 0x0,	/* FC_RP */
/* 850 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (592) */
/* 852 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 854 */	NdrFcShort( 0x1 ),	/* 1 */
/* 856 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 860 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 862 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 864 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 868 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 870 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 872 */	NdrFcShort( 0x4 ),	/* 4 */
/* 874 */	NdrFcShort( 0x4 ),	/* 4 */
/* 876 */	0x12, 0x0,	/* FC_UP */
/* 878 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (852) */
/* 880 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 882 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 884 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 886 */	NdrFcShort( 0x2 ),	/* 2 */
/* 888 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 894 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 896 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x8 ),	/* 8 */
/* 900 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 902 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 904 */	NdrFcShort( 0x4 ),	/* 4 */
/* 906 */	NdrFcShort( 0x4 ),	/* 4 */
/* 908 */	0x12, 0x0,	/* FC_UP */
/* 910 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (884) */
/* 912 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 914 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 916 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 918 */	NdrFcShort( 0x4 ),	/* 4 */
/* 920 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 926 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 928 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 932 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 934 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 936 */	NdrFcShort( 0x4 ),	/* 4 */
/* 938 */	NdrFcShort( 0x4 ),	/* 4 */
/* 940 */	0x12, 0x0,	/* FC_UP */
/* 942 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (916) */
/* 944 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 946 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 948 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 950 */	NdrFcShort( 0x8 ),	/* 8 */
/* 952 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 958 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 960 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 964 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 966 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 968 */	NdrFcShort( 0x4 ),	/* 4 */
/* 970 */	NdrFcShort( 0x4 ),	/* 4 */
/* 972 */	0x12, 0x0,	/* FC_UP */
/* 974 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (948) */
/* 976 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 978 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 980 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 984 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 986 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 988 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 994 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 996 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 998 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1000 */	NdrFcShort( 0xffec ),	/* Offset= -20 (980) */
/* 1002 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1004 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1006 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1008 */	NdrFcShort( 0xffec ),	/* Offset= -20 (988) */
/* 1010 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1010) */
/* 1012 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1014 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1016 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1018 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (472) */
/* 1020 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1022 */	
			0x12, 0x0,	/* FC_UP */
/* 1024 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (746) */
/* 1026 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1028 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1030 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1032 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1034 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1036 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1038 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1040 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1042 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1044 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1046 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1048 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1050 */	
			0x12, 0x0,	/* FC_UP */
/* 1052 */	NdrFcShort( 0xfd8a ),	/* Offset= -630 (422) */
/* 1054 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1056 */	NdrFcShort( 0xfc42 ),	/* Offset= -958 (98) */
/* 1058 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1060 */	NdrFcShort( 0xfd88 ),	/* Offset= -632 (428) */
/* 1062 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1064 */	NdrFcShort( 0xfd96 ),	/* Offset= -618 (446) */
/* 1066 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1068 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (464) */
/* 1070 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1072 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1074) */
/* 1074 */	
			0x12, 0x0,	/* FC_UP */
/* 1076 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1096) */
/* 1078 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1080 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1082 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1084 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1086 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1088 */	
			0x12, 0x0,	/* FC_UP */
/* 1090 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1078) */
/* 1092 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1094 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1096 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1098 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1102) */
/* 1104 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1106 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1108 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1110 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1112 */	NdrFcShort( 0xfc24 ),	/* Offset= -988 (124) */
/* 1114 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1116 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1118 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1120 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1124 */	NdrFcShort( 0xfc14 ),	/* Offset= -1004 (120) */
/* 1126 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1128 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1130) */
/* 1130 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1132 */	NdrFcLong( 0x8fd93c3c ),	/* -1881588676 */
/* 1136 */	NdrFcShort( 0x28fd ),	/* 10493 */
/* 1138 */	NdrFcShort( 0x43e0 ),	/* 17376 */
/* 1140 */	0x88,		/* 136 */
			0xb5,		/* 181 */
/* 1142 */	0x61,		/* 97 */
			0xd1,		/* 209 */
/* 1144 */	0xac,		/* 172 */
			0x4c,		/* 76 */
/* 1146 */	0x42,		/* 66 */
			0xf0,		/* 240 */
/* 1148 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1150 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1152) */
/* 1152 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1154 */	NdrFcLong( 0x701090ab ),	/* 1880133803 */
/* 1158 */	NdrFcShort( 0x4965 ),	/* 18789 */
/* 1160 */	NdrFcShort( 0x46f3 ),	/* 18163 */
/* 1162 */	0xae,		/* 174 */
			0xa5,		/* 165 */
/* 1164 */	0x36,		/* 54 */
			0xc3,		/* 195 */
/* 1166 */	0xf,		/* 15 */
			0xbd,		/* 189 */
/* 1168 */	0xc5,		/* 197 */
			0x7a,		/* 122 */
/* 1170 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1172 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1174) */
/* 1174 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1176 */	NdrFcLong( 0x279ee392 ),	/* 664724370 */
/* 1180 */	NdrFcShort( 0x286f ),	/* 10351 */
/* 1182 */	NdrFcShort( 0x4423 ),	/* 17443 */
/* 1184 */	0x81,		/* 129 */
			0xb9,		/* 185 */
/* 1186 */	0x4d,		/* 77 */
			0xf9,		/* 249 */
/* 1188 */	0xe2,		/* 226 */
			0xf4,		/* 244 */
/* 1190 */	0x30,		/* 48 */
			0x9e,		/* 158 */
/* 1192 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1194 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1196) */
/* 1196 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1198 */	NdrFcLong( 0x982d1b17 ),	/* -1741874409 */
/* 1202 */	NdrFcShort( 0xc848 ),	/* -14264 */
/* 1204 */	NdrFcShort( 0x4701 ),	/* 18177 */
/* 1206 */	0xa1,		/* 161 */
			0x12,		/* 18 */
/* 1208 */	0x7f,		/* 127 */
			0x3,		/* 3 */
/* 1210 */	0xe4,		/* 228 */
			0xd5,		/* 213 */
/* 1212 */	0xc,		/* 12 */
			0x70,		/* 112 */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_notecore_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ICoreNotify, ver. 0.0,
   GUID={0x956003E7,0x3C5A,0x42EB,{0xBA,0x19,0xD8,0xEE,0xF5,0x4D,0xAB,0xFF}} */

#pragma code_seg(".orpc")
static const unsigned short ICoreNotify_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ICoreNotify_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &ICoreNotify_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICoreNotify_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &ICoreNotify_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _ICoreNotifyProxyVtbl = 
{
    &ICoreNotify_ProxyInfo,
    &IID_ICoreNotify,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* ICoreNotify::onCoreNotify */
};

const CInterfaceStubVtbl _ICoreNotifyStubVtbl =
{
    &IID_ICoreNotify,
    &ICoreNotify_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INoteCoreObj, ver. 0.0,
   GUID={0xA13F359A,0x007B,0x472A,{0xAC,0x14,0x97,0xFE,0x79,0xC4,0x04,0xFE}} */

#pragma code_seg(".orpc")
static const unsigned short INoteCoreObj_FormatStringOffsetTable[] =
    {
    42
    };

static const MIDL_STUBLESS_PROXY_INFO INoteCoreObj_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INoteCoreObj_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INoteCoreObj_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INoteCoreObj_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _INoteCoreObjProxyVtbl = 
{
    &INoteCoreObj_ProxyInfo,
    &IID_INoteCoreObj,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */
};

const CInterfaceStubVtbl _INoteCoreObjStubVtbl =
{
    &IID_INoteCoreObj,
    &INoteCoreObj_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INote, ver. 0.0,
   GUID={0x8FD93C3C,0x28FD,0x43E0,{0x88,0xB5,0x61,0xD1,0xAC,0x4C,0x42,0xF0}} */

#pragma code_seg(".orpc")
static const unsigned short INote_FormatStringOffsetTable[] =
    {
    42,
    78,
    114,
    150,
    186,
    222,
    258,
    294,
    330,
    366,
    402,
    438,
    474,
    510,
    546,
    582,
    618
    };

static const MIDL_STUBLESS_PROXY_INFO INote_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INote_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INote_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INote_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(20) _INoteProxyVtbl = 
{
    &INote_ProxyInfo,
    &IID_INote,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INote::GetId */ ,
    (void *) (INT_PTR) -1 /* INote::SetId */ ,
    (void *) (INT_PTR) -1 /* INote::GetBookId */ ,
    (void *) (INT_PTR) -1 /* INote::SetBookId */ ,
    (void *) (INT_PTR) -1 /* INote::GetTitle */ ,
    (void *) (INT_PTR) -1 /* INote::SetTitle */ ,
    (void *) (INT_PTR) -1 /* INote::GetType */ ,
    (void *) (INT_PTR) -1 /* INote::SetType */ ,
    (void *) (INT_PTR) -1 /* INote::GetContent */ ,
    (void *) (INT_PTR) -1 /* INote::SetContent */ ,
    (void *) (INT_PTR) -1 /* INote::GetAbbreText */ ,
    (void *) (INT_PTR) -1 /* INote::SetPlainText */ ,
    (void *) (INT_PTR) -1 /* INote::GetCreateTime */ ,
    (void *) (INT_PTR) -1 /* INote::SetCreateTime */ ,
    (void *) (INT_PTR) -1 /* INote::GetModifyTime */ ,
    (void *) (INT_PTR) -1 /* INote::SetModifyTime */
};

const CInterfaceStubVtbl _INoteStubVtbl =
{
    &IID_INote,
    &INote_ServerInfo,
    20,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INoteCollection, ver. 0.0,
   GUID={0x6FC8017C,0xD8CD,0x41D0,{0x8C,0x27,0x4C,0x9F,0xB7,0xCC,0x4C,0xA5}} */

#pragma code_seg(".orpc")
static const unsigned short INoteCollection_FormatStringOffsetTable[] =
    {
    42,
    78,
    654,
    690,
    732,
    768,
    804
    };

static const MIDL_STUBLESS_PROXY_INFO INoteCollection_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INoteCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INoteCollection_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INoteCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _INoteCollectionProxyVtbl = 
{
    &INoteCollection_ProxyInfo,
    &IID_INoteCollection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetName */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetCount */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Item */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::AddNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::RemoveNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Clear */
};

const CInterfaceStubVtbl _INoteCollectionStubVtbl =
{
    &IID_INoteCollection,
    &INoteCollection_ServerInfo,
    10,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INotebook, ver. 0.0,
   GUID={0x701090AB,0x4965,0x46F3,{0xAE,0xA5,0x36,0xC3,0x0F,0xBD,0xC5,0x7A}} */

#pragma code_seg(".orpc")
static const unsigned short INotebook_FormatStringOffsetTable[] =
    {
    42,
    78,
    654,
    690,
    732,
    768,
    804,
    834,
    870,
    906,
    942,
    978,
    1014,
    1050,
    1086
    };

static const MIDL_STUBLESS_PROXY_INFO INotebook_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INotebook_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INotebook_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INotebook_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) _INotebookProxyVtbl = 
{
    &INotebook_ProxyInfo,
    &IID_INotebook,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetName */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetCount */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Item */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::AddNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::RemoveNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Clear */ ,
    (void *) (INT_PTR) -1 /* INotebook::GetId */ ,
    (void *) (INT_PTR) -1 /* INotebook::SetId */ ,
    (void *) (INT_PTR) -1 /* INotebook::SetName */ ,
    (void *) (INT_PTR) -1 /* INotebook::GetCreateTime */ ,
    (void *) (INT_PTR) -1 /* INotebook::SetCreateTime */ ,
    (void *) (INT_PTR) -1 /* INotebook::GetModifyTime */ ,
    (void *) (INT_PTR) -1 /* INotebook::SetModifyTime */ ,
    (void *) (INT_PTR) -1 /* INotebook::GetNoteIdx */
};

const CInterfaceStubVtbl _INotebookStubVtbl =
{
    &IID_INotebook,
    &INotebook_ServerInfo,
    18,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INotebooks, ver. 0.0,
   GUID={0x279EE392,0x286F,0x4423,{0x81,0xB9,0x4D,0xF9,0xE2,0xF4,0x30,0x9E}} */

#pragma code_seg(".orpc")
static const unsigned short INotebooks_FormatStringOffsetTable[] =
    {
    42,
    1128,
    1164,
    1206,
    1242
    };

static const MIDL_STUBLESS_PROXY_INFO INotebooks_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INotebooks_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INotebooks_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INotebooks_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) _INotebooksProxyVtbl = 
{
    &INotebooks_ProxyInfo,
    &IID_INotebooks,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INotebooks::GetCount */ ,
    (void *) (INT_PTR) -1 /* INotebooks::Item */ ,
    (void *) (INT_PTR) -1 /* INotebooks::AddNotebook */ ,
    (void *) (INT_PTR) -1 /* INotebooks::DeleteNotebook */
};

const CInterfaceStubVtbl _INotebooksStubVtbl =
{
    &IID_INotebooks,
    &INotebooks_ServerInfo,
    8,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: INoteApplication, ver. 0.0,
   GUID={0x9ACA0D9D,0x3CFF,0x4644,{0x92,0xE9,0x86,0x3B,0x5F,0xBB,0x3D,0x02}} */

#pragma code_seg(".orpc")
static const unsigned short INoteApplication_FormatStringOffsetTable[] =
    {
    42,
    1278,
    1314,
    1350,
    1386,
    222,
    258
    };

static const MIDL_STUBLESS_PROXY_INFO INoteApplication_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &INoteApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO INoteApplication_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &INoteApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _INoteApplicationProxyVtbl = 
{
    &INoteApplication_ProxyInfo,
    &IID_INoteApplication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::GetNotebooks */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::SetNotebooks */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::GetTrash */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::SetTrash */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::GetUserId */ ,
    (void *) (INT_PTR) -1 /* INoteApplication::SetUserId */
};

const CInterfaceStubVtbl _INoteApplicationStubVtbl =
{
    &IID_INoteApplication,
    &INoteApplication_ServerInfo,
    10,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: ITrash, ver. 0.0,
   GUID={0x982D1B17,0xC848,0x4701,{0xA1,0x12,0x7F,0x03,0xE4,0xD5,0x0C,0x70}} */

#pragma code_seg(".orpc")
static const unsigned short ITrash_FormatStringOffsetTable[] =
    {
    42,
    78,
    654,
    690,
    732,
    768,
    804,
    1422
    };

static const MIDL_STUBLESS_PROXY_INFO ITrash_ProxyInfo =
    {
    &Object_StubDesc,
    notecore__MIDL_ProcFormatString.Format,
    &ITrash_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ITrash_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    notecore__MIDL_ProcFormatString.Format,
    &ITrash_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) _ITrashProxyVtbl = 
{
    &ITrash_ProxyInfo,
    &IID_ITrash,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* INoteCoreObj::addWatcher */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetName */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::GetCount */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Item */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::AddNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::RemoveNote */ ,
    (void *) (INT_PTR) -1 /* INoteCollection::Clear */ ,
    (void *) (INT_PTR) -1 /* ITrash::DeleteNote */
};

const CInterfaceStubVtbl _ITrashStubVtbl =
{
    &IID_ITrash,
    &ITrash_ServerInfo,
    11,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    notecore__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _notecore_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITrashProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INoteProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INoteCollectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INotebooksProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INoteCoreObjProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INoteApplicationProxyVtbl,
    ( CInterfaceProxyVtbl *) &_INotebookProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ICoreNotifyProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _notecore_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITrashStubVtbl,
    ( CInterfaceStubVtbl *) &_INoteStubVtbl,
    ( CInterfaceStubVtbl *) &_INoteCollectionStubVtbl,
    ( CInterfaceStubVtbl *) &_INotebooksStubVtbl,
    ( CInterfaceStubVtbl *) &_INoteCoreObjStubVtbl,
    ( CInterfaceStubVtbl *) &_INoteApplicationStubVtbl,
    ( CInterfaceStubVtbl *) &_INotebookStubVtbl,
    ( CInterfaceStubVtbl *) &_ICoreNotifyStubVtbl,
    0
};

PCInterfaceName const _notecore_InterfaceNamesList[] = 
{
    "ITrash",
    "INote",
    "INoteCollection",
    "INotebooks",
    "INoteCoreObj",
    "INoteApplication",
    "INotebook",
    "ICoreNotify",
    0
};


#define _notecore_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _notecore, pIID, n)

int __stdcall _notecore_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _notecore, 8, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _notecore, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _notecore, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _notecore, 8, *pIndex )
    
}

const ExtendedProxyFileInfo notecore_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _notecore_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _notecore_StubVtblList,
    (const PCInterfaceName * ) & _notecore_InterfaceNamesList,
    0, /* no delegation */
    & _notecore_IID_Lookup, 
    8,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

