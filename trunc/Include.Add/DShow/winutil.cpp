/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>
#include <limits.h>
#include <dvdmedia.h>

static UINT MsgDestroy;

CBaseWindow::CBaseWindow(BOOL bDoGetDC, bool bDoPostToDestroy) :
    m_hInstance(g_hInst),
    m_hwnd(NULL),
    m_hdc(NULL),
    m_bActivated(FALSE),
    m_pClassName(NULL),
    m_ClassStyles(0),
    m_WindowStyles(0),
    m_WindowStylesEx(0),
    m_ShowStageMessage(0),
    m_ShowStageTop(0),
    m_MemoryDC(NULL),
    m_hPalette(NULL),
    m_bBackground(FALSE),
#ifdef DEBUG
    m_bRealizing(FALSE),
#endif
    m_bNoRealize(FALSE),
    m_bDoPostToDestroy(bDoPostToDestroy)
{
    m_bDoGetDC = bDoGetDC;
}

HRESULT CBaseWindow::PrepareWindow()
{
    if (m_hwnd) return NOERROR;
    ASSERT(m_hwnd == NULL);
    ASSERT(m_hdc == NULL);

    

    m_pClassName = GetClassWindowStyles(&m_ClassStyles,
                                        &m_WindowStyles,
                                        &m_WindowStylesEx);
    if (m_pClassName == NULL) {
        return E_FAIL;
    }

    
    m_ShowStageMessage = RegisterWindowMessage(SHOWSTAGE);
    m_ShowStageTop = RegisterWindowMessage(SHOWSTAGETOP);
    m_RealizePalette = RegisterWindowMessage(REALIZEPALETTE);

    return DoCreateWindow();
}

#ifdef DEBUG
CBaseWindow::~CBaseWindow()
{
    ASSERT(m_hwnd == NULL);
    ASSERT(m_hdc == NULL);
}
#endif

HRESULT CBaseWindow::DoneWithWindow()
{
    
    
    
    
    
    
    
    if (!IsWindow(m_hwnd)) {

        
        
        
        
        
        m_hdc = NULL;

        
        
        
        
        if (m_MemoryDC)
        {
            EXECUTE_ASSERT(DeleteDC(m_MemoryDC));
            m_MemoryDC = NULL;
        }

        
        m_hwnd = NULL;
        return NOERROR;
    }

    if (GetWindowThreadProcessId(m_hwnd, NULL) != GetCurrentThreadId()) {
        CAMEvent m_evDone;

        
        
        
        
        MsgDestroy = RegisterWindowMessage(TEXT("AM_DESTROY"));
        if (m_bDoPostToDestroy) {
            PostMessage(m_hwnd, MsgDestroy, (WPARAM)(HANDLE)m_evDone, 0);
            WaitDispatchingMessages(m_evDone, INFINITE);
        } else {
            SendMessage(m_hwnd, MsgDestroy, 0, 0);
        }
        return NOERROR;
    }
    const HWND hwnd = m_hwnd;
    if (hwnd == NULL) {
        return NOERROR;
    }

    InactivateWindow();
    NOTE("Inactivated");

    

    SetWindowLong(hwnd,GWL_STYLE,m_WindowStyles);
    ASSERT(GetParent(hwnd) == NULL);
    NOTE1("Reset window styles %d",m_WindowStyles);

    
    UninitialiseWindow();
    DbgLog((LOG_TRACE, 2, TEXT("Destroying 0x%8.8X"), hwnd));
    if (!DestroyWindow(hwnd)) {
        DbgLog((LOG_TRACE, 0, TEXT("DestroyWindow %8.8X failed code %d"),
                hwnd, GetLastError()));
        DbgBreak("");
    }

    

    m_pClassName = NULL;
    m_ClassStyles = 0;
    m_WindowStyles = 0;
    m_WindowStylesEx = 0;
    m_ShowStageMessage = 0;
    m_ShowStageTop = 0;

    return NOERROR;
}

HRESULT CBaseWindow::InactivateWindow()
{
    
    if (m_bActivated == FALSE) {
        return S_FALSE;
    }

    m_bActivated = FALSE;
    ShowWindow(m_hwnd,SW_HIDE);
    return NOERROR;
}

HRESULT CBaseWindow::CompleteConnect()
{
    m_bActivated = FALSE;
    return NOERROR;
}

HRESULT CBaseWindow::ActivateWindow()
{
    

    if (m_bActivated == TRUE || GetParent(m_hwnd) != NULL) {

        SetWindowPos(m_hwnd,            
                     HWND_TOP,          
                     0, 0, 0, 0,        
                     SWP_NOMOVE |       
                     SWP_NOSIZE);       

        m_bActivated = TRUE;
        return S_FALSE;
    }

    

    RECT WindowRect, ClientRect = GetDefaultRect();
    GetWindowRect(m_hwnd,&WindowRect);
    AdjustWindowRectEx(&ClientRect,GetWindowLong(m_hwnd,GWL_STYLE),
                       FALSE,GetWindowLong(m_hwnd,GWL_EXSTYLE));

    

    UINT WindowFlags = (SWP_NOACTIVATE | SWP_FRAMECHANGED);
    WindowRect.left -= (WindowRect.left & 3);
    WindowRect.top -= (WindowRect.top & 3);

    SetWindowPos(m_hwnd,                
                 HWND_TOP,              
                 WindowRect.left,       
                 WindowRect.top,        
                 WIDTH(&ClientRect),    
                 HEIGHT(&ClientRect),   
                 WindowFlags);          

    m_bActivated = TRUE;
    return NOERROR;
}

HRESULT CBaseWindow::PerformanceAlignWindow()
{
    RECT ClientRect,WindowRect;
    GetWindowRect(m_hwnd,&WindowRect);
    ASSERT(m_bActivated == TRUE);

    

    if (GetParent(m_hwnd)) {
        return NOERROR;
    }

    

    GetClientRect(m_hwnd, &ClientRect);
    MapWindowPoints(m_hwnd, HWND_DESKTOP, (LPPOINT) &ClientRect, 2);
    WindowRect.left -= (ClientRect.left & 3);
    WindowRect.top  -= (ClientRect.top  & 3);
    UINT WindowFlags = (SWP_NOACTIVATE | SWP_NOSIZE);

    SetWindowPos(m_hwnd,                
                 HWND_TOP,              
                 WindowRect.left,       
                 WindowRect.top,        
                 (int) 0,(int) 0,       
                 WindowFlags);          

    return NOERROR;
}

HRESULT CBaseWindow::SetPalette(HPALETTE hPalette)
{
    
    {
        CAutoLock cWindowLock(&m_WindowLock);
        ASSERT(hPalette);
        m_hPalette = hPalette;
    }
    return SetPalette();
}

HRESULT CBaseWindow::SetPalette()
{
    if (!m_bNoRealize) {
        SendMessage(m_hwnd, m_RealizePalette, 0, 0);
        
        return (GdiFlush() == FALSE ? S_FALSE : S_OK);
    } else {
        
        ASSERT(m_hdc);
        ASSERT(m_MemoryDC);
        SelectPalette(m_hdc,m_hPalette,m_bBackground);
        SelectPalette(m_MemoryDC,m_hPalette,m_bBackground);
        return S_OK;
    }
}

