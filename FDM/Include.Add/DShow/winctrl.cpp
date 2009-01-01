/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>    

#define CheckConnected(pin,code)                    \
{                                                   \
    if (pin == NULL) {                              \
        ASSERT(!TEXT("Pin not set"));               \
    } else if (pin->IsConnected() == FALSE) {       \
        return (code);                              \
    }                                               \
}            

BOOL WINAPI PossiblyEatMessage(HWND hwndDrain, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (hwndDrain != NULL && !InSendMessage())
    {
        switch (uMsg)
        {
            case WM_CHAR:
            case WM_DEADCHAR:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_LBUTTONDBLCLK:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONDBLCLK:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MOUSEACTIVATE:
            case WM_MOUSEMOVE:
            
            
            case WM_NCLBUTTONDBLCLK:
            case WM_NCLBUTTONDOWN:
            case WM_NCLBUTTONUP:
            case WM_NCMBUTTONDBLCLK:
            case WM_NCMBUTTONDOWN:
            case WM_NCMBUTTONUP:
            case WM_NCMOUSEMOVE:
            case WM_NCRBUTTONDBLCLK:
            case WM_NCRBUTTONDOWN:
            case WM_NCRBUTTONUP:
            case WM_RBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_SYSCHAR:
            case WM_SYSDEADCHAR:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:

                DbgLog((LOG_TRACE, 2, TEXT("Forwarding %x to drain")));
                PostMessage(hwndDrain, uMsg, wParam, lParam);

                return TRUE;
        }
    }
    return FALSE;
}                

CBaseControlWindow::CBaseControlWindow(
                        CBaseFilter *pFilter,        
                        CCritSec *pInterfaceLock,    
                        TCHAR *pName,                
                        LPUNKNOWN pUnk,              
                        HRESULT *phr) :              

    CBaseVideoWindow(pName,pUnk),
    m_pInterfaceLock(pInterfaceLock),
    m_hwndOwner(NULL),
    m_hwndDrain(NULL),
    m_bAutoShow(TRUE),
    m_pFilter(pFilter),
    m_bCursorHidden(FALSE),
    m_pPin(NULL)
{
    ASSERT(m_pFilter);
    ASSERT(m_pInterfaceLock);
    ASSERT(phr);
    m_BorderColour = VIDEO_COLOUR;
}            

