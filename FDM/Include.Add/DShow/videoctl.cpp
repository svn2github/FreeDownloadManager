/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#include <streams.h>
#include "ddmm.h"          

TCHAR *WINAPI StringFromResource(TCHAR *pBuffer, int iResourceID)
{
    if (LoadString(g_hInst,iResourceID,pBuffer,STR_MAX_LENGTH) == 0) {
        return TEXT("");
    }
    return pBuffer;
}

#ifdef UNICODE
char *WINAPI StringFromResource(char *pBuffer, int iResourceID)
{
    if (LoadStringA(g_hInst,iResourceID,pBuffer,STR_MAX_LENGTH) == 0) {
        return "";
    }
    return pBuffer;
}
#endif                  

#ifndef UNICODE

WCHAR * WINAPI WideStringFromResource(WCHAR *pBuffer, int iResourceID)
{
    *pBuffer = 0;

    if (g_amPlatform == VER_PLATFORM_WIN32_NT) {
	LoadStringW(g_hInst,iResourceID,pBuffer,STR_MAX_LENGTH);
    } else {

	CHAR szBuffer[STR_MAX_LENGTH];
	DWORD dwStringLength = LoadString(g_hInst,iResourceID,szBuffer,STR_MAX_LENGTH);
	
	
	if (dwStringLength++) {
	    MultiByteToWideChar(CP_ACP,0,szBuffer,dwStringLength,pBuffer,STR_MAX_LENGTH);
	}
    }
    return pBuffer;
}

#endif      

BOOL WINAPI GetDialogSize(int iResourceID,
                          DLGPROC pDlgProc,
                          LPARAM lParam,
                          SIZE *pResult)
{
    RECT rc;
    HWND hwnd;

    

    hwnd = CreateDialogParam(g_hInst,
                             MAKEINTRESOURCE(iResourceID),
                             GetDesktopWindow(),
                             pDlgProc,
                             lParam);
    if (hwnd == NULL) {
        return FALSE;
    }

    GetWindowRect(hwnd, &rc);
    pResult->cx = rc.right - rc.left;
    pResult->cy = rc.bottom - rc.top;

    DestroyWindow(hwnd);
    return TRUE;
}                  

STDMETHODIMP CAggDirectDraw::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ASSERT(m_pDirectDraw);

    

    if (riid == IID_IDirectDraw) {
        return GetInterface((IDirectDraw *)this,ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid,ppv);
    }
}  

STDMETHODIMP CAggDirectDraw::Compact()
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->Compact();
}  

STDMETHODIMP CAggDirectDraw::CreateClipper(DWORD dwFlags,LPDIRECTDRAWCLIPPER *lplpDDClipper,IUnknown *pUnkOuter)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->CreateClipper(dwFlags,lplpDDClipper,pUnkOuter);
}  

STDMETHODIMP CAggDirectDraw::CreatePalette(DWORD dwFlags,LPPALETTEENTRY lpColorTable,LPDIRECTDRAWPALETTE *lplpDDPalette,IUnknown *pUnkOuter)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->CreatePalette(dwFlags,lpColorTable,lplpDDPalette,pUnkOuter);
}  

STDMETHODIMP CAggDirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,LPDIRECTDRAWSURFACE *lplpDDSurface,IUnknown *pUnkOuter)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->CreateSurface(lpDDSurfaceDesc,lplpDDSurface,pUnkOuter);
}  

STDMETHODIMP CAggDirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,LPDIRECTDRAWSURFACE *lplpDupDDSurface)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->DuplicateSurface(lpDDSurface,lplpDupDDSurface);
}  

STDMETHODIMP CAggDirectDraw::EnumDisplayModes(DWORD dwSurfaceDescCount,LPDDSURFACEDESC lplpDDSurfaceDescList,LPVOID lpContext,LPDDENUMMODESCALLBACK lpEnumCallback)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->EnumDisplayModes(dwSurfaceDescCount,lplpDDSurfaceDescList,lpContext,lpEnumCallback);
}  

STDMETHODIMP CAggDirectDraw::EnumSurfaces(DWORD dwFlags,LPDDSURFACEDESC lpDDSD,LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpEnumCallback)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->EnumSurfaces(dwFlags,lpDDSD,lpContext,lpEnumCallback);
}  

STDMETHODIMP CAggDirectDraw::FlipToGDISurface()
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->FlipToGDISurface();
}  

