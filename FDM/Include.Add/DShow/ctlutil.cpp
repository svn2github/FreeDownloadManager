/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                              

#include <streams.h>
#include <limits.h>
#include "seekpt.h"  

#pragma warning(disable:4237)    

CBaseDispatch::~CBaseDispatch()
{
    if (m_pti) {
	m_pti->Release();
    }
}      

STDMETHODIMP
CBaseDispatch::GetTypeInfoCount(UINT * pctinfo)
{
    CheckPointer(pctinfo,E_POINTER);
    ValidateReadWritePtr(pctinfo,sizeof(UINT *));
    *pctinfo = 1;
    return S_OK;
}  

typedef HRESULT (STDAPICALLTYPE *LPLOADTYPELIB)(
			    const OLECHAR FAR *szFile,
			    ITypeLib FAR* FAR* pptlib);

typedef HRESULT (STDAPICALLTYPE *LPLOADREGTYPELIB)(REFGUID rguid,
			    WORD wVerMajor,
			    WORD wVerMinor,
			    LCID lcid,
			    ITypeLib FAR* FAR* pptlib);    

STDMETHODIMP
CBaseDispatch::GetTypeInfo(
  REFIID riid,
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    CheckPointer(pptinfo,E_POINTER);
    ValidateReadWritePtr(pptinfo,sizeof(ITypeInfo *));
    HRESULT hr;

    *pptinfo = NULL;

    
    if (0 != itinfo) {
	return TYPE_E_ELEMENTNOTFOUND;
    }

    if (NULL == pptinfo) {
	return E_POINTER;
    }

    
    if (NULL == m_pti) {

	LPLOADTYPELIB	    lpfnLoadTypeLib;
	LPLOADREGTYPELIB    lpfnLoadRegTypeLib;
	ITypeLib	    *ptlib;
	HINSTANCE	    hInst;

	static const char  szTypeLib[]	  = "LoadTypeLib";
	static const char  szRegTypeLib[] = "LoadRegTypeLib";
	static const WCHAR szControl[]	  = L"control.tlb";

	
	
	

	hInst = LoadOLEAut32();
	if (hInst == NULL) {
	    DWORD dwError = GetLastError();
	    return AmHresultFromWin32(dwError);
	}
	lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst,
							      szRegTypeLib);
	if (lpfnLoadRegTypeLib == NULL) {
	    DWORD dwError = GetLastError();
	    return AmHresultFromWin32(dwError);
	}

	hr = (*lpfnLoadRegTypeLib)(LIBID_QuartzTypeLib, 1, 0, 
				   lcid, &ptlib);

	if (FAILED(hr)) {

	    
	    

	    lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
	    if (lpfnLoadTypeLib == NULL) {
		DWORD dwError = GetLastError();
		return AmHresultFromWin32(dwError);
	    }

	    hr = (*lpfnLoadTypeLib)(szControl, &ptlib);
	    if (FAILED(hr)) {
		return hr;
	    }
	}

	hr = ptlib->GetTypeInfoOfGuid(
		    riid,
		    &m_pti);

	ptlib->Release();

	if (FAILED(hr)) {
	    return hr;
	}
    }

    *pptinfo = m_pti;
    m_pti->AddRef();
    return S_OK;
}  

STDMETHODIMP
CBaseDispatch::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    
    

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(riid, 0, lcid, &pti);

    if (SUCCEEDED(hr)) {
	hr = pti->GetIDsOfNames(rgszNames, cNames, rgdispid);

	pti->Release();
    }
    return hr;
}      

CMediaControl::CMediaControl(const TCHAR * name,LPUNKNOWN pUnk) :
    CUnknown(name, pUnk)
{
}    

STDMETHODIMP
CMediaControl::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IMediaControl) {
	return GetInterface( (IMediaControl *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP
CMediaControl::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}      

STDMETHODIMP
CMediaControl::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IMediaControl,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CMediaControl::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IMediaControl,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CMediaControl::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IMediaControl *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}        

CMediaEvent::CMediaEvent(const TCHAR * name,LPUNKNOWN pUnk) :
    CUnknown(name, pUnk)
{
}      

STDMETHODIMP
CMediaEvent::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IMediaEvent || riid == IID_IMediaEventEx) {
	return GetInterface( (IMediaEventEx *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP
CMediaEvent::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}      

STDMETHODIMP
CMediaEvent::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IMediaEvent,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CMediaEvent::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IMediaEvent,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CMediaEvent::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IMediaEvent *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}        

CMediaPosition::CMediaPosition(const TCHAR * name,LPUNKNOWN pUnk) :
    CUnknown(name, pUnk)
{
}

CMediaPosition::CMediaPosition(const TCHAR * name,
                               LPUNKNOWN pUnk,
                               HRESULT * phr) :
    CUnknown(name, pUnk)
{
    UNREFERENCED_PARAMETER(phr);
}      

STDMETHODIMP
CMediaPosition::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IMediaPosition) {
	return GetInterface( (IMediaPosition *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP
CMediaPosition::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}      

STDMETHODIMP
CMediaPosition::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IMediaPosition,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CMediaPosition::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IMediaPosition,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CMediaPosition::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IMediaPosition *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}        

CPosPassThru::CPosPassThru(const TCHAR *pName,
			   LPUNKNOWN pUnk,
			   HRESULT *phr,
			   IPin *pPin) :
    CMediaPosition(pName,pUnk),
    m_pPin(pPin)
{
    if (pPin == NULL) {
	*phr = E_POINTER;
	return;
    }
}      

STDMETHODIMP
CPosPassThru::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    *ppv = NULL;

    if (riid == IID_IMediaSeeking) {
	return GetInterface( static_cast<IMediaSeeking *>(this), ppv);
    }
    return CMediaPosition::NonDelegatingQueryInterface(riid,ppv);
}      

