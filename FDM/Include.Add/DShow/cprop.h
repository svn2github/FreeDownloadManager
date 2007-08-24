/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#ifndef __CPROP__
#define __CPROP__                                        

class AM_NOVTABLE CBasePropertyPage : public IPropertyPage, public CUnknown
{
protected:

    LPPROPERTYPAGESITE m_pPageSite;       
    HWND m_hwnd;                          
    HWND m_Dlg;                           
    BOOL m_bDirty;                        
    int m_TitleId;                        
    int m_DialogId;                       

    static INT_PTR CALLBACK DialogProc(HWND hwnd,
                                       UINT uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam);

private:
    BOOL m_bObjectSet ;                  
public:

    CBasePropertyPage(TCHAR *pName,      
                      LPUNKNOWN pUnk,    
                      int DialogId,      
                      int TitleId);      

#ifdef UNICODE
    CBasePropertyPage(CHAR *pName,
                      LPUNKNOWN pUnk,
                      int DialogId,  
                      int TitleId);
#endif
    virtual ~CBasePropertyPage() { };
    DECLARE_IUNKNOWN

    

    virtual HRESULT OnConnect(IUnknown *pUnknown) { return NOERROR; };
    virtual HRESULT OnDisconnect() { return NOERROR; };
    virtual HRESULT OnActivate() { return NOERROR; };
    virtual HRESULT OnDeactivate() { return NOERROR; };
    virtual HRESULT OnApplyChanges() { return NOERROR; };
    virtual INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

    

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void **ppv);
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP SetPageSite(LPPROPERTYPAGESITE pPageSite);
    STDMETHODIMP Activate(HWND hwndParent,LPCRECT prect,BOOL fModal);
    STDMETHODIMP Deactivate(void);
    STDMETHODIMP GetPageInfo(LPPROPPAGEINFO pPageInfo);
    STDMETHODIMP SetObjects(ULONG cObjects, LPUNKNOWN *ppUnk);
    STDMETHODIMP Show(UINT nCmdShow);
    STDMETHODIMP Move(LPCRECT prect);
    STDMETHODIMP IsPageDirty(void) { return m_bDirty ? S_OK : S_FALSE; }
    STDMETHODIMP Apply(void);
    STDMETHODIMP Help(LPCWSTR lpszHelpDir) { return E_NOTIMPL; }
    STDMETHODIMP TranslateAccelerator(LPMSG lpMsg) { return E_NOTIMPL; }
};

#endif 

