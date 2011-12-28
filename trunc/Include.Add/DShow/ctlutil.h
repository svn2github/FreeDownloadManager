/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CTLUTIL__
#define __CTLUTIL__

#define OATRUE (-1)
#define OAFALSE (0)

class CBaseDispatch
{
    ITypeInfo * m_pti;

public:

    CBaseDispatch() : m_pti(NULL) {}
    ~CBaseDispatch();

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      REFIID riid,
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);
};

class AM_NOVTABLE CMediaControl :
    public IMediaControl,
    public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CMediaControl(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);
};

class AM_NOVTABLE CMediaEvent :
    public IMediaEventEx,
    public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CMediaEvent(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);
};

class AM_NOVTABLE CMediaPosition :
    public IMediaPosition,
    public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CMediaPosition(const TCHAR *, LPUNKNOWN);
    CMediaPosition(const TCHAR *, LPUNKNOWN, HRESULT *phr);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);

};

class COARefTime : public CRefTime {
public:

    COARefTime() {
    };

    COARefTime(CRefTime t)
        : CRefTime(t)
    {
    };

    COARefTime(REFERENCE_TIME t)
        : CRefTime(t)
    {
    };

    COARefTime(double d) {
        m_time = (LONGLONG) (d * 10000000);
    };

    operator double() {
        return double(m_time) / 10000000;
    };

    operator REFERENCE_TIME() {
        return m_time;
    };

    COARefTime& operator=(const double& rd)  {
        m_time = (LONGLONG) (rd * 10000000);
        return *this;
    }

    COARefTime& operator=(const REFERENCE_TIME& rt)  {
        m_time = rt;
        return *this;
    }

    inline BOOL operator==(const COARefTime& rt)
    {
        return m_time == rt.m_time;
    };

    inline BOOL operator!=(const COARefTime& rt)
    {
        return m_time != rt.m_time;
    };

    inline BOOL operator < (const COARefTime& rt)
    {
        return m_time < rt.m_time;
    };

    inline BOOL operator > (const COARefTime& rt)
    {
        return m_time > rt.m_time;
    };

    inline BOOL operator >= (const COARefTime& rt)
    {
        return m_time >= rt.m_time;
    };

    inline BOOL operator <= (const COARefTime& rt)
    {
        return m_time <= rt.m_time;
    };

    inline COARefTime operator+(const COARefTime& rt)
    {
        return COARefTime(m_time + rt.m_time);
    };

    inline COARefTime operator-(const COARefTime& rt)
    {
        return COARefTime(m_time - rt.m_time);
    };

    inline COARefTime operator*(LONG l)
    {
        return COARefTime(m_time * l);
    };

    inline COARefTime operator/(LONG l)
    {
        return COARefTime(m_time / l);
    };

private:
    
    
    COARefTime(LONG);
    void operator=(LONG);
};

class CPosPassThru : public IMediaSeeking, public CMediaPosition
{
    IPin *m_pPin;

    HRESULT GetPeer(IMediaPosition **ppMP);
    HRESULT GetPeerSeeking(IMediaSeeking **ppMS);

public:

    CPosPassThru(const TCHAR *, LPUNKNOWN, HRESULT*, IPin *);
    DECLARE_IUNKNOWN

    HRESULT ForceRefresh() {
        return S_OK;
    };

    
    virtual HRESULT GetMediaTime(LONGLONG *pStartTime,LONGLONG *pEndTime) {
        return E_FAIL;
    }

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void **ppv);

    
    STDMETHODIMP GetCapabilities( DWORD * pCapabilities );
    STDMETHODIMP CheckCapabilities( DWORD * pCapabilities );
    STDMETHODIMP SetTimeFormat(const GUID * pFormat);
    STDMETHODIMP GetTimeFormat(GUID *pFormat);
    STDMETHODIMP IsUsingTimeFormat(const GUID * pFormat);
    STDMETHODIMP IsFormatSupported( const GUID * pFormat);
    STDMETHODIMP QueryPreferredFormat( GUID *pFormat);
    STDMETHODIMP ConvertTimeFormat(LONGLONG * pTarget, const GUID * pTargetFormat,
                                   LONGLONG    Source, const GUID * pSourceFormat );
    STDMETHODIMP SetPositions( LONGLONG * pCurrent, DWORD CurrentFlags
                             , LONGLONG * pStop, DWORD StopFlags );

    STDMETHODIMP GetPositions( LONGLONG * pCurrent, LONGLONG * pStop );
    STDMETHODIMP GetCurrentPosition( LONGLONG * pCurrent );
    STDMETHODIMP GetStopPosition( LONGLONG * pStop );
    STDMETHODIMP SetRate( double dRate);
    STDMETHODIMP GetRate( double * pdRate);
    STDMETHODIMP GetDuration( LONGLONG *pDuration);
    STDMETHODIMP GetAvailable( LONGLONG *pEarliest, LONGLONG *pLatest );
    STDMETHODIMP GetPreroll( LONGLONG *pllPreroll );

    
    STDMETHODIMP get_Duration(REFTIME * plength);
    STDMETHODIMP put_CurrentPosition(REFTIME llTime);
    STDMETHODIMP get_StopTime(REFTIME * pllTime);
    STDMETHODIMP put_StopTime(REFTIME llTime);
    STDMETHODIMP get_PrerollTime(REFTIME * pllTime);
    STDMETHODIMP put_PrerollTime(REFTIME llTime);
    STDMETHODIMP get_Rate(double * pdRate);
    STDMETHODIMP put_Rate(double dRate);
    STDMETHODIMP get_CurrentPosition(REFTIME * pllTime);
    STDMETHODIMP CanSeekForward(LONG *pCanSeekForward);
    STDMETHODIMP CanSeekBackward(LONG *pCanSeekBackward);

