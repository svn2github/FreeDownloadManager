/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#ifndef __WINCTRL__
#define __WINCTRL__

#define ABSOL(x) (x < 0 ? -x : x)
#define NEGAT(x) (x > 0 ? -x : x)  

BOOL WINAPI PossiblyEatMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CBaseControlWindow : public CBaseVideoWindow, public CBaseWindow
{
protected:

    CBaseFilter *m_pFilter;            
    CBasePin *m_pPin;                  
    CCritSec *m_pInterfaceLock;        
    COLORREF m_BorderColour;           
    BOOL m_bAutoShow;                  
    HWND m_hwndOwner;                  
    HWND m_hwndDrain;                  
    BOOL m_bCursorHidden;              

public:

    

    HRESULT DoSetWindowStyle(long Style,long WindowLong);
    HRESULT DoGetWindowStyle(long *pStyle,long WindowLong);
    BOOL IsAutoShowEnabled() { return m_bAutoShow; };
    COLORREF GetBorderColour() { return m_BorderColour; };
    HWND GetOwnerWindow() { return m_hwndOwner; };
    BOOL IsCursorHidden() { return m_bCursorHidden; };

    inline BOOL PossiblyEatMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return ::PossiblyEatMessage(m_hwndDrain, uMsg, wParam, lParam);
    }

    
    
    
    
    

    void SetControlWindowPin(CBasePin *pPin) {
        m_pPin = pPin;
    }

public:

    CBaseControlWindow(CBaseFilter *pFilter,   
                       CCritSec *pInterfaceLock,    
                       TCHAR *pName,                
                       LPUNKNOWN pUnk,              
                       HRESULT *phr);               

    

    STDMETHODIMP put_Caption(BSTR strCaption);
    STDMETHODIMP get_Caption(BSTR *pstrCaption);
    STDMETHODIMP put_AutoShow(long AutoShow);
    STDMETHODIMP get_AutoShow(long *AutoShow);
    STDMETHODIMP put_WindowStyle(long WindowStyle);
    STDMETHODIMP get_WindowStyle(long *pWindowStyle);
    STDMETHODIMP put_WindowStyleEx(long WindowStyleEx);
    STDMETHODIMP get_WindowStyleEx(long *pWindowStyleEx);
    STDMETHODIMP put_WindowState(long WindowState);
    STDMETHODIMP get_WindowState(long *pWindowState);
    STDMETHODIMP put_BackgroundPalette(long BackgroundPalette);
    STDMETHODIMP get_BackgroundPalette(long *pBackgroundPalette);
    STDMETHODIMP put_Visible(long Visible);
    STDMETHODIMP get_Visible(long *pVisible);
    STDMETHODIMP put_Left(long Left);
    STDMETHODIMP get_Left(long *pLeft);
    STDMETHODIMP put_Width(long Width);
    STDMETHODIMP get_Width(long *pWidth);
    STDMETHODIMP put_Top(long Top);
    STDMETHODIMP get_Top(long *pTop);
    STDMETHODIMP put_Height(long Height);
    STDMETHODIMP get_Height(long *pHeight);
    STDMETHODIMP put_Owner(OAHWND Owner);
    STDMETHODIMP get_Owner(OAHWND *Owner);
    STDMETHODIMP put_MessageDrain(OAHWND Drain);
    STDMETHODIMP get_MessageDrain(OAHWND *Drain);
    STDMETHODIMP get_BorderColor(long *Color);
    STDMETHODIMP put_BorderColor(long Color);
    STDMETHODIMP get_FullScreenMode(long *FullScreenMode);
    STDMETHODIMP put_FullScreenMode(long FullScreenMode);

    

    STDMETHODIMP SetWindowForeground(long Focus);
    STDMETHODIMP NotifyOwnerMessage(OAHWND hwnd,long uMsg,LONG_PTR wParam,LONG_PTR lParam);
    STDMETHODIMP GetMinIdealImageSize(long *pWidth,long *pHeight);
    STDMETHODIMP GetMaxIdealImageSize(long *pWidth,long *pHeight);
    STDMETHODIMP SetWindowPosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetWindowPosition(long *pLeft,long *pTop,long *pWidth,long *pHeight);
    STDMETHODIMP GetRestorePosition(long *pLeft,long *pTop,long *pWidth,long *pHeight);
	STDMETHODIMP HideCursor(long HideCursor);
    STDMETHODIMP IsCursorHidden(long *CursorHidden);
};    

