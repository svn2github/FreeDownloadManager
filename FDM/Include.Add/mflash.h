/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __Flash_h__
#define __Flash_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IShockwaveFlash_FWD_DEFINED__
#define __IShockwaveFlash_FWD_DEFINED__
typedef interface IShockwaveFlash IShockwaveFlash;
#endif 	  

#ifndef ___IShockwaveFlashEvents_FWD_DEFINED__
#define ___IShockwaveFlashEvents_FWD_DEFINED__
typedef interface _IShockwaveFlashEvents _IShockwaveFlashEvents;
#endif 	  

#ifndef __IFlashFactory_FWD_DEFINED__
#define __IFlashFactory_FWD_DEFINED__
typedef interface IFlashFactory IFlashFactory;
#endif 	  

#ifndef __IDispatchEx_FWD_DEFINED__
#define __IDispatchEx_FWD_DEFINED__
typedef interface IDispatchEx IDispatchEx;
#endif 	  

#ifndef __IFlashObjectInterface_FWD_DEFINED__
#define __IFlashObjectInterface_FWD_DEFINED__
typedef interface IFlashObjectInterface IFlashObjectInterface;
#endif 	  

#ifndef __IServiceProvider_FWD_DEFINED__
#define __IServiceProvider_FWD_DEFINED__
typedef interface IServiceProvider IServiceProvider;
#endif 	  

#ifndef __ShockwaveFlash_FWD_DEFINED__
#define __ShockwaveFlash_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShockwaveFlash ShockwaveFlash;
#else
typedef struct ShockwaveFlash ShockwaveFlash;
#endif 

#endif 	  

#ifndef __FlashProp_FWD_DEFINED__
#define __FlashProp_FWD_DEFINED__

#ifdef __cplusplus
typedef class FlashProp FlashProp;
#else
typedef struct FlashProp FlashProp;
#endif 

#endif 	  

#ifndef __FlashObjectInterface_FWD_DEFINED__
#define __FlashObjectInterface_FWD_DEFINED__

#ifdef __cplusplus
typedef class FlashObjectInterface FlashObjectInterface;
#else
typedef struct FlashObjectInterface FlashObjectInterface;
#endif 

#endif 	  

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );   

#ifndef __ShockwaveFlashObjects_LIBRARY_DEFINED__
#define __ShockwaveFlashObjects_LIBRARY_DEFINED__  

               

EXTERN_C const IID LIBID_ShockwaveFlashObjects;