STDMETHODIMP CBaseControlWindow::put_Caption(BSTR strCaption)
{
    CheckPointer(strCaption,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
#ifdef UNICODE
    SetWindowText(m_hwnd, strCaption);
#else
    CHAR Caption[CAPTION];

    WideCharToMultiByte(CP_ACP,0,strCaption,-1,Caption,CAPTION,NULL,NULL);
    SetWindowText(m_hwnd, Caption);
#endif
    return NOERROR;
}              

STDMETHODIMP CBaseControlWindow::get_Caption(BSTR *pstrCaption)
{
    CheckPointer(pstrCaption,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    WCHAR WideCaption[CAPTION];

#ifdef UNICODE
    GetWindowText(m_hwnd,WideCaption,CAPTION);
#else
    

    TCHAR Caption[CAPTION];
    GetWindowText(m_hwnd,Caption,CAPTION);
    MultiByteToWideChar(CP_ACP,0,Caption,-1,WideCaption,CAPTION);
#endif
    return WriteBSTR(pstrCaption,WideCaption);
}      

STDMETHODIMP CBaseControlWindow::put_WindowStyleEx(long WindowStyleEx)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    if (GetWindowLong(m_hwnd,GWL_EXSTYLE) & WS_EX_TOPMOST) {
        if ((WindowStyleEx & WS_EX_TOPMOST) == 0) {
            SendMessage(m_hwnd,m_ShowStageTop,(WPARAM) FALSE,(LPARAM) 0);
        }
    }

    

    if (WindowStyleEx & WS_EX_TOPMOST) {
        SendMessage(m_hwnd,m_ShowStageTop,(WPARAM) TRUE,(LPARAM) 0);
        WindowStyleEx &= (~WS_EX_TOPMOST);
        if (WindowStyleEx == 0) return NOERROR;
    }
    return DoSetWindowStyle(WindowStyleEx,GWL_EXSTYLE);
}      

STDMETHODIMP CBaseControlWindow::get_WindowStyleEx(long *pWindowStyleEx)
{
    CheckPointer(pWindowStyleEx,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    return DoGetWindowStyle(pWindowStyleEx,GWL_EXSTYLE);
}      

STDMETHODIMP CBaseControlWindow::put_WindowStyle(long WindowStyle)
{
    

    if ((WindowStyle & WS_DISABLED) ||
        (WindowStyle & WS_ICONIC) ||
        (WindowStyle & WS_MAXIMIZE) ||
        (WindowStyle & WS_MINIMIZE) ||
        (WindowStyle & WS_HSCROLL) ||
        (WindowStyle & WS_VSCROLL)) {

            return E_INVALIDARG;
    }

    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    return DoSetWindowStyle(WindowStyle,GWL_STYLE);
}      

STDMETHODIMP CBaseControlWindow::get_WindowStyle(long *pWindowStyle)
{
    CheckPointer(pWindowStyle,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    return DoGetWindowStyle(pWindowStyle,GWL_STYLE);
}                

HRESULT CBaseControlWindow::DoSetWindowStyle(long Style,long WindowLong)
{
    RECT WindowRect;

    
    BOOL bVisible = IsWindowVisible(m_hwnd);
    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    
    SetWindowLong(m_hwnd,WindowLong,Style);
    UINT WindowFlags = SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_NOACTIVATE;
    WindowFlags |= SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE;

    

    if (bVisible == TRUE) {

        SetWindowPos(m_hwnd,            
                     HWND_TOP,          
                     0,0,0,0,           
                     WindowFlags);      

        return NOERROR;
    }

    

    MoveWindow((HWND) m_hwnd,                     
               GetSystemMetrics(SM_CXSCREEN),     
               GetSystemMetrics(SM_CYSCREEN),     
               WIDTH(&WindowRect),                
               HEIGHT(&WindowRect),               
               TRUE);                             

    

    SetWindowPos(m_hwnd,            
                 HWND_TOP,          
                 0,0,0,0,           
                 WindowFlags);      

    EXECUTE_ASSERT(ShowWindow(m_hwnd,SW_HIDE));

    if (GetParent(m_hwnd)) {

        MapWindowPoints(HWND_DESKTOP, GetParent(m_hwnd), (LPPOINT)&WindowRect, 2);
    }

    MoveWindow((HWND) m_hwnd,        
               WindowRect.left,      
               WindowRect.top,       
               WIDTH(&WindowRect),   
               HEIGHT(&WindowRect),  
               TRUE);                

    return NOERROR;
}      

HRESULT CBaseControlWindow::DoGetWindowStyle(long *pStyle,long WindowLong)
{
    *pStyle = GetWindowLong(m_hwnd,WindowLong);
    return NOERROR;
}            

STDMETHODIMP CBaseControlWindow::put_WindowState(long WindowState)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    DoShowWindow(WindowState);
    return NOERROR;
}              

STDMETHODIMP CBaseControlWindow::get_WindowState(long *pWindowState)
{
    CheckPointer(pWindowState,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    ASSERT(pWindowState);
    *pWindowState = FALSE;

    
    
    

    if (IsWindowVisible(m_hwnd) == TRUE) {

        
        if (IsIconic(m_hwnd) == TRUE) {
            *pWindowState |= SW_MINIMIZE;
        }

        
        else if (IsZoomed(m_hwnd) == TRUE) {
            *pWindowState |= SW_MAXIMIZE;
        }

        
        else {
            *pWindowState |= SW_SHOW;
        }

    } else {
        *pWindowState |= SW_HIDE;
    }
    return NOERROR;
}              

STDMETHODIMP CBaseControlWindow::put_BackgroundPalette(long BackgroundPalette)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cWindowLock(&m_WindowLock);

    

    if (BackgroundPalette != OATRUE) {
        if (BackgroundPalette != OAFALSE) {
            return E_INVALIDARG;
        }
    }

    

    m_bBackground = (BackgroundPalette == OATRUE ? TRUE : FALSE);
    PostMessage(m_hwnd,m_RealizePalette,0,0);
    PaintWindow(FALSE);

    return NOERROR;
}      

STDMETHODIMP
CBaseControlWindow::get_BackgroundPalette(long *pBackgroundPalette)
{
    CheckPointer(pBackgroundPalette,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cWindowLock(&m_WindowLock);

    

    *pBackgroundPalette = (m_bBackground == TRUE ? OATRUE : OAFALSE);
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::put_Visible(long Visible)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    if (Visible != OATRUE) {
        if (Visible != OAFALSE) {
            return E_INVALIDARG;
        }
    }

    

    INT Mode = (Visible == OATRUE ? SW_SHOWNORMAL : SW_HIDE);
    DoShowWindow(Mode);
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_Visible(long *pVisible)
{
    CheckPointer(pVisible,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    
    
    

    BOOL Mode = IsWindowVisible(m_hwnd);
    *pVisible = (Mode == TRUE ? OATRUE : OAFALSE);
    return NOERROR;
}          

STDMETHODIMP CBaseControlWindow::put_Left(long Left)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bSuccess;
    RECT WindowRect;

    
    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    if (GetParent(m_hwnd)) {

        MapWindowPoints(HWND_DESKTOP, GetParent(m_hwnd), (LPPOINT)&WindowRect, 2);
    }

    
    
    

    WindowRect.bottom = WindowRect.bottom - WindowRect.top;
    WindowRect.right = WindowRect.right - WindowRect.left;
    UINT WindowFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;

    bSuccess = SetWindowPos(m_hwnd,                
                            HWND_TOP,              
                            Left,                  
                            WindowRect.top,        
                            WindowRect.right,      
                            WindowRect.bottom,     
                            WindowFlags);          

    if (bSuccess == FALSE) {
        return E_INVALIDARG;
    }
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_Left(long *pLeft)
{
    CheckPointer(pLeft,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT WindowRect;

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));
    *pLeft = WindowRect.left;
    return NOERROR;
}            

STDMETHODIMP CBaseControlWindow::put_Width(long Width)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bSuccess;
    RECT WindowRect;

    
    
    

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    if (GetParent(m_hwnd)) {

        MapWindowPoints(HWND_DESKTOP, GetParent(m_hwnd), (LPPOINT)&WindowRect, 2);
    }

    WindowRect.bottom = WindowRect.bottom - WindowRect.top;
    UINT WindowFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;

    
    
    

    bSuccess = SetWindowPos(m_hwnd,                
                            HWND_TOP,              
                            WindowRect.left,       
                            WindowRect.top,        
                            Width,                 
                            WindowRect.bottom,     
                            WindowFlags);          

    if (bSuccess == FALSE) {
        return E_INVALIDARG;
    }
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_Width(long *pWidth)
{
    CheckPointer(pWidth,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT WindowRect;

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));
    *pWidth = WindowRect.right - WindowRect.left;
    return NOERROR;
}          

STDMETHODIMP CBaseControlWindow::put_Top(long Top)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bSuccess;
    RECT WindowRect;

    
    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    if (GetParent(m_hwnd)) {

        MapWindowPoints(HWND_DESKTOP, GetParent(m_hwnd), (LPPOINT)&WindowRect, 2);
    }

    
    
    

    WindowRect.bottom = WindowRect.bottom - WindowRect.top;
    WindowRect.right = WindowRect.right - WindowRect.left;
    UINT WindowFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;

    bSuccess = SetWindowPos(m_hwnd,                
                            HWND_TOP,              
                            WindowRect.left,       
                            Top,                   
                            WindowRect.right,      
                            WindowRect.bottom,     
                            WindowFlags);          

    if (bSuccess == FALSE) {
        return E_INVALIDARG;
    }
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_Top(long *pTop)
{
    CheckPointer(pTop,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT WindowRect;

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));
    *pTop = WindowRect.top;
    return NOERROR;
}            

STDMETHODIMP CBaseControlWindow::put_Height(long Height)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bSuccess;
    RECT WindowRect;

    
    
    

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    if (GetParent(m_hwnd)) {

        MapWindowPoints(HWND_DESKTOP, GetParent(m_hwnd), (LPPOINT)&WindowRect, 2);
    }

    WindowRect.right = WindowRect.right - WindowRect.left;
    UINT WindowFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;

    bSuccess = SetWindowPos(m_hwnd,                
                            HWND_TOP,              
                            WindowRect.left,       
                            WindowRect.top,        
                            WindowRect.right,      
                            Height,                
                            WindowFlags);          

    if (bSuccess == FALSE) {
        return E_INVALIDARG;
    }
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_Height(long *pHeight)
{
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT WindowRect;

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));
    *pHeight = WindowRect.bottom - WindowRect.top;
    return NOERROR;
}                        