HRESULT CBaseWindow::DoRealisePalette(BOOL bForceBackground)
{
    
    
    
    
    
    
    
    
    
    
    
    if (m_hPalette == NULL) {

        return NOERROR;
    }

    
    ASSERT(m_hdc);
    ASSERT(m_MemoryDC);
    SelectPalette(m_hdc,m_hPalette,m_bBackground || bForceBackground);
    EXECUTE_ASSERT(RealizePalette(m_hdc) != GDI_ERROR);
    SelectPalette(m_MemoryDC,m_hPalette,m_bBackground);
    EXECUTE_ASSERT(RealizePalette(m_MemoryDC) != GDI_ERROR);

    return (GdiFlush() == FALSE ? S_FALSE : S_OK);
}

LRESULT CALLBACK WndProc(HWND hwnd,         
                         UINT uMsg,         
                         WPARAM wParam,     
                         LPARAM lParam)     
{

    
    
    
    
    

    CBaseWindow *pBaseWindow = (CBaseWindow *)GetWindowLongPtr(hwnd,0);
    if (pBaseWindow == NULL) {

	
	
	
	

	
	

        
        if (uMsg == WM_NCCREATE) {
            SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~0x400000);
        }

	if ((uMsg != WM_NCCREATE)
	    || (NULL == (pBaseWindow = *(CBaseWindow**) ((LPCREATESTRUCT)lParam)->lpCreateParams)))
	{
	    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

        
#ifdef DEBUG
	SetLastError(0);  
#endif
        LONG_PTR rc = SetWindowLongPtr(hwnd, (DWORD) 0, (LONG_PTR) pBaseWindow);
#ifdef DEBUG
	if (0 == rc) {
	    
	    
	    LONG lasterror = GetLastError();
	    ASSERT(0 == lasterror);
	    
	    
	}
#endif

    }
    
    if (uMsg == MsgDestroy && uMsg != 0) {
        pBaseWindow->DoneWithWindow();
        if (pBaseWindow->m_bDoPostToDestroy) {
            EXECUTE_ASSERT(SetEvent((HANDLE)wParam));
        }
        return 0;
    }
    return pBaseWindow->OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}

BOOL CBaseWindow::OnSize(LONG Width, LONG Height)
{
    m_Width = Width;
    m_Height = Height;
    return TRUE;
}

BOOL CBaseWindow::OnClose()
{
    ShowWindow(m_hwnd,SW_HIDE);
    return TRUE;
}

HRESULT CBaseWindow::UninitialiseWindow()
{
    

    if (m_hwnd == NULL) {
        ASSERT(m_hdc == NULL);
        ASSERT(m_MemoryDC == NULL);
        return NOERROR;
    }

    

    EXECUTE_ASSERT(GdiFlush());

    if (m_hdc)
    {
        EXECUTE_ASSERT(ReleaseDC(m_hwnd,m_hdc));
        m_hdc = NULL;
    }

    if (m_MemoryDC)
    {
        EXECUTE_ASSERT(DeleteDC(m_MemoryDC));
        m_MemoryDC = NULL;
    }

    
    m_hwnd = NULL;

    return NOERROR;
}

HRESULT CBaseWindow::InitialiseWindow(HWND hwnd)
{
    

    ASSERT(IsWindow(hwnd));
    m_hwnd = hwnd;

    if (m_bDoGetDC)
    {
        EXECUTE_ASSERT(m_hdc = GetDC(hwnd));
        EXECUTE_ASSERT(m_MemoryDC = CreateCompatibleDC(m_hdc));

        EXECUTE_ASSERT(SetStretchBltMode(m_hdc,COLORONCOLOR));
        EXECUTE_ASSERT(SetStretchBltMode(m_MemoryDC,COLORONCOLOR));
    }

    return NOERROR;
}

HRESULT CBaseWindow::DoCreateWindow()
{
    WNDCLASS wndclass;                  
    BOOL bRegistered;                   
    HWND hwnd;                          

    bRegistered = GetClassInfo(m_hInstance,   
                               m_pClassName,  
                               &wndclass);                 

    
    
    
    
    if (m_bDoGetDC)
    {
        m_ClassStyles |= CS_OWNDC;
    }

    if (bRegistered == FALSE) {

        

        wndclass.lpszClassName = m_pClassName;
        wndclass.style         = m_ClassStyles;
        wndclass.lpfnWndProc   = WndProc;
        wndclass.cbClsExtra    = 0;
        wndclass.cbWndExtra    = sizeof(CBaseWindow *);
        wndclass.hInstance     = m_hInstance;
        wndclass.hIcon         = NULL;
        wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH) NULL;
        wndclass.lpszMenuName  = NULL;

        RegisterClass(&wndclass);
    }

    
    
    

    CBaseWindow *pBaseWindow = this;           
    hwnd = CreateWindowEx(m_WindowStylesEx,  
                          m_pClassName,      
                          TEXT("ActiveMovie Window"),     
                          m_WindowStyles,    
                          CW_USEDEFAULT,                  
                          CW_USEDEFAULT,                  
                          DEFWIDTH,                       
                          DEFHEIGHT,                      
                          NULL,                           
                          NULL,                           
                          m_hInstance,       
                          &pBaseWindow);                  

    
    
    

    if (hwnd == NULL) {
        DWORD Error = GetLastError();
        return AmHresultFromWin32(Error);
    }

    
    ASSERT(GetWindowLongPtr(hwnd, 0) == (LONG_PTR)this);

    
    
    
    
    

    InitialiseWindow(hwnd);

    DbgLog((LOG_TRACE, 2, TEXT("Created window class (%s) HWND(%8.8X)"),
            m_pClassName, hwnd));

    return S_OK;
}

LRESULT CBaseWindow::OnReceiveMessage(HWND hwnd,         
                                      UINT uMsg,         
                                      WPARAM wParam,     
                                      LPARAM lParam)     
{
    ASSERT(IsWindow(hwnd));

    if (PossiblyEatMessage(uMsg, wParam, lParam))
	return 0;

    
    
    
    
    

    if (uMsg == m_ShowStageMessage) {

        BOOL bVisible = IsWindowVisible(hwnd);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW |
                     (bVisible ? SWP_NOACTIVATE : 0));

        
        if (wParam == TRUE) {
            SetForegroundWindow(hwnd);
        }
        return (LRESULT) 1;
    }

    
    
    
    
    

    if (uMsg == m_ShowStageTop) {
        HWND HwndTop = (wParam == TRUE ? HWND_TOPMOST : HWND_NOTOPMOST);
        BOOL bVisible = IsWindowVisible(hwnd);
        SetWindowPos(hwnd, HwndTop, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE |
                     (wParam == TRUE ? SWP_SHOWWINDOW : 0) |
                     (bVisible ? SWP_NOACTIVATE : 0));
        return (LRESULT) 1;
    }

    
    if (uMsg == m_RealizePalette) {
        ASSERT(m_hwnd == hwnd);
        return OnPaletteChange(m_hwnd,WM_QUERYNEWPALETTE);
    }

    switch (uMsg) {

        

        case WM_SYSCOLORCHANGE:

            InvalidateRect(hwnd,NULL,FALSE);
            return (LRESULT) 1;

        
        case WM_PALETTECHANGED:

            OnPaletteChange((HWND)wParam,uMsg);
            return (LRESULT) 0;

        
        
        

	case WM_QUERYNEWPALETTE:
	    ASSERT(m_hwnd == hwnd);
            return OnPaletteChange(m_hwnd,uMsg);

        
        
        
        
        case WM_MOVE:
            if (IsWindowVisible(m_hwnd)) {
                PostMessage(m_hwnd,WM_PAINT,0,0);
            }
            break;

        

        case WM_SIZE:

	    OnSize(LOWORD(lParam), HIWORD(lParam));
            return (LRESULT) 0;

        

        case WM_CLOSE:

            OnClose();
            return (LRESULT) 0;
    }
    return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