#ifndef __IShockwaveFlash_INTERFACE_DEFINED__
#define __IShockwaveFlash_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IShockwaveFlash;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D27CDB6C-AE6D-11CF-96B8-444553540000")
    IShockwaveFlash : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_ReadyState( 
             long __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_TotalFrames( 
             long __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Playing( 
             VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Playing( 
             VARIANT_BOOL pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Quality( 
             int __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Quality( 
             int pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_ScaleMode( 
             int __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_ScaleMode( 
             int pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_AlignMode( 
             int __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_AlignMode( 
             int pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_BackgroundColor( 
             long __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_BackgroundColor( 
             long pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Loop( 
             VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Loop( 
             VARIANT_BOOL pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Movie( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Movie( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_FrameNum( 
             long __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_FrameNum( 
             long pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE SetZoomRect( 
             long left,
             long top,
             long right,
             long bottom) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Zoom( 
             int factor) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Pan( 
             long x,
             long y,
             int mode) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Play( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Back( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Forward( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE Rewind( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE StopPlay( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE GotoFrame( 
             long FrameNum) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE CurrentFrame( 
             long __RPC_FAR *FrameNum) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE IsPlaying( 
             VARIANT_BOOL __RPC_FAR *Playing) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE PercentLoaded( 
             long __RPC_FAR *percent) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE FrameLoaded( 
             long FrameNum,
             VARIANT_BOOL __RPC_FAR *loaded) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE FlashVersion( 
             long __RPC_FAR *version) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_WMode( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_WMode( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_SAlign( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_SAlign( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Menu( 
             VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Menu( 
             VARIANT_BOOL pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Base( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Base( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Scale( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Scale( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_DeviceFont( 
             VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_DeviceFont( 
             VARIANT_BOOL pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_EmbedMovie( 
             VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_EmbedMovie( 
             VARIANT_BOOL pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_BGColor( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_BGColor( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_Quality2( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_Quality2( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE LoadMovie( 
             int layer,
             BSTR url) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TGotoFrame( 
             BSTR target,
             long FrameNum) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TGotoLabel( 
             BSTR target,
             BSTR label) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TCurrentFrame( 
             BSTR target,
             long __RPC_FAR *FrameNum) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TCurrentLabel( 
             BSTR target,
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TPlay( 
             BSTR target) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TStopPlay( 
             BSTR target) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE SetVariable( 
             BSTR name,
             BSTR value) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE GetVariable( 
             BSTR name,
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TSetProperty( 
             BSTR target,
             int property,
             BSTR value) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TGetProperty( 
             BSTR target,
             int property,
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TCallFrame( 
             BSTR target,
             int FrameNum) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TCallLabel( 
             BSTR target,
             BSTR label) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TSetPropertyNum( 
             BSTR target,
             int property,
             double value) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE TGetPropertyNum( 
             BSTR target,
             int property,
             double __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_SWRemote( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_SWRemote( 
             BSTR pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE get_FlashVars( 
             BSTR __RPC_FAR *pVal) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE put_FlashVars( 
             BSTR pVal) = 0;
        
    };
    
#else 	

    typedef struct IShockwaveFlashVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IShockwaveFlash __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IShockwaveFlash __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IShockwaveFlash __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IShockwaveFlash __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IShockwaveFlash __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IShockwaveFlash __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IShockwaveFlash __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ReadyState )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TotalFrames )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Playing )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Playing )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Quality )( 
            IShockwaveFlash __RPC_FAR * This,
             int __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Quality )( 
            IShockwaveFlash __RPC_FAR * This,
             int pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ScaleMode )( 
            IShockwaveFlash __RPC_FAR * This,
             int __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ScaleMode )( 
            IShockwaveFlash __RPC_FAR * This,
             int pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AlignMode )( 
            IShockwaveFlash __RPC_FAR * This,
             int __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AlignMode )( 
            IShockwaveFlash __RPC_FAR * This,
             int pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BackgroundColor )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BackgroundColor )( 
            IShockwaveFlash __RPC_FAR * This,
             long pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Loop )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Loop )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Movie )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Movie )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FrameNum )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FrameNum )( 
            IShockwaveFlash __RPC_FAR * This,
             long pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetZoomRect )( 
            IShockwaveFlash __RPC_FAR * This,
             long left,
             long top,
             long right,
             long bottom);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Zoom )( 
            IShockwaveFlash __RPC_FAR * This,
             int factor);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pan )( 
            IShockwaveFlash __RPC_FAR * This,
             long x,
             long y,
             int mode);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Play )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Stop )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Back )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Forward )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rewind )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopPlay )( 
            IShockwaveFlash __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GotoFrame )( 
            IShockwaveFlash __RPC_FAR * This,
             long FrameNum);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CurrentFrame )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *FrameNum);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsPlaying )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *Playing);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PercentLoaded )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *percent);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FrameLoaded )( 
            IShockwaveFlash __RPC_FAR * This,
             long FrameNum,
             VARIANT_BOOL __RPC_FAR *loaded);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FlashVersion )( 
            IShockwaveFlash __RPC_FAR * This,
             long __RPC_FAR *version);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WMode )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_WMode )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SAlign )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SAlign )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Menu )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Menu )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Base )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Base )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Scale )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Scale )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DeviceFont )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DeviceFont )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_EmbedMovie )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_EmbedMovie )( 
            IShockwaveFlash __RPC_FAR * This,
             VARIANT_BOOL pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BGColor )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BGColor )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Quality2 )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Quality2 )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LoadMovie )( 
            IShockwaveFlash __RPC_FAR * This,
             int layer,
             BSTR url);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TGotoFrame )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             long FrameNum);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TGotoLabel )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             BSTR label);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TCurrentFrame )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             long __RPC_FAR *FrameNum);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TCurrentLabel )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TPlay )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TStopPlay )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetVariable )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR name,
             BSTR value);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVariable )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR name,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TSetProperty )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             int property,
             BSTR value);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TGetProperty )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             int property,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TCallFrame )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             int FrameNum);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TCallLabel )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             BSTR label);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TSetPropertyNum )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             int property,
             double value);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TGetPropertyNum )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR target,
             int property,
             double __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SWRemote )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SWRemote )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_FlashVars )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR __RPC_FAR *pVal);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_FlashVars )( 
            IShockwaveFlash __RPC_FAR * This,
             BSTR pVal);
        
        END_INTERFACE
    } IShockwaveFlashVtbl;

    interface IShockwaveFlash
    {
        CONST_VTBL struct IShockwaveFlashVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IShockwaveFlash_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IShockwaveFlash_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IShockwaveFlash_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IShockwaveFlash_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IShockwaveFlash_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IShockwaveFlash_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IShockwaveFlash_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IShockwaveFlash_get_ReadyState(This,pVal)	\
    (This)->lpVtbl -> get_ReadyState(This,pVal)

#define IShockwaveFlash_get_TotalFrames(This,pVal)	\
    (This)->lpVtbl -> get_TotalFrames(This,pVal)

#define IShockwaveFlash_get_Playing(This,pVal)	\
    (This)->lpVtbl -> get_Playing(This,pVal)

#define IShockwaveFlash_put_Playing(This,pVal)	\
    (This)->lpVtbl -> put_Playing(This,pVal)

#define IShockwaveFlash_get_Quality(This,pVal)	\
    (This)->lpVtbl -> get_Quality(This,pVal)

#define IShockwaveFlash_put_Quality(This,pVal)	\
    (This)->lpVtbl -> put_Quality(This,pVal)

#define IShockwaveFlash_get_ScaleMode(This,pVal)	\
    (This)->lpVtbl -> get_ScaleMode(This,pVal)

#define IShockwaveFlash_put_ScaleMode(This,pVal)	\
    (This)->lpVtbl -> put_ScaleMode(This,pVal)

#define IShockwaveFlash_get_AlignMode(This,pVal)	\
    (This)->lpVtbl -> get_AlignMode(This,pVal)

#define IShockwaveFlash_put_AlignMode(This,pVal)	\
    (This)->lpVtbl -> put_AlignMode(This,pVal)

#define IShockwaveFlash_get_BackgroundColor(This,pVal)	\
    (This)->lpVtbl -> get_BackgroundColor(This,pVal)

#define IShockwaveFlash_put_BackgroundColor(This,pVal)	\
    (This)->lpVtbl -> put_BackgroundColor(This,pVal)

#define IShockwaveFlash_get_Loop(This,pVal)	\
    (This)->lpVtbl -> get_Loop(This,pVal)

#define IShockwaveFlash_put_Loop(This,pVal)	\
    (This)->lpVtbl -> put_Loop(This,pVal)

#define IShockwaveFlash_get_Movie(This,pVal)	\
    (This)->lpVtbl -> get_Movie(This,pVal)

#define IShockwaveFlash_put_Movie(This,pVal)	\
    (This)->lpVtbl -> put_Movie(This,pVal)

#define IShockwaveFlash_get_FrameNum(This,pVal)	\
    (This)->lpVtbl -> get_FrameNum(This,pVal)

#define IShockwaveFlash_put_FrameNum(This,pVal)	\
    (This)->lpVtbl -> put_FrameNum(This,pVal)

#define IShockwaveFlash_SetZoomRect(This,left,top,right,bottom)	\
    (This)->lpVtbl -> SetZoomRect(This,left,top,right,bottom)

#define IShockwaveFlash_Zoom(This,factor)	\
    (This)->lpVtbl -> Zoom(This,factor)

#define IShockwaveFlash_Pan(This,x,y,mode)	\
    (This)->lpVtbl -> Pan(This,x,y,mode)

#define IShockwaveFlash_Play(This)	\
    (This)->lpVtbl -> Play(This)

#define IShockwaveFlash_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IShockwaveFlash_Back(This)	\
    (This)->lpVtbl -> Back(This)

#define IShockwaveFlash_Forward(This)	\
    (This)->lpVtbl -> Forward(This)

#define IShockwaveFlash_Rewind(This)	\
    (This)->lpVtbl -> Rewind(This)

#define IShockwaveFlash_StopPlay(This)	\
    (This)->lpVtbl -> StopPlay(This)

#define IShockwaveFlash_GotoFrame(This,FrameNum)	\
    (This)->lpVtbl -> GotoFrame(This,FrameNum)

#define IShockwaveFlash_CurrentFrame(This,FrameNum)	\
    (This)->lpVtbl -> CurrentFrame(This,FrameNum)

#define IShockwaveFlash_IsPlaying(This,Playing)	\
    (This)->lpVtbl -> IsPlaying(This,Playing)

#define IShockwaveFlash_PercentLoaded(This,percent)	\
    (This)->lpVtbl -> PercentLoaded(This,percent)

#define IShockwaveFlash_FrameLoaded(This,FrameNum,loaded)	\
    (This)->lpVtbl -> FrameLoaded(This,FrameNum,loaded)

#define IShockwaveFlash_FlashVersion(This,version)	\
    (This)->lpVtbl -> FlashVersion(This,version)

#define IShockwaveFlash_get_WMode(This,pVal)	\
    (This)->lpVtbl -> get_WMode(This,pVal)

#define IShockwaveFlash_put_WMode(This,pVal)	\
    (This)->lpVtbl -> put_WMode(This,pVal)

#define IShockwaveFlash_get_SAlign(This,pVal)	\
    (This)->lpVtbl -> get_SAlign(This,pVal)

#define IShockwaveFlash_put_SAlign(This,pVal)	\
    (This)->lpVtbl -> put_SAlign(This,pVal)

#define IShockwaveFlash_get_Menu(This,pVal)	\
    (This)->lpVtbl -> get_Menu(This,pVal)

#define IShockwaveFlash_put_Menu(This,pVal)	\
    (This)->lpVtbl -> put_Menu(This,pVal)

#define IShockwaveFlash_get_Base(This,pVal)	\
    (This)->lpVtbl -> get_Base(This,pVal)

#define IShockwaveFlash_put_Base(This,pVal)	\
    (This)->lpVtbl -> put_Base(This,pVal)

#define IShockwaveFlash_get_Scale(This,pVal)	\
    (This)->lpVtbl -> get_Scale(This,pVal)

#define IShockwaveFlash_put_Scale(This,pVal)	\
    (This)->lpVtbl -> put_Scale(This,pVal)

#define IShockwaveFlash_get_DeviceFont(This,pVal)	\
    (This)->lpVtbl -> get_DeviceFont(This,pVal)

#define IShockwaveFlash_put_DeviceFont(This,pVal)	\
    (This)->lpVtbl -> put_DeviceFont(This,pVal)

#define IShockwaveFlash_get_EmbedMovie(This,pVal)	\
    (This)->lpVtbl -> get_EmbedMovie(This,pVal)

#define IShockwaveFlash_put_EmbedMovie(This,pVal)	\
    (This)->lpVtbl -> put_EmbedMovie(This,pVal)

#define IShockwaveFlash_get_BGColor(This,pVal)	\
    (This)->lpVtbl -> get_BGColor(This,pVal)

#define IShockwaveFlash_put_BGColor(This,pVal)	\
    (This)->lpVtbl -> put_BGColor(This,pVal)

#define IShockwaveFlash_get_Quality2(This,pVal)	\
    (This)->lpVtbl -> get_Quality2(This,pVal)

#define IShockwaveFlash_put_Quality2(This,pVal)	\
    (This)->lpVtbl -> put_Quality2(This,pVal)

#define IShockwaveFlash_LoadMovie(This,layer,url)	\
    (This)->lpVtbl -> LoadMovie(This,layer,url)

#define IShockwaveFlash_TGotoFrame(This,target,FrameNum)	\
    (This)->lpVtbl -> TGotoFrame(This,target,FrameNum)

#define IShockwaveFlash_TGotoLabel(This,target,label)	\
    (This)->lpVtbl -> TGotoLabel(This,target,label)

#define IShockwaveFlash_TCurrentFrame(This,target,FrameNum)	\
    (This)->lpVtbl -> TCurrentFrame(This,target,FrameNum)

#define IShockwaveFlash_TCurrentLabel(This,target,pVal)	\
    (This)->lpVtbl -> TCurrentLabel(This,target,pVal)

#define IShockwaveFlash_TPlay(This,target)	\
    (This)->lpVtbl -> TPlay(This,target)

#define IShockwaveFlash_TStopPlay(This,target)	\
    (This)->lpVtbl -> TStopPlay(This,target)

#define IShockwaveFlash_SetVariable(This,name,value)	\
    (This)->lpVtbl -> SetVariable(This,name,value)

#define IShockwaveFlash_GetVariable(This,name,pVal)	\
    (This)->lpVtbl -> GetVariable(This,name,pVal)

#define IShockwaveFlash_TSetProperty(This,target,property,value)	\
    (This)->lpVtbl -> TSetProperty(This,target,property,value)

#define IShockwaveFlash_TGetProperty(This,target,property,pVal)	\
    (This)->lpVtbl -> TGetProperty(This,target,property,pVal)

#define IShockwaveFlash_TCallFrame(This,target,FrameNum)	\
    (This)->lpVtbl -> TCallFrame(This,target,FrameNum)

#define IShockwaveFlash_TCallLabel(This,target,label)	\
    (This)->lpVtbl -> TCallLabel(This,target,label)

#define IShockwaveFlash_TSetPropertyNum(This,target,property,value)	\
    (This)->lpVtbl -> TSetPropertyNum(This,target,property,value)

#define IShockwaveFlash_TGetPropertyNum(This,target,property,pVal)	\
    (This)->lpVtbl -> TGetPropertyNum(This,target,property,pVal)

#define IShockwaveFlash_get_SWRemote(This,pVal)	\
    (This)->lpVtbl -> get_SWRemote(This,pVal)

#define IShockwaveFlash_put_SWRemote(This,pVal)	\
    (This)->lpVtbl -> put_SWRemote(This,pVal)

#define IShockwaveFlash_get_FlashVars(This,pVal)	\
    (This)->lpVtbl -> get_FlashVars(This,pVal)

#define IShockwaveFlash_put_FlashVars(This,pVal)	\
    (This)->lpVtbl -> put_FlashVars(This,pVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_ReadyState_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_ReadyState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_TotalFrames_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_TotalFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Playing_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Playing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Playing_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL pVal);  

void __RPC_STUB IShockwaveFlash_put_Playing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Quality_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Quality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Quality_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int pVal);  

