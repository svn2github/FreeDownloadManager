/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                                

#ifndef __WINUTIL__
#define __WINUTIL__

const int DEFWIDTH = 320;                    
const int DEFHEIGHT = 240;                   
const int CAPTION = 256;                     
const int TIMELENGTH = 50;                   
const int PROFILESTR = 128;                  
const WORD PALVERSION = 0x300;               
const LONG PALETTE_VERSION = (LONG) 1;       
const COLORREF VIDEO_COLOUR = 0;             
const HANDLE hMEMORY = (HANDLE) (-1);        

#define WIDTH(x) ((*(x)).right - (*(x)).left)
#define HEIGHT(x) ((*(x)).bottom - (*(x)).top)
#define SHOWSTAGE TEXT("WM_SHOWSTAGE")
#define SHOWSTAGETOP TEXT("WM_SHOWSTAGETOP")
#define REALIZEPALETTE TEXT("WM_REALIZEPALETTE")

class AM_NOVTABLE CBaseWindow
{
protected:

    HINSTANCE m_hInstance;          
    HWND m_hwnd;                    
    HDC m_hdc;                      
    LONG m_Width;                   
    LONG m_Height;                  
    BOOL m_bActivated;              
    LPTSTR m_pClassName;            
    DWORD m_ClassStyles;            
    DWORD m_WindowStyles;           
    DWORD m_WindowStylesEx;         
    UINT m_ShowStageMessage;        
    UINT m_ShowStageTop;            
    UINT m_RealizePalette;          
    HDC m_MemoryDC;                 
    HPALETTE m_hPalette;            
    BYTE m_bNoRealize;              
    BYTE m_bBackground;             
    BYTE m_bRealizing;              
    CCritSec m_WindowLock;          
    BOOL m_bDoGetDC;                
    bool m_bDoPostToDestroy;          

    
    friend LRESULT CALLBACK WndProc(HWND hwnd,      
                                    UINT uMsg,      
                                    WPARAM wParam,  
                                    LPARAM lParam); 

    virtual LRESULT OnPaletteChange(HWND hwnd, UINT Message);

public:

    CBaseWindow(BOOL bDoGetDC = TRUE, bool bPostToDestroy = false);

#ifdef DEBUG
    virtual ~CBaseWindow();
#endif

    virtual HRESULT DoneWithWindow();
    virtual HRESULT PrepareWindow();
    virtual HRESULT InactivateWindow();
    virtual HRESULT ActivateWindow();
    virtual BOOL OnSize(LONG Width, LONG Height);
    virtual BOOL OnClose();
    virtual RECT GetDefaultRect();
    virtual HRESULT UninitialiseWindow();
    virtual HRESULT InitialiseWindow(HWND hwnd);

    HRESULT CompleteConnect();
    HRESULT DoCreateWindow();

    HRESULT PerformanceAlignWindow();
    HRESULT DoShowWindow(LONG ShowCmd);
    void PaintWindow(BOOL bErase);
    void DoSetWindowForeground(BOOL bFocus);
    virtual HRESULT SetPalette(HPALETTE hPalette);
    void SetRealize(BOOL bRealize)
    {
        m_bNoRealize = !bRealize;
    }

    
    HRESULT SetPalette();

    virtual HRESULT DoRealisePalette(BOOL bForceBackground = FALSE);

    virtual BOOL PossiblyEatMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	    { return FALSE; };

    

    LONG GetWindowWidth();
    LONG GetWindowHeight();
    HWND GetWindowHWND();
    HDC GetMemoryHDC();
    HDC GetWindowHDC();

    

    virtual LRESULT OnReceiveMessage(HWND hwnd,          
                                     UINT uMsg,          
                                     WPARAM wParam,      
                                     LPARAM lParam);     

    

    virtual LPTSTR GetClassWindowStyles(
                            DWORD *pClassStyles,          
                            DWORD *pWindowStyles,         
                            DWORD *pWindowStylesEx) PURE; 
};                                    

class CDrawImage
{
protected:

    CBaseWindow *m_pBaseWindow;     
    CRefTime m_StartSample;         
    CRefTime m_EndSample;           
    HDC m_hdc;                      
    HDC m_MemoryDC;                 
    RECT m_TargetRect;              
    RECT m_SourceRect;              
    BOOL m_bStretch;                
    BOOL m_bUsingImageAllocator;    
    CMediaType *m_pMediaType;       
    int m_perfidRenderTime;         
    LONG m_PaletteVersion;          

    

    void SlowRender(IMediaSample *pMediaSample);
    void FastRender(IMediaSample *pMediaSample);
    void DisplaySampleTimes(IMediaSample *pSample);
    void UpdateColourTable(HDC hdc,BITMAPINFOHEADER *pbmi);
    void SetStretchMode();

public:

    

