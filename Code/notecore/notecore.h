

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __notecore_h__
#define __notecore_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICoreNotify_FWD_DEFINED__
#define __ICoreNotify_FWD_DEFINED__
typedef interface ICoreNotify ICoreNotify;

#endif 	/* __ICoreNotify_FWD_DEFINED__ */


#ifndef __INoteCoreObj_FWD_DEFINED__
#define __INoteCoreObj_FWD_DEFINED__
typedef interface INoteCoreObj INoteCoreObj;

#endif 	/* __INoteCoreObj_FWD_DEFINED__ */


#ifndef __INote_FWD_DEFINED__
#define __INote_FWD_DEFINED__
typedef interface INote INote;

#endif 	/* __INote_FWD_DEFINED__ */


#ifndef __INoteCollection_FWD_DEFINED__
#define __INoteCollection_FWD_DEFINED__
typedef interface INoteCollection INoteCollection;

#endif 	/* __INoteCollection_FWD_DEFINED__ */


#ifndef __INotebook_FWD_DEFINED__
#define __INotebook_FWD_DEFINED__
typedef interface INotebook INotebook;

#endif 	/* __INotebook_FWD_DEFINED__ */


#ifndef __INotebooks_FWD_DEFINED__
#define __INotebooks_FWD_DEFINED__
typedef interface INotebooks INotebooks;

#endif 	/* __INotebooks_FWD_DEFINED__ */


#ifndef __INoteApplication_FWD_DEFINED__
#define __INoteApplication_FWD_DEFINED__
typedef interface INoteApplication INoteApplication;

#endif 	/* __INoteApplication_FWD_DEFINED__ */


#ifndef __ITrashRecord_FWD_DEFINED__
#define __ITrashRecord_FWD_DEFINED__
typedef interface ITrashRecord ITrashRecord;

#endif 	/* __ITrashRecord_FWD_DEFINED__ */


#ifndef __ITrash_FWD_DEFINED__
#define __ITrash_FWD_DEFINED__
typedef interface ITrash ITrash;

#endif 	/* __ITrash_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_notecore_0000_0000 */
/* [local] */ 




typedef 
enum NOTE_TYPE
    {
        NORMAL_NOTE	= 0,
        UNKNOWN_TYPE	= ( NORMAL_NOTE + 1 ) 
    } 	NOTE_TYPE;

typedef 
enum NotifyOperator
    {
        Add	= 0,
        Delete	= ( Add + 1 ) ,
        Update	= ( Delete + 1 ) ,
        Other	= ( Update + 1 ) 
    } 	NotifyOperator;

typedef /* [public][public] */ struct __MIDL___MIDL_itf_notecore_0000_0000_0001
    {
    enum NotifyOperator ope;
    int idx;
    wchar_t *wid;
    INoteCoreObj *pObj;
    } 	NotifyArg;



extern RPC_IF_HANDLE __MIDL_itf_notecore_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_notecore_0000_0000_v0_0_s_ifspec;

#ifndef __ICoreNotify_INTERFACE_DEFINED__
#define __ICoreNotify_INTERFACE_DEFINED__

/* interface ICoreNotify */
/* [uuid][object] */ 


