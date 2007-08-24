/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#ifndef __VIDEOCTL__
#define __VIDEOCTL__            

#define STR_MAX_LENGTH 256
TCHAR * WINAPI StringFromResource(TCHAR *pBuffer, int iResourceID);

#ifdef UNICODE
#define WideStringFromResource StringFromResource
char* WINAPI StringFromResource(char*pBuffer, int iResourceID);
#else
WCHAR * WINAPI WideStringFromResource(WCHAR *pBuffer, int iResourceID);
#endif  

BOOL WINAPI GetDialogSize(int iResourceID,     
                          DLGPROC pDlgProc,    
                          LPARAM lParam,       
                          SIZE *pResult);          

class CAggDirectDraw : public IDirectDraw, public CUnknown
{
protected:

    LPDIRECTDRAW m_pDirectDraw;

public:

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void **ppv);

    

    CAggDirectDraw(TCHAR *pName,LPUNKNOWN pUnk) :
        CUnknown(pName,pUnk),
        m_pDirectDraw(NULL) { };

    virtual CAggDirectDraw::~CAggDirectDraw() { };

    
    void SetDirectDraw(LPDIRECTDRAW pDirectDraw) {
        m_pDirectDraw = pDirectDraw;
    }

    

    STDMETHODIMP Compact();
    STDMETHODIMP CreateClipper(DWORD dwFlags,LPDIRECTDRAWCLIPPER *lplpDDClipper,IUnknown *pUnkOuter);
    STDMETHODIMP CreatePalette(DWORD dwFlags,LPPALETTEENTRY lpColorTable,LPDIRECTDRAWPALETTE *lplpDDPalette,IUnknown *pUnkOuter);
    STDMETHODIMP CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,LPDIRECTDRAWSURFACE *lplpDDSurface,IUnknown *pUnkOuter);
    STDMETHODIMP DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface,LPDIRECTDRAWSURFACE *lplpDupDDSurface);
    STDMETHODIMP EnumDisplayModes(DWORD dwSurfaceDescCount,LPDDSURFACEDESC lplpDDSurfaceDescList,LPVOID lpContext,LPDDENUMMODESCALLBACK lpEnumCallback);
    STDMETHODIMP EnumSurfaces(DWORD dwFlags,LPDDSURFACEDESC lpDDSD,LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpEnumCallback);
    STDMETHODIMP FlipToGDISurface();
    STDMETHODIMP GetCaps(LPDDCAPS lpDDDriverCaps,LPDDCAPS lpDDHELCaps);
    STDMETHODIMP GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP GetFourCCCodes(LPDWORD lpNumCodes,LPDWORD lpCodes);
    STDMETHODIMP GetGDISurface(LPDIRECTDRAWSURFACE *lplpGDIDDSurface);
    STDMETHODIMP GetMonitorFrequency(LPDWORD lpdwFrequency);
    STDMETHODIMP GetScanLine(LPDWORD lpdwScanLine);
    STDMETHODIMP GetVerticalBlankStatus(LPBOOL lpblsInVB);
    STDMETHODIMP Initialize(GUID *lpGUID);
    STDMETHODIMP RestoreDisplayMode();
    STDMETHODIMP SetCooperativeLevel(HWND hWnd,DWORD dwFlags);
    STDMETHODIMP SetDisplayMode(DWORD dwWidth,DWORD dwHeight,DWORD dwBpp);
    STDMETHODIMP WaitForVerticalBlank(DWORD dwFlags,HANDLE hEvent);
};      

class CAggDrawSurface : public IDirectDrawSurface, public CUnknown
{
protected:

    LPDIRECTDRAWSURFACE m_pDirectDrawSurface;

public:

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void **ppv);

    

    CAggDrawSurface(TCHAR *pName,LPUNKNOWN pUnk) :
        CUnknown(pName,pUnk),
        m_pDirectDrawSurface(NULL) { };

    virtual ~CAggDrawSurface() { };

    
    void SetDirectDrawSurface(LPDIRECTDRAWSURFACE pDirectDrawSurface) {
        m_pDirectDrawSurface = pDirectDrawSurface;
    }

    

    STDMETHODIMP AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSAttachedSurface);
    STDMETHODIMP AddOverlayDirtyRect(LPRECT lpRect);
    STDMETHODIMP Blt(LPRECT lpDestRect,LPDIRECTDRAWSURFACE lpDDSrcSurface,LPRECT lpSrcRect,DWORD dwFlags,LPDDBLTFX lpDDBltFx);
    STDMETHODIMP BltBatch(LPDDBLTBATCH lpDDBltBatch,DWORD dwCount,DWORD dwFlags);
    STDMETHODIMP BltFast(DWORD dwX,DWORD dwY,LPDIRECTDRAWSURFACE lpDDSrcSurface,LPRECT lpSrcRect,DWORD dwTrans);
    STDMETHODIMP DeleteAttachedSurface(DWORD dwFlags,LPDIRECTDRAWSURFACE lpDDSAttachedSurface);
    STDMETHODIMP EnumAttachedSurfaces(LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);
    STDMETHODIMP EnumOverlayZOrders(DWORD dwFlags,LPVOID lpContext,LPDDENUMSURFACESCALLBACK lpfnCallback);
    STDMETHODIMP Flip(LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride,DWORD dwFlags);
    STDMETHODIMP GetAttachedSurface(LPDDSCAPS lpDDSCaps,LPDIRECTDRAWSURFACE *lplpDDAttachedSurface);
    STDMETHODIMP GetBltStatus(DWORD dwFlags);
    STDMETHODIMP GetCaps(LPDDSCAPS lpDDSCaps);
    STDMETHODIMP GetClipper(LPDIRECTDRAWCLIPPER *lplpDDClipper);
    STDMETHODIMP GetColorKey(DWORD dwFlags,LPDDCOLORKEY lpDDColorKey);
    STDMETHODIMP GetDC(HDC *lphDC);
    STDMETHODIMP GetFlipStatus(DWORD dwFlags);
    STDMETHODIMP GetOverlayPosition(LPLONG lpdwX,LPLONG lpdwY);
    STDMETHODIMP GetPalette(LPDIRECTDRAWPALETTE *lplpDDPalette);
    STDMETHODIMP GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat);
    STDMETHODIMP GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP Initialize(LPDIRECTDRAW lpDD,LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP IsLost();
    STDMETHODIMP Lock(LPRECT lpDestRect,LPDDSURFACEDESC lpDDSurfaceDesc,DWORD dwFlags,HANDLE hEvent);
    STDMETHODIMP ReleaseDC(HDC hDC);
    STDMETHODIMP Restore();
    STDMETHODIMP SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper);
    STDMETHODIMP SetColorKey(DWORD dwFlags,LPDDCOLORKEY lpDDColorKey);
    STDMETHODIMP SetOverlayPosition(LONG dwX,LONG dwY);
    STDMETHODIMP SetPalette(LPDIRECTDRAWPALETTE lpDDPalette);
    STDMETHODIMP Unlock(LPVOID lpSurfaceData);
    STDMETHODIMP UpdateOverlay(LPRECT lpSrcRect,LPDIRECTDRAWSURFACE lpDDDestSurface,LPRECT lpDestRect,DWORD dwFlags,LPDDOVERLAYFX lpDDOverlayFX);
    STDMETHODIMP UpdateOverlayDisplay(DWORD dwFlags);
    STDMETHODIMP UpdateOverlayZOrder(DWORD dwFlags,LPDIRECTDRAWSURFACE lpDDSReference);
};              

typedef DWORD (WINAPI *PGETFILEVERSIONINFOSIZE)(LPTSTR,LPDWORD);
typedef BOOL (WINAPI *PGETFILEVERSIONINFO)(LPTSTR,DWORD,DWORD,LPVOID);
typedef BOOL (WINAPI *PVERQUERYVALUE)(LPVOID,LPTSTR,LPVOID,PUINT);

class CLoadDirectDraw
{
    LPDIRECTDRAW m_pDirectDraw;     
    HINSTANCE m_hDirectDraw;        

public:

    CLoadDirectDraw();
    ~CLoadDirectDraw();

    HRESULT LoadDirectDraw(LPSTR szDevice);
    void ReleaseDirectDraw();
    HRESULT IsDirectDrawLoaded();
    LPDIRECTDRAW GetDirectDraw();
    BOOL IsDirectDrawVersion1();
};

#endif 