void __RPC_STUB IShockwaveFlash_put_Quality_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_ScaleMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_ScaleMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_ScaleMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int pVal);  

void __RPC_STUB IShockwaveFlash_put_ScaleMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_AlignMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_AlignMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_AlignMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int pVal);  

void __RPC_STUB IShockwaveFlash_put_AlignMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_BackgroundColor_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_BackgroundColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_BackgroundColor_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long pVal);  

void __RPC_STUB IShockwaveFlash_put_BackgroundColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Loop_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Loop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Loop_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL pVal);  

void __RPC_STUB IShockwaveFlash_put_Loop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Movie_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Movie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Movie_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_Movie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_FrameNum_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_FrameNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_FrameNum_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long pVal);  

void __RPC_STUB IShockwaveFlash_put_FrameNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_SetZoomRect_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long left,
     long top,
     long right,
     long bottom);  

void __RPC_STUB IShockwaveFlash_SetZoomRect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Zoom_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int factor);  

void __RPC_STUB IShockwaveFlash_Zoom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Pan_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long x,
     long y,
     int mode);  

void __RPC_STUB IShockwaveFlash_Pan_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Play_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_Play_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Stop_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Back_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_Back_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Forward_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_Forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_Rewind_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_Rewind_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_StopPlay_Proxy( 
    IShockwaveFlash __RPC_FAR * This);  