STDMETHODIMP CAggDirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps,LPDDCAPS lpDDHELCaps)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetCaps(lpDDDriverCaps,lpDDHELCaps);
}  

STDMETHODIMP CAggDirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetDisplayMode(lpDDSurfaceDesc);
}  

STDMETHODIMP CAggDirectDraw::GetFourCCCodes(LPDWORD lpNumCodes,LPDWORD lpCodes)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetFourCCCodes(lpNumCodes,lpCodes);
}  

STDMETHODIMP CAggDirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE *lplpGDIDDSurface)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetGDISurface(lplpGDIDDSurface);
}  

STDMETHODIMP CAggDirectDraw::GetMonitorFrequency(LPDWORD lpdwFrequency)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetMonitorFrequency(lpdwFrequency);
}  

STDMETHODIMP CAggDirectDraw::GetScanLine(LPDWORD lpdwScanLine)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetScanLine(lpdwScanLine);
}  

STDMETHODIMP CAggDirectDraw::GetVerticalBlankStatus(LPBOOL lpblsInVB)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->GetVerticalBlankStatus(lpblsInVB);
}  

STDMETHODIMP CAggDirectDraw::Initialize(GUID *lpGUID)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->Initialize(lpGUID);
}  

STDMETHODIMP CAggDirectDraw::RestoreDisplayMode()
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->RestoreDisplayMode();
}  

STDMETHODIMP CAggDirectDraw::SetCooperativeLevel(HWND hWnd,DWORD dwFlags)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->SetCooperativeLevel(hWnd,dwFlags);
}  

STDMETHODIMP CAggDirectDraw::SetDisplayMode(DWORD dwWidth,DWORD dwHeight,DWORD dwBpp)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->SetDisplayMode(dwWidth,dwHeight,dwBpp);
}  

STDMETHODIMP CAggDirectDraw::WaitForVerticalBlank(DWORD dwFlags,HANDLE hEvent)
{
    ASSERT(m_pDirectDraw);
    return m_pDirectDraw->WaitForVerticalBlank(dwFlags,hEvent);
}                  

STDMETHODIMP CAggDrawSurface::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    ASSERT(m_pDirectDrawSurface);

    

    if (riid == IID_IDirectDrawSurface) {
        return GetInterface((IDirectDrawSurface *)this,ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid,ppv);
    }
}  

STDMETHODIMP CAggDrawSurface::AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->AddAttachedSurface(lpDDSAttachedSurface);
}  

STDMETHODIMP CAggDrawSurface::AddOverlayDirtyRect(LPRECT lpRect)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->AddOverlayDirtyRect(lpRect);
}  

STDMETHODIMP CAggDrawSurface::Blt(LPRECT lpDestRect,LPDIRECTDRAWSURFACE lpDDSrcSurface,LPRECT lpSrcRect,DWORD dwFlags,LPDDBLTFX lpDDBltFx)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Blt(lpDestRect,lpDDSrcSurface,lpSrcRect,dwFlags,lpDDBltFx);
}  

STDMETHODIMP CAggDrawSurface::BltBatch(LPDDBLTBATCH lpDDBltBatch,DWORD dwCount,DWORD dwFlags)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->BltBatch(lpDDBltBatch,dwCount,dwFlags);
}  

STDMETHODIMP CAggDrawSurface::BltFast(DWORD dwX,DWORD dwY,LPDIRECTDRAWSURFACE lpDDSrcSurface,LPRECT lpSrcRect,DWORD dwTrans)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->BltFast(dwX,dwY,lpDDSrcSurface,lpSrcRect,dwTrans);
}  

STDMETHODIMP CAggDrawSurface::DeleteAttachedSurface(DWORD dwFlags,LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->DeleteAttachedSurface(dwFlags,lpDDSAttachedSurface);
}  

STDMETHODIMP CAggDrawSurface::EnumAttachedSurfaces(LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->EnumAttachedSurfaces(lpContext,lpEnumSurfacesCallback);
}  

STDMETHODIMP CAggDrawSurface::EnumOverlayZOrders(DWORD dwFlags,LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpfnCallback)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->EnumOverlayZOrders(dwFlags,lpContext,lpfnCallback);
}  

STDMETHODIMP CAggDrawSurface::Flip(LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride,DWORD dwFlags)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Flip(lpDDSurfaceTargetOverride,dwFlags);
}  

STDMETHODIMP CAggDrawSurface::GetAttachedSurface(LPDDSCAPS lpDDSCaps,LPDIRECTDRAWSURFACE *lplpDDAttachedSurface)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetAttachedSurface(lpDDSCaps,lplpDDAttachedSurface);
}  