LRESULT CBaseWindow::OnPaletteChange(HWND hwnd,UINT Message)
{
    

    if (m_hwnd == NULL || hwnd == NULL) {
        return (LRESULT) 0;
    }
    ASSERT(!m_bRealizing);

    

    if ((Message == WM_QUERYNEWPALETTE || hwnd != m_hwnd)) {
        
        
        
        
        if (!IsWindowVisible(m_hwnd)) {
            DbgLog((LOG_TRACE, 1, TEXT("Realizing when invisible!")));
            return (LRESULT) 0;
        }

        
#ifdef DEBUG
        m_bRealizing = TRUE;
#endif
        DoRealisePalette(Message != WM_QUERYNEWPALETTE);
#ifdef DEBUG
        m_bRealizing = FALSE;
#endif

        
        if (Message == WM_PALETTECHANGED) {
            InvalidateRect(m_hwnd,NULL,FALSE);
        }
    }

    return (LRESULT) 1;
}

RECT CBaseWindow::GetDefaultRect()
{
    RECT DefaultRect = {0,0,DEFWIDTH,DEFHEIGHT};
    ASSERT(m_hwnd);
    
    return DefaultRect;
}

LONG CBaseWindow::GetWindowWidth()
{
    ASSERT(m_hwnd);
    
    return m_Width;
}

LONG CBaseWindow::GetWindowHeight()
{
    ASSERT(m_hwnd);
    
    return m_Height;
}

HWND CBaseWindow::GetWindowHWND()
{
    ASSERT(m_hwnd);
    
    return m_hwnd;
}

HDC CBaseWindow::GetWindowHDC()
{
    ASSERT(m_hwnd);
    ASSERT(m_hdc);
    return m_hdc;
}

HDC CBaseWindow::GetMemoryHDC()
{
    ASSERT(m_hwnd);
    ASSERT(m_MemoryDC);
    return m_MemoryDC;
}

HRESULT CBaseWindow::DoShowWindow(LONG ShowCmd)
{
    ShowWindow(m_hwnd,ShowCmd);
    return NOERROR;
}

void CBaseWindow::PaintWindow(BOOL bErase)
{
    InvalidateRect(m_hwnd,NULL,bErase);
}

void CBaseWindow::DoSetWindowForeground(BOOL bFocus)
{
    SendMessage(m_hwnd,m_ShowStageMessage,(WPARAM) bFocus,(LPARAM) 0);
}

CDrawImage::CDrawImage(CBaseWindow *pBaseWindow) :
    m_pBaseWindow(pBaseWindow),
    m_hdc(NULL),
    m_MemoryDC(NULL),
    m_bStretch(FALSE),
    m_pMediaType(NULL),
    m_bUsingImageAllocator(FALSE)
{
    ASSERT(pBaseWindow);
    ResetPaletteVersion();
    SetRectEmpty(&m_TargetRect);
    SetRectEmpty(&m_SourceRect);

    m_perfidRenderTime = MSR_REGISTER("Single Blt time");
}

void CDrawImage::DisplaySampleTimes(IMediaSample *pSample)
{
    TCHAR szTimes[TIMELENGTH];      
    ASSERT(pSample);                
    RECT ClientRect;                
    SIZE Size;                      

    

    pSample->GetTime((REFERENCE_TIME*)&m_StartSample, (REFERENCE_TIME*)&m_EndSample);
    HWND hwnd = m_pBaseWindow->GetWindowHWND();
    EXECUTE_ASSERT(GetClientRect(hwnd,&ClientRect));

    

    wsprintf(szTimes,TEXT("%08d : %08d"),
             m_StartSample.Millisecs(),
             m_EndSample.Millisecs());

    ASSERT(lstrlen(szTimes) < TIMELENGTH);

    

    GetTextExtentPoint32(m_hdc,szTimes,lstrlen(szTimes),&Size);
    INT XPos = ((ClientRect.right - ClientRect.left) - Size.cx) / 2;
    INT YPos = ((ClientRect.bottom - ClientRect.top) - Size.cy) * 4 / 5;

    

    if ((XPos > 0) && (YPos > 0)) {
        TextOut(m_hdc,XPos,YPos,szTimes,lstrlen(szTimes));
    }
}

void CDrawImage::UpdateColourTable(HDC hdc,BITMAPINFOHEADER *pbmi)
{
    ASSERT(pbmi->biClrUsed);
    RGBQUAD *pColourTable = (RGBQUAD *)(pbmi+1);

    

    UINT uiReturn = SetDIBColorTable(hdc,(UINT) 0,
                                     pbmi->biClrUsed,
                                     pColourTable);

    
    ASSERT(uiReturn == pbmi->biClrUsed);
}

RECT CDrawImage::ScaleSourceRect(const RECT *pSource)
{
    ASSERT(pSource);
    return *pSource;
}

void CDrawImage::FastRender(IMediaSample *pMediaSample)
{
    BITMAPINFOHEADER *pbmi;     
    DIBDATA *pDibData;          
    BYTE *pImage;               
    HBITMAP hOldBitmap;         
    CImageSample *pSample;      

    ASSERT(m_pMediaType);

    
    
    

    pbmi = HEADER(m_pMediaType->Format());
    pSample = (CImageSample *) pMediaSample;
    pDibData = pSample->GetDIBData();
    hOldBitmap = (HBITMAP) SelectObject(m_MemoryDC,pDibData->hBitmap);

    

    HRESULT hr = pMediaSample->GetPointer(&pImage);
    if (FAILED(hr)) {
        return;
    }

    
    
    
    
    

    if (pDibData->PaletteVersion < GetPaletteVersion()) {
        ASSERT(pbmi->biBitCount <= iPALETTE);
        UpdateColourTable(m_MemoryDC,pbmi);
        pDibData->PaletteVersion = GetPaletteVersion();
    }

    
    
    
    
    
    

    RECT SourceRect = ScaleSourceRect(&m_SourceRect);

    

    if (m_bStretch == FALSE) {

        

        BitBlt(
            (HDC) m_hdc,                            
            m_TargetRect.left,                      
            m_TargetRect.top,                       
            m_TargetRect.right - m_TargetRect.left, 
            m_TargetRect.bottom - m_TargetRect.top, 
            m_MemoryDC,                             
            SourceRect.left,                        
            SourceRect.top,                         
            SRCCOPY);                               

    } else {

        

        StretchBlt(
            (HDC) m_hdc,                            
            m_TargetRect.left,                      
            m_TargetRect.top,                       
            m_TargetRect.right - m_TargetRect.left, 
            m_TargetRect.bottom - m_TargetRect.top, 
            m_MemoryDC,                             
            SourceRect.left,                        
            SourceRect.top,                         
            SourceRect.right - SourceRect.left,     
            SourceRect.bottom - SourceRect.top,     
            SRCCOPY);                               
    }

    
    
    

    #ifdef DEBUG
    DisplaySampleTimes(pMediaSample);
    #endif

    
    SelectObject(m_MemoryDC,hOldBitmap);
}

