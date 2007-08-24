/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#include <streams.h>              

CBasePropertyPage::CBasePropertyPage(TCHAR *pName,      
                                     LPUNKNOWN pUnk,    
                                     int DialogId,      
                                     int TitleId) :     
    CUnknown(pName,pUnk),
    m_DialogId(DialogId),
    m_TitleId(TitleId),
    m_hwnd(NULL),
    m_Dlg(NULL),
    m_pPageSite(NULL),
    m_bObjectSet(FALSE),
    m_bDirty(FALSE)
{
}

#ifdef UNICODE
CBasePropertyPage::CBasePropertyPage(CHAR *pName,      
                                     LPUNKNOWN pUnk,    
                                     int DialogId,      
                                     int TitleId) :     
    CUnknown(pName,pUnk),
    m_DialogId(DialogId),
    m_TitleId(TitleId),
    m_hwnd(NULL),
    m_Dlg(NULL),
    m_pPageSite(NULL),
    m_bObjectSet(FALSE),
    m_bDirty(FALSE)
{
}
#endif    

STDMETHODIMP_(ULONG) CBasePropertyPage::NonDelegatingAddRef()
{
    LONG lRef = InterlockedIncrement(&m_cRef);
    ASSERT(lRef > 0);
    return max(ULONG(m_cRef),1ul);
}      

STDMETHODIMP_(ULONG) CBasePropertyPage::NonDelegatingRelease()
{
    

    if (InterlockedDecrement(&m_cRef) == 0) {
        m_cRef++;
        SetPageSite(NULL);
        SetObjects(0,NULL);
        delete this;
        return ULONG(0);
    } else {
        return max(ULONG(m_cRef),1ul);
    }
}      

STDMETHODIMP
CBasePropertyPage::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    if (riid == IID_IPropertyPage) {
        return GetInterface((IPropertyPage *)this,ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid,ppv);
    }
}      

STDMETHODIMP CBasePropertyPage::GetPageInfo(LPPROPPAGEINFO pPageInfo)
{
    CheckPointer(pPageInfo,E_POINTER);
    WCHAR wszTitle[STR_MAX_LENGTH];
    WideStringFromResource(wszTitle,m_TitleId);

    

    LPOLESTR pszTitle;
    HRESULT hr = AMGetWideString(wszTitle, &pszTitle);
    if (FAILED(hr)) {
        NOTE("No caption memory");
        return hr;
    }

    pPageInfo->cb               = sizeof(PROPPAGEINFO);
    pPageInfo->pszTitle         = pszTitle;
    pPageInfo->pszDocString     = NULL;
    pPageInfo->pszHelpFile      = NULL;
    pPageInfo->dwHelpContext    = 0;

    
    pPageInfo->size.cx          = 340;
    pPageInfo->size.cy          = 150;

    GetDialogSize(m_DialogId, DialogProc,0L,&pPageInfo->size);
    return NOERROR;
}      

INT_PTR CALLBACK CBasePropertyPage::DialogProc(HWND hwnd,
                                            UINT uMsg,
                                            WPARAM wParam,
                                            LPARAM lParam)
{
    CBasePropertyPage *pPropertyPage;

    switch (uMsg) {

        case WM_INITDIALOG:

            SetWindowLongPtr(hwnd, DWLP_USER, lParam);

            

            pPropertyPage = (CBasePropertyPage *) lParam;
            if (pPropertyPage == NULL) {
                return (LRESULT) 1;
            }
            pPropertyPage->m_Dlg = hwnd;
    }

    

    pPropertyPage = (CBasePropertyPage *) GetWindowLongPtr(hwnd, DWLP_USER);
    if (pPropertyPage == NULL) {
        return (LRESULT) 1;
    }
    return pPropertyPage->OnReceiveMessage(hwnd,uMsg,wParam,lParam);
}      

STDMETHODIMP CBasePropertyPage::SetObjects(ULONG cObjects,LPUNKNOWN *ppUnk)
{
    if (cObjects == 1) {

        if ((ppUnk == NULL) || (*ppUnk == NULL)) {
            return E_POINTER;
        }

        
        m_bObjectSet = TRUE ;
        return OnConnect(*ppUnk);

    } else if (cObjects == 0) {

        
        m_bObjectSet = FALSE ;
        return OnDisconnect();
    }

    DbgBreak("No support for more than one object");
    return E_UNEXPECTED;
}      