STDMETHODIMP CAggDrawSurface::GetBltStatus(DWORD dwFlags)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetBltStatus(dwFlags);
}  

STDMETHODIMP CAggDrawSurface::GetCaps(LPDDSCAPS lpDDSCaps)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetCaps(lpDDSCaps);
}  

STDMETHODIMP CAggDrawSurface::GetClipper(LPDIRECTDRAWCLIPPER *lplpDDClipper)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetClipper(lplpDDClipper);
}  

STDMETHODIMP CAggDrawSurface::GetColorKey(DWORD dwFlags,LPDDCOLORKEY lpDDColorKey)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetColorKey(dwFlags,lpDDColorKey);
}  

STDMETHODIMP CAggDrawSurface::GetDC(HDC *lphDC)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetDC(lphDC);
}  

STDMETHODIMP CAggDrawSurface::GetFlipStatus(DWORD dwFlags)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetFlipStatus(dwFlags);
}  

STDMETHODIMP CAggDrawSurface::GetOverlayPosition(LPLONG lpdwX,LPLONG lpdwY)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetOverlayPosition(lpdwX,lpdwY);
}  

STDMETHODIMP CAggDrawSurface::GetPalette(LPDIRECTDRAWPALETTE *lplpDDPalette)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetPalette(lplpDDPalette);
}  

STDMETHODIMP CAggDrawSurface::GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->GetPixelFormat(lpDDPixelFormat);
}                      

STDMETHODIMP CAggDrawSurface::GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    ASSERT(m_pDirectDrawSurface);

    

    HRESULT hr = m_pDirectDrawSurface->GetSurfaceDesc(lpDDSurfaceDesc);
    if (FAILED(hr)) {
        return hr;
    }

    
    lpDDSurfaceDesc->lpSurface = m_pDirectDrawSurface;
    return hr;
}  

STDMETHODIMP CAggDrawSurface::Initialize(LPDIRECTDRAW lpDD,LPDDSURFACEDESC lpDDSurfaceDesc)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Initialize(lpDD,lpDDSurfaceDesc);
}  

STDMETHODIMP CAggDrawSurface::IsLost()
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->IsLost();
}  

STDMETHODIMP CAggDrawSurface::Lock(LPRECT lpDestRect,LPDDSURFACEDESC lpDDSurfaceDesc,DWORD dwFlags,HANDLE hEvent)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Lock(lpDestRect,lpDDSurfaceDesc,dwFlags,hEvent);
}  

STDMETHODIMP CAggDrawSurface::ReleaseDC(HDC hDC)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->ReleaseDC(hDC);
}  

STDMETHODIMP CAggDrawSurface::Restore()
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Restore();
}  

STDMETHODIMP CAggDrawSurface::SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->SetClipper(lpDDClipper);
}  

STDMETHODIMP CAggDrawSurface::SetColorKey(DWORD dwFlags,LPDDCOLORKEY lpDDColorKey)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->SetColorKey(dwFlags,lpDDColorKey);
}  

STDMETHODIMP CAggDrawSurface::SetOverlayPosition(LONG dwX,LONG dwY)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->SetOverlayPosition(dwX,dwY);
}  

STDMETHODIMP CAggDrawSurface::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->SetPalette(lpDDPalette);
}  

STDMETHODIMP CAggDrawSurface::Unlock(LPVOID lpSurfaceData)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->Unlock(lpSurfaceData);
}  

STDMETHODIMP CAggDrawSurface::UpdateOverlay(LPRECT lpSrcRect,LPDIRECTDRAWSURFACE lpDDDestSurface,LPRECT lpDestRect,DWORD dwFlags,LPDDOVERLAYFX lpDDOverlayFX)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->UpdateOverlay(lpSrcRect,lpDDDestSurface,lpDestRect,dwFlags,lpDDOverlayFX);
}  

STDMETHODIMP CAggDrawSurface::UpdateOverlayDisplay(DWORD dwFlags)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->UpdateOverlayDisplay(dwFlags);
}  

STDMETHODIMP CAggDrawSurface::UpdateOverlayZOrder(DWORD dwFlags,LPDIRECTDRAWSURFACE lpDDSReference)
{
    ASSERT(m_pDirectDrawSurface);
    return m_pDirectDrawSurface->UpdateOverlayZOrder(dwFlags,lpDDSReference);
}              