    CDrawImage(CBaseWindow *pBaseWindow);
    BOOL DrawImage(IMediaSample *pMediaSample);
    BOOL DrawVideoImageHere(HDC hdc, IMediaSample *pMediaSample,
                            LPRECT lprcSrc, LPRECT lprcDst);
    void SetDrawContext();
    void SetTargetRect(RECT *pTargetRect);
    void SetSourceRect(RECT *pSourceRect);
    void GetTargetRect(RECT *pTargetRect);
    void GetSourceRect(RECT *pSourceRect);
    virtual RECT ScaleSourceRect(const RECT *pSource);

    

    LONG GetPaletteVersion();
    void ResetPaletteVersion();
    void IncrementPaletteVersion();

    

    void NotifyAllocator(BOOL bUsingImageAllocator);
    void NotifyMediaType(CMediaType *pMediaType);
    BOOL UsingImageAllocator();

    

    void NotifyStartDraw() {
        MSR_START(m_perfidRenderTime);
    };

    

    void NotifyEndDraw() {
        MSR_STOP(m_perfidRenderTime);
    };
};          

typedef struct tagDIBDATA {

    LONG        PaletteVersion;     
    DIBSECTION  DibSection;         
    HBITMAP     hBitmap;            
    HANDLE      hMapping;           
    BYTE        *pBase;             

} DIBDATA;              

class CImageSample : public CMediaSample
{
protected:

    DIBDATA m_DibData;      
    BOOL m_bInit;           

public:

    

    CImageSample(CBaseAllocator *pAllocator,
                 TCHAR *pName,
                 HRESULT *phr,
                 LPBYTE pBuffer,
                 LONG length);

    

    void SetDIBData(DIBDATA *pDibData);
    DIBDATA *GetDIBData();
};                

class CImageAllocator : public CBaseAllocator
{
protected:

    CBaseFilter *m_pFilter;   
    CMediaType *m_pMediaType;           

    

    HRESULT Alloc();
    void Free();

    

    HRESULT CreateDIB(LONG InSize,DIBDATA &DibData);
    STDMETHODIMP CheckSizes(ALLOCATOR_PROPERTIES *pRequest);
    virtual CImageSample *CreateImageSample(LPBYTE pData,LONG Length);

public:

    

    CImageAllocator(CBaseFilter *pFilter,TCHAR *pName,HRESULT *phr);
#ifdef DEBUG
    ~CImageAllocator();
#endif

    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    void NotifyMediaType(CMediaType *pMediaType);

    

    STDMETHODIMP SetProperties(
        ALLOCATOR_PROPERTIES *pRequest,
        ALLOCATOR_PROPERTIES *pActual);
};                    

class CImagePalette
{
protected:

    CBaseWindow *m_pBaseWindow;             
    CBaseFilter *m_pFilter;       
    CDrawImage *m_pDrawImage;               
    HPALETTE m_hPalette;                    

public:

    CImagePalette(CBaseFilter *pBaseFilter,
                  CBaseWindow *pBaseWindow,
                  CDrawImage *pDrawImage);

#ifdef DEBUG
    virtual ~CImagePalette();
#endif

    static HPALETTE MakePalette(const VIDEOINFOHEADER *pVideoInfo, LPSTR szDevice);
    HRESULT RemovePalette();
    static HRESULT MakeIdentityPalette(PALETTEENTRY *pEntry,INT iColours, LPSTR szDevice);
    HRESULT CopyPalette(const CMediaType *pSrc,CMediaType *pDest);
    BOOL ShouldUpdate(const VIDEOINFOHEADER *pNewInfo,const VIDEOINFOHEADER *pOldInfo);
    HRESULT PreparePalette(const CMediaType *pmtNew,const CMediaType *pmtOld,LPSTR szDevice);

    BOOL DrawVideoImageHere(HDC hdc, IMediaSample *pMediaSample, LPRECT lprcSrc, LPRECT lprcDst)
    {
        return m_pDrawImage->DrawVideoImageHere(hdc, pMediaSample, lprcSrc,lprcDst);
    }
};                      

class CImageDisplay : public CCritSec
{
protected:

    
    
    VIDEOINFO m_Display;

    static DWORD CountSetBits(const DWORD Field);
    static DWORD CountPrefixBits(const DWORD Field);
    static BOOL CheckBitFields(const VIDEOINFO *pInput);

public:

    

    CImageDisplay();

    

    const VIDEOINFO *GetDisplayFormat();
    HRESULT RefreshDisplayType(LPSTR szDeviceName);
    static BOOL CheckHeaderValidity(const VIDEOINFO *pInput);
    static BOOL CheckPaletteHeader(const VIDEOINFO *pInput);
    BOOL IsPalettised();
    WORD GetDisplayDepth();

    

    HRESULT CheckMediaType(const CMediaType *pmtIn);
    HRESULT CheckVideoType(const VIDEOINFO *pInput);
    HRESULT UpdateFormat(VIDEOINFO *pVideoInfo);
    const DWORD *GetBitMasks(const VIDEOINFO *pVideoInfo);

    BOOL GetColourMask(DWORD *pMaskRed,
                       DWORD *pMaskGreen,
                       DWORD *pMaskBlue);
};  

STDAPI ConvertVideoInfoToVideoInfo2(AM_MEDIA_TYPE *pmt);

#endif 