STDMETHODIMP CBaseControlWindow::put_Owner(OAHWND Owner)
{
    

    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    m_hwndOwner = (HWND) Owner;
    HWND hwndParent = m_hwndOwner;

    

    LONG Style = GetWindowLong(m_hwnd,GWL_STYLE);
    if (Owner == NULL) {
        Style &= (~WS_CHILD);
    } else {
        Style |= (WS_CHILD);
    }
    SetWindowLong(m_hwnd,GWL_STYLE,Style);

    

    SetParent(m_hwnd,hwndParent);

    PaintWindow(TRUE);
    NOTE1("Changed parent %lx",hwndParent);

    return NOERROR;
}              

STDMETHODIMP CBaseControlWindow::get_Owner(OAHWND *Owner)
{
    CheckPointer(Owner,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    *Owner = (OAHWND) m_hwndOwner;
    return NOERROR;
}            

STDMETHODIMP CBaseControlWindow::put_MessageDrain(OAHWND Drain)
{
    

    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    m_hwndDrain = (HWND) Drain;
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_MessageDrain(OAHWND *Drain)
{
    CheckPointer(Drain,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    *Drain = (OAHWND) m_hwndDrain;
    return NOERROR;
}                

STDMETHODIMP
CBaseControlWindow::NotifyOwnerMessage(OAHWND hwnd,    
                                       long uMsg,    
                                       LONG_PTR wParam,  
                                       LONG_PTR lParam)  
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    switch (uMsg) {

        case WM_SYSCOLORCHANGE:
        case WM_PALETTECHANGED:
        case WM_PALETTEISCHANGING:
        case WM_QUERYNEWPALETTE:
        case WM_DEVMODECHANGE:
        case WM_DISPLAYCHANGE:
        case WM_ACTIVATEAPP:

            

            if (m_hwndOwner == NULL) {
                return NOERROR;
            }
            SendMessage(m_hwnd,uMsg,(WPARAM)wParam,(LPARAM)lParam);
	    break;

	
	
	
	
	case WM_MOVE:
	    PostMessage(m_hwnd,WM_PAINT,0,0);
	    break;
    }
    return NOERROR;
}          

STDMETHODIMP CBaseControlWindow::SetWindowForeground(long Focus)
{
    

    if (Focus != OATRUE) {
        if (Focus != OAFALSE) {
            return E_INVALIDARG;
        }
    }

    

    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bFocus = (Focus == OATRUE ? TRUE : FALSE);
    DoSetWindowForeground(bFocus);

    return NOERROR;
}            

STDMETHODIMP
CBaseControlWindow::SetWindowPosition(long Left,long Top,long Width,long Height)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    BOOL bSuccess;

    
    UINT WindowFlags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;

    ASSERT(IsWindow(m_hwnd));
    bSuccess = SetWindowPos(m_hwnd,         
                            HWND_TOP,       
                            Left,           
                            Top,            
                            Width,          
                            Height,         
                            WindowFlags);   
    ASSERT(bSuccess);
#ifdef DEBUG
    DbgLog((LOG_TRACE, 1, TEXT("SWP failed error %d"), GetLastError()));
#endif
    if (bSuccess == FALSE) {
        return E_INVALIDARG;
    }
    return NOERROR;
}            

STDMETHODIMP
CBaseControlWindow::GetWindowPosition(long *pLeft,long *pTop,long *pWidth,long *pHeight)
{
    

    CheckPointer(pLeft,E_POINTER);
    CheckPointer(pTop,E_POINTER);
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT WindowRect;

    

    EXECUTE_ASSERT(GetWindowRect(m_hwnd,&WindowRect));

    

    *pLeft = WindowRect.left;
    *pTop = WindowRect.top;
    *pWidth = WindowRect.right - WindowRect.left;
    *pHeight = WindowRect.bottom - WindowRect.top;

    return NOERROR;
}              

STDMETHODIMP
CBaseControlWindow::GetRestorePosition(long *pLeft,long *pTop,long *pWidth,long *pHeight)
{
    

    CheckPointer(pLeft,E_POINTER);
    CheckPointer(pTop,E_POINTER);
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    WINDOWPLACEMENT Place;
    Place.length = sizeof(WINDOWPLACEMENT);
    EXECUTE_ASSERT(GetWindowPlacement(m_hwnd,&Place));

    RECT WorkArea;

    

    if (SystemParametersInfo(SPI_GETWORKAREA,0,&WorkArea,FALSE) == TRUE) {
        if (GetParent(m_hwnd) == NULL) {
            Place.rcNormalPosition.top += WorkArea.top;
            Place.rcNormalPosition.bottom += WorkArea.top;
            Place.rcNormalPosition.left += WorkArea.left;
            Place.rcNormalPosition.right += WorkArea.left;
        }
    }

    

    *pLeft = Place.rcNormalPosition.left;
    *pTop = Place.rcNormalPosition.top;
    *pWidth = Place.rcNormalPosition.right - Place.rcNormalPosition.left;
    *pHeight = Place.rcNormalPosition.bottom - Place.rcNormalPosition.top;

    return NOERROR;
}            

STDMETHODIMP CBaseControlWindow::get_BorderColor(long *Color)
{
    CheckPointer(Color,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    *Color = (long) m_BorderColour;
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::put_BorderColor(long Color)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    m_BorderColour = (COLORREF) Color;
    PaintWindow(TRUE);
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::get_FullScreenMode(long *FullScreenMode)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CheckPointer(FullScreenMode,E_POINTER);
    return E_NOTIMPL;
}      

STDMETHODIMP CBaseControlWindow::put_FullScreenMode(long FullScreenMode)
{
    return E_NOTIMPL;
}            

STDMETHODIMP CBaseControlWindow::put_AutoShow(long AutoShow)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    if (AutoShow != OATRUE) {
        if (AutoShow != OAFALSE) {
            return E_INVALIDARG;
        }
    }

    m_bAutoShow = (AutoShow == OATRUE ? TRUE : FALSE);
    return NOERROR;
}          

STDMETHODIMP CBaseControlWindow::get_AutoShow(long *AutoShow)
{
    CheckPointer(AutoShow,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    *AutoShow = (m_bAutoShow == TRUE ? OATRUE : OAFALSE);
    return NOERROR;
}            

STDMETHODIMP
CBaseControlWindow::GetMinIdealImageSize(long *pWidth,long *pHeight)
{
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    FILTER_STATE State;

    

    m_pFilter->GetState(0,&State);
    if (State == State_Stopped) {
        return VFW_E_WRONG_STATE;
    }

    RECT DefaultRect = GetDefaultRect();
    *pWidth = WIDTH(&DefaultRect);
    *pHeight = HEIGHT(&DefaultRect);
    return NOERROR;
}            

STDMETHODIMP
CBaseControlWindow::GetMaxIdealImageSize(long *pWidth,long *pHeight)
{
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    FILTER_STATE State;

    

    m_pFilter->GetState(0,&State);
    if (State == State_Stopped) {
        return VFW_E_WRONG_STATE;
    }

    RECT DefaultRect = GetDefaultRect();
    *pWidth = WIDTH(&DefaultRect);
    *pHeight = HEIGHT(&DefaultRect);
    return NOERROR;
}      

STDMETHODIMP
CBaseControlWindow::HideCursor(long HideCursor)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);

    

    if (HideCursor != OATRUE) {
        if (HideCursor != OAFALSE) {
            return E_INVALIDARG;
        }
    }

    m_bCursorHidden = (HideCursor == OATRUE ? TRUE : FALSE);
    return NOERROR;
}      