HRESULT
CPosPassThru::GetPeer(IMediaPosition ** ppMP)
{
    *ppMP = NULL;

    IPin *pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (FAILED(hr)) {
	return E_NOTIMPL;
    }
    IMediaPosition * pMP;
    hr = pConnected->QueryInterface(IID_IMediaPosition, (void **) &pMP);
    pConnected->Release();
    if (FAILED(hr)) {
	return E_NOTIMPL;
    }

    *ppMP = pMP;
    return S_OK;
}      

HRESULT
CPosPassThru::GetPeerSeeking(IMediaSeeking ** ppMS)
{
    *ppMS = NULL;

    IPin *pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (FAILED(hr)) {
	return E_NOTIMPL;
    }
    IMediaSeeking * pMS;
    hr = pConnected->QueryInterface(IID_IMediaSeeking, (void **) &pMS);
    pConnected->Release();
    if (FAILED(hr)) {
	return E_NOTIMPL;
    }

    *ppMS = pMS;
    return S_OK;
}        

STDMETHODIMP
CPosPassThru::GetCapabilities(DWORD * pCaps)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->GetCapabilities(pCaps);
    pMS->Release();
    return hr;
}

STDMETHODIMP
CPosPassThru::CheckCapabilities(DWORD * pCaps)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->CheckCapabilities(pCaps);
    pMS->Release();
    return hr;
}