EXTERN_C const IID IID_ICoreNotify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("956003E7-3C5A-42EB-BA19-D8EEF54DABFF")
    ICoreNotify : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onCoreNotify( 
            INoteCoreObj *pCoreObj,
            NotifyArg arg) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreNotifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreNotify * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreNotify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreNotify * This);
        
        HRESULT ( STDMETHODCALLTYPE *onCoreNotify )( 
            ICoreNotify * This,
            INoteCoreObj *pCoreObj,
            NotifyArg arg);
        
        END_INTERFACE
    } ICoreNotifyVtbl;

    interface ICoreNotify
    {
        CONST_VTBL struct ICoreNotifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreNotify_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreNotify_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreNotify_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreNotify_onCoreNotify(This,pCoreObj,arg)	\
    ( (This)->lpVtbl -> onCoreNotify(This,pCoreObj,arg) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreNotify_INTERFACE_DEFINED__ */


#ifndef __INoteCoreObj_INTERFACE_DEFINED__
#define __INoteCoreObj_INTERFACE_DEFINED__

/* interface INoteCoreObj */
/* [uuid][object] */ 


EXTERN_C const IID IID_INoteCoreObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A13F359A-007B-472A-AC14-97FE79C404FE")
    INoteCoreObj : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE addWatcher( 
            ICoreNotify *pNotify) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INoteCoreObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INoteCoreObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INoteCoreObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INoteCoreObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INoteCoreObj * This,
            ICoreNotify *pNotify);
        
        END_INTERFACE
    } INoteCoreObjVtbl;

    interface INoteCoreObj
    {
        CONST_VTBL struct INoteCoreObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INoteCoreObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INoteCoreObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INoteCoreObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INoteCoreObj_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INoteCoreObj_INTERFACE_DEFINED__ */


#ifndef __INote_INTERFACE_DEFINED__
#define __INote_INTERFACE_DEFINED__

/* interface INote */
/* [uuid][object] */ 


EXTERN_C const IID IID_INote;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8FD93C3C-28FD-43E0-88B5-61D1AC4C42F0")
    INote : public INoteCoreObj
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetId( 
            /* [retval][out] */ BSTR *pbstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetId( 
            /* [in] */ BSTR bstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBookId( 
            /* [out] */ BSTR *pBookId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBookId( 
            /* [in] */ BSTR bookId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTitle( 
            /* [out] */ BSTR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTitle( 
            /* [in] */ BSTR title) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetType( 
            /* [out] */ NOTE_TYPE *pType) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetType( 
            /* [in] */ NOTE_TYPE type) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContent( 
            /* [out] */ BSTR *pbstrContent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetContent( 
            /* [in] */ BSTR content) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetAbbreText( 
            /* [out] */ BSTR *pbstrAbbre) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetPlainText( 
            /* [in] */ BSTR content) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCreateTime( 
            /* [out] */ long *pTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCreateTime( 
            /* [in] */ long time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModifyTime( 
            /* [out] */ long *pTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetModifyTime( 
            /* [in] */ long time) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INoteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INote * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INote * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INote * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INote * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            INote * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *SetId )( 
            INote * This,
            /* [in] */ BSTR bstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetBookId )( 
            INote * This,
            /* [out] */ BSTR *pBookId);
        
        HRESULT ( STDMETHODCALLTYPE *SetBookId )( 
            INote * This,
            /* [in] */ BSTR bookId);
        
        HRESULT ( STDMETHODCALLTYPE *GetTitle )( 
            INote * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *SetTitle )( 
            INote * This,
            /* [in] */ BSTR title);
        
        HRESULT ( STDMETHODCALLTYPE *GetType )( 
            INote * This,
            /* [out] */ NOTE_TYPE *pType);
        
        HRESULT ( STDMETHODCALLTYPE *SetType )( 
            INote * This,
            /* [in] */ NOTE_TYPE type);
        
        HRESULT ( STDMETHODCALLTYPE *GetContent )( 
            INote * This,
            /* [out] */ BSTR *pbstrContent);
        
        HRESULT ( STDMETHODCALLTYPE *SetContent )( 
            INote * This,
            /* [in] */ BSTR content);
        
        HRESULT ( STDMETHODCALLTYPE *GetAbbreText )( 
            INote * This,
            /* [out] */ BSTR *pbstrAbbre);
        
        HRESULT ( STDMETHODCALLTYPE *SetPlainText )( 
            INote * This,
            /* [in] */ BSTR content);
        
        HRESULT ( STDMETHODCALLTYPE *GetCreateTime )( 
            INote * This,
            /* [out] */ long *pTime);
        
        HRESULT ( STDMETHODCALLTYPE *SetCreateTime )( 
            INote * This,
            /* [in] */ long time);
        
        HRESULT ( STDMETHODCALLTYPE *GetModifyTime )( 
            INote * This,
            /* [out] */ long *pTime);
        
        HRESULT ( STDMETHODCALLTYPE *SetModifyTime )( 
            INote * This,
            /* [in] */ long time);
        
        END_INTERFACE
    } INoteVtbl;

    interface INote
    {
        CONST_VTBL struct INoteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INote_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INote_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INote_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INote_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define INote_GetId(This,pbstrId)	\
    ( (This)->lpVtbl -> GetId(This,pbstrId) ) 

#define INote_SetId(This,bstrId)	\
    ( (This)->lpVtbl -> SetId(This,bstrId) ) 

#define INote_GetBookId(This,pBookId)	\
    ( (This)->lpVtbl -> GetBookId(This,pBookId) ) 

#define INote_SetBookId(This,bookId)	\
    ( (This)->lpVtbl -> SetBookId(This,bookId) ) 

#define INote_GetTitle(This,pbstrName)	\
    ( (This)->lpVtbl -> GetTitle(This,pbstrName) ) 

#define INote_SetTitle(This,title)	\
    ( (This)->lpVtbl -> SetTitle(This,title) ) 

#define INote_GetType(This,pType)	\
    ( (This)->lpVtbl -> GetType(This,pType) ) 

#define INote_SetType(This,type)	\
    ( (This)->lpVtbl -> SetType(This,type) ) 

#define INote_GetContent(This,pbstrContent)	\
    ( (This)->lpVtbl -> GetContent(This,pbstrContent) ) 

#define INote_SetContent(This,content)	\
    ( (This)->lpVtbl -> SetContent(This,content) ) 

#define INote_GetAbbreText(This,pbstrAbbre)	\
    ( (This)->lpVtbl -> GetAbbreText(This,pbstrAbbre) ) 

#define INote_SetPlainText(This,content)	\
    ( (This)->lpVtbl -> SetPlainText(This,content) ) 

#define INote_GetCreateTime(This,pTime)	\
    ( (This)->lpVtbl -> GetCreateTime(This,pTime) ) 

#define INote_SetCreateTime(This,time)	\
    ( (This)->lpVtbl -> SetCreateTime(This,time) ) 

#define INote_GetModifyTime(This,pTime)	\
    ( (This)->lpVtbl -> GetModifyTime(This,pTime) ) 

#define INote_SetModifyTime(This,time)	\
    ( (This)->lpVtbl -> SetModifyTime(This,time) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INote_INTERFACE_DEFINED__ */


#ifndef __INoteCollection_INTERFACE_DEFINED__
#define __INoteCollection_INTERFACE_DEFINED__

/* interface INoteCollection */
/* [uuid][object] */ 


EXTERN_C const IID IID_INoteCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6FC8017C-D8CD-41D0-8C27-4C9FB7CC4CA5")
    INoteCollection : public INoteCoreObj
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [out] */ INote **ppNote) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddNote( 
            /* [in] */ INote *pNote) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveNote( 
            /* [in] */ INote *pNote) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INoteCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INoteCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INoteCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INoteCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INoteCollection * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            INoteCollection * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            INoteCollection * This,
            /* [out] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *Item )( 
            INoteCollection * This,
            /* [in] */ VARIANT index,
            /* [out] */ INote **ppNote);
        
        HRESULT ( STDMETHODCALLTYPE *AddNote )( 
            INoteCollection * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveNote )( 
            INoteCollection * This,
            /* [in] */ INote *pNote);
        
        END_INTERFACE
    } INoteCollectionVtbl;

    interface INoteCollection
    {
        CONST_VTBL struct INoteCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INoteCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INoteCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INoteCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INoteCollection_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define INoteCollection_GetName(This,pbstrName)	\
    ( (This)->lpVtbl -> GetName(This,pbstrName) ) 

#define INoteCollection_GetCount(This,pCount)	\
    ( (This)->lpVtbl -> GetCount(This,pCount) ) 

#define INoteCollection_Item(This,index,ppNote)	\
    ( (This)->lpVtbl -> Item(This,index,ppNote) ) 

#define INoteCollection_AddNote(This,pNote)	\
    ( (This)->lpVtbl -> AddNote(This,pNote) ) 

#define INoteCollection_RemoveNote(This,pNote)	\
    ( (This)->lpVtbl -> RemoveNote(This,pNote) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INoteCollection_INTERFACE_DEFINED__ */


#ifndef __INotebook_INTERFACE_DEFINED__
#define __INotebook_INTERFACE_DEFINED__

/* interface INotebook */
/* [uuid][object] */ 


EXTERN_C const IID IID_INotebook;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("701090AB-4965-46F3-AEA5-36C30FBDC57A")
    INotebook : public INoteCollection
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetId( 
            /* [retval][out] */ BSTR *pbstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetId( 
            /* [in] */ BSTR bstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCreateTime( 
            /* [out] */ long *pTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetCreateTime( 
            /* [in] */ long time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModifyTime( 
            /* [out] */ long *pTime) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetModifyTime( 
            /* [in] */ long time) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNoteIdx( 
            /* [in] */ INote *pNote,
            /* [out] */ int *pIndex) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INotebookVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INotebook * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INotebook * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INotebook * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INotebook * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            INotebook * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            INotebook * This,
            /* [out] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *Item )( 
            INotebook * This,
            /* [in] */ VARIANT index,
            /* [out] */ INote **ppNote);
        
        HRESULT ( STDMETHODCALLTYPE *AddNote )( 
            INotebook * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveNote )( 
            INotebook * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            INotebook * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *SetId )( 
            INotebook * This,
            /* [in] */ BSTR bstrId);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            INotebook * This,
            /* [in] */ BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetCreateTime )( 
            INotebook * This,
            /* [out] */ long *pTime);
        
        HRESULT ( STDMETHODCALLTYPE *SetCreateTime )( 
            INotebook * This,
            /* [in] */ long time);
        
        HRESULT ( STDMETHODCALLTYPE *GetModifyTime )( 
            INotebook * This,
            /* [out] */ long *pTime);
        
        HRESULT ( STDMETHODCALLTYPE *SetModifyTime )( 
            INotebook * This,
            /* [in] */ long time);
        
        HRESULT ( STDMETHODCALLTYPE *GetNoteIdx )( 
            INotebook * This,
            /* [in] */ INote *pNote,
            /* [out] */ int *pIndex);
        
        END_INTERFACE
    } INotebookVtbl;

    interface INotebook
    {
        CONST_VTBL struct INotebookVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INotebook_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INotebook_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INotebook_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INotebook_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define INotebook_GetName(This,pbstrName)	\
    ( (This)->lpVtbl -> GetName(This,pbstrName) ) 

#define INotebook_GetCount(This,pCount)	\
    ( (This)->lpVtbl -> GetCount(This,pCount) ) 

#define INotebook_Item(This,index,ppNote)	\
    ( (This)->lpVtbl -> Item(This,index,ppNote) ) 

#define INotebook_AddNote(This,pNote)	\
    ( (This)->lpVtbl -> AddNote(This,pNote) ) 

#define INotebook_RemoveNote(This,pNote)	\
    ( (This)->lpVtbl -> RemoveNote(This,pNote) ) 


#define INotebook_GetId(This,pbstrId)	\
    ( (This)->lpVtbl -> GetId(This,pbstrId) ) 

#define INotebook_SetId(This,bstrId)	\
    ( (This)->lpVtbl -> SetId(This,bstrId) ) 

#define INotebook_SetName(This,bstrName)	\
    ( (This)->lpVtbl -> SetName(This,bstrName) ) 

#define INotebook_GetCreateTime(This,pTime)	\
    ( (This)->lpVtbl -> GetCreateTime(This,pTime) ) 

#define INotebook_SetCreateTime(This,time)	\
    ( (This)->lpVtbl -> SetCreateTime(This,time) ) 

#define INotebook_GetModifyTime(This,pTime)	\
    ( (This)->lpVtbl -> GetModifyTime(This,pTime) ) 

#define INotebook_SetModifyTime(This,time)	\
    ( (This)->lpVtbl -> SetModifyTime(This,time) ) 

#define INotebook_GetNoteIdx(This,pNote,pIndex)	\
    ( (This)->lpVtbl -> GetNoteIdx(This,pNote,pIndex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INotebook_INTERFACE_DEFINED__ */


#ifndef __INotebooks_INTERFACE_DEFINED__
#define __INotebooks_INTERFACE_DEFINED__

/* interface INotebooks */
/* [uuid][object] */ 


EXTERN_C const IID IID_INotebooks;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("279EE392-286F-4423-81B9-4DF9E2F4309E")
    INotebooks : public INoteCoreObj
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ int *pCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Item( 
            /* [in] */ VARIANT index,
            /* [out] */ INotebook **ppNote) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddNotebook( 
            /* [in] */ INotebook *pNotebook) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DeleteNotebook( 
            /* [in] */ INotebook *pNotebook) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INotebooksVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INotebooks * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INotebooks * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INotebooks * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INotebooks * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            INotebooks * This,
            /* [out] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *Item )( 
            INotebooks * This,
            /* [in] */ VARIANT index,
            /* [out] */ INotebook **ppNote);
        
        HRESULT ( STDMETHODCALLTYPE *AddNotebook )( 
            INotebooks * This,
            /* [in] */ INotebook *pNotebook);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteNotebook )( 
            INotebooks * This,
            /* [in] */ INotebook *pNotebook);
        
        END_INTERFACE
    } INotebooksVtbl;

    interface INotebooks
    {
        CONST_VTBL struct INotebooksVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INotebooks_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INotebooks_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INotebooks_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INotebooks_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define INotebooks_GetCount(This,pCount)	\
    ( (This)->lpVtbl -> GetCount(This,pCount) ) 

#define INotebooks_Item(This,index,ppNote)	\
    ( (This)->lpVtbl -> Item(This,index,ppNote) ) 

#define INotebooks_AddNotebook(This,pNotebook)	\
    ( (This)->lpVtbl -> AddNotebook(This,pNotebook) ) 

#define INotebooks_DeleteNotebook(This,pNotebook)	\
    ( (This)->lpVtbl -> DeleteNotebook(This,pNotebook) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INotebooks_INTERFACE_DEFINED__ */


#ifndef __INoteApplication_INTERFACE_DEFINED__
#define __INoteApplication_INTERFACE_DEFINED__

/* interface INoteApplication */
/* [uuid][object] */ 


EXTERN_C const IID IID_INoteApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9ACA0D9D-3CFF-4644-92E9-863B5FBB3D02")
    INoteApplication : public INoteCoreObj
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetNotebooks( 
            /* [out] */ INotebooks **ppNotebooks) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNotebooks( 
            /* [in] */ INotebooks *pNotebooks) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetTrash( 
            /* [out] */ ITrash **ppTrash) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTrash( 
            /* [in] */ ITrash *pTrash) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetUserId( 
            /* [out] */ BSTR *pbstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetUserId( 
            /* [in] */ BSTR bstrId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct INoteApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INoteApplication * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INoteApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INoteApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            INoteApplication * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetNotebooks )( 
            INoteApplication * This,
            /* [out] */ INotebooks **ppNotebooks);
        
        HRESULT ( STDMETHODCALLTYPE *SetNotebooks )( 
            INoteApplication * This,
            /* [in] */ INotebooks *pNotebooks);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrash )( 
            INoteApplication * This,
            /* [out] */ ITrash **ppTrash);
        
        HRESULT ( STDMETHODCALLTYPE *SetTrash )( 
            INoteApplication * This,
            /* [in] */ ITrash *pTrash);
        
        HRESULT ( STDMETHODCALLTYPE *GetUserId )( 
            INoteApplication * This,
            /* [out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *SetUserId )( 
            INoteApplication * This,
            /* [in] */ BSTR bstrId);
        
        END_INTERFACE
    } INoteApplicationVtbl;

    interface INoteApplication
    {
        CONST_VTBL struct INoteApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INoteApplication_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INoteApplication_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INoteApplication_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INoteApplication_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define INoteApplication_GetNotebooks(This,ppNotebooks)	\
    ( (This)->lpVtbl -> GetNotebooks(This,ppNotebooks) ) 

#define INoteApplication_SetNotebooks(This,pNotebooks)	\
    ( (This)->lpVtbl -> SetNotebooks(This,pNotebooks) ) 

#define INoteApplication_GetTrash(This,ppTrash)	\
    ( (This)->lpVtbl -> GetTrash(This,ppTrash) ) 

#define INoteApplication_SetTrash(This,pTrash)	\
    ( (This)->lpVtbl -> SetTrash(This,pTrash) ) 

#define INoteApplication_GetUserId(This,pbstrId)	\
    ( (This)->lpVtbl -> GetUserId(This,pbstrId) ) 

#define INoteApplication_SetUserId(This,bstrId)	\
    ( (This)->lpVtbl -> SetUserId(This,bstrId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INoteApplication_INTERFACE_DEFINED__ */


#ifndef __ITrashRecord_INTERFACE_DEFINED__
#define __ITrashRecord_INTERFACE_DEFINED__

/* interface ITrashRecord */
/* [uuid][object] */ 


EXTERN_C const IID IID_ITrashRecord;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BEDCB647-62CD-4B76-A6ED-33DE73BC2E7A")
    ITrashRecord : public INoteCoreObj
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetId( 
            /* [out] */ BSTR *pbstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetId( 
            /* [in] */ BSTR bstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNote( 
            /* [out] */ INote **ppNote) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNote( 
            /* [in] */ INote *pNote) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNotebook( 
            /* [out] */ INotebook **ppNotebook) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNotebook( 
            /* [in] */ INotebook *pNotebook) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITrashRecordVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrashRecord * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrashRecord * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrashRecord * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            ITrashRecord * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            ITrashRecord * This,
            /* [out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *SetId )( 
            ITrashRecord * This,
            /* [in] */ BSTR bstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetNote )( 
            ITrashRecord * This,
            /* [out] */ INote **ppNote);
        
        HRESULT ( STDMETHODCALLTYPE *SetNote )( 
            ITrashRecord * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *GetNotebook )( 
            ITrashRecord * This,
            /* [out] */ INotebook **ppNotebook);
        
        HRESULT ( STDMETHODCALLTYPE *SetNotebook )( 
            ITrashRecord * This,
            /* [in] */ INotebook *pNotebook);
        
        END_INTERFACE
    } ITrashRecordVtbl;

    interface ITrashRecord
    {
        CONST_VTBL struct ITrashRecordVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrashRecord_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITrashRecord_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITrashRecord_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITrashRecord_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define ITrashRecord_GetId(This,pbstrId)	\
    ( (This)->lpVtbl -> GetId(This,pbstrId) ) 

#define ITrashRecord_SetId(This,bstrId)	\
    ( (This)->lpVtbl -> SetId(This,bstrId) ) 

#define ITrashRecord_GetNote(This,ppNote)	\
    ( (This)->lpVtbl -> GetNote(This,ppNote) ) 

#define ITrashRecord_SetNote(This,pNote)	\
    ( (This)->lpVtbl -> SetNote(This,pNote) ) 

#define ITrashRecord_GetNotebook(This,ppNotebook)	\
    ( (This)->lpVtbl -> GetNotebook(This,ppNotebook) ) 

#define ITrashRecord_SetNotebook(This,pNotebook)	\
    ( (This)->lpVtbl -> SetNotebook(This,pNotebook) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITrashRecord_INTERFACE_DEFINED__ */


#ifndef __ITrash_INTERFACE_DEFINED__
#define __ITrash_INTERFACE_DEFINED__

/* interface ITrash */
/* [uuid][object] */ 


EXTERN_C const IID IID_ITrash;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("982D1B17-C848-4701-A112-7F03E4D50C70")
    ITrash : public INoteCollection
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DeleteNote( 
            /* [in] */ INote *pNote) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITrashVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITrash * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITrash * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITrash * This);
        
        HRESULT ( STDMETHODCALLTYPE *addWatcher )( 
            ITrash * This,
            ICoreNotify *pNotify);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            ITrash * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            ITrash * This,
            /* [out] */ int *pCount);
        
        HRESULT ( STDMETHODCALLTYPE *Item )( 
            ITrash * This,
            /* [in] */ VARIANT index,
            /* [out] */ INote **ppNote);
        
        HRESULT ( STDMETHODCALLTYPE *AddNote )( 
            ITrash * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveNote )( 
            ITrash * This,
            /* [in] */ INote *pNote);
        
        HRESULT ( STDMETHODCALLTYPE *DeleteNote )( 
            ITrash * This,
            /* [in] */ INote *pNote);
        
        END_INTERFACE
    } ITrashVtbl;

    interface ITrash
    {
        CONST_VTBL struct ITrashVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITrash_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITrash_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITrash_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITrash_addWatcher(This,pNotify)	\
    ( (This)->lpVtbl -> addWatcher(This,pNotify) ) 


#define ITrash_GetName(This,pbstrName)	\
    ( (This)->lpVtbl -> GetName(This,pbstrName) ) 

#define ITrash_GetCount(This,pCount)	\
    ( (This)->lpVtbl -> GetCount(This,pCount) ) 

#define ITrash_Item(This,index,ppNote)	\
    ( (This)->lpVtbl -> Item(This,index,ppNote) ) 

#define ITrash_AddNote(This,pNote)	\
    ( (This)->lpVtbl -> AddNote(This,pNote) ) 

#define ITrash_RemoveNote(This,pNote)	\
    ( (This)->lpVtbl -> RemoveNote(This,pNote) ) 


#define ITrash_DeleteNote(This,pNote)	\
    ( (This)->lpVtbl -> DeleteNote(This,pNote) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITrash_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