void CDrawImage::SlowRender(IMediaSample *pMediaSample)
{
    

    ASSERT(m_pMediaType);
    BITMAPINFOHEADER *pbmi = HEADER(m_pMediaType->Format());
    BYTE *pImage;

    

    HRESULT hr = pMediaSample->GetPointer(&pImage);
    if (FAILED(hr)) {
        return;
    }

    
    
    
    
    
    

    RECT SourceRect = ScaleSourceRect(&m_SourceRect);

    LONG lAdjustedSourceTop = SourceRect.top;
    
    
    if (pbmi->biHeight > 0) {
       lAdjustedSourceTop = pbmi->biHeight - SourceRect.bottom;
    }
    

    if (m_bStretch == FALSE) {

        

        SetDIBitsToDevice(
            (HDC) m_hdc,                            
            m_TargetRect.left,                      
            m_TargetRect.top,                       
            m_TargetRect.right - m_TargetRect.left, 
            m_TargetRect.bottom - m_TargetRect.top, 
            SourceRect.left,                        
            lAdjustedSourceTop,                     
            (UINT) 0,                               
            pbmi->biHeight,                         
            pImage,                                 
            (BITMAPINFO *) pbmi,                    
            DIB_RGB_COLORS);                        

    } else {

        

        StretchDIBits(
            (HDC) m_hdc,                            
            m_TargetRect.left,                      
            m_TargetRect.top,                       
            m_TargetRect.right - m_TargetRect.left, 
            m_TargetRect.bottom - m_TargetRect.top, 
            SourceRect.left,                        
            lAdjustedSourceTop,                     
            SourceRect.right - SourceRect.left,     
            SourceRect.bottom - SourceRect.top,     
            pImage,                                 
            (BITMAPINFO *) pbmi,                    
            DIB_RGB_COLORS,                         
            SRCCOPY);                               
    }

    
    
    
    
    
    
    

    #ifdef DEBUG
    DisplaySampleTimes(pMediaSample);
    #endif
}

BOOL CDrawImage::DrawImage(IMediaSample *pMediaSample)
{
    ASSERT(m_hdc);
    ASSERT(m_MemoryDC);
    NotifyStartDraw();

    
    
    
    

    if (m_bUsingImageAllocator == FALSE) {
        SlowRender(pMediaSample);
        EXECUTE_ASSERT(GdiFlush());
        NotifyEndDraw();
        return TRUE;
    }

    

    FastRender(pMediaSample);
    EXECUTE_ASSERT(GdiFlush());
    NotifyEndDraw();
    return TRUE;
}

BOOL CDrawImage::DrawVideoImageHere(
    HDC hdc,
    IMediaSample *pMediaSample,
    LPRECT lprcSrc,
    LPRECT lprcDst
    )
{
    ASSERT(m_pMediaType);
    BITMAPINFOHEADER *pbmi = HEADER(m_pMediaType->Format());
    BYTE *pImage;

    

    HRESULT hr = pMediaSample->GetPointer(&pImage);
    if (FAILED(hr)) {
        return FALSE;
    }

    RECT SourceRect;
    RECT TargetRect;

    if (lprcSrc) {
        SourceRect = *lprcSrc;
    }
    else  SourceRect = ScaleSourceRect(&m_SourceRect);

    if (lprcDst) {
        TargetRect = *lprcDst;
    }
    else  TargetRect = m_TargetRect;

    LONG lAdjustedSourceTop = SourceRect.top;
    
    
    if (pbmi->biHeight > 0) {
       lAdjustedSourceTop = pbmi->biHeight - SourceRect.bottom;
    }

    

    BOOL bRet = (0 != StretchDIBits(hdc,
                                    TargetRect.left,
                                    TargetRect.top,
                                    TargetRect.right - TargetRect.left,
                                    TargetRect.bottom - TargetRect.top,
                                    SourceRect.left,
                                    lAdjustedSourceTop,
                                    SourceRect.right - SourceRect.left,
                                    SourceRect.bottom - SourceRect.top,
                                    pImage,
                                    (BITMAPINFO *)pbmi,
                                    DIB_RGB_COLORS,
                                    SRCCOPY));
    return bRet;
}

void CDrawImage::SetDrawContext()
{
    m_MemoryDC = m_pBaseWindow->GetMemoryHDC();
    m_hdc = m_pBaseWindow->GetWindowHDC();
}

void CDrawImage::SetTargetRect(RECT *pTargetRect)
{
    ASSERT(pTargetRect);
    m_TargetRect = *pTargetRect;
    SetStretchMode();
}

void CDrawImage::GetTargetRect(RECT *pTargetRect)
{
    ASSERT(pTargetRect);
    *pTargetRect = m_TargetRect;
}

void CDrawImage::SetSourceRect(RECT *pSourceRect)
{
    ASSERT(pSourceRect);
    m_SourceRect = *pSourceRect;
    SetStretchMode();
}

void CDrawImage::GetSourceRect(RECT *pSourceRect)
{
    ASSERT(pSourceRect);
    *pSourceRect = m_SourceRect;
}

void CDrawImage::SetStretchMode()
{
    

    LONG SourceWidth = m_SourceRect.right - m_SourceRect.left;
    LONG SinkWidth = m_TargetRect.right - m_TargetRect.left;
    LONG SourceHeight = m_SourceRect.bottom - m_SourceRect.top;
    LONG SinkHeight = m_TargetRect.bottom - m_TargetRect.top;

    m_bStretch = TRUE;
    if (SourceWidth == SinkWidth) {
        if (SourceHeight == SinkHeight) {
            m_bStretch = FALSE;
        }
    }
}

void CDrawImage::NotifyAllocator(BOOL bUsingImageAllocator)
{
    m_bUsingImageAllocator = bUsingImageAllocator;
}

BOOL CDrawImage::UsingImageAllocator()
{
    return m_bUsingImageAllocator;
}

void CDrawImage::NotifyMediaType(CMediaType *pMediaType)
{
    m_pMediaType = pMediaType;
}

LONG CDrawImage::GetPaletteVersion()
{
    return m_PaletteVersion;
}

void CDrawImage::ResetPaletteVersion()
{
    m_PaletteVersion = PALETTE_VERSION;
}

void CDrawImage::IncrementPaletteVersion()
{
    m_PaletteVersion++;
}

CImageAllocator::CImageAllocator(CBaseFilter *pFilter,
                                 TCHAR *pName,
                                 HRESULT *phr) :
    CBaseAllocator(pName,NULL,phr,TRUE,TRUE),
    m_pFilter(pFilter)
{
    ASSERT(phr);
    ASSERT(pFilter);
}

#ifdef DEBUG
CImageAllocator::~CImageAllocator()
{
    ASSERT(m_bCommitted == FALSE);
}
#endif

void CImageAllocator::Free()
{
    ASSERT(m_lAllocated == m_lFree.GetCount());
    EXECUTE_ASSERT(GdiFlush());
    CImageSample *pSample;
    DIBDATA *pDibData;

    while (m_lFree.GetCount() != 0) {
        pSample = (CImageSample *) m_lFree.RemoveHead();
        pDibData = pSample->GetDIBData();
        EXECUTE_ASSERT(DeleteObject(pDibData->hBitmap));
        EXECUTE_ASSERT(CloseHandle(pDibData->hMapping));
        delete pSample;
    }

    m_lAllocated = 0;
}

