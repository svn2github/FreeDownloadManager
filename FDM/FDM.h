/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif 

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif 

#ifndef __Fdm_h__
#define __Fdm_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IWGUrlReceiver_FWD_DEFINED__
#define __IWGUrlReceiver_FWD_DEFINED__
typedef interface IWGUrlReceiver IWGUrlReceiver;
#endif 	  

#ifndef __IWGUrlListReceiver_FWD_DEFINED__
#define __IWGUrlListReceiver_FWD_DEFINED__
typedef interface IWGUrlListReceiver IWGUrlListReceiver;
#endif 	  

#ifndef __IFDM_FWD_DEFINED__
#define __IFDM_FWD_DEFINED__
typedef interface IFDM IFDM;
#endif 	  

#ifndef __IFDMDownload_FWD_DEFINED__
#define __IFDMDownload_FWD_DEFINED__
typedef interface IFDMDownload IFDMDownload;
#endif 	  

#ifndef __IFDMDownloadsStat_FWD_DEFINED__
#define __IFDMDownloadsStat_FWD_DEFINED__
typedef interface IFDMDownloadsStat IFDMDownloadsStat;
#endif 	  

#ifndef __IFdmTorrentFilesRcvr_FWD_DEFINED__
#define __IFdmTorrentFilesRcvr_FWD_DEFINED__
typedef interface IFdmTorrentFilesRcvr IFdmTorrentFilesRcvr;
#endif 	  

#ifndef __IFDMFlashVideoDownloads_FWD_DEFINED__
#define __IFDMFlashVideoDownloads_FWD_DEFINED__
typedef interface IFDMFlashVideoDownloads IFDMFlashVideoDownloads;
#endif 	  

#ifndef __WGUrlReceiver_FWD_DEFINED__
#define __WGUrlReceiver_FWD_DEFINED__

#ifdef __cplusplus
typedef class WGUrlReceiver WGUrlReceiver;
#else
typedef struct WGUrlReceiver WGUrlReceiver;
#endif 

#endif 	  

#ifndef __WGUrlListReceiver_FWD_DEFINED__
#define __WGUrlListReceiver_FWD_DEFINED__

#ifdef __cplusplus
typedef class WGUrlListReceiver WGUrlListReceiver;
#else
typedef struct WGUrlListReceiver WGUrlListReceiver;
#endif 

#endif 	  

#ifndef __FDM_FWD_DEFINED__
#define __FDM_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDM FDM;
#else
typedef struct FDM FDM;
#endif 

#endif 	  

#ifndef __FDMDownloadsStat_FWD_DEFINED__
#define __FDMDownloadsStat_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMDownloadsStat FDMDownloadsStat;
#else
typedef struct FDMDownloadsStat FDMDownloadsStat;
#endif 

#endif 	  

#ifndef __FDMDownload_FWD_DEFINED__
#define __FDMDownload_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMDownload FDMDownload;
#else
typedef struct FDMDownload FDMDownload;
#endif 

#endif 	  

#ifndef __FDMUploader_FWD_DEFINED__
#define __FDMUploader_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMUploader FDMUploader;
#else
typedef struct FDMUploader FDMUploader;
#endif 

#endif 	  

#ifndef __FDMUploadPackage_FWD_DEFINED__
#define __FDMUploadPackage_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMUploadPackage FDMUploadPackage;
#else
typedef struct FDMUploadPackage FDMUploadPackage;
#endif 

#endif 	  

#ifndef __FdmTorrentFilesRcvr_FWD_DEFINED__
#define __FdmTorrentFilesRcvr_FWD_DEFINED__

#ifdef __cplusplus
typedef class FdmTorrentFilesRcvr FdmTorrentFilesRcvr;
#else
typedef struct FdmTorrentFilesRcvr FdmTorrentFilesRcvr;
#endif 

#endif 	  