private:
    HRESULT GetSeekingLongLong( HRESULT (__stdcall IMediaSeeking::*pMethod)( LONGLONG * ),
                                LONGLONG * pll );
};

class CRendererPosPassThru : public CPosPassThru
{
    CCritSec m_PositionLock;    
    LONGLONG m_StartMedia;      
    LONGLONG m_EndMedia;        
    BOOL m_bReset;              

public:

    

    CRendererPosPassThru(const TCHAR *, LPUNKNOWN, HRESULT*, IPin *);
    HRESULT RegisterMediaTime(IMediaSample *pMediaSample);
    HRESULT RegisterMediaTime(LONGLONG StartTime,LONGLONG EndTime);
    HRESULT GetMediaTime(LONGLONG *pStartTime,LONGLONG *pEndTime);
    HRESULT ResetMediaTime();
    HRESULT EOS();
};

STDAPI CreatePosPassThru(
    LPUNKNOWN pAgg,
    BOOL bRenderer,
    IPin *pPin,
    IUnknown **ppPassThru
);

class AM_NOVTABLE CBasicAudio : public IBasicAudio, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBasicAudio(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);
};

class AM_NOVTABLE CBaseBasicVideo : public IBasicVideo2, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBaseBasicVideo(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);

    STDMETHODIMP GetPreferredAspectRatio(
      long *plAspectX,
      long *plAspectY)
    {
        return E_NOTIMPL;
    }
};

class AM_NOVTABLE CBaseVideoWindow : public IVideoWindow, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBaseVideoWindow(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP GetTypeInfoCount(UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      OLECHAR  ** rgszNames,
      UINT cNames,
      LCID lcid,
      DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      DISPPARAMS * pdispparams,
      VARIANT * pvarResult,
      EXCEPINFO * pexcepinfo,
      UINT * puArgErr);
};

class AM_NOVTABLE CSourcePosition : public CMediaPosition
{

public:
    CSourcePosition(const TCHAR *, LPUNKNOWN, HRESULT*, CCritSec *);

    
    STDMETHODIMP get_Duration(REFTIME * plength);
    STDMETHODIMP put_CurrentPosition(REFTIME llTime);
    STDMETHODIMP get_StopTime(REFTIME * pllTime);
    STDMETHODIMP put_StopTime(REFTIME llTime);
    STDMETHODIMP get_PrerollTime(REFTIME * pllTime);
    STDMETHODIMP put_PrerollTime(REFTIME llTime);
    STDMETHODIMP get_Rate(double * pdRate);
    STDMETHODIMP put_Rate(double dRate);
    STDMETHODIMP CanSeekForward(LONG *pCanSeekForward);
    STDMETHODIMP CanSeekBackward(LONG *pCanSeekBackward);

    
    STDMETHODIMP get_CurrentPosition(REFTIME * pllTime) {
        return E_NOTIMPL;
    };

protected:

    
    virtual HRESULT ChangeStart() PURE;
    virtual HRESULT ChangeStop() PURE;
    virtual HRESULT ChangeRate() PURE;

    COARefTime m_Duration;
    COARefTime m_Start;
    COARefTime m_Stop;
    double m_Rate;

    CCritSec * m_pLock;
};

class AM_NOVTABLE CSourceSeeking :
    public IMediaSeeking,
    public CUnknown
{

public:

    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    

    STDMETHODIMP IsFormatSupported(const GUID * pFormat);
    STDMETHODIMP QueryPreferredFormat(GUID *pFormat);
    STDMETHODIMP SetTimeFormat(const GUID * pFormat);
    STDMETHODIMP IsUsingTimeFormat(const GUID * pFormat);
    STDMETHODIMP GetTimeFormat(GUID *pFormat);
    STDMETHODIMP GetDuration(LONGLONG *pDuration);
    STDMETHODIMP GetStopPosition(LONGLONG *pStop);
    STDMETHODIMP GetCurrentPosition(LONGLONG *pCurrent);
    STDMETHODIMP GetCapabilities( DWORD * pCapabilities );
    STDMETHODIMP CheckCapabilities( DWORD * pCapabilities );
    STDMETHODIMP ConvertTimeFormat( LONGLONG * pTarget, const GUID * pTargetFormat,
                                    LONGLONG    Source, const GUID * pSourceFormat );

    STDMETHODIMP SetPositions( LONGLONG * pCurrent,  DWORD CurrentFlags
			     , LONGLONG * pStop,  DWORD StopFlags );

    STDMETHODIMP GetPositions( LONGLONG * pCurrent, LONGLONG * pStop );

    STDMETHODIMP GetAvailable( LONGLONG * pEarliest, LONGLONG * pLatest );
    STDMETHODIMP SetRate( double dRate);
    STDMETHODIMP GetRate( double * pdRate);
    STDMETHODIMP GetPreroll(LONGLONG *pPreroll);

protected:

    
    CSourceSeeking(const TCHAR *, LPUNKNOWN, HRESULT*, CCritSec *);

    
    virtual HRESULT ChangeStart() PURE;
    virtual HRESULT ChangeStop() PURE;
    virtual HRESULT ChangeRate() PURE;

    CRefTime m_rtDuration;      
    CRefTime m_rtStart;         
    CRefTime m_rtStop;          
    double m_dRateSeeking;

    
    DWORD m_dwSeekingCaps;

    CCritSec * m_pLock;
};