STDMETHODIMP CBaseControlWindow::IsCursorHidden(long *CursorHidden)
{
    CheckPointer(CursorHidden,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    *CursorHidden = (m_bCursorHidden == TRUE ? OATRUE : OAFALSE);
    return NOERROR;
}                

CBaseControlVideo::CBaseControlVideo(
                        CBaseFilter *pFilter,        
                        CCritSec *pInterfaceLock,    
                        TCHAR *pName,                
                        LPUNKNOWN pUnk,              
                        HRESULT *phr) :              

    CBaseBasicVideo(pName,pUnk),
    m_pFilter(pFilter),
    m_pInterfaceLock(pInterfaceLock),
    m_pPin(NULL)
{
    ASSERT(m_pFilter);
    ASSERT(m_pInterfaceLock);
    ASSERT(phr);
}    

STDMETHODIMP CBaseControlVideo::get_AvgTimePerFrame(REFTIME *pAvgTimePerFrame)
{
    CheckPointer(pAvgTimePerFrame,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    COARefTime AvgTime(pVideoInfo->AvgTimePerFrame);
    *pAvgTimePerFrame = (REFTIME) AvgTime;

    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::get_BitRate(long *pBitRate)
{
    CheckPointer(pBitRate,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    *pBitRate = pVideoInfo->dwBitRate;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::get_BitErrorRate(long *pBitErrorRate)
{
    CheckPointer(pBitErrorRate,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    *pBitErrorRate = pVideoInfo->dwBitErrorRate;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::get_VideoWidth(long *pVideoWidth)
{
    CheckPointer(pVideoWidth,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    *pVideoWidth = pVideoInfo->bmiHeader.biWidth;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::get_VideoHeight(long *pVideoHeight)
{
    CheckPointer(pVideoHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    *pVideoHeight = pVideoInfo->bmiHeader.biHeight;
    return NOERROR;
}                

STDMETHODIMP CBaseControlVideo::GetVideoPaletteEntries(long StartIndex,
                                                       long Entries,
                                                       long *pRetrieved,
                                                       long *pPalette)
{
    CheckPointer(pRetrieved,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    CMediaType MediaType;

    

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    BITMAPINFOHEADER *pHeader = HEADER(pVideoInfo);

    

    if (PALETTISED(pVideoInfo) == FALSE) {
        *pRetrieved = 0;
        return VFW_E_NO_PALETTE_AVAILABLE;
    }

    

    if (pPalette == NULL) {
        *pRetrieved = pHeader->biClrUsed;
        return NOERROR;
    }

    

    if (StartIndex >= (LONG) pHeader->biClrUsed || StartIndex < 0) {
        *pRetrieved = 0;
        return E_INVALIDARG;
    }

    

    LONG Available = (LONG) pHeader->biClrUsed - StartIndex;
    *pRetrieved = max(0,min(Available,Entries));
    if (*pRetrieved == 0) {
        return S_FALSE;
    }

    

    PALETTEENTRY *pEntries = (PALETTEENTRY *) pPalette;
    RGBQUAD *pColours = COLORS(pVideoInfo) + StartIndex;

    for (LONG Count = 0;Count < *pRetrieved;Count++) {
        pEntries[Count].peRed = pColours[Count].rgbRed;
        pEntries[Count].peGreen = pColours[Count].rgbGreen;
        pEntries[Count].peBlue = pColours[Count].rgbBlue;
        pEntries[Count].peFlags = 0;
    }
    return NOERROR;
}            

STDMETHODIMP CBaseControlVideo::GetVideoSize(long *pWidth,long *pHeight)
{
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);

    
    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    *pWidth = pVideoInfo->bmiHeader.biWidth;
    *pHeight = pVideoInfo->bmiHeader.biHeight;
    return NOERROR;
}          

STDMETHODIMP
CBaseControlVideo::SetSourcePosition(long Left,long Top,long Width,long Height)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;
    SourceRect.left = Left;
    SourceRect.top = Top;
    SourceRect.right = Left + Width;
    SourceRect.bottom = Top + Height;

    

    HRESULT hr = CheckSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}          

STDMETHODIMP
CBaseControlVideo::GetSourcePosition(long *pLeft,long *pTop,long *pWidth,long *pHeight)
{
    

    CheckPointer(pLeft,E_POINTER);
    CheckPointer(pTop,E_POINTER);
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT SourceRect;

    CAutoLock cInterfaceLock(m_pInterfaceLock);
    GetSourceRect(&SourceRect);

    *pLeft = SourceRect.left;
    *pTop = SourceRect.top;
    *pWidth = WIDTH(&SourceRect);
    *pHeight = HEIGHT(&SourceRect);

    return NOERROR;
}          

STDMETHODIMP
CBaseControlVideo::SetDestinationPosition(long Left,long Top,long Width,long Height)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;

    DestinationRect.left = Left;
    DestinationRect.top = Top;
    DestinationRect.right = Left + Width;
    DestinationRect.bottom = Top + Height;

    

    HRESULT hr = CheckTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}          

STDMETHODIMP
CBaseControlVideo::GetDestinationPosition(long *pLeft,long *pTop,long *pWidth,long *pHeight)
{
    

    CheckPointer(pLeft,E_POINTER);
    CheckPointer(pTop,E_POINTER);
    CheckPointer(pWidth,E_POINTER);
    CheckPointer(pHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    RECT DestinationRect;

    CAutoLock cInterfaceLock(m_pInterfaceLock);
    GetTargetRect(&DestinationRect);

    *pLeft = DestinationRect.left;
    *pTop = DestinationRect.top;
    *pWidth = WIDTH(&DestinationRect);
    *pHeight = HEIGHT(&DestinationRect);

    return NOERROR;
}            

STDMETHODIMP CBaseControlVideo::put_SourceLeft(long SourceLeft)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;
    GetSourceRect(&SourceRect);
    SourceRect.right = SourceLeft + WIDTH(&SourceRect);
    SourceRect.left = SourceLeft;

    

    HRESULT hr = CheckSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_SourceLeft(long *pSourceLeft)
{
    CheckPointer(pSourceLeft,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;

    GetSourceRect(&SourceRect);
    *pSourceLeft = SourceRect.left;
    return NOERROR;
}          

STDMETHODIMP CBaseControlVideo::put_SourceWidth(long SourceWidth)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;
    GetSourceRect(&SourceRect);
    SourceRect.right = SourceRect.left + SourceWidth;

    

    HRESULT hr = CheckSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_SourceWidth(long *pSourceWidth)
{
    CheckPointer(pSourceWidth,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;

    GetSourceRect(&SourceRect);
    *pSourceWidth = WIDTH(&SourceRect);
    return NOERROR;
}            

STDMETHODIMP CBaseControlVideo::put_SourceTop(long SourceTop)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;
    GetSourceRect(&SourceRect);
    SourceRect.bottom = SourceTop + HEIGHT(&SourceRect);
    SourceRect.top = SourceTop;

    

    HRESULT hr = CheckSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_SourceTop(long *pSourceTop)
{
    CheckPointer(pSourceTop,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;

    GetSourceRect(&SourceRect);
    *pSourceTop = SourceRect.top;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::put_SourceHeight(long SourceHeight)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;
    GetSourceRect(&SourceRect);
    SourceRect.bottom = SourceRect.top + SourceHeight;

    

    HRESULT hr = CheckSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetSourceRect(&SourceRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_SourceHeight(long *pSourceHeight)
{
    CheckPointer(pSourceHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT SourceRect;

    GetSourceRect(&SourceRect);
    *pSourceHeight = HEIGHT(&SourceRect);
    return NOERROR;
}            

STDMETHODIMP CBaseControlVideo::put_DestinationLeft(long DestinationLeft)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;
    GetTargetRect(&DestinationRect);
    DestinationRect.right = DestinationLeft + WIDTH(&DestinationRect);
    DestinationRect.left = DestinationLeft;

    

    HRESULT hr = CheckTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_DestinationLeft(long *pDestinationLeft)
{
    CheckPointer(pDestinationLeft,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;

    GetTargetRect(&DestinationRect);
    *pDestinationLeft = DestinationRect.left;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::put_DestinationWidth(long DestinationWidth)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;
    GetTargetRect(&DestinationRect);
    DestinationRect.right = DestinationRect.left + DestinationWidth;

    

    HRESULT hr = CheckTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_DestinationWidth(long *pDestinationWidth)
{
    CheckPointer(pDestinationWidth,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;

    GetTargetRect(&DestinationRect);
    *pDestinationWidth = WIDTH(&DestinationRect);
    return NOERROR;
}            

STDMETHODIMP CBaseControlVideo::put_DestinationTop(long DestinationTop)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;
    GetTargetRect(&DestinationRect);
    DestinationRect.bottom = DestinationTop + HEIGHT(&DestinationRect);
    DestinationRect.top = DestinationTop;

    

    HRESULT hr = CheckTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_DestinationTop(long *pDestinationTop)
{
    CheckPointer(pDestinationTop,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;

    GetTargetRect(&DestinationRect);
    *pDestinationTop = DestinationRect.top;
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::put_DestinationHeight(long DestinationHeight)
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;
    GetTargetRect(&DestinationRect);
    DestinationRect.bottom = DestinationRect.top + DestinationHeight;

    

    HRESULT hr = CheckTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }

    

    hr = SetTargetRect(&DestinationRect);
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::get_DestinationHeight(long *pDestinationHeight)
{
    CheckPointer(pDestinationHeight,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    RECT DestinationRect;

    GetTargetRect(&DestinationRect);
    *pDestinationHeight = HEIGHT(&DestinationRect);
    return NOERROR;
}      

STDMETHODIMP CBaseControlVideo::SetDefaultSourcePosition()
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    HRESULT hr = SetDefaultSourceRect();
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::IsUsingDefaultSource()
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    return IsDefaultSourceRect();
}      

STDMETHODIMP CBaseControlVideo::SetDefaultDestinationPosition()
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    HRESULT hr = SetDefaultTargetRect();
    if (FAILED(hr)) {
        return hr;
    }
    return OnUpdateRectangles();
}      

STDMETHODIMP CBaseControlVideo::IsUsingDefaultDestination()
{
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    return IsDefaultTargetRect();
}      

STDMETHODIMP
CBaseControlVideo::GetCurrentImage(long *pBufferSize,long *pVideoImage)
{
    CheckPointer(pBufferSize,E_POINTER);
    CheckConnected(m_pPin,VFW_E_NOT_CONNECTED);
    CAutoLock cInterfaceLock(m_pInterfaceLock);
    FILTER_STATE State;

    

    if (pVideoImage != NULL) {
        m_pFilter->GetState(0,&State);
        if (State != State_Paused) {
            return VFW_E_NOT_PAUSED;
        }
        return GetStaticImage(pBufferSize,pVideoImage);
    }

    

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    RECT SourceRect;
    GetSourceRect(&SourceRect);
    return GetImageSize(pVideoInfo,pBufferSize,&SourceRect);
}                

HRESULT CBaseControlVideo::GetImageSize(VIDEOINFOHEADER *pVideoInfo,
                                        LONG *pBufferSize,
                                        RECT *pSourceRect)
{
    NOTE("Entering GetImageSize");
    ASSERT(pSourceRect);

    

    if (pSourceRect == NULL ||
            pVideoInfo == NULL ||
            pBufferSize == NULL) {

        return E_UNEXPECTED;
    }

    

    if (pVideoInfo->bmiHeader.biCompression != BI_RGB) {
        if (pVideoInfo->bmiHeader.biCompression != BI_BITFIELDS) {
            return E_INVALIDARG;
        }
    }

    ASSERT(IsRectEmpty(pSourceRect) == FALSE);

    BITMAPINFOHEADER bih;
    bih.biWidth = WIDTH(pSourceRect);
    bih.biHeight = HEIGHT(pSourceRect);
    bih.biBitCount = pVideoInfo->bmiHeader.biBitCount;
    LONG Size = DIBSIZE(bih);
    Size += GetBitmapFormatSize(HEADER(pVideoInfo)) - SIZE_PREHEADER;
    *pBufferSize = Size;

    return NOERROR;
}                

HRESULT CBaseControlVideo::CopyImage(IMediaSample *pMediaSample,
                                     VIDEOINFOHEADER *pVideoInfo,
                                     LONG *pBufferSize,
                                     BYTE *pVideoImage,
                                     RECT *pSourceRect)
{
    NOTE("Entering CopyImage");
    ASSERT(pSourceRect);
    BYTE *pCurrentImage;

    

    if (pMediaSample == NULL || pSourceRect == NULL ||
            pVideoInfo == NULL || pVideoImage == NULL ||
            pBufferSize == NULL) {

        return E_UNEXPECTED;
    }

    

    if (pVideoInfo->bmiHeader.biCompression != BI_RGB) {
        if (pVideoInfo->bmiHeader.biCompression != BI_BITFIELDS) {
            return E_INVALIDARG;
        }
    }

    ASSERT(IsRectEmpty(pSourceRect) == FALSE);

    BITMAPINFOHEADER bih;
    bih.biWidth = WIDTH(pSourceRect);
    bih.biHeight = HEIGHT(pSourceRect);
    bih.biBitCount = pVideoInfo->bmiHeader.biBitCount;
    LONG Size = GetBitmapFormatSize(HEADER(pVideoInfo)) - SIZE_PREHEADER;
    LONG Total = Size + DIBSIZE(bih);

    

    if (*pBufferSize < Total) {
        return E_OUTOFMEMORY;
    }

    

    CopyMemory((PVOID)pVideoImage, (PVOID)&pVideoInfo->bmiHeader, Size);
    ((BITMAPINFOHEADER *)pVideoImage)->biWidth = WIDTH(pSourceRect);
    ((BITMAPINFOHEADER *)pVideoImage)->biHeight = HEIGHT(pSourceRect);
    ((BITMAPINFOHEADER *)pVideoImage)->biSizeImage = DIBSIZE(bih);
    BYTE *pImageData = pVideoImage + Size;

    

    HRESULT hr = pMediaSample->GetPointer(&pCurrentImage);
    if (FAILED(hr)) {
        return hr;
    }

    

    LONG ScanLine = (pVideoInfo->bmiHeader.biBitCount / 8) * WIDTH(pSourceRect);
    LONG LinesToSkip = pVideoInfo->bmiHeader.biHeight;
    LinesToSkip -= pSourceRect->top + HEIGHT(pSourceRect);
    pCurrentImage += LinesToSkip * DIBWIDTHBYTES(pVideoInfo->bmiHeader);
    pCurrentImage += pSourceRect->left * (pVideoInfo->bmiHeader.biBitCount / 8);

    

    for (LONG Line = 0;Line < HEIGHT(pSourceRect);Line++) {
        CopyMemory((PVOID)pImageData, (PVOID)pCurrentImage, ScanLine);
        pImageData += DIBWIDTHBYTES(*(BITMAPINFOHEADER *)pVideoImage);
        pCurrentImage += DIBWIDTHBYTES(pVideoInfo->bmiHeader);
    }
    return NOERROR;
}            

HRESULT CBaseControlVideo::OnVideoSizeChange()
{
    

    VIDEOINFOHEADER *pVideoInfo = GetVideoFormat();
    if (pVideoInfo == NULL)
    return E_OUTOFMEMORY;
    WORD Width = (WORD) pVideoInfo->bmiHeader.biWidth;
    WORD Height = (WORD) pVideoInfo->bmiHeader.biHeight;

    return m_pFilter->NotifyEvent(EC_VIDEO_SIZE_CHANGED,
                                  MAKELPARAM(Width,Height),
                                  MAKEWPARAM(0,0));
}                  

HRESULT CBaseControlVideo::CheckSourceRect(RECT *pSourceRect)
{
    CheckPointer(pSourceRect,E_POINTER);
    LONG Width,Height;
    GetVideoSize(&Width,&Height);

    
    

    if ((pSourceRect->left >= pSourceRect->right) ||
       (pSourceRect->left < 0) ||
       (pSourceRect->top >= pSourceRect->bottom) ||
       (pSourceRect->top < 0)) {

        return E_INVALIDARG;
    }

    

    if ((pSourceRect->right > Width) ||
        (pSourceRect->bottom > Height)) {

        return E_INVALIDARG;
    }
    return NOERROR;
}                    

HRESULT CBaseControlVideo::CheckTargetRect(RECT *pTargetRect)
{
    

    if (pTargetRect == NULL) {
        return E_POINTER;
    }

    

    if (pTargetRect->left > pTargetRect->right ||
            pTargetRect->top > pTargetRect->bottom) {
                return E_INVALIDARG;
    }

    

    if (WIDTH(pTargetRect) <= 0 || HEIGHT(pTargetRect) <= 0) {
        return E_INVALIDARG;
    }

    ASSERT(IsRectEmpty(pTargetRect) == FALSE);
    return NOERROR;
}