#ifndef __FDMFlashVideoDownloads_FWD_DEFINED__
#define __FDMFlashVideoDownloads_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMFlashVideoDownloads FDMFlashVideoDownloads;
#else
typedef struct FDMFlashVideoDownloads FDMFlashVideoDownloads;
#endif 

#endif 	    

#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IWGUrlReceiver_INTERFACE_DEFINED__
#define __IWGUrlReceiver_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IWGUrlReceiver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("454A4044-16EC-4D64-9069-C5B8832B7B55")
    IWGUrlReceiver : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_Url( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Url( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE ShowAddDownloadDialog( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE ShowAddPageDownloadDialog( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Comment( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Comment( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Referer( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Referer( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE AddDownload( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_CheckExtension( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_CheckExtension( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_IgnoreSize( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_IgnoreSize( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_UseSkipExtsList( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_UseSkipExtsList( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_UIState( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_ForceSilent( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_ForceSilent( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Cookies( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Cookies( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_PostData( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_PostData( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_ForceSilentEx( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_ForceSilentEx( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_ForceDownloadAutoStart( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_ForceDownloadAutoStart( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_DisableMaliciousChecking( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_DisableMaliciousChecking( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_DisableURLExistsCheck( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_DisableURLExistsCheck( 
             BOOL newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_FileName( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_FileName( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_FlashVideoDownload( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_FlashVideoDownload( 
             BOOL newVal) = 0;
        
    };
    
#else 	

    typedef struct IWGUrlReceiverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWGUrlReceiver __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWGUrlReceiver __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWGUrlReceiver __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWGUrlReceiver __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWGUrlReceiver __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWGUrlReceiver __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWGUrlReceiver __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Url )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Url )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowAddDownloadDialog )( 
            IWGUrlReceiver __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowAddPageDownloadDialog )( 
            IWGUrlReceiver __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Comment )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Comment )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Referer )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Referer )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddDownload )( 
            IWGUrlReceiver __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CheckExtension )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_CheckExtension )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IgnoreSize )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_IgnoreSize )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UseSkipExtsList )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UseSkipExtsList )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UIState )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ForceSilent )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ForceSilent )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Cookies )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Cookies )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PostData )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PostData )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ForceSilentEx )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ForceSilentEx )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ForceDownloadAutoStart )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ForceDownloadAutoStart )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DisableMaliciousChecking )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DisableMaliciousChecking )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DisableURLExistsCheck )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DisableURLExistsCheck )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FileName )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FileName )( 
            IWGUrlReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FlashVideoDownload )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FlashVideoDownload )( 
            IWGUrlReceiver __RPC_FAR * This,
             BOOL newVal);
        
        END_INTERFACE
    } IWGUrlReceiverVtbl;

    interface IWGUrlReceiver
    {
        CONST_VTBL struct IWGUrlReceiverVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IWGUrlReceiver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWGUrlReceiver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWGUrlReceiver_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IWGUrlReceiver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWGUrlReceiver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWGUrlReceiver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWGUrlReceiver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IWGUrlReceiver_get_Url(This,pVal)	\
    (This)->lpVtbl -> get_Url(This,pVal)

#define IWGUrlReceiver_put_Url(This,newVal)	\
    (This)->lpVtbl -> put_Url(This,newVal)

#define IWGUrlReceiver_ShowAddDownloadDialog(This)	\
    (This)->lpVtbl -> ShowAddDownloadDialog(This)

#define IWGUrlReceiver_ShowAddPageDownloadDialog(This)	\
    (This)->lpVtbl -> ShowAddPageDownloadDialog(This)

#define IWGUrlReceiver_get_Comment(This,pVal)	\
    (This)->lpVtbl -> get_Comment(This,pVal)

#define IWGUrlReceiver_put_Comment(This,newVal)	\
    (This)->lpVtbl -> put_Comment(This,newVal)

#define IWGUrlReceiver_get_Referer(This,pVal)	\
    (This)->lpVtbl -> get_Referer(This,pVal)

#define IWGUrlReceiver_put_Referer(This,newVal)	\
    (This)->lpVtbl -> put_Referer(This,newVal)

#define IWGUrlReceiver_AddDownload(This)	\
    (This)->lpVtbl -> AddDownload(This)

#define IWGUrlReceiver_get_CheckExtension(This,pVal)	\
    (This)->lpVtbl -> get_CheckExtension(This,pVal)

#define IWGUrlReceiver_put_CheckExtension(This,newVal)	\
    (This)->lpVtbl -> put_CheckExtension(This,newVal)

#define IWGUrlReceiver_get_IgnoreSize(This,pVal)	\
    (This)->lpVtbl -> get_IgnoreSize(This,pVal)

#define IWGUrlReceiver_put_IgnoreSize(This,newVal)	\
    (This)->lpVtbl -> put_IgnoreSize(This,newVal)

#define IWGUrlReceiver_get_UseSkipExtsList(This,pVal)	\
    (This)->lpVtbl -> get_UseSkipExtsList(This,pVal)

#define IWGUrlReceiver_put_UseSkipExtsList(This,newVal)	\
    (This)->lpVtbl -> put_UseSkipExtsList(This,newVal)

#define IWGUrlReceiver_get_UIState(This,pVal)	\
    (This)->lpVtbl -> get_UIState(This,pVal)

#define IWGUrlReceiver_get_ForceSilent(This,pVal)	\
    (This)->lpVtbl -> get_ForceSilent(This,pVal)

#define IWGUrlReceiver_put_ForceSilent(This,newVal)	\
    (This)->lpVtbl -> put_ForceSilent(This,newVal)

#define IWGUrlReceiver_get_Cookies(This,pVal)	\
    (This)->lpVtbl -> get_Cookies(This,pVal)

#define IWGUrlReceiver_put_Cookies(This,newVal)	\
    (This)->lpVtbl -> put_Cookies(This,newVal)

#define IWGUrlReceiver_get_PostData(This,pVal)	\
    (This)->lpVtbl -> get_PostData(This,pVal)

#define IWGUrlReceiver_put_PostData(This,newVal)	\
    (This)->lpVtbl -> put_PostData(This,newVal)

#define IWGUrlReceiver_get_ForceSilentEx(This,pVal)	\
    (This)->lpVtbl -> get_ForceSilentEx(This,pVal)

#define IWGUrlReceiver_put_ForceSilentEx(This,newVal)	\
    (This)->lpVtbl -> put_ForceSilentEx(This,newVal)

#define IWGUrlReceiver_get_ForceDownloadAutoStart(This,pVal)	\
    (This)->lpVtbl -> get_ForceDownloadAutoStart(This,pVal)

#define IWGUrlReceiver_put_ForceDownloadAutoStart(This,newVal)	\
    (This)->lpVtbl -> put_ForceDownloadAutoStart(This,newVal)

#define IWGUrlReceiver_get_DisableMaliciousChecking(This,pVal)	\
    (This)->lpVtbl -> get_DisableMaliciousChecking(This,pVal)

#define IWGUrlReceiver_put_DisableMaliciousChecking(This,newVal)	\
    (This)->lpVtbl -> put_DisableMaliciousChecking(This,newVal)

#define IWGUrlReceiver_get_DisableURLExistsCheck(This,pVal)	\
    (This)->lpVtbl -> get_DisableURLExistsCheck(This,pVal)

#define IWGUrlReceiver_put_DisableURLExistsCheck(This,newVal)	\
    (This)->lpVtbl -> put_DisableURLExistsCheck(This,newVal)

#define IWGUrlReceiver_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define IWGUrlReceiver_put_FileName(This,newVal)	\
    (This)->lpVtbl -> put_FileName(This,newVal)

#define IWGUrlReceiver_get_FlashVideoDownload(This,pVal)	\
    (This)->lpVtbl -> get_FlashVideoDownload(This,pVal)

#define IWGUrlReceiver_put_FlashVideoDownload(This,newVal)	\
    (This)->lpVtbl -> put_FlashVideoDownload(This,newVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_Url_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_Url_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_ShowAddDownloadDialog_Proxy( 
    IWGUrlReceiver __RPC_FAR * This);  

void __RPC_STUB IWGUrlReceiver_ShowAddDownloadDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_ShowAddPageDownloadDialog_Proxy( 
    IWGUrlReceiver __RPC_FAR * This);  

void __RPC_STUB IWGUrlReceiver_ShowAddPageDownloadDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_Comment_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_Comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_Comment_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_Comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_Referer_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_Referer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_Referer_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_Referer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_AddDownload_Proxy( 
    IWGUrlReceiver __RPC_FAR * This);  

void __RPC_STUB IWGUrlReceiver_AddDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_CheckExtension_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_CheckExtension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_CheckExtension_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_CheckExtension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_IgnoreSize_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_IgnoreSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_IgnoreSize_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_IgnoreSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_UseSkipExtsList_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_UseSkipExtsList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_UseSkipExtsList_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_UseSkipExtsList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_UIState_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_UIState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_ForceSilent_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_ForceSilent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_ForceSilent_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_ForceSilent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_Cookies_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_Cookies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_Cookies_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_Cookies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_PostData_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_PostData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_PostData_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_PostData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_ForceSilentEx_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_ForceSilentEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_ForceSilentEx_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_ForceSilentEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_ForceDownloadAutoStart_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_ForceDownloadAutoStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_ForceDownloadAutoStart_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_ForceDownloadAutoStart_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_DisableMaliciousChecking_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_DisableMaliciousChecking_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_DisableMaliciousChecking_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_DisableMaliciousChecking_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_DisableURLExistsCheck_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_DisableURLExistsCheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_DisableURLExistsCheck_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_DisableURLExistsCheck_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_FileName_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_FileName_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlReceiver_put_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_get_FlashVideoDownload_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlReceiver_get_FlashVideoDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlReceiver_put_FlashVideoDownload_Proxy( 
    IWGUrlReceiver __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IWGUrlReceiver_put_FlashVideoDownload_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IWGUrlListReceiver_INTERFACE_DEFINED__
#define __IWGUrlListReceiver_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IWGUrlListReceiver;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42E8D680-A18B-4CAA-ACE0-18EA05E4A056")
    IWGUrlListReceiver : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_Url( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Url( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Comment( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Comment( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Referer( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Referer( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE AddUrlToList( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE ShowAddUrlListDialog( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE GetUrlListFromDocumentSelection( 
             IDispatch __RPC_FAR *pDispatch) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Cookies( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Cookies( 
             BSTR newVal) = 0;
        
    };
    
#else 	

    typedef struct IWGUrlListReceiverVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWGUrlListReceiver __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWGUrlListReceiver __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWGUrlListReceiver __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWGUrlListReceiver __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWGUrlListReceiver __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWGUrlListReceiver __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWGUrlListReceiver __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Url )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Url )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Comment )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Comment )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Referer )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Referer )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddUrlToList )( 
            IWGUrlListReceiver __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowAddUrlListDialog )( 
            IWGUrlListReceiver __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUrlListFromDocumentSelection )( 
            IWGUrlListReceiver __RPC_FAR * This,
             IDispatch __RPC_FAR *pDispatch);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Cookies )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Cookies )( 
            IWGUrlListReceiver __RPC_FAR * This,
             BSTR newVal);
        
        END_INTERFACE
    } IWGUrlListReceiverVtbl;

    interface IWGUrlListReceiver
    {
        CONST_VTBL struct IWGUrlListReceiverVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IWGUrlListReceiver_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWGUrlListReceiver_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWGUrlListReceiver_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IWGUrlListReceiver_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWGUrlListReceiver_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWGUrlListReceiver_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWGUrlListReceiver_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IWGUrlListReceiver_get_Url(This,pVal)	\
    (This)->lpVtbl -> get_Url(This,pVal)

#define IWGUrlListReceiver_put_Url(This,newVal)	\
    (This)->lpVtbl -> put_Url(This,newVal)

#define IWGUrlListReceiver_get_Comment(This,pVal)	\
    (This)->lpVtbl -> get_Comment(This,pVal)

#define IWGUrlListReceiver_put_Comment(This,newVal)	\
    (This)->lpVtbl -> put_Comment(This,newVal)

#define IWGUrlListReceiver_get_Referer(This,pVal)	\
    (This)->lpVtbl -> get_Referer(This,pVal)

#define IWGUrlListReceiver_put_Referer(This,newVal)	\
    (This)->lpVtbl -> put_Referer(This,newVal)

#define IWGUrlListReceiver_AddUrlToList(This)	\
    (This)->lpVtbl -> AddUrlToList(This)

#define IWGUrlListReceiver_ShowAddUrlListDialog(This)	\
    (This)->lpVtbl -> ShowAddUrlListDialog(This)

#define IWGUrlListReceiver_GetUrlListFromDocumentSelection(This,pDispatch)	\
    (This)->lpVtbl -> GetUrlListFromDocumentSelection(This,pDispatch)

#define IWGUrlListReceiver_get_Cookies(This,pVal)	\
    (This)->lpVtbl -> get_Cookies(This,pVal)

#define IWGUrlListReceiver_put_Cookies(This,newVal)	\
    (This)->lpVtbl -> put_Cookies(This,newVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_get_Url_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlListReceiver_get_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_put_Url_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlListReceiver_put_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_get_Comment_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlListReceiver_get_Comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_put_Comment_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlListReceiver_put_Comment_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_get_Referer_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlListReceiver_get_Referer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_put_Referer_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlListReceiver_put_Referer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_AddUrlToList_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This);  

void __RPC_STUB IWGUrlListReceiver_AddUrlToList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_ShowAddUrlListDialog_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This);  

void __RPC_STUB IWGUrlListReceiver_ShowAddUrlListDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_GetUrlListFromDocumentSelection_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     IDispatch __RPC_FAR *pDispatch);  

void __RPC_STUB IWGUrlListReceiver_GetUrlListFromDocumentSelection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_get_Cookies_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IWGUrlListReceiver_get_Cookies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IWGUrlListReceiver_put_Cookies_Proxy( 
    IWGUrlListReceiver __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IWGUrlListReceiver_put_Cookies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFDM_INTERFACE_DEFINED__
#define __IFDM_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDM;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F8FA5B48-B7A2-4BC6-8389-9587643A4660")
    IFDM : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE Shutdown( 
             BOOL bAskUser) = 0;
        
    };
    
#else 	

    typedef struct IFDMVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDM __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDM __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDM __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDM __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDM __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDM __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDM __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Shutdown )( 
            IFDM __RPC_FAR * This,
             BOOL bAskUser);
        
        END_INTERFACE
    } IFDMVtbl;

    interface IFDM
    {
        CONST_VTBL struct IFDMVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDM_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDM_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDM_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDM_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDM_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDM_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDM_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDM_Shutdown(This,bAskUser)	\
    (This)->lpVtbl -> Shutdown(This,bAskUser)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDM_Shutdown_Proxy( 
    IFDM __RPC_FAR * This,
     BOOL bAskUser);  

void __RPC_STUB IFDM_Shutdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFDMDownload_INTERFACE_DEFINED__
#define __IFDMDownload_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMDownload;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8F2B3016-17D4-447A-B207-FFA8957A834A")
    IFDMDownload : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_Url( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Url( 
             BSTR newVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_DownloadText( 
             long nTextIndex,
             BSTR __RPC_FAR *pVal) = 0;
        
    };
    
#else 	

    typedef struct IFDMDownloadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMDownload __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMDownload __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMDownload __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMDownload __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMDownload __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMDownload __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMDownload __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Url )( 
            IFDMDownload __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Url )( 
            IFDMDownload __RPC_FAR * This,
             BSTR newVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DownloadText )( 
            IFDMDownload __RPC_FAR * This,
             long nTextIndex,
             BSTR __RPC_FAR *pVal);
        
        END_INTERFACE
    } IFDMDownloadVtbl;

    interface IFDMDownload
    {
        CONST_VTBL struct IFDMDownloadVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMDownload_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMDownload_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMDownload_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMDownload_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMDownload_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMDownload_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMDownload_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMDownload_get_Url(This,pVal)	\
    (This)->lpVtbl -> get_Url(This,pVal)

#define IFDMDownload_put_Url(This,newVal)	\
    (This)->lpVtbl -> put_Url(This,newVal)

#define IFDMDownload_get_DownloadText(This,nTextIndex,pVal)	\
    (This)->lpVtbl -> get_DownloadText(This,nTextIndex,pVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMDownload_get_Url_Proxy( 
    IFDMDownload __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IFDMDownload_get_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMDownload_put_Url_Proxy( 
    IFDMDownload __RPC_FAR * This,
     BSTR newVal);  

void __RPC_STUB IFDMDownload_put_Url_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMDownload_get_DownloadText_Proxy( 
    IFDMDownload __RPC_FAR * This,
     long nTextIndex,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IFDMDownload_get_DownloadText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFDMDownloadsStat_INTERFACE_DEFINED__
#define __IFDMDownloadsStat_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMDownloadsStat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1B26E4A2-7F09-4365-9AB8-13E6891E42CB")
    IFDMDownloadsStat : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_DownloadCount( 
             long __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Download( 
             long nIndex,
             IFDMDownload __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE BuildListOfDownloads( 
             BOOL bIncludeCompleted,
             BOOL bIncludeRunning) = 0;
        
    };
    
#else 	

    typedef struct IFDMDownloadsStatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMDownloadsStat __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMDownloadsStat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMDownloadsStat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMDownloadsStat __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMDownloadsStat __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMDownloadsStat __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMDownloadsStat __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DownloadCount )( 
            IFDMDownloadsStat __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Download )( 
            IFDMDownloadsStat __RPC_FAR * This,
             long nIndex,
             IFDMDownload __RPC_FAR *__RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BuildListOfDownloads )( 
            IFDMDownloadsStat __RPC_FAR * This,
             BOOL bIncludeCompleted,
             BOOL bIncludeRunning);
        
        END_INTERFACE
    } IFDMDownloadsStatVtbl;

    interface IFDMDownloadsStat
    {
        CONST_VTBL struct IFDMDownloadsStatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMDownloadsStat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMDownloadsStat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMDownloadsStat_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMDownloadsStat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMDownloadsStat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMDownloadsStat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMDownloadsStat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMDownloadsStat_get_DownloadCount(This,pVal)	\
    (This)->lpVtbl -> get_DownloadCount(This,pVal)

#define IFDMDownloadsStat_get_Download(This,nIndex,pVal)	\
    (This)->lpVtbl -> get_Download(This,nIndex,pVal)

#define IFDMDownloadsStat_BuildListOfDownloads(This,bIncludeCompleted,bIncludeRunning)	\
    (This)->lpVtbl -> BuildListOfDownloads(This,bIncludeCompleted,bIncludeRunning)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMDownloadsStat_get_DownloadCount_Proxy( 
    IFDMDownloadsStat __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IFDMDownloadsStat_get_DownloadCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMDownloadsStat_get_Download_Proxy( 
    IFDMDownloadsStat __RPC_FAR * This,
     long nIndex,
     IFDMDownload __RPC_FAR *__RPC_FAR *pVal);  

void __RPC_STUB IFDMDownloadsStat_get_Download_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMDownloadsStat_BuildListOfDownloads_Proxy( 
    IFDMDownloadsStat __RPC_FAR * This,
     BOOL bIncludeCompleted,
     BOOL bIncludeRunning);  

void __RPC_STUB IFDMDownloadsStat_BuildListOfDownloads_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFdmTorrentFilesRcvr_INTERFACE_DEFINED__
#define __IFdmTorrentFilesRcvr_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFdmTorrentFilesRcvr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("21402197-BB5B-476C-AA1D-3FFED8ED813A")
    IFdmTorrentFilesRcvr : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE CreateBtDownloadFromFile( 
             BSTR bstrFile) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_ForceSilent( 
             BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_ForceSilent( 
             BOOL newVal) = 0;
        
    };
    
#else 	

    typedef struct IFdmTorrentFilesRcvrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateBtDownloadFromFile )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             BSTR bstrFile);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ForceSilent )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ForceSilent )( 
            IFdmTorrentFilesRcvr __RPC_FAR * This,
             BOOL newVal);
        
        END_INTERFACE
    } IFdmTorrentFilesRcvrVtbl;

    interface IFdmTorrentFilesRcvr
    {
        CONST_VTBL struct IFdmTorrentFilesRcvrVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFdmTorrentFilesRcvr_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFdmTorrentFilesRcvr_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFdmTorrentFilesRcvr_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFdmTorrentFilesRcvr_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFdmTorrentFilesRcvr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFdmTorrentFilesRcvr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFdmTorrentFilesRcvr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFdmTorrentFilesRcvr_CreateBtDownloadFromFile(This,bstrFile)	\
    (This)->lpVtbl -> CreateBtDownloadFromFile(This,bstrFile)

#define IFdmTorrentFilesRcvr_get_ForceSilent(This,pVal)	\
    (This)->lpVtbl -> get_ForceSilent(This,pVal)

#define IFdmTorrentFilesRcvr_put_ForceSilent(This,newVal)	\
    (This)->lpVtbl -> put_ForceSilent(This,newVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFdmTorrentFilesRcvr_CreateBtDownloadFromFile_Proxy( 
    IFdmTorrentFilesRcvr __RPC_FAR * This,
     BSTR bstrFile);  

void __RPC_STUB IFdmTorrentFilesRcvr_CreateBtDownloadFromFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFdmTorrentFilesRcvr_get_ForceSilent_Proxy( 
    IFdmTorrentFilesRcvr __RPC_FAR * This,
     BOOL __RPC_FAR *pVal);  

void __RPC_STUB IFdmTorrentFilesRcvr_get_ForceSilent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFdmTorrentFilesRcvr_put_ForceSilent_Proxy( 
    IFdmTorrentFilesRcvr __RPC_FAR * This,
     BOOL newVal);  

void __RPC_STUB IFdmTorrentFilesRcvr_put_ForceSilent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFDMFlashVideoDownloads_INTERFACE_DEFINED__
#define __IFDMFlashVideoDownloads_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMFlashVideoDownloads;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0DC81A74-1FBD-4EF6-82B2-DE3FA05E8233")
    IFDMFlashVideoDownloads : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE ProcessIeDocument( 
             IDispatch __RPC_FAR *pDispatch) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE ProcessHtml( 
             BSTR bstrHost,
             BSTR bstrHtml) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE CreateFromUrl( 
             BSTR bstrUrl) = 0;
        
    };
    
#else 	

    typedef struct IFDMFlashVideoDownloadsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMFlashVideoDownloads __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMFlashVideoDownloads __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessIeDocument )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             IDispatch __RPC_FAR *pDispatch);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProcessHtml )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             BSTR bstrHost,
             BSTR bstrHtml);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateFromUrl )( 
            IFDMFlashVideoDownloads __RPC_FAR * This,
             BSTR bstrUrl);
        
        END_INTERFACE
    } IFDMFlashVideoDownloadsVtbl;

    interface IFDMFlashVideoDownloads
    {
        CONST_VTBL struct IFDMFlashVideoDownloadsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMFlashVideoDownloads_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMFlashVideoDownloads_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMFlashVideoDownloads_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMFlashVideoDownloads_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMFlashVideoDownloads_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMFlashVideoDownloads_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMFlashVideoDownloads_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMFlashVideoDownloads_ProcessIeDocument(This,pDispatch)	\
    (This)->lpVtbl -> ProcessIeDocument(This,pDispatch)

#define IFDMFlashVideoDownloads_ProcessHtml(This,bstrHost,bstrHtml)	\
    (This)->lpVtbl -> ProcessHtml(This,bstrHost,bstrHtml)

#define IFDMFlashVideoDownloads_CreateFromUrl(This,bstrUrl)	\
    (This)->lpVtbl -> CreateFromUrl(This,bstrUrl)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMFlashVideoDownloads_ProcessIeDocument_Proxy( 
    IFDMFlashVideoDownloads __RPC_FAR * This,
     IDispatch __RPC_FAR *pDispatch);  

void __RPC_STUB IFDMFlashVideoDownloads_ProcessIeDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMFlashVideoDownloads_ProcessHtml_Proxy( 
    IFDMFlashVideoDownloads __RPC_FAR * This,
     BSTR bstrHost,
     BSTR bstrHtml);  

void __RPC_STUB IFDMFlashVideoDownloads_ProcessHtml_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMFlashVideoDownloads_CreateFromUrl_Proxy( 
    IFDMFlashVideoDownloads __RPC_FAR * This,
     BSTR bstrUrl);  

void __RPC_STUB IFDMFlashVideoDownloads_CreateFromUrl_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	    

#ifndef __FdmLib_LIBRARY_DEFINED__
#define __FdmLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_FdmLib;

EXTERN_C const CLSID CLSID_WGUrlReceiver;

#ifdef __cplusplus

class DECLSPEC_UUID("959BA0A4-0893-48B4-8B02-BA0DA0A401FE")
WGUrlReceiver;
#endif

EXTERN_C const CLSID CLSID_WGUrlListReceiver;

#ifdef __cplusplus

class DECLSPEC_UUID("83E6F60E-7147-4475-9DF6-5F1E237FE2CE")
WGUrlListReceiver;
#endif

EXTERN_C const CLSID CLSID_FDM;

#ifdef __cplusplus

class DECLSPEC_UUID("01483019-D8C9-47D8-8E93-AF032EBFADA6")
FDM;
#endif

EXTERN_C const CLSID CLSID_FDMDownloadsStat;

#ifdef __cplusplus

class DECLSPEC_UUID("F01F76EC-3376-4E62-B201-8074C8239376")
FDMDownloadsStat;
#endif

EXTERN_C const CLSID CLSID_FDMDownload;

#ifdef __cplusplus

class DECLSPEC_UUID("DEBBD32E-1D08-4F6A-8A26-E1B3D768A1E5")
FDMDownload;
#endif

EXTERN_C const CLSID CLSID_FDMUploader;

#ifdef __cplusplus

class DECLSPEC_UUID("4D6295C9-2765-49B0-A45B-4136B610954C")
FDMUploader;
#endif

EXTERN_C const CLSID CLSID_FDMUploadPackage;

#ifdef __cplusplus

class DECLSPEC_UUID("56101D38-6A8B-49D6-8C9D-939595AB2D19")
FDMUploadPackage;
#endif

EXTERN_C const CLSID CLSID_FdmTorrentFilesRcvr;

#ifdef __cplusplus

class DECLSPEC_UUID("19CAD08F-0413-47F8-B1D9-5D66826E1E39")
FdmTorrentFilesRcvr;
#endif

EXTERN_C const CLSID CLSID_FDMFlashVideoDownloads;

#ifdef __cplusplus

class DECLSPEC_UUID("D8E9E2ED-846D-4711-A9B8-A29312157DB4")
FDMFlashVideoDownloads;
#endif
#endif     

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * );     

#ifdef __cplusplus
}
#endif

#endif