class CCmdQueue;

class CDispParams : public DISPPARAMS
{
public:
    CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT *phr = NULL);
    ~CDispParams();
};

class CDeferredCommand
    : public CUnknown,
      public IDeferredCommand
{
public:

    CDeferredCommand(
        CCmdQueue * pQ,
        LPUNKNOWN   pUnk,               
        HRESULT *   phr,
        LPUNKNOWN   pUnkExecutor,       
        REFTIME     time,
        GUID*       iid,
        long        dispidMethod,
        short       wFlags,
        long        cArgs,
        VARIANT*    pDispParams,
        VARIANT*    pvarResult,
        short*      puArgErr,
        BOOL        bStream
        );

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP Cancel();
    STDMETHODIMP Confidence(
                    LONG* pConfidence);
    STDMETHODIMP Postpone(
                    REFTIME newtime);
    STDMETHODIMP GetHResult(
                    HRESULT* phrResult);

    

    HRESULT Invoke();

    

    
    BOOL IsStreamTime() {
       return m_bStream;
    };

    CRefTime GetTime() {
        return m_time;
    };

    REFIID GetIID() {
        return *m_iid;
    };

    long GetMethod() {
        return m_dispidMethod;
    };

    short GetFlags() {
        return m_wFlags;
    };

    DISPPARAMS* GetParams() {
        return &m_DispParams;
    };

    VARIANT* GetResult() {
        return m_pvarResult;
    };

protected:

    CCmdQueue* m_pQueue;

    
    LPUNKNOWN   m_pUnk;

    
    REFERENCE_TIME     m_time;
    GUID*       m_iid;
    long        m_dispidMethod;
    short       m_wFlags;
    VARIANT*    m_pvarResult;
    BOOL        m_bStream;
    CDispParams m_DispParams;
    DISPID      m_DispId;         

    
    CBaseDispatch   m_Dispatch;

    
    HRESULT     m_hrResult;
};

class AM_NOVTABLE CCmdQueue
{
public:
    CCmdQueue();
    virtual ~CCmdQueue();

    
    
    
    
    virtual HRESULT  New(
        CDeferredCommand **ppCmd,
        LPUNKNOWN   pUnk,
        REFTIME     time,
        GUID*       iid,
        long        dispidMethod,
        short       wFlags,
        long        cArgs,
        VARIANT*    pDispParams,
        VARIANT*    pvarResult,
        short*      puArgErr,
        BOOL        bStream
    );

    
    
    virtual HRESULT Insert(CDeferredCommand* pCmd);
    virtual HRESULT Remove(CDeferredCommand* pCmd);

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    virtual HRESULT SetSyncSource(IReferenceClock*);

    
    virtual HRESULT Run(REFERENCE_TIME tStreamTimeOffset);

    
    virtual HRESULT EndRun();

    
    
    
    
    
    
    virtual HRESULT GetDueCommand(CDeferredCommand ** ppCmd, long msTimeout);

    
    
    
    HANDLE GetDueHandle() {
        return HANDLE(m_evDue);
    };

    
    
    
    
    
    
    
    virtual HRESULT GetCommandDueFor(REFERENCE_TIME tStream, CDeferredCommand**ppCmd);

    
    BOOL CheckTime(CRefTime time, BOOL bStream) {

        
        if (!m_pClock) {
            return FALSE;
        }

        
        if (bStream) {

            
            if (!m_bRunning) {
                return FALSE;
            }
            
            time += m_StreamTimeOffset;
        }

        CRefTime Now;
        m_pClock->GetTime((REFERENCE_TIME*)&Now);
        return (time <= Now);
    };

protected:

    
    CCritSec m_Lock;

    
    CGenericList<CDeferredCommand> m_listPresentation;

    
    CGenericList<CDeferredCommand> m_listStream;

    
    CAMEvent m_evDue;

    
    void SetTimeAdvise(void);

    
    DWORD_PTR m_dwAdvise;

    
    CRefTime m_tCurrentAdvise;

    
    IReferenceClock* m_pClock;

    
    BOOL m_bRunning;

    
    CRefTime m_StreamTimeOffset;
};

#endif 