class CBaseControlVideo : public CBaseBasicVideo
{
protected:

    CBaseFilter *m_pFilter;   
    CBasePin *m_pPin;                   
    CCritSec *m_pInterfaceLock;         

public:

    

    virtual HRESULT IsDefaultTargetRect() PURE;
    virtual HRESULT SetDefaultTargetRect() PURE;
    virtual HRESULT SetTargetRect(RECT *pTargetRect) PURE;
    virtual HRESULT GetTargetRect(RECT *pTargetRect) PURE;
    virtual HRESULT IsDefaultSourceRect() PURE;
    virtual HRESULT SetDefaultSourceRect() PURE;
    virtual HRESULT SetSourceRect(RECT *pSourceRect) PURE;
    virtual HRESULT GetSourceRect(RECT *pSourceRect) PURE;
    virtual HRESULT GetStaticImage(long *pBufferSize,long *pDIBImage) PURE;

    
    
    
    
    
    

    virtual VIDEOINFOHEADER *GetVideoFormat() PURE;

    

    HRESULT GetImageSize(VIDEOINFOHEADER *pVideoInfo,
                         LONG *pBufferSize,
                         RECT *pSourceRect);

    HRESULT CopyImage(IMediaSample *pMediaSample,
                      VIDEOINFOHEADER *pVideoInfo,
                      LONG *pBufferSize,
                      BYTE *pVideoImage,
                      RECT *pSourceRect);

    
    virtual HRESULT OnUpdateRectangles() { return NOERROR; };
    virtual HRESULT OnVideoSizeChange();

    
    
    
    
    

    void SetControlVideoPin(CBasePin *pPin) {
        m_pPin = pPin;
    }

    
    virtual HRESULT CheckSourceRect(RECT *pSourceRect);
    virtual HRESULT CheckTargetRect(RECT *pTargetRect);

public:

    CBaseControlVideo(CBaseFilter *pFilter,    
                      CCritSec *pInterfaceLock,     
                      TCHAR *pName,                 
                      LPUNKNOWN pUnk,               
                      HRESULT *phr);                

    

    STDMETHODIMP get_AvgTimePerFrame(REFTIME *pAvgTimePerFrame);
    STDMETHODIMP get_BitRate(long *pBitRate);
    STDMETHODIMP get_BitErrorRate(long *pBitErrorRate);
    STDMETHODIMP get_VideoWidth(long *pVideoWidth);
    STDMETHODIMP get_VideoHeight(long *pVideoHeight);
    STDMETHODIMP put_SourceLeft(long SourceLeft);
    STDMETHODIMP get_SourceLeft(long *pSourceLeft);
    STDMETHODIMP put_SourceWidth(long SourceWidth);
    STDMETHODIMP get_SourceWidth(long *pSourceWidth);
    STDMETHODIMP put_SourceTop(long SourceTop);
    STDMETHODIMP get_SourceTop(long *pSourceTop);
    STDMETHODIMP put_SourceHeight(long SourceHeight);
    STDMETHODIMP get_SourceHeight(long *pSourceHeight);
    STDMETHODIMP put_DestinationLeft(long DestinationLeft);
    STDMETHODIMP get_DestinationLeft(long *pDestinationLeft);
    STDMETHODIMP put_DestinationWidth(long DestinationWidth);
    STDMETHODIMP get_DestinationWidth(long *pDestinationWidth);
    STDMETHODIMP put_DestinationTop(long DestinationTop);
    STDMETHODIMP get_DestinationTop(long *pDestinationTop);
    STDMETHODIMP put_DestinationHeight(long DestinationHeight);
    STDMETHODIMP get_DestinationHeight(long *pDestinationHeight);

    

    STDMETHODIMP GetVideoSize(long *pWidth,long *pHeight);
    STDMETHODIMP SetSourcePosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetSourcePosition(long *pLeft,long *pTop,long *pWidth,long *pHeight);
    STDMETHODIMP GetVideoPaletteEntries(long StartIndex,long Entries,long *pRetrieved,long *pPalette);
    STDMETHODIMP SetDefaultSourcePosition();
    STDMETHODIMP IsUsingDefaultSource();
    STDMETHODIMP SetDestinationPosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetDestinationPosition(long *pLeft,long *pTop,long *pWidth,long *pHeight);
    STDMETHODIMP SetDefaultDestinationPosition();
    STDMETHODIMP IsUsingDefaultDestination();
    STDMETHODIMP GetCurrentImage(long *pBufferSize,long *pVideoImage);
};

#endif 