STDMETHODIMP CBasePropertyPage::Activate(HWND hwndParent,
                                         LPCRECT pRect,
                                         BOOL fModal)
{
    CheckPointer(pRect,E_POINTER);

    
    if (m_bObjectSet == FALSE) {
        return E_UNEXPECTED;
    }

    if (m_hwnd) {
        return E_UNEXPECTED;
    }

    m_hwnd = CreateDialogParam(g_hInst,
                               MAKEINTRESOURCE(m_DialogId),
                               hwndParent,
                               DialogProc,
                               (LPARAM) this);
    if (m_hwnd == NULL) {
        return E_OUTOFMEMORY;
    }

    OnActivate();
    Move(pRect);
    return Show(SW_SHOWNORMAL);
}      

STDMETHODIMP CBasePropertyPage::Move(LPCRECT pRect)
{
    CheckPointer(pRect,E_POINTER);

    if (m_hwnd == NULL) {
        return E_UNEXPECTED;
    }

    MoveWindow(m_hwnd,              
               pRect->left,         
               pRect->top,          
               WIDTH(pRect),        
               HEIGHT(pRect),       
               TRUE);               

    return NOERROR;
}      

STDMETHODIMP CBasePropertyPage::Show(UINT nCmdShow)
{
   

    if (m_hwnd == NULL) {
        return E_UNEXPECTED;
    }

    

    if ((nCmdShow != SW_SHOW) && (nCmdShow != SW_SHOWNORMAL) && (nCmdShow != SW_HIDE)) {
        return E_INVALIDARG;
    }

    ShowWindow(m_hwnd,nCmdShow);
    InvalidateRect(m_hwnd,NULL,TRUE);
    return NOERROR;
}      

STDMETHODIMP CBasePropertyPage::Deactivate(void)
{
    if (m_hwnd == NULL) {
        return E_UNEXPECTED;
    }

    

    DWORD dwStyle = GetWindowLong(m_hwnd, GWL_EXSTYLE);
    dwStyle = dwStyle & (~WS_EX_CONTROLPARENT);

    
    
    
    HWND hwnd = m_hwnd;
    m_hwnd = NULL;
    SetWindowLong(hwnd, GWL_EXSTYLE, dwStyle);
    m_hwnd = hwnd;

    OnDeactivate();

    

    DestroyWindow(m_hwnd);
    m_hwnd = NULL;
    return NOERROR;
}      

STDMETHODIMP CBasePropertyPage::SetPageSite(LPPROPERTYPAGESITE pPageSite)
{
    if (pPageSite) {

        if (m_pPageSite) {
            return E_UNEXPECTED;
        }

        m_pPageSite = pPageSite;
        m_pPageSite->AddRef();

    } else {

        if (m_pPageSite == NULL) {
            return E_UNEXPECTED;
        }

        m_pPageSite->Release();
        m_pPageSite = NULL;
    }
    return NOERROR;
}      

STDMETHODIMP CBasePropertyPage::Apply()
{
    
    
    
    
    

    if (m_bObjectSet == FALSE) {
        return E_UNEXPECTED;
    }

    

    if (m_pPageSite == NULL) {
        return E_UNEXPECTED;
    }

    

    if (m_bDirty == FALSE) {
        return NOERROR;
    }

    

    HRESULT hr = OnApplyChanges();
    if (SUCCEEDED(hr)) {
        m_bDirty = FALSE;
    }
    return hr;
}      

INT_PTR CBasePropertyPage::OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    
    
    
    
    

    CBasePropertyPage *pPropertyPage;
    {
        pPropertyPage = (CBasePropertyPage *) GetWindowLongPtr(hwnd, DWLP_USER);
        if (pPropertyPage->m_hwnd == NULL) {
            return 0;
        }
        switch (uMsg) {
          case WM_STYLECHANGING:
              if (wParam == GWL_EXSTYLE) {
                  LPSTYLESTRUCT lpss = (LPSTYLESTRUCT)lParam;
                  lpss->styleNew |= WS_EX_CONTROLPARENT;
                  return 0;
              }
        }
    }
		
    return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