void __RPC_STUB IShockwaveFlash_StopPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_GotoFrame_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long FrameNum);  

void __RPC_STUB IShockwaveFlash_GotoFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_CurrentFrame_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *FrameNum);  

void __RPC_STUB IShockwaveFlash_CurrentFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_IsPlaying_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *Playing);  

void __RPC_STUB IShockwaveFlash_IsPlaying_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_PercentLoaded_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *percent);  

void __RPC_STUB IShockwaveFlash_PercentLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_FrameLoaded_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long FrameNum,
     VARIANT_BOOL __RPC_FAR *loaded);  

void __RPC_STUB IShockwaveFlash_FrameLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_FlashVersion_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     long __RPC_FAR *version);  

void __RPC_STUB IShockwaveFlash_FlashVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_WMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_WMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_WMode_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_WMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_SAlign_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_SAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_SAlign_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_SAlign_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Menu_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Menu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Menu_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL pVal);  

void __RPC_STUB IShockwaveFlash_put_Menu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Base_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Base_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Base_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_Base_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Scale_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Scale_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_DeviceFont_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_DeviceFont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_DeviceFont_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL pVal);  

void __RPC_STUB IShockwaveFlash_put_DeviceFont_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_EmbedMovie_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_EmbedMovie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_EmbedMovie_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     VARIANT_BOOL pVal);  