STDMETHODIMP
CPosPassThru::IsFormatSupported(const GUID * pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->IsFormatSupported(pFormat);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::QueryPreferredFormat(GUID *pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->QueryPreferredFormat(pFormat);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::SetTimeFormat(const GUID * pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->SetTimeFormat(pFormat);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::GetTimeFormat(GUID *pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->GetTimeFormat(pFormat);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::IsUsingTimeFormat(const GUID * pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->IsUsingTimeFormat(pFormat);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::ConvertTimeFormat(LONGLONG * pTarget, const GUID * pTargetFormat,
				LONGLONG    Source, const GUID * pSourceFormat )
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->ConvertTimeFormat(pTarget, pTargetFormat, Source, pSourceFormat );
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::SetPositions( LONGLONG * pCurrent, DWORD CurrentFlags
			  , LONGLONG * pStop, DWORD StopFlags )
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->SetPositions(pCurrent, CurrentFlags, pStop, StopFlags );
    pMS->Release();
    return hr;
}

STDMETHODIMP
CPosPassThru::GetPositions(LONGLONG *pCurrent, LONGLONG * pStop)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->GetPositions(pCurrent,pStop);
    pMS->Release();
    return hr;
}

HRESULT
CPosPassThru::GetSeekingLongLong
( HRESULT (__stdcall IMediaSeeking::*pMethod)( LONGLONG * )
, LONGLONG * pll
)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (SUCCEEDED(hr))
    {
	hr = (pMS->*pMethod)(pll);
	pMS->Release();
    }
    return hr;
}    

STDMETHODIMP
CPosPassThru::GetCurrentPosition(LONGLONG *pCurrent)
{
    
    HRESULT hr = GetMediaTime(pCurrent,NULL);
    if (SUCCEEDED(hr)) hr = NOERROR;
    else hr = GetSeekingLongLong( &IMediaSeeking::GetCurrentPosition, pCurrent );
    return hr;
}  

STDMETHODIMP
CPosPassThru::GetStopPosition(LONGLONG *pStop)
{
    return GetSeekingLongLong( &IMediaSeeking::GetStopPosition, pStop );;
}

STDMETHODIMP
CPosPassThru::GetDuration(LONGLONG *pDuration)
{
    return GetSeekingLongLong( &IMediaSeeking::GetDuration, pDuration );;
}  

STDMETHODIMP
CPosPassThru::GetPreroll(LONGLONG *pllPreroll)
{
    return GetSeekingLongLong( &IMediaSeeking::GetPreroll, pllPreroll );;
}  

STDMETHODIMP
CPosPassThru::GetAvailable( LONGLONG *pEarliest, LONGLONG *pLatest )
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMS->GetAvailable( pEarliest, pLatest );
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::GetRate(double * pdRate)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMS->GetRate(pdRate);
    pMS->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::SetRate(double dRate)
{
    if (0.0 == dRate) {
		return E_INVALIDARG;
    }

    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMS->SetRate(dRate);
    pMS->Release();
    return hr;
}            

STDMETHODIMP
CPosPassThru::get_Duration(REFTIME * plength)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }

    hr = pMP->get_Duration(plength);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::get_CurrentPosition(REFTIME * pllTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->get_CurrentPosition(pllTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::put_CurrentPosition(REFTIME llTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->put_CurrentPosition(llTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::get_StopTime(REFTIME * pllTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->get_StopTime(pllTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::put_StopTime(REFTIME llTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->put_StopTime(llTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::get_PrerollTime(REFTIME * pllTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->get_PrerollTime(pllTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::put_PrerollTime(REFTIME llTime)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->put_PrerollTime(llTime);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::get_Rate(double * pdRate)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->get_Rate(pdRate);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::put_Rate(double dRate)
{
    if (0.0 == dRate) {
		return E_INVALIDARG;
    }

    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->put_Rate(dRate);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::CanSeekForward(LONG *pCanSeekForward)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->CanSeekForward(pCanSeekForward);
    pMP->Release();
    return hr;
}  

STDMETHODIMP
CPosPassThru::CanSeekBackward(LONG *pCanSeekBackward)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (FAILED(hr)) {
	return hr;
    }
    hr = pMP->CanSeekBackward(pCanSeekBackward);
    pMP->Release();
    return hr;
}                    

CRendererPosPassThru::CRendererPosPassThru(const TCHAR *pName,
					   LPUNKNOWN pUnk,
					   HRESULT *phr,
					   IPin *pPin) :
    CPosPassThru(pName,pUnk,phr,pPin),
    m_StartMedia(0),
    m_EndMedia(0),
    m_bReset(TRUE)
{
}      

HRESULT
CRendererPosPassThru::RegisterMediaTime(IMediaSample *pMediaSample)
{
    ASSERT(pMediaSample);
    LONGLONG StartMedia;
    LONGLONG EndMedia;

    CAutoLock cAutoLock(&m_PositionLock);

    

    HRESULT hr = pMediaSample->GetTime(&StartMedia,&EndMedia);
    if (FAILED(hr))
    {
	ASSERT(hr == VFW_E_SAMPLE_TIME_NOT_SET);
	return hr;
    }

    m_StartMedia = StartMedia;
    m_EndMedia = EndMedia;
    m_bReset = FALSE;
    return NOERROR;
}      

HRESULT
CRendererPosPassThru::RegisterMediaTime(LONGLONG StartTime,LONGLONG EndTime)
{
    CAutoLock cAutoLock(&m_PositionLock);
    m_StartMedia = StartTime;
    m_EndMedia = EndTime;
    m_bReset = FALSE;
    return NOERROR;
}      

HRESULT
CRendererPosPassThru::GetMediaTime(LONGLONG *pStartTime,LONGLONG *pEndTime)
{
    ASSERT(pStartTime);

    CAutoLock cAutoLock(&m_PositionLock);
    if (m_bReset == TRUE) {
	return E_FAIL;
    }

    

    HRESULT hr = ConvertTimeFormat( pStartTime, 0, m_StartMedia, &TIME_FORMAT_MEDIA_TIME );
    if (pEndTime && SUCCEEDED(hr)) {
	hr = ConvertTimeFormat( pEndTime, 0, m_EndMedia, &TIME_FORMAT_MEDIA_TIME );
    }
    return hr;
}      

HRESULT
CRendererPosPassThru::ResetMediaTime()
{
    CAutoLock cAutoLock(&m_PositionLock);
    m_StartMedia = 0;
    m_EndMedia = 0;
    m_bReset = TRUE;
    return NOERROR;
}      

HRESULT
CRendererPosPassThru::EOS()
{
    HRESULT hr;

    if ( m_bReset == TRUE ) hr = E_FAIL;
    else
    {
	LONGLONG llStop;
	if SUCCEEDED(hr=GetStopPosition(&llStop))
	{
	    CAutoLock cAutoLock(&m_PositionLock);
	    m_StartMedia =
	    m_EndMedia	 = llStop;
	}
    }
    return hr;
}    

CSourceSeeking::CSourceSeeking(
    const TCHAR * pName,
    LPUNKNOWN pUnk,
    HRESULT* phr,
    CCritSec * pLock) :
        CUnknown(pName, pUnk),
        m_pLock(pLock),
        m_rtStart((long)0)
{
    m_rtStop = _I64_MAX / 2;
    m_rtDuration = m_rtStop;
    m_dRateSeeking = 1.0;

    m_dwSeekingCaps = AM_SEEKING_CanSeekForwards
        | AM_SEEKING_CanSeekBackwards
        | AM_SEEKING_CanSeekAbsolute
        | AM_SEEKING_CanGetStopPos
        | AM_SEEKING_CanGetDuration;
}

HRESULT CSourceSeeking::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if(riid == IID_IMediaSeeking) {
        CheckPointer(ppv, E_POINTER);
        return GetInterface(static_cast<IMediaSeeking *>(this), ppv);
    }
    else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}  

HRESULT CSourceSeeking::IsFormatSupported(const GUID * pFormat)
{
    CheckPointer(pFormat, E_POINTER);
    
    return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : S_FALSE;
}

HRESULT CSourceSeeking::QueryPreferredFormat(GUID *pFormat)
{
    CheckPointer(pFormat, E_POINTER);
    *pFormat = TIME_FORMAT_MEDIA_TIME;
    return S_OK;
}

HRESULT CSourceSeeking::SetTimeFormat(const GUID * pFormat)
{
    CheckPointer(pFormat, E_POINTER);

    
    return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : E_INVALIDARG;
}

HRESULT CSourceSeeking::IsUsingTimeFormat(const GUID * pFormat)
{
    CheckPointer(pFormat, E_POINTER);
    return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : S_FALSE;
}

HRESULT CSourceSeeking::GetTimeFormat(GUID *pFormat)
{
    CheckPointer(pFormat, E_POINTER);
    *pFormat = TIME_FORMAT_MEDIA_TIME;
    return S_OK;
}

HRESULT CSourceSeeking::GetDuration(LONGLONG *pDuration)
{
    CheckPointer(pDuration, E_POINTER);
    CAutoLock lock(m_pLock);
    *pDuration = m_rtDuration;
    return S_OK;
}

HRESULT CSourceSeeking::GetStopPosition(LONGLONG *pStop)
{
    CheckPointer(pStop, E_POINTER);
    CAutoLock lock(m_pLock);
    *pStop = m_rtStop;
    return S_OK;
}

HRESULT CSourceSeeking::GetCurrentPosition(LONGLONG *pCurrent)
{
    
    
    return E_NOTIMPL;
}

HRESULT CSourceSeeking::GetCapabilities( DWORD * pCapabilities )
{
    CheckPointer(pCapabilities, E_POINTER);
    *pCapabilities = m_dwSeekingCaps;
    return S_OK;
}

HRESULT CSourceSeeking::CheckCapabilities( DWORD * pCapabilities )
{
    CheckPointer(pCapabilities, E_POINTER);

    
    return (~m_dwSeekingCaps & *pCapabilities) ? S_FALSE : S_OK;
}

HRESULT CSourceSeeking::ConvertTimeFormat( LONGLONG * pTarget, const GUID * pTargetFormat,
                           LONGLONG    Source, const GUID * pSourceFormat )
{
    CheckPointer(pTarget, E_POINTER);
    

    
    
    if(pTargetFormat == 0 || *pTargetFormat == TIME_FORMAT_MEDIA_TIME)
    {
        if(pSourceFormat == 0 || *pSourceFormat == TIME_FORMAT_MEDIA_TIME)
        {
            *pTarget = Source;
            return S_OK;
        }
    }

    return E_INVALIDARG;
}  

HRESULT CSourceSeeking::SetPositions( LONGLONG * pCurrent,  DWORD CurrentFlags
                      , LONGLONG * pStop,  DWORD StopFlags )
{
    DWORD StopPosBits = StopFlags & AM_SEEKING_PositioningBitsMask;
    DWORD StartPosBits = CurrentFlags & AM_SEEKING_PositioningBitsMask;

    if(StopFlags) {
        CheckPointer(pStop, E_POINTER);

        
        if(StopPosBits != StopFlags) {
            return E_INVALIDARG;
        }
    }

    if(CurrentFlags) {
        CheckPointer(pCurrent, E_POINTER);
        if(StartPosBits != AM_SEEKING_AbsolutePositioning &&
           StartPosBits != AM_SEEKING_RelativePositioning) {
            return E_INVALIDARG;
        }
    }  

    
    {
        CAutoLock lock(m_pLock);

        
        if(StartPosBits == AM_SEEKING_AbsolutePositioning)
        {
            m_rtStart = *pCurrent;
        }
        else if(StartPosBits == AM_SEEKING_RelativePositioning)
        {
            m_rtStart += *pCurrent;
        }

        
        if(StopPosBits == AM_SEEKING_AbsolutePositioning)
        {
            m_rtStop = *pStop;
        }
        else if(StopPosBits == AM_SEEKING_IncrementalPositioning)
        {
            m_rtStop = m_rtStart + *pStop;
        }
        else if(StopPosBits == AM_SEEKING_RelativePositioning)
        {
            m_rtStop = m_rtStop + *pStop;
        }
    }  

    HRESULT hr = S_OK;
    if(SUCCEEDED(hr) && StopPosBits) {
        hr = ChangeStop();
    }
    if(StartPosBits) {
        hr = ChangeStart();
    }

    return hr;
}  

HRESULT CSourceSeeking::GetPositions( LONGLONG * pCurrent, LONGLONG * pStop )
{
    if(pCurrent) {
        *pCurrent = m_rtStart;
    }
    if(pStop) {
        *pStop = m_rtStop;
    }

    return S_OK;;
}  

HRESULT CSourceSeeking::GetAvailable( LONGLONG * pEarliest, LONGLONG * pLatest )
{
    if(pEarliest) {
        *pEarliest = 0;
    }
    if(pLatest) {
        CAutoLock lock(m_pLock);
        *pLatest = m_rtDuration;
    }
    return S_OK;
}

HRESULT CSourceSeeking::SetRate( double dRate)
{
    {
        CAutoLock lock(m_pLock);
        m_dRateSeeking = dRate;
    }
    return ChangeRate();
}

HRESULT CSourceSeeking::GetRate( double * pdRate)
{
    CheckPointer(pdRate, E_POINTER);
    CAutoLock lock(m_pLock);
    *pdRate = m_dRateSeeking;
    return S_OK;
}

HRESULT CSourceSeeking::GetPreroll(LONGLONG *pPreroll)
{
    CheckPointer(pPreroll, E_POINTER);
    *pPreroll = 0;
    return S_OK;
}              

CSourcePosition::CSourcePosition(const TCHAR * pName,
				 LPUNKNOWN pUnk,
				 HRESULT* phr,
				 CCritSec * pLock) :
    CMediaPosition(pName, pUnk),
    m_pLock(pLock),
    m_Start(CRefTime((LONGLONG)0))
{
    m_Stop = _I64_MAX;
    m_Rate = 1.0;
}  

STDMETHODIMP
CSourcePosition::get_Duration(REFTIME * plength)
{
    CheckPointer(plength,E_POINTER);
    ValidateReadWritePtr(plength,sizeof(REFTIME));
    CAutoLock lock(m_pLock);

    *plength = m_Duration;
    return S_OK;
}  

STDMETHODIMP
CSourcePosition::put_CurrentPosition(REFTIME llTime)
{
    m_pLock->Lock();
    m_Start = llTime;
    m_pLock->Unlock();

    return ChangeStart();
}  

STDMETHODIMP
CSourcePosition::get_StopTime(REFTIME * pllTime)
{
    CheckPointer(pllTime,E_POINTER);
    ValidateReadWritePtr(pllTime,sizeof(REFTIME));
    CAutoLock lock(m_pLock);

    *pllTime = m_Stop;
    return S_OK;
}  

STDMETHODIMP
CSourcePosition::put_StopTime(REFTIME llTime)
{
    m_pLock->Lock();
    m_Stop = llTime;
    m_pLock->Unlock();

    return ChangeStop();
}  

STDMETHODIMP
CSourcePosition::get_PrerollTime(REFTIME * pllTime)
{
    CheckPointer(pllTime,E_POINTER);
    ValidateReadWritePtr(pllTime,sizeof(REFTIME));
    return E_NOTIMPL;
}  

STDMETHODIMP
CSourcePosition::put_PrerollTime(REFTIME llTime)
{
    return E_NOTIMPL;
}  

STDMETHODIMP
CSourcePosition::get_Rate(double * pdRate)
{
    CheckPointer(pdRate,E_POINTER);
    ValidateReadWritePtr(pdRate,sizeof(double));
    CAutoLock lock(m_pLock);

    *pdRate = m_Rate;
    return S_OK;
}  

STDMETHODIMP
CSourcePosition::put_Rate(double dRate)
{
    m_pLock->Lock();
    m_Rate = dRate;
    m_pLock->Unlock();

    return ChangeRate();
}      

STDMETHODIMP
CSourcePosition::CanSeekForward(LONG *pCanSeekForward)
{
    CheckPointer(pCanSeekForward,E_POINTER);
    *pCanSeekForward = OATRUE;
    return S_OK;
}      

STDMETHODIMP
CSourcePosition::CanSeekBackward(LONG *pCanSeekBackward)
{
    CheckPointer(pCanSeekBackward,E_POINTER);
    *pCanSeekBackward = OATRUE;
    return S_OK;
}        

CBasicAudio::CBasicAudio(const TCHAR * pName,LPUNKNOWN punk) :
    CUnknown(pName, punk)
{
}    

STDMETHODIMP
CBasicAudio::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IBasicAudio) {
	return GetInterface( (IBasicAudio *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}  

STDMETHODIMP
CBasicAudio::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}  

STDMETHODIMP
CBasicAudio::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IBasicAudio,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CBasicAudio::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IBasicAudio,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CBasicAudio::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IBasicAudio *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}      

CBaseVideoWindow::CBaseVideoWindow(const TCHAR * pName,LPUNKNOWN punk) :
    CUnknown(pName, punk)
{
}      

STDMETHODIMP
CBaseVideoWindow::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IVideoWindow) {
	return GetInterface( (IVideoWindow *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}  

STDMETHODIMP
CBaseVideoWindow::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}  

STDMETHODIMP
CBaseVideoWindow::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IVideoWindow,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CBaseVideoWindow::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IVideoWindow,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CBaseVideoWindow::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IVideoWindow *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}        

CBaseBasicVideo::CBaseBasicVideo(const TCHAR * pName,LPUNKNOWN punk) :
    CUnknown(pName, punk)
{
}      

STDMETHODIMP
CBaseBasicVideo::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IBasicVideo || riid == IID_IBasicVideo2) {
	return GetInterface( static_cast<IBasicVideo2 *>(this), ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}  

STDMETHODIMP
CBaseBasicVideo::GetTypeInfoCount(UINT * pctinfo)
{
    return m_basedisp.GetTypeInfoCount(pctinfo);
}  

STDMETHODIMP
CBaseBasicVideo::GetTypeInfo(
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    return m_basedisp.GetTypeInfo(
		IID_IBasicVideo,
		itinfo,
		lcid,
		pptinfo);
}  

STDMETHODIMP
CBaseBasicVideo::GetIDsOfNames(
  REFIID riid,
  OLECHAR  ** rgszNames,
  UINT cNames,
  LCID lcid,
  DISPID * rgdispid)
{
    return m_basedisp.GetIDsOfNames(
			IID_IBasicVideo,
			rgszNames,
			cNames,
			lcid,
			rgdispid);
}  

STDMETHODIMP
CBaseBasicVideo::Invoke(
  DISPID dispidMember,
  REFIID riid,
  LCID lcid,
  WORD wFlags,
  DISPPARAMS * pdispparams,
  VARIANT * pvarResult,
  EXCEPINFO * pexcepinfo,
  UINT * puArgErr)
{
    
    if (IID_NULL != riid) {
	return DISP_E_UNKNOWNINTERFACE;
    }

    ITypeInfo * pti;
    HRESULT hr = GetTypeInfo(0, lcid, &pti);

    if (FAILED(hr)) {
	return hr;
    }

    hr = pti->Invoke(
	    (IBasicVideo *)this,
	    dispidMember,
	    wFlags,
	    pdispparams,
	    pvarResult,
	    pexcepinfo,
	    puArgErr);

    pti->Release();
    return hr;
}        

CDispParams::CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT *phr)
{
   cNamedArgs = 0;
   rgdispidNamedArgs = NULL;
   cArgs = nArgs;

    if (cArgs) {
	rgvarg = new VARIANT[cArgs];
        if (NULL == rgvarg) {
            cArgs = 0;
            if (phr) {
                *phr = E_OUTOFMEMORY;
            }
            return;
        }

	for (UINT i = 0; i < cArgs; i++) {

	    VARIANT * pDest = &rgvarg[i];
	    VARIANT * pSrc = &pArgs[i];

	    pDest->vt = pSrc->vt;
	    switch(pDest->vt) {

	    case VT_I4:
		pDest->lVal = pSrc->lVal;
		break;

	    case VT_UI1:
		pDest->bVal = pSrc->bVal;
		break;

	    case VT_I2:
		pDest->iVal = pSrc->iVal;
		break;

	    case VT_R4:
		pDest->fltVal = pSrc->fltVal;
		break;

	    case VT_R8:
		pDest->dblVal = pSrc->dblVal;
		break;

	    case VT_BOOL:
		pDest->boolVal = pSrc->boolVal;
		break;

	    case VT_ERROR:
		pDest->scode = pSrc->scode;
		break;

	    case VT_CY:
		pDest->cyVal = pSrc->cyVal;
		break;

	    case VT_DATE:
		pDest->date = pSrc->date;
		break;

	    case VT_BSTR:
		if (pSrc->bstrVal == NULL) {
		    pDest->bstrVal = NULL;
		} else {

		    
		    

		    WORD len = * (WORD*) (pSrc->bstrVal - (sizeof(WORD) / sizeof(OLECHAR)));
		    OLECHAR* pch = new OLECHAR[len + (sizeof(WORD)/sizeof(OLECHAR))];
                    if (pch) {
        		WORD *pui = (WORD*)pch;
        		*pui = len;
         	        pDest->bstrVal = pch + (sizeof(WORD)/sizeof(OLECHAR));
         		CopyMemory(pDest->bstrVal, pSrc->bstrVal, len*sizeof(OLECHAR));
                    } else {
                        cArgs = i;
                        if (phr) {
                            *phr = E_OUTOFMEMORY;
                        }
                    }
		}
		pDest->bstrVal = pSrc->bstrVal;
		break;

	    case VT_UNKNOWN:
		pDest->punkVal = pSrc->punkVal;
		pDest->punkVal->AddRef();
		break;

	    case VT_DISPATCH:
		pDest->pdispVal = pSrc->pdispVal;
		pDest->pdispVal->AddRef();
		break;

	    default:
		
		ASSERT(0);
		break;
	    }
	}

    } else {
	rgvarg = NULL;
    }

}  

CDispParams::~CDispParams()
{
    for (UINT i = 0; i < cArgs; i++) {
	switch(rgvarg[i].vt) {
	case VT_BSTR:
	    if (rgvarg[i].bstrVal != NULL) {
		OLECHAR * pch = rgvarg[i].bstrVal - (sizeof(WORD)/sizeof(OLECHAR));
		delete pch;
	    }
	    break;

	case VT_UNKNOWN:
	    rgvarg[i].punkVal->Release();
	    break;

	case VT_DISPATCH:
	    rgvarg[i].pdispVal->Release();
	    break;
	}
    }
    delete[] rgvarg;
}      

CDeferredCommand::CDeferredCommand(
    CCmdQueue * pQ,
    LPUNKNOWN	pUnk,
    HRESULT *	phr,
    LPUNKNOWN	pUnkExecutor,
    REFTIME	time,
    GUID*	iid,
    long	dispidMethod,
    short	wFlags,
    long	nArgs,
    VARIANT*	pDispParams,
    VARIANT*	pvarResult,
    short*	puArgErr,
    BOOL	bStream
    ) :
	CUnknown(NAME("DeferredCommand"), pUnk),
	m_pQueue(pQ),
	m_pUnk(pUnkExecutor),
	m_iid(iid),
	m_dispidMethod(dispidMethod),
	m_wFlags(wFlags),
	m_DispParams(nArgs, pDispParams, phr),
	m_pvarResult(pvarResult),
	m_bStream(bStream),
	m_hrResult(E_ABORT)

{
    
    COARefTime convertor(time);
    m_time = convertor;

    
    

    
    IUnknown * pInterface;
    HRESULT hr = m_pUnk->QueryInterface(GetIID(), (void**) &pInterface);
    if (FAILED(hr)) {
	*phr = hr;
	return;
    }
    pInterface->Release();  

    
    ITypeInfo *pti;
    hr = m_Dispatch.GetTypeInfo(*iid, 0, 0, &pti);
    if (FAILED(hr)) {
	*phr = hr;
	return;
    }
    
    pti->Release();  

    
    if (wFlags == DISPATCH_PROPERTYPUT) {
        m_DispParams.cNamedArgs = 1;
        m_DispId = DISPID_PROPERTYPUT;
        m_DispParams.rgdispidNamedArgs = &m_DispId;
    }

    
    hr = pQ->Insert(this);
    if (FAILED(hr)) {
	*phr = hr;
    }
}        

#if 0
CDeferredCommand::~CDeferredCommand()
{
    
    
    
    

    
    
    
    
    
    
    
    

}
#endif      

STDMETHODIMP
CDeferredCommand::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    if (riid == IID_IDeferredCommand) {
	return GetInterface( (IDeferredCommand *) this, ppv);
    } else {
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}          

STDMETHODIMP
CDeferredCommand::Cancel()
{
    if (m_pQueue == NULL) {
	return VFW_E_ALREADY_CANCELLED;
    }

    HRESULT hr = m_pQueue->Remove(this);
    if (FAILED(hr)) {
	return hr;
    }

    m_pQueue = NULL;
    return S_OK;
}  

STDMETHODIMP
CDeferredCommand::Confidence(LONG* pConfidence)
{
    return E_NOTIMPL;
}  

STDMETHODIMP
CDeferredCommand::GetHResult(HRESULT * phrResult)
{
    CheckPointer(phrResult,E_POINTER);
    ValidateReadWritePtr(phrResult,sizeof(HRESULT));

    if (m_pQueue != NULL) {
	return E_ABORT;
    }
    *phrResult = m_hrResult;
    return S_OK;
}        

STDMETHODIMP
CDeferredCommand::Postpone(REFTIME newtime)
{

    
    
    COARefTime convertor(newtime);

    
    if (m_pQueue->CheckTime(convertor, IsStreamTime())) {
	return VFW_E_TIME_ALREADY_PASSED;
    }

    
    HRESULT hr = m_pQueue->Remove(this);
    if (FAILED(hr)) {
	return hr;
    }

    
    m_time = convertor;

    
    hr = m_pQueue->Insert(this);

    return hr;
}  

HRESULT
CDeferredCommand::Invoke()
{
    
    if (m_pQueue == NULL) {
	return VFW_E_ALREADY_CANCELLED;
    }

    
    ITypeInfo* pti;
    HRESULT hr = m_Dispatch.GetTypeInfo(GetIID(), 0, 0, &pti);
    if (FAILED(hr)) {
	return hr;
    }

    
    
    IUnknown* pInterface;

    hr = m_pUnk->QueryInterface(GetIID(), (void**) &pInterface);
    if (FAILED(hr)) {
	pti->Release();
	return hr;
    }

    EXCEPINFO expinfo;
    UINT uArgErr;
    m_hrResult = pti->Invoke(
	pInterface,
	GetMethod(),
	GetFlags(),
	GetParams(),
	GetResult(),
	&expinfo,
	&uArgErr);

    
    pInterface->Release();
    pti->Release();  

    
    
    hr = m_pQueue->Remove(this);
    m_pQueue = NULL;
    return hr;
}          

CCmdQueue::CCmdQueue() :
    m_listPresentation(NAME("Presentation time command list")),
    m_listStream(NAME("Stream time command list")),
    m_evDue(TRUE),    
    m_dwAdvise(0),
    m_pClock(NULL),
    m_bRunning(FALSE)
{
}  

CCmdQueue::~CCmdQueue()
{
    

    
    
    POSITION pos = m_listPresentation.GetHeadPosition();

    while(pos) {
	CDeferredCommand* pCmd = m_listPresentation.GetNext(pos);
	pCmd->Release();
    }
    m_listPresentation.RemoveAll();

    pos = m_listStream.GetHeadPosition();

    while(pos) {
	CDeferredCommand* pCmd = m_listStream.GetNext(pos);
	pCmd->Release();
    }
    m_listStream.RemoveAll();

    if (m_pClock) {
	if (m_dwAdvise) {
	    m_pClock->Unadvise(m_dwAdvise);
	    m_dwAdvise = 0;
	}
	m_pClock->Release();
    }
}            

HRESULT
CCmdQueue::New(
    CDeferredCommand **ppCmd,
    LPUNKNOWN	pUnk,		
    REFTIME	time,
    GUID*	iid,
    long	dispidMethod,
    short	wFlags,
    long	cArgs,
    VARIANT*	pDispParams,
    VARIANT*	pvarResult,
    short*	puArgErr,
    BOOL	bStream
)
{
    CAutoLock lock(&m_Lock);

    HRESULT hr = S_OK;
    *ppCmd = NULL;

    CDeferredCommand* pCmd;
    pCmd = new CDeferredCommand(
		    this,
		    NULL,	    
		    &hr,
		    pUnk,	    
		    time,
		    iid,
		    dispidMethod,
		    wFlags,
		    cArgs,
		    pDispParams,
		    pvarResult,
		    puArgErr,
		    bStream);

    if (pCmd == NULL) {
	hr = E_OUTOFMEMORY;
    } else {
	*ppCmd = pCmd;
    }
    return hr;
}  

HRESULT
CCmdQueue::Insert(CDeferredCommand* pCmd)
{
    CAutoLock lock(&m_Lock);

    
    pCmd->AddRef();

    CGenericList<CDeferredCommand> * pList;
    if (pCmd->IsStreamTime()) {
	pList = &m_listStream;
    } else {
	pList = &m_listPresentation;
    }
    POSITION pos = pList->GetHeadPosition();

    
    while (pos &&
	(pList->Get(pos)->GetTime() <= pCmd->GetTime())) {

	pList->GetNext(pos);
    }

    
    if (!pos) {
	pList->AddTail(pCmd);
    } else {
	pList->AddBefore(pos, pCmd);
    }

    SetTimeAdvise();
    return S_OK;
}  

HRESULT
CCmdQueue::Remove(CDeferredCommand* pCmd)
{
    CAutoLock lock(&m_Lock);
    HRESULT hr = S_OK;

    CGenericList<CDeferredCommand> * pList;
    if (pCmd->IsStreamTime()) {
	pList = &m_listStream;
    } else {
	pList = &m_listPresentation;
    }
    POSITION pos = pList->GetHeadPosition();

    
    while (pos && (pList->Get(pos) != pCmd)) {
	pList->GetNext(pos);
    }

    
    if (!pos) {
	hr = VFW_E_NOT_FOUND;
    } else {

	
	pList->Remove(pos);

	
	pCmd->Release();

	
	SetTimeAdvise();
    }
    return hr;
}      

HRESULT
CCmdQueue::SetSyncSource(IReferenceClock* pClock)
{
    CAutoLock lock(&m_Lock);

    
    if (pClock) {
	pClock->AddRef();
    }

    
    if (m_pClock) {
	if (m_dwAdvise) {
	    m_pClock->Unadvise(m_dwAdvise);
	    m_dwAdvise = 0;
	}
	m_pClock->Release();
    }
    m_pClock = pClock;

    
    SetTimeAdvise();
    return S_OK;
}      

void
CCmdQueue::SetTimeAdvise(void)
{
    
    if (!m_pClock) {
	return;
    }

    
    m_evDue.Reset();

    
    CRefTime current;

    
    if (m_listPresentation.GetCount() > 0) {

	POSITION pos = m_listPresentation.GetHeadPosition();
	current = m_listPresentation.Get(pos)->GetTime();
    }

    
    if (m_bRunning) {

	CRefTime t;

	if (m_listStream.GetCount() > 0) {

	    POSITION pos = m_listStream.GetHeadPosition();
	    t = m_listStream.Get(pos)->GetTime();

	    
	    t += m_StreamTimeOffset;

	    
	    if ((current == TimeZero) || (t < current)) {
		current = t;
	    }
	}
    }

    
    if ((current > TimeZero) && (current != m_tCurrentAdvise)) {
	if (m_dwAdvise) {
	    m_pClock->Unadvise(m_dwAdvise);
	    
	    m_evDue.Reset();
	}

	
	
	
	HRESULT hr = m_pClock->AdviseTime(
		    (REFERENCE_TIME)current,
		    TimeZero,
		    (HEVENT) HANDLE(m_evDue),
		    &m_dwAdvise);

	ASSERT(SUCCEEDED(hr));
	m_tCurrentAdvise = current;
    }
}      

HRESULT
CCmdQueue::Run(REFERENCE_TIME tStreamTimeOffset)
{
    CAutoLock lock(&m_Lock);

    m_StreamTimeOffset = tStreamTimeOffset;
    m_bRunning = TRUE;

    
    SetTimeAdvise();
    return S_OK;
}      

HRESULT
CCmdQueue::EndRun()
{
    CAutoLock lock(&m_Lock);

    m_bRunning = FALSE;

    
    SetTimeAdvise();
    return S_OK;
}                  

HRESULT
CCmdQueue::GetDueCommand(CDeferredCommand ** ppCmd, long msTimeout)
{
    
    for (;;) {

	{
	    CAutoLock lock(&m_Lock);  

	    
	    CDeferredCommand * pCmd = NULL;

	    
	    

	    if (m_listPresentation.GetCount() > 0) {
		POSITION pos = m_listPresentation.GetHeadPosition();
		pCmd = m_listPresentation.Get(pos);
	    }

	    if (m_bRunning && (m_listStream.GetCount() > 0)) {
		POSITION pos = m_listStream.GetHeadPosition();
		CDeferredCommand* pStrm = m_listStream.Get(pos);

		CRefTime t = pStrm->GetTime() + m_StreamTimeOffset;
		if (!pCmd || (t < pCmd->GetTime())) {
		    pCmd = pStrm;
		}
	    }

	    
	    if (pCmd) {
		if (CheckTime(pCmd->GetTime(), pCmd->IsStreamTime())) {

		    
		    pCmd->AddRef();
		    *ppCmd = pCmd;
		    return S_OK;
		}
	    }
	}

	
	if (WaitForSingleObject(m_evDue, msTimeout) != WAIT_OBJECT_0) {
	    return E_ABORT;
	}
    }
}                    

HRESULT
CCmdQueue::GetCommandDueFor(REFERENCE_TIME rtStream, CDeferredCommand**ppCmd)
{
    CAutoLock lock(&m_Lock);

    CRefTime tStream(rtStream);

    
    CDeferredCommand* pStream = NULL;
    if (m_listStream.GetCount() > 0) {
	POSITION pos = m_listStream.GetHeadPosition();
	pStream = m_listStream.Get(pos);
    }
    CDeferredCommand* pPresent = NULL;
    if (m_listPresentation.GetCount() > 0) {
	POSITION pos = m_listPresentation.GetHeadPosition();
	pPresent = m_listPresentation.Get(pos);
    }

    
    if (pPresent && CheckTime(pPresent->GetTime(), FALSE)) {
	pPresent->AddRef();
	*ppCmd = pPresent;
	return S_OK;
    }

    
    if (pStream && (pStream->GetTime() <= tStream)) {
	pPresent->AddRef();
	*ppCmd = pStream;
	return S_OK;
    }

    
    
    
    if (m_bRunning && pPresent) {

	
	tStream += m_StreamTimeOffset;

	
	if (pPresent->GetTime() <= tStream) {
	    *ppCmd = pPresent;
	    return S_OK;
	}
    }

    
    return VFW_E_NOT_FOUND;
}

