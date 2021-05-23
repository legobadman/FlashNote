

/* this ALWAYS GENERATED file contains the RPC client stubs */


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
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "notecore.h"

#define TYPE_FORMAT_STRING_SIZE   41                                
#define PROC_FORMAT_STRING_SIZE   47                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

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

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: __MIDL_itf_notecore_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: ICoreNotify, ver. 0.0,
   GUID={0x956003E7,0x3C5A,0x42EB,{0xBA,0x19,0xD8,0xEE,0xF5,0x4D,0xAB,0xFF}} */



static const RPC_CLIENT_INTERFACE ICoreNotify___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x956003E7,0x3C5A,0x42EB,{0xBA,0x19,0xD8,0xEE,0xF5,0x4D,0xAB,0xFF}},{0,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE ICoreNotify_v0_0_c_ifspec = (RPC_IF_HANDLE)& ICoreNotify___RpcClientInterface;

extern const MIDL_STUB_DESC ICoreNotify_StubDesc;

static RPC_BINDING_HANDLE ICoreNotify__MIDL_AutoBindHandle;


HRESULT onCoreNotify( 
    /* [in] */ handle_t IDL_handle,
    INoteCoreObj *pCoreObj,
    NotifyArg arg)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&ICoreNotify_StubDesc,
                  (PFORMAT_STRING) &notecore__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&IDL_handle);
    return ( HRESULT  )_RetVal.Simple;
    
}


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

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x8 ),	/* 8 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCoreObj */

/* 28 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter arg */

/* 34 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	NdrFcShort( 0x14 ),	/* Type Offset=20 */

	/* Return value */

/* 40 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 42 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 44 */	0x8,		/* FC_LONG */
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

			0x0
        }
    };

static const unsigned short ICoreNotify_FormatStringOffsetTable[] =
    {
    0
    };


static const MIDL_STUB_DESC ICoreNotify_StubDesc = 
    {
    (void *)& ICoreNotify___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &ICoreNotify__MIDL_AutoBindHandle,
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
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