STDMETHODIMP CImageAllocator::CheckSizes(ALLOCATOR_PROPERTIES *pRequest)
{
    

    if (m_pMediaType == NULL) {
        return VFW_E_NOT_CONNECTED;
    }

    
    
    
    

    VIDEOINFOHEADER *pVideoInfo = (VIDEOINFOHEADER *) m_pMediaType->Format();

    
    
    
    

    if ((DWORD) pRequest->cbBuffer < pVideoInfo->bmiHeader.biSizeImage) {
        return E_INVALIDARG;
    }

    

    if (pRequest->cbPrefix > 0) {
        return E_INVALIDARG;
    }

    pRequest->cbBuffer = pVideoInfo->bmiHeader.biSizeImage;
    return NOERROR;
}

STDMETHODIMP CImageAllocator::SetProperties(
    ALLOCATOR_PROPERTIES * pRequest,
    ALLOCATOR_PROPERTIES * pActual)
{
    ALLOCATOR_PROPERTIES Adjusted = *pRequest;

    

    HRESULT hr = CheckSizes(&Adjusted);
    if (FAILED(hr)) {
        return hr;
    }
    return CBaseAllocator::SetProperties(&Adjusted, pActual);
}

HRESULT CImageAllocator::Alloc(void)
{
    ASSERT(m_pMediaType);
    CImageSample *pSample;
    DIBDATA DibData;

    

    HRESULT hr = CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    
    
    
    

    ASSERT(m_lAllocated == 0);
    while (m_lAllocated < m_lCount) {

        

        HRESULT hr = CreateDIB(m_lSize,DibData);
        if (FAILED(hr)) {
            return hr;
        }

        

        pSample = CreateImageSample(DibData.pBase,m_lSize);
        if (pSample == NULL) {
            EXECUTE_ASSERT(DeleteObject(DibData.hBitmap));
            EXECUTE_ASSERT(CloseHandle(DibData.hMapping));
            return E_OUTOFMEMORY;
        }

        

        pSample->SetDIBData(&DibData);
        m_lFree.Add(pSample);
        m_lAllocated++;
    }
    return NOERROR;
}

CImageSample *CImageAllocator::CreateImageSample(LPBYTE pData,LONG Length)
{
    HRESULT hr = NOERROR;
    CImageSample *pSample;

    

    pSample = new CImageSample((CBaseAllocator *) this,   
                               NAME("Video sample"),      
                               (HRESULT *) &hr,           
                               (LPBYTE) pData,            
                               (LONG) Length);            

    if (pSample == NULL || FAILED(hr)) {
        delete pSample;
        return NULL;
    }
    return pSample;
}

HRESULT CImageAllocator::CreateDIB(LONG InSize,DIBDATA &DibData)
{
    BITMAPINFO *pbmi;       
    BYTE *pBase;            
    HANDLE hMapping;        
    HBITMAP hBitmap;        

    

    hMapping = CreateFileMapping(hMEMORY,         
                                 NULL,            
                                 PAGE_READWRITE,  
                                 (DWORD) 0,       
                                 InSize,          
                                 NULL);           
    if (hMapping == NULL) {
        DWORD Error = GetLastError();
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, Error);
    }

    
    
    
    

    pbmi = (BITMAPINFO *) HEADER(m_pMediaType->Format());
    if (m_pMediaType == NULL) {
        DbgBreak("Invalid media type");
    }

    hBitmap = CreateDIBSection((HDC) NULL,          
                               pbmi,                
                               DIB_RGB_COLORS,      
                               (VOID **) &pBase,    
                               hMapping,            
                               (DWORD) 0);          

    if (hBitmap == NULL || pBase == NULL) {
        EXECUTE_ASSERT(CloseHandle(hMapping));
        DWORD Error = GetLastError();
        return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, Error);
    }

    

    DibData.hBitmap = hBitmap;
    DibData.hMapping = hMapping;
    DibData.pBase = pBase;
    DibData.PaletteVersion = PALETTE_VERSION;
    GetObject(hBitmap,sizeof(DIBSECTION),(VOID *)&DibData.DibSection);

    return NOERROR;
}

void CImageAllocator::NotifyMediaType(CMediaType *pMediaType)
{
    m_pMediaType = pMediaType;
}

STDMETHODIMP_(ULONG) CImageAllocator::NonDelegatingAddRef()
{
    return m_pFilter->AddRef();
}

STDMETHODIMP_(ULONG) CImageAllocator::NonDelegatingRelease()
{
    return m_pFilter->Release();
}

CImageSample::CImageSample(CBaseAllocator *pAllocator,
                           TCHAR *pName,
                           HRESULT *phr,
                           LPBYTE pBuffer,
                           LONG length) :
    CMediaSample(pName,pAllocator,phr,pBuffer,length),
    m_bInit(FALSE)
{
    ASSERT(pAllocator);
    ASSERT(pBuffer);
}

void CImageSample::SetDIBData(DIBDATA *pDibData)
{
    ASSERT(pDibData);
    m_DibData = *pDibData;
    m_bInit = TRUE;
}

DIBDATA *CImageSample::GetDIBData()
{
    ASSERT(m_bInit == TRUE);
    return &m_DibData;
}

CImagePalette::CImagePalette(CBaseFilter *pBaseFilter,
                             CBaseWindow *pBaseWindow,
                             CDrawImage *pDrawImage) :
    m_pBaseWindow(pBaseWindow),
    m_pFilter(pBaseFilter),
    m_pDrawImage(pDrawImage),
    m_hPalette(NULL)
{
    ASSERT(m_pFilter);
}

#ifdef DEBUG
CImagePalette::~CImagePalette()
{
    ASSERT(m_hPalette == NULL);
}
#endif

BOOL CImagePalette::ShouldUpdate(const VIDEOINFOHEADER *pNewInfo,
                                 const VIDEOINFOHEADER *pOldInfo)
{
    

    if (pOldInfo == NULL) {
        return TRUE;
    }

    

    if (ContainsPalette(pNewInfo) == FALSE) {
        if (ContainsPalette(pOldInfo) == FALSE) {
            return FALSE;
        }
    }

    

    DWORD VideoEntries = pNewInfo->bmiHeader.biClrUsed;
    if (ContainsPalette(pNewInfo) == TRUE)
        if (ContainsPalette(pOldInfo) == TRUE)
            if (pOldInfo->bmiHeader.biClrUsed == VideoEntries)
                if (pOldInfo->bmiHeader.biClrUsed > 0)
                    if (memcmp((PVOID) GetBitmapPalette(pNewInfo),
                               (PVOID) GetBitmapPalette(pOldInfo),
                               VideoEntries * sizeof(RGBQUAD)) == 0) {

                        return FALSE;
                    }
    return TRUE;
}