CLoadDirectDraw::CLoadDirectDraw() :
    m_pDirectDraw(NULL),
    m_hDirectDraw(NULL)
{
}      

CLoadDirectDraw::~CLoadDirectDraw()
{
    ReleaseDirectDraw();

    if (m_hDirectDraw) {
        NOTE("Unloading library");
        FreeLibrary(m_hDirectDraw);
    }
}                      

HRESULT CLoadDirectDraw::LoadDirectDraw(LPSTR szDevice)
{
    PDRAWCREATE pDrawCreate;
    PDRAWENUM pDrawEnum;
    LPDIRECTDRAWENUMERATEEXA pDrawEnumEx;
    HRESULT hr = NOERROR;

    NOTE("Entering DoLoadDirectDraw");

    

    if (m_pDirectDraw) {
        NOTE("Already loaded");
        ASSERT(m_hDirectDraw);
        return NOERROR;
    }

    

    if(!m_hDirectDraw)
    {
        UINT ErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX);
        m_hDirectDraw = LoadLibrary(TEXT("DDRAW.DLL"));
        SetErrorMode(ErrorMode);

        if (m_hDirectDraw == NULL) {
            DbgLog((LOG_ERROR,1,TEXT("Can't load DDRAW.DLL")));
            NOTE("No library");
            return E_NOINTERFACE;
        }
    }

    

    pDrawCreate = (PDRAWCREATE)GetProcAddress(m_hDirectDraw,"DirectDrawCreate");
    
    pDrawEnum = (PDRAWENUM)GetProcAddress(m_hDirectDraw,"DirectDrawEnumerateA");
    pDrawEnumEx = (LPDIRECTDRAWENUMERATEEXA)GetProcAddress(m_hDirectDraw,
						"DirectDrawEnumerateExA");

    
    if (pDrawCreate == NULL || pDrawEnum == NULL) {
        DbgLog((LOG_ERROR,1,TEXT("Can't get functions: Create=%x Enum=%x"),
			pDrawCreate, pDrawEnum));
        NOTE("No entry point");
        ReleaseDirectDraw();
        return E_NOINTERFACE;
    }

    DbgLog((LOG_TRACE,3,TEXT("Creating DDraw for device %s"),
					szDevice ? szDevice : "<NULL>"));

    
    
    if (pDrawEnumEx)
        m_pDirectDraw = DirectDrawCreateFromDeviceEx(szDevice, pDrawCreate,
								pDrawEnumEx);
    else
        m_pDirectDraw = DirectDrawCreateFromDevice(szDevice, pDrawCreate,
								pDrawEnum);

    if (m_pDirectDraw == NULL) {
            DbgLog((LOG_ERROR,1,TEXT("Can't create DDraw")));
            NOTE("No instance");
            ReleaseDirectDraw();
            return E_NOINTERFACE;
    }
    return NOERROR;
}              

void CLoadDirectDraw::ReleaseDirectDraw()
{
    NOTE("Releasing DirectDraw driver");

    

    if (m_pDirectDraw) {
        NOTE("Releasing instance");
        m_pDirectDraw->Release();
        m_pDirectDraw = NULL;
    }

}      

HRESULT CLoadDirectDraw::IsDirectDrawLoaded()
{
    NOTE("Entering IsDirectDrawLoaded");

    if (m_pDirectDraw == NULL) {
        NOTE("DirectDraw not loaded");
        return S_FALSE;
    }
    return NOERROR;
}      

LPDIRECTDRAW CLoadDirectDraw::GetDirectDraw()
{
    NOTE("Entering GetDirectDraw");

    if (m_pDirectDraw == NULL) {
        NOTE("No DirectDraw");
        return NULL;
    }

    NOTE("Returning DirectDraw");
    m_pDirectDraw->AddRef();
    return m_pDirectDraw;
}            

BOOL CLoadDirectDraw::IsDirectDrawVersion1()
{

    if (m_pDirectDraw == NULL)
	return FALSE;

    IDirectDraw2 *p = NULL;
    HRESULT hr = m_pDirectDraw->QueryInterface(IID_IDirectDraw2, (void **)&p);
    if (p)
	p->Release();
    if (hr == NOERROR) {
        DbgLog((LOG_TRACE,3,TEXT("Direct Draw Version 2 or greater")));
	return FALSE;
    } else {
        DbgLog((LOG_TRACE,3,TEXT("Direct Draw Version 1")));
	return TRUE;
    }
}