void __RPC_STUB IShockwaveFlash_put_EmbedMovie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_BGColor_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_BGColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_BGColor_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_BGColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_Quality2_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_Quality2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_Quality2_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_Quality2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_LoadMovie_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     int layer,
     BSTR url);  

void __RPC_STUB IShockwaveFlash_LoadMovie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TGotoFrame_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     long FrameNum);  

void __RPC_STUB IShockwaveFlash_TGotoFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TGotoLabel_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     BSTR label);  

void __RPC_STUB IShockwaveFlash_TGotoLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TCurrentFrame_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     long __RPC_FAR *FrameNum);  

void __RPC_STUB IShockwaveFlash_TCurrentFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TCurrentLabel_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_TCurrentLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TPlay_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target);  

void __RPC_STUB IShockwaveFlash_TPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TStopPlay_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target);  

void __RPC_STUB IShockwaveFlash_TStopPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_SetVariable_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR name,
     BSTR value);  

void __RPC_STUB IShockwaveFlash_SetVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_GetVariable_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR name,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_GetVariable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TSetProperty_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     int property,
     BSTR value);  

void __RPC_STUB IShockwaveFlash_TSetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TGetProperty_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     int property,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_TGetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TCallFrame_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     int FrameNum);  

void __RPC_STUB IShockwaveFlash_TCallFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TCallLabel_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     BSTR label);  

void __RPC_STUB IShockwaveFlash_TCallLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TSetPropertyNum_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     int property,
     double value);  

void __RPC_STUB IShockwaveFlash_TSetPropertyNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_TGetPropertyNum_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR target,
     int property,
     double __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_TGetPropertyNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_SWRemote_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_SWRemote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_SWRemote_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_SWRemote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_get_FlashVars_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR __RPC_FAR *pVal);  

void __RPC_STUB IShockwaveFlash_get_FlashVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IShockwaveFlash_put_FlashVars_Proxy( 
    IShockwaveFlash __RPC_FAR * This,
     BSTR pVal);  

void __RPC_STUB IShockwaveFlash_put_FlashVars_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef ___IShockwaveFlashEvents_DISPINTERFACE_DEFINED__
#define ___IShockwaveFlashEvents_DISPINTERFACE_DEFINED__  

   

EXTERN_C const IID DIID__IShockwaveFlashEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D27CDB6D-AE6D-11CF-96B8-444553540000")
    _IShockwaveFlashEvents : public IDispatch
    {
    };
    