HRESULT CImagePalette::PreparePalette(const CMediaType *pmtNew,
                                      const CMediaType *pmtOld,
				      LPSTR szDevice)
{
    const VIDEOINFOHEADER *pNewInfo = (VIDEOINFOHEADER *) pmtNew->Format();
    const VIDEOINFOHEADER *pOldInfo = (VIDEOINFOHEADER *) pmtOld->Format();
    ASSERT(pNewInfo);

    
    
    
    

    if (ShouldUpdate(pNewInfo,pOldInfo) == FALSE) {
        NOTE("No update needed");
        return S_FALSE;
    }

    
    
    
    

    RemovePalette();
    m_pFilter->NotifyEvent(EC_PALETTE_CHANGED,0,0);

    

    if (ContainsPalette(pNewInfo) == FALSE) {
        NOTE("New has no palette");
        return S_FALSE;
    }

    
    
    
    

    NOTE("Making new colour palette");
    m_hPalette = MakePalette(pNewInfo, szDevice);
    ASSERT(m_hPalette != NULL);

    
    
    

    if (m_pBaseWindow) m_pBaseWindow->SetPalette(m_hPalette);

    
    
    

    if (m_pDrawImage) m_pDrawImage->IncrementPaletteVersion();
    return NOERROR;
}

HRESULT CImagePalette::CopyPalette(const CMediaType *pSrc,CMediaType *pDest)
{
    

    VIDEOINFOHEADER *pDestInfo = (VIDEOINFOHEADER *) pDest->Format();
    pDestInfo->bmiHeader.biClrUsed = 0;
    pDestInfo->bmiHeader.biClrImportant = 0;

    

    if (PALETTISED(pDestInfo) == FALSE) {
        NOTE("No destination palette");
        return S_FALSE;
    }

    

    const VIDEOINFOHEADER *pSrcInfo = (VIDEOINFOHEADER *) pSrc->Format();
    if (ContainsPalette(pSrcInfo) == FALSE) {
        NOTE("No source palette");
        return S_FALSE;
    }

    

    DWORD PaletteEntries = pSrcInfo->bmiHeader.biClrUsed;
    if (PaletteEntries == 0) {
        DWORD Maximum  = (1 << pSrcInfo->bmiHeader.biBitCount);
        NOTE1("Setting maximum colours (%d)",Maximum);
        PaletteEntries = Maximum;
    }

    

    ASSERT(pSrcInfo->bmiHeader.biClrUsed <= iPALETTE_COLORS);
    ASSERT(pSrcInfo->bmiHeader.biClrImportant <= PaletteEntries);
    ASSERT(COLORS(pDestInfo) == GetBitmapPalette(pDestInfo));
    pDestInfo->bmiHeader.biClrUsed = PaletteEntries;
    pDestInfo->bmiHeader.biClrImportant = pSrcInfo->bmiHeader.biClrImportant;
    ULONG BitmapSize = GetBitmapFormatSize(HEADER(pSrcInfo));

    if (pDest->FormatLength() < BitmapSize) {
        NOTE("Reallocating destination");
        pDest->ReallocFormatBuffer(BitmapSize);
    }

    

    CopyMemory((PVOID) COLORS(pDestInfo),
               (PVOID) GetBitmapPalette(pSrcInfo),
               PaletteEntries * sizeof(RGBQUAD));

    return NOERROR;
}

HRESULT CImagePalette::RemovePalette()
{
    

    if (m_hPalette == NULL) {
        return NOERROR;
    }

    

    HPALETTE hPalette = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
    ASSERT(hPalette);
    const HPALETTE hPalOurs = m_hPalette;

    
    
    
    

    if (m_pBaseWindow) {
        SelectPalette(m_pBaseWindow->GetWindowHDC(), hPalette, TRUE);
        SelectPalette(m_pBaseWindow->GetMemoryHDC(), hPalette, TRUE);
    }

    EXECUTE_ASSERT(DeleteObject(hPalOurs));
    m_hPalette = NULL;
    return NOERROR;
}

HPALETTE CImagePalette::MakePalette(const VIDEOINFOHEADER *pVideoInfo, LPSTR szDevice)
{
    ASSERT(ContainsPalette(pVideoInfo) == TRUE);
    ASSERT(pVideoInfo->bmiHeader.biClrUsed <= iPALETTE_COLORS);
    BITMAPINFOHEADER *pHeader = HEADER(pVideoInfo);

    const RGBQUAD *pColours;            
    LOGPALETTE *lp;                     
    HPALETTE hPalette;                  

    lp = (LOGPALETTE *) new BYTE[sizeof(LOGPALETTE) + SIZE_PALETTE];
    if (lp == NULL) {
        return NULL;
    }

    
    
    
    
    

    lp->palVersion = PALVERSION;
    lp->palNumEntries = (USHORT) pHeader->biClrUsed;
    if (lp->palNumEntries == 0) lp->palNumEntries = (1 << pHeader->biBitCount);
    pColours = GetBitmapPalette(pVideoInfo);

    for (DWORD dwCount = 0;dwCount < lp->palNumEntries;dwCount++) {
        lp->palPalEntry[dwCount].peRed = pColours[dwCount].rgbRed;
        lp->palPalEntry[dwCount].peGreen = pColours[dwCount].rgbGreen;
        lp->palPalEntry[dwCount].peBlue = pColours[dwCount].rgbBlue;
        lp->palPalEntry[dwCount].peFlags = 0;
    }

    MakeIdentityPalette(lp->palPalEntry, lp->palNumEntries, szDevice);

    

    hPalette = CreatePalette(lp);
    ASSERT(hPalette != NULL);
    delete[] lp;
    return hPalette;
}

HRESULT CImagePalette::MakeIdentityPalette(PALETTEENTRY *pEntry,INT iColours, LPSTR szDevice)
{
    PALETTEENTRY SystemEntries[10];         
    BOOL bIdentityPalette = TRUE;           
    ASSERT(iColours <= iPALETTE_COLORS);    
    const int PalLoCount = 10;              
    const int PalHiStart = 246;             

    

    if (iColours < 10) {
        return S_FALSE;
    }

    

    
    
    HDC hdc;
    if (szDevice == NULL || lstrcmpiA(szDevice, "DISPLAY") == 0)
        hdc = CreateDCA("DISPLAY", NULL, NULL, NULL);
    else
        hdc = CreateDCA(NULL, szDevice, NULL, NULL);
    if (NULL == hdc) {
        return E_OUTOFMEMORY;
    }
    INT Reserved = GetDeviceCaps(hdc,NUMRESERVED);
    if (Reserved != 20) {
        DeleteDC(hdc);
        return S_FALSE;
    }

    
    
    

    UINT Result = GetSystemPaletteEntries(hdc,0,PalLoCount,SystemEntries);
	UINT Count = 0;
    for (Count = 0;Count < Result;Count++) {
        if (SystemEntries[Count].peRed != pEntry[Count].peRed ||
                SystemEntries[Count].peGreen != pEntry[Count].peGreen ||
                    SystemEntries[Count].peBlue != pEntry[Count].peBlue) {
                        bIdentityPalette = FALSE;
        }
    }

    

    Result = GetSystemPaletteEntries(hdc,PalHiStart,PalLoCount,SystemEntries);
    for (Count = 0;Count < Result;Count++) {
        if (INT(Count) + PalHiStart < iColours) {
            if (SystemEntries[Count].peRed != pEntry[PalHiStart + Count].peRed ||
                    SystemEntries[Count].peGreen != pEntry[PalHiStart + Count].peGreen ||
                        SystemEntries[Count].peBlue != pEntry[PalHiStart + Count].peBlue) {
                            bIdentityPalette = FALSE;
            }
        }
    }

    

    DeleteDC(hdc);
    if (bIdentityPalette == FALSE) {
        return S_FALSE;
    }

    

    for (Count = PalLoCount;INT(Count) < min(PalHiStart,iColours);Count++) {
        pEntry[Count].peFlags = PC_NOCOLLAPSE;
    }
    return NOERROR;
}