#else 	

    typedef struct _IShockwaveFlashEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IShockwaveFlashEvents __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IShockwaveFlashEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IShockwaveFlashEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IShockwaveFlashEvents __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IShockwaveFlashEvents __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IShockwaveFlashEvents __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IShockwaveFlashEvents __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IShockwaveFlashEventsVtbl;

    interface _IShockwaveFlashEvents
    {
        CONST_VTBL struct _IShockwaveFlashEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define _IShockwaveFlashEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IShockwaveFlashEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IShockwaveFlashEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define _IShockwaveFlashEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IShockwaveFlashEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IShockwaveFlashEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IShockwaveFlashEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif   

#endif 	  

#endif 	  

#ifndef __IFlashFactory_INTERFACE_DEFINED__
#define __IFlashFactory_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFlashFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D27CDB70-AE6D-11CF-96B8-444553540000")
    IFlashFactory : public IUnknown
    {
    public:
    };
    
#else 	

    typedef struct IFlashFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFlashFactory __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFlashFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFlashFactory __RPC_FAR * This);
        
        END_INTERFACE
    } IFlashFactoryVtbl;

    interface IFlashFactory
    {
        CONST_VTBL struct IFlashFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFlashFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFlashFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFlashFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#endif   

#endif 	      

#endif 	  

#ifndef __IFlashObjectInterface_INTERFACE_DEFINED__
#define __IFlashObjectInterface_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFlashObjectInterface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D27CDB72-AE6D-11CF-96B8-444553540000")
    IFlashObjectInterface : public IDispatch
    {
    public:
    };
    
#else 	

    typedef struct IFlashObjectInterfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFlashObjectInterface __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFlashObjectInterface __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFlashObjectInterface __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFlashObjectInterface __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFlashObjectInterface __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFlashObjectInterface __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFlashObjectInterface __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
        HRESULT ( __stdcall __RPC_FAR *GetDispID )( 
            IFlashObjectInterface __RPC_FAR * This,
             BSTR bstrName,
             unsigned long grfdex,
             long __RPC_FAR *pid);
        
        HRESULT ( __stdcall __RPC_FAR *RemoteInvokeEx )( 
            IFlashObjectInterface __RPC_FAR * This,
             long id,
             unsigned long lcid,
             unsigned long dwFlags,
             DISPPARAMS __RPC_FAR *pdp,
             VARIANT __RPC_FAR *pvarRes,
             EXCEPINFO __RPC_FAR *pei,
             IServiceProvider __RPC_FAR *pspCaller,
             unsigned int cvarRefArg,
             unsigned int __RPC_FAR *rgiRefArg,
             VARIANT __RPC_FAR *rgvarRefArg);
        
        HRESULT ( __stdcall __RPC_FAR *DeleteMemberByName )( 
            IFlashObjectInterface __RPC_FAR * This,
             BSTR bstrName,
             unsigned long grfdex);
        
        HRESULT ( __stdcall __RPC_FAR *DeleteMemberByDispID )( 
            IFlashObjectInterface __RPC_FAR * This,
             long id);
        
        HRESULT ( __stdcall __RPC_FAR *GetMemberProperties )( 
            IFlashObjectInterface __RPC_FAR * This,
             long id,
             unsigned long grfdexFetch,
             unsigned long __RPC_FAR *pgrfdex);
        
        HRESULT ( __stdcall __RPC_FAR *GetMemberName )( 
            IFlashObjectInterface __RPC_FAR * This,
             long id,
             BSTR __RPC_FAR *pbstrName);
        
        HRESULT ( __stdcall __RPC_FAR *GetNextDispID )( 
            IFlashObjectInterface __RPC_FAR * This,
             unsigned long grfdex,
             long id,
             long __RPC_FAR *pid);
        
        HRESULT ( __stdcall __RPC_FAR *GetNameSpaceParent )( 
            IFlashObjectInterface __RPC_FAR * This,
             IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        END_INTERFACE
    } IFlashObjectInterfaceVtbl;

    interface IFlashObjectInterface
    {
        CONST_VTBL struct IFlashObjectInterfaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFlashObjectInterface_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFlashObjectInterface_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFlashObjectInterface_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFlashObjectInterface_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFlashObjectInterface_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFlashObjectInterface_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFlashObjectInterface_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFlashObjectInterface_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IFlashObjectInterface_RemoteInvokeEx(This,id,lcid,dwFlags,pdp,pvarRes,pei,pspCaller,cvarRefArg,rgiRefArg,rgvarRefArg)	\
    (This)->lpVtbl -> RemoteInvokeEx(This,id,lcid,dwFlags,pdp,pvarRes,pei,pspCaller,cvarRefArg,rgiRefArg,rgvarRefArg)

#define IFlashObjectInterface_DeleteMemberByName(This,bstrName,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstrName,grfdex)

#define IFlashObjectInterface_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IFlashObjectInterface_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IFlashObjectInterface_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IFlashObjectInterface_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IFlashObjectInterface_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)  

#endif   

#endif 	      

#endif 	  

#ifndef __IDispatchEx_INTERFACE_DEFINED__
#define __IDispatchEx_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IDispatchEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A6EF9860-C720-11D0-9337-00A0C90DCAA9")
    IDispatchEx : public IDispatch
    {
    public:
        virtual HRESULT __stdcall GetDispID( 
             BSTR bstrName,
             unsigned long grfdex,
             long __RPC_FAR *pid) = 0;
        
        virtual HRESULT __stdcall RemoteInvokeEx( 
             long id,
             unsigned long lcid,
             unsigned long dwFlags,
             DISPPARAMS __RPC_FAR *pdp,
             VARIANT __RPC_FAR *pvarRes,
             EXCEPINFO __RPC_FAR *pei,
             IServiceProvider __RPC_FAR *pspCaller,
             unsigned int cvarRefArg,
             unsigned int __RPC_FAR *rgiRefArg,
             VARIANT __RPC_FAR *rgvarRefArg) = 0;
        
        virtual HRESULT __stdcall DeleteMemberByName( 
             BSTR bstrName,
             unsigned long grfdex) = 0;
        
        virtual HRESULT __stdcall DeleteMemberByDispID( 
             long id) = 0;
        
        virtual HRESULT __stdcall GetMemberProperties( 
             long id,
             unsigned long grfdexFetch,
             unsigned long __RPC_FAR *pgrfdex) = 0;
        
        virtual HRESULT __stdcall GetMemberName( 
             long id,
             BSTR __RPC_FAR *pbstrName) = 0;
        
        virtual HRESULT __stdcall GetNextDispID( 
             unsigned long grfdex,
             long id,
             long __RPC_FAR *pid) = 0;
        
        virtual HRESULT __stdcall GetNameSpaceParent( 
             IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
    };
    
#else 	

    typedef struct IDispatchExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDispatchEx __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDispatchEx __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDispatchEx __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDispatchEx __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDispatchEx __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDispatchEx __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDispatchEx __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
        HRESULT ( __stdcall __RPC_FAR *GetDispID )( 
            IDispatchEx __RPC_FAR * This,
             BSTR bstrName,
             unsigned long grfdex,
             long __RPC_FAR *pid);
        
        HRESULT ( __stdcall __RPC_FAR *RemoteInvokeEx )( 
            IDispatchEx __RPC_FAR * This,
             long id,
             unsigned long lcid,
             unsigned long dwFlags,
             DISPPARAMS __RPC_FAR *pdp,
             VARIANT __RPC_FAR *pvarRes,
             EXCEPINFO __RPC_FAR *pei,
             IServiceProvider __RPC_FAR *pspCaller,
             unsigned int cvarRefArg,
             unsigned int __RPC_FAR *rgiRefArg,
             VARIANT __RPC_FAR *rgvarRefArg);
        
        HRESULT ( __stdcall __RPC_FAR *DeleteMemberByName )( 
            IDispatchEx __RPC_FAR * This,
             BSTR bstrName,
             unsigned long grfdex);
        
        HRESULT ( __stdcall __RPC_FAR *DeleteMemberByDispID )( 
            IDispatchEx __RPC_FAR * This,
             long id);
        
        HRESULT ( __stdcall __RPC_FAR *GetMemberProperties )( 
            IDispatchEx __RPC_FAR * This,
             long id,
             unsigned long grfdexFetch,
             unsigned long __RPC_FAR *pgrfdex);
        
        HRESULT ( __stdcall __RPC_FAR *GetMemberName )( 
            IDispatchEx __RPC_FAR * This,
             long id,
             BSTR __RPC_FAR *pbstrName);
        
        HRESULT ( __stdcall __RPC_FAR *GetNextDispID )( 
            IDispatchEx __RPC_FAR * This,
             unsigned long grfdex,
             long id,
             long __RPC_FAR *pid);
        
        HRESULT ( __stdcall __RPC_FAR *GetNameSpaceParent )( 
            IDispatchEx __RPC_FAR * This,
             IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        END_INTERFACE
    } IDispatchExVtbl;

    interface IDispatchEx
    {
        CONST_VTBL struct IDispatchExVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IDispatchEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDispatchEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDispatchEx_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IDispatchEx_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDispatchEx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDispatchEx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDispatchEx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IDispatchEx_GetDispID(This,bstrName,grfdex,pid)	\
    (This)->lpVtbl -> GetDispID(This,bstrName,grfdex,pid)

#define IDispatchEx_RemoteInvokeEx(This,id,lcid,dwFlags,pdp,pvarRes,pei,pspCaller,cvarRefArg,rgiRefArg,rgvarRefArg)	\
    (This)->lpVtbl -> RemoteInvokeEx(This,id,lcid,dwFlags,pdp,pvarRes,pei,pspCaller,cvarRefArg,rgiRefArg,rgvarRefArg)

#define IDispatchEx_DeleteMemberByName(This,bstrName,grfdex)	\
    (This)->lpVtbl -> DeleteMemberByName(This,bstrName,grfdex)

#define IDispatchEx_DeleteMemberByDispID(This,id)	\
    (This)->lpVtbl -> DeleteMemberByDispID(This,id)

#define IDispatchEx_GetMemberProperties(This,id,grfdexFetch,pgrfdex)	\
    (This)->lpVtbl -> GetMemberProperties(This,id,grfdexFetch,pgrfdex)

#define IDispatchEx_GetMemberName(This,id,pbstrName)	\
    (This)->lpVtbl -> GetMemberName(This,id,pbstrName)

#define IDispatchEx_GetNextDispID(This,grfdex,id,pid)	\
    (This)->lpVtbl -> GetNextDispID(This,grfdex,id,pid)

#define IDispatchEx_GetNameSpaceParent(This,ppunk)	\
    (This)->lpVtbl -> GetNameSpaceParent(This,ppunk)

#endif   

#endif 	    

HRESULT __stdcall IDispatchEx_GetDispID_Proxy( 
    IDispatchEx __RPC_FAR * This,
     BSTR bstrName,
     unsigned long grfdex,
     long __RPC_FAR *pid);  

void __RPC_STUB IDispatchEx_GetDispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_RemoteInvokeEx_Proxy( 
    IDispatchEx __RPC_FAR * This,
     long id,
     unsigned long lcid,
     unsigned long dwFlags,
     DISPPARAMS __RPC_FAR *pdp,
     VARIANT __RPC_FAR *pvarRes,
     EXCEPINFO __RPC_FAR *pei,
     IServiceProvider __RPC_FAR *pspCaller,
     unsigned int cvarRefArg,
     unsigned int __RPC_FAR *rgiRefArg,
     VARIANT __RPC_FAR *rgvarRefArg);  

void __RPC_STUB IDispatchEx_RemoteInvokeEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_DeleteMemberByName_Proxy( 
    IDispatchEx __RPC_FAR * This,
     BSTR bstrName,
     unsigned long grfdex);  

void __RPC_STUB IDispatchEx_DeleteMemberByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_DeleteMemberByDispID_Proxy( 
    IDispatchEx __RPC_FAR * This,
     long id);  

void __RPC_STUB IDispatchEx_DeleteMemberByDispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_GetMemberProperties_Proxy( 
    IDispatchEx __RPC_FAR * This,
     long id,
     unsigned long grfdexFetch,
     unsigned long __RPC_FAR *pgrfdex);  

void __RPC_STUB IDispatchEx_GetMemberProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_GetMemberName_Proxy( 
    IDispatchEx __RPC_FAR * This,
     long id,
     BSTR __RPC_FAR *pbstrName);  

void __RPC_STUB IDispatchEx_GetMemberName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_GetNextDispID_Proxy( 
    IDispatchEx __RPC_FAR * This,
     unsigned long grfdex,
     long id,
     long __RPC_FAR *pid);  

void __RPC_STUB IDispatchEx_GetNextDispID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

HRESULT __stdcall IDispatchEx_GetNameSpaceParent_Proxy( 
    IDispatchEx __RPC_FAR * This,
     IUnknown __RPC_FAR *__RPC_FAR *ppunk);  

void __RPC_STUB IDispatchEx_GetNameSpaceParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IServiceProvider_INTERFACE_DEFINED__
#define __IServiceProvider_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IServiceProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D5140C1-7436-11CE-8034-00AA006009FA")
    IServiceProvider : public IUnknown
    {
    public:
        virtual HRESULT __stdcall RemoteQueryService( 
             GUID __RPC_FAR *guidService,
             GUID __RPC_FAR *riid,
             IUnknown __RPC_FAR *__RPC_FAR *ppvObject) = 0;
        
    };
    
#else 	

    typedef struct IServiceProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IServiceProvider __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IServiceProvider __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IServiceProvider __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *RemoteQueryService )( 
            IServiceProvider __RPC_FAR * This,
             GUID __RPC_FAR *guidService,
             GUID __RPC_FAR *riid,
             IUnknown __RPC_FAR *__RPC_FAR *ppvObject);
        
        END_INTERFACE
    } IServiceProviderVtbl;

    interface IServiceProvider
    {
        CONST_VTBL struct IServiceProviderVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IServiceProvider_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IServiceProvider_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IServiceProvider_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IServiceProvider_RemoteQueryService(This,guidService,riid,ppvObject)	\
    (This)->lpVtbl -> RemoteQueryService(This,guidService,riid,ppvObject)

#endif   

#endif 	    

HRESULT __stdcall IServiceProvider_RemoteQueryService_Proxy( 
    IServiceProvider __RPC_FAR * This,
     GUID __RPC_FAR *guidService,
     GUID __RPC_FAR *riid,
     IUnknown __RPC_FAR *__RPC_FAR *ppvObject);  

void __RPC_STUB IServiceProvider_RemoteQueryService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

EXTERN_C const CLSID CLSID_ShockwaveFlash;

#ifdef __cplusplus

class DECLSPEC_UUID("D27CDB6E-AE6D-11CF-96B8-444553540000")
ShockwaveFlash;
#endif

EXTERN_C const CLSID CLSID_FlashProp;

#ifdef __cplusplus

class DECLSPEC_UUID("1171A62F-05D2-11D1-83FC-00A0C9089C5A")
FlashProp;
#endif

EXTERN_C const CLSID CLSID_FlashObjectInterface;

#ifdef __cplusplus

class DECLSPEC_UUID("D27CDB71-AE6D-11CF-96B8-444553540000")
FlashObjectInterface;
#endif
#endif         

#ifdef __cplusplus
}
#endif

#endif