CImageDisplay::CImageDisplay()
{
    RefreshDisplayType(NULL);
}

HRESULT CImageDisplay::RefreshDisplayType(LPSTR szDeviceName)
{
    CAutoLock cDisplayLock(this);

    

    ZeroMemory((PVOID)&m_Display,sizeof(VIDEOINFOHEADER)+sizeof(TRUECOLORINFO));
    m_Display.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_Display.bmiHeader.biBitCount = FALSE;

    

    
    HDC hdcDisplay;
    
    if (szDeviceName == NULL || lstrcmpiA(szDeviceName, "DISPLAY") == 0)
        hdcDisplay = CreateDCA("DISPLAY", NULL, NULL, NULL);
    else
        hdcDisplay = CreateDCA(NULL, szDeviceName, NULL, NULL);
    if (hdcDisplay == NULL) {
	ASSERT(FALSE);
	DbgLog((LOG_ERROR,1,TEXT("ACK! Can't get a DC for %hs"),
				szDeviceName ? szDeviceName : "<NULL>"));
	return E_FAIL;
    } else {
	DbgLog((LOG_TRACE,3,TEXT("Created a DC for %s"),
				szDeviceName ? szDeviceName : "<NULL>"));
    }
    HBITMAP hbm = CreateCompatibleBitmap(hdcDisplay,1,1);
    GetDIBits(hdcDisplay,hbm,0,1,NULL,(BITMAPINFO *)&m_Display.bmiHeader,DIB_RGB_COLORS);

    
    GetDIBits(hdcDisplay,hbm,0,1,NULL,(BITMAPINFO *)&m_Display.bmiHeader,DIB_RGB_COLORS);
    DeleteObject(hbm);
    DeleteDC(hdcDisplay);

    

    ASSERT(CheckHeaderValidity(&m_Display));
    UpdateFormat(&m_Display);
    DbgLog((LOG_TRACE,3,TEXT("New DISPLAY bit depth =%d"),
				m_Display.bmiHeader.biBitCount));
    return NOERROR;
}

BOOL CImageDisplay::CheckBitFields(const VIDEOINFO *pInput)
{
    DWORD *pBitFields = (DWORD *) BITMASKS(pInput);

    for (INT iColour = iRED;iColour <= iBLUE;iColour++) {

        

        DWORD SetBits = CountSetBits(pBitFields[iColour]);
        if (SetBits > iMAXBITS || SetBits == 0) {
            NOTE1("Bit fields for component %d invalid",iColour);
            return FALSE;
        }

        
        DWORD PrefixBits = CountPrefixBits(pBitFields[iColour]);

        
        
        
        
        
        

        DWORD TestField = pBitFields[iColour] >> PrefixBits;
        DWORD Mask = ULONG_MAX << SetBits;
        if (TestField & Mask) {
            NOTE1("Bit fields for component %d not contiguous",iColour);
            return FALSE;
        }
    }
    return TRUE;
}

DWORD CImageDisplay::CountSetBits(DWORD Field)
{
    

    DWORD Count = 0;
    DWORD init = Field;

    

    while (init) {
        init = init & (init - 1);  
        Count++;
    }
    return Count;
}

DWORD CImageDisplay::CountPrefixBits(DWORD Field)
{
    DWORD Mask = 1;
    DWORD Count = 0;

    while (TRUE) {
        if (Field & Mask) {
            return Count;
        }
        Count++;

        ASSERT(Mask != 0x80000000);
        if (Mask == 0x80000000) {
            return Count;
        }
        Mask <<= 1;
    }
}

BOOL CImageDisplay::CheckHeaderValidity(const VIDEOINFO *pInput)
{
    

    if (pInput->bmiHeader.biWidth <= 0 ||
	pInput->bmiHeader.biHeight <= 0) {
        NOTE("Invalid bitmap dimensions");
        return FALSE;
    }

    

    if (pInput->bmiHeader.biCompression != BI_RGB) {
        if (pInput->bmiHeader.biCompression != BI_BITFIELDS) {
            NOTE("Invalid compression format");
            return FALSE;
        }
    }

    

    if (pInput->bmiHeader.biCompression == BI_BITFIELDS) {
        if (pInput->bmiHeader.biBitCount != 16) {
            if (pInput->bmiHeader.biBitCount != 32) {
                NOTE("BI_BITFIELDS not 16/32 bit depth");
                return FALSE;
            }
        }
    }

    

    if (pInput->bmiHeader.biCompression == BI_BITFIELDS) {
        if (CheckBitFields(pInput) == FALSE) {
            NOTE("Bit fields are not valid");
            return FALSE;
        }
    }

    

    if (pInput->bmiHeader.biPlanes != 1) {
        NOTE("Number of planes not one");
        return FALSE;
    }

    

    if (pInput->bmiHeader.biSizeImage != GetBitmapSize(&pInput->bmiHeader)) {
        if (pInput->bmiHeader.biSizeImage) {
            NOTE("Image size incorrectly set");
            return FALSE;
        }
    }

    

    if (pInput->bmiHeader.biSize != sizeof(BITMAPINFOHEADER)) {
        NOTE("Size of BITMAPINFOHEADER wrong");
        return FALSE;
    }
    return CheckPaletteHeader(pInput);
}

BOOL CImageDisplay::CheckPaletteHeader(const VIDEOINFO *pInput)
{
    

    if (PALETTISED(pInput) == FALSE) {
        if (pInput->bmiHeader.biClrUsed) {
            NOTE("Invalid palette entries");
            return FALSE;
        }
        return TRUE;
    }

    

    if (pInput->bmiHeader.biCompression != BI_RGB) {
        NOTE("Palettised video must be BI_RGB");
        return FALSE;
    }

    

    if (pInput->bmiHeader.biClrUsed > PALETTE_ENTRIES(pInput)) {
        NOTE("Too many colours in palette");
        return FALSE;
    }

    

    if (pInput->bmiHeader.biClrImportant > pInput->bmiHeader.biClrUsed) {
        NOTE("Too many important colours");
        return FALSE;
    }
    return TRUE;
}

const VIDEOINFO *CImageDisplay::GetDisplayFormat()
{
    return &m_Display;
}

BOOL CImageDisplay::IsPalettised()
{
    return PALETTISED(&m_Display);
}

WORD CImageDisplay::GetDisplayDepth()
{
    return m_Display.bmiHeader.biBitCount;
}

HRESULT CImageDisplay::UpdateFormat(VIDEOINFO *pVideoInfo)
{
    ASSERT(pVideoInfo);

    BITMAPINFOHEADER *pbmi = HEADER(pVideoInfo);
    SetRectEmpty(&pVideoInfo->rcSource);
    SetRectEmpty(&pVideoInfo->rcTarget);

    

    if (PALETTISED(pVideoInfo)) {
        if (pVideoInfo->bmiHeader.biClrUsed == 0) {
            pVideoInfo->bmiHeader.biClrUsed = PALETTE_ENTRIES(pVideoInfo);
        }
    }

    
    
    

    if (pVideoInfo->bmiHeader.biClrImportant > pVideoInfo->bmiHeader.biClrUsed) {
        pVideoInfo->bmiHeader.biClrImportant = PALETTE_ENTRIES(pVideoInfo);
    }

    

    if (pVideoInfo->bmiHeader.biSizeImage == 0) {
        pVideoInfo->bmiHeader.biSizeImage = GetBitmapSize(&pVideoInfo->bmiHeader);
    }
    return NOERROR;
}

HRESULT CImageDisplay::CheckVideoType(const VIDEOINFO *pInput)
{
    

    if (CheckHeaderValidity(pInput) == FALSE) {
        return E_INVALIDARG;
    }

    

    if (m_Display.bmiHeader.biBitCount == pInput->bmiHeader.biBitCount) {
        if (PALETTISED(pInput) == TRUE) {
            ASSERT(PALETTISED(&m_Display) == TRUE);
            NOTE("(Video) Type connection ACCEPTED");
            return NOERROR;
        }
    }

    

    if (m_Display.bmiHeader.biBitCount > pInput->bmiHeader.biBitCount) {
        NOTE("(Video) Mismatch agreed");
        return NOERROR;
    }

    

    if (m_Display.bmiHeader.biBitCount < pInput->bmiHeader.biBitCount) {
        NOTE("(Video) Format mismatch");
        return E_INVALIDARG;
    }

    

    ASSERT(m_Display.bmiHeader.biBitCount == pInput->bmiHeader.biBitCount);
    ASSERT(PALETTISED(pInput) == FALSE);
    ASSERT(PALETTISED(&m_Display) == FALSE);

    
    
    
    

    const DWORD *pInputMask = GetBitMasks(pInput);
    const DWORD *pDisplayMask = GetBitMasks((VIDEOINFO *)&m_Display);

    if (pInputMask[iRED] != pDisplayMask[iRED] ||
            pInputMask[iGREEN] != pDisplayMask[iGREEN] ||
                pInputMask[iBLUE] != pDisplayMask[iBLUE]) {

        NOTE("(Video) Bit field mismatch");
        return E_INVALIDARG;
    }

    NOTE("(Video) Type connection ACCEPTED");
    return NOERROR;
}

const DWORD *CImageDisplay::GetBitMasks(const VIDEOINFO *pVideoInfo)
{
    static const DWORD FailMasks[] = {0,0,0};

    if (pVideoInfo->bmiHeader.biCompression == BI_BITFIELDS) {
        return BITMASKS(pVideoInfo);
    }

    ASSERT(pVideoInfo->bmiHeader.biCompression == BI_RGB);

    switch (pVideoInfo->bmiHeader.biBitCount) {
        case 16: return bits555;
        case 24: return bits888;
        case 32: return bits888;
        default: return FailMasks;
    }
}

HRESULT CImageDisplay::CheckMediaType(const CMediaType *pmtIn)
{
    

    const GUID *pFormatType = pmtIn->FormatType();
    if (*pFormatType != FORMAT_VideoInfo) {
        NOTE("Format GUID not a VIDEOINFOHEADER");
        return E_INVALIDARG;
    }
    ASSERT(pmtIn->Format());

    

    ULONG Length = pmtIn->FormatLength();
    if (Length < SIZE_VIDEOHEADER) {
        NOTE("Format smaller than a VIDEOHEADER");
        return E_FAIL;
    }

    VIDEOINFO *pInput = (VIDEOINFO *) pmtIn->Format();

    

    const GUID *pMajorType = pmtIn->Type();
    if (*pMajorType != MEDIATYPE_Video) {
        NOTE("Major type not MEDIATYPE_Video");
        return E_INVALIDARG;
    }

    

    const GUID *pSubType = pmtIn->Subtype();
    if (GetBitCount(pSubType) == USHRT_MAX) {
        NOTE("Invalid video media subtype");
        return E_INVALIDARG;
    }
    return CheckVideoType(pInput);
}

BOOL CImageDisplay::GetColourMask(DWORD *pMaskRed,
                                  DWORD *pMaskGreen,
                                  DWORD *pMaskBlue)
{
    CAutoLock cDisplayLock(this);
    *pMaskRed = 0xFF;
    *pMaskGreen = 0xFF;
    *pMaskBlue = 0xFF;

    

    if (m_Display.bmiHeader.biBitCount < 16) {
        return FALSE;
    }

    
    
    

    if (m_Display.bmiHeader.biBitCount == 24) {
        ASSERT(m_Display.bmiHeader.biCompression == BI_RGB);
        return TRUE;
    }

    

    const DWORD *pBitFields = (DWORD *) GetBitMasks((VIDEOINFO *)&m_Display);
    DWORD *pOutputMask[] = { pMaskRed, pMaskGreen, pMaskBlue };

    
    
    

    for (INT iColour = iRED;iColour <= iBLUE;iColour++) {

        

        DWORD PrefixBits = CountPrefixBits(pBitFields[iColour]);
        DWORD SetBits = CountSetBits(pBitFields[iColour]);

        
        
        

        *(pOutputMask[iColour]) = pBitFields[iColour] >> PrefixBits;
        *(pOutputMask[iColour]) <<= (iMAXBITS - SetBits);
    }
    return TRUE;
}

STDAPI ConvertVideoInfoToVideoInfo2(AM_MEDIA_TYPE *pmt)
{
    ASSERT(pmt->formattype == FORMAT_VideoInfo);
    VIDEOINFO *pVideoInfo = (VIDEOINFO *)pmt->pbFormat;
    PVOID pvNew = CoTaskMemAlloc(pmt->cbFormat + sizeof(VIDEOINFOHEADER2) -
                                 sizeof(VIDEOINFOHEADER));
    if (pvNew == NULL) {
        return E_OUTOFMEMORY;
    }
    CopyMemory(pvNew, pmt->pbFormat, FIELD_OFFSET(VIDEOINFOHEADER, bmiHeader));
    ZeroMemory((PBYTE)pvNew + FIELD_OFFSET(VIDEOINFOHEADER, bmiHeader),
               sizeof(VIDEOINFOHEADER2) - sizeof(VIDEOINFOHEADER));
    CopyMemory((PBYTE)pvNew + FIELD_OFFSET(VIDEOINFOHEADER2, bmiHeader),
               pmt->pbFormat + FIELD_OFFSET(VIDEOINFOHEADER, bmiHeader),
               pmt->cbFormat - FIELD_OFFSET(VIDEOINFOHEADER, bmiHeader));
    VIDEOINFOHEADER2 *pVideoInfo2 = (VIDEOINFOHEADER2 *)pvNew;
    pVideoInfo2->dwPictAspectRatioX = (DWORD)pVideoInfo2->bmiHeader.biWidth;
    pVideoInfo2->dwPictAspectRatioY = (DWORD)pVideoInfo2->bmiHeader.biHeight;
    pmt->formattype = FORMAT_VideoInfo2;
    CoTaskMemFree(pmt->pbFormat);
    pmt->pbFormat = (PBYTE)pvNew;
    pmt->cbFormat += sizeof(VIDEOINFOHEADER2) - sizeof(VIDEOINFOHEADER);
    return S_OK;
}
