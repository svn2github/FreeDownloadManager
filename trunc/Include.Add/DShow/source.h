/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CSOURCE__
#define __CSOURCE__

class CSourceStream;  

class CSource : public CBaseFilter {
public:

    CSource(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr);
    CSource(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid);
#ifdef UNICODE
    CSource(CHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr);
    CSource(CHAR *pName, LPUNKNOWN lpunk, CLSID clsid);
#endif
    ~CSource();

    int       GetPinCount(void);
    CBasePin *GetPin(int n);

    

    CCritSec*	pStateLock(void) { return &m_cStateLock; }	

    HRESULT     AddPin(CSourceStream *);
    HRESULT     RemovePin(CSourceStream *);

    STDMETHODIMP FindPin(
        LPCWSTR Id,
        IPin ** ppPin
    );

    int FindPinNumber(IPin *iPin);
    
protected:

    int             m_iPins;       
    	   			   
    CSourceStream **m_paStreams;   

    CCritSec m_cStateLock;	

};

class CSourceStream : public CAMThread, public CBaseOutputPin {
public:

    CSourceStream(TCHAR *pObjectName,
                  HRESULT *phr,
                  CSource *pms,
                  LPCWSTR pName);
#ifdef UNICODE
    CSourceStream(CHAR *pObjectName,
                  HRESULT *phr,
                  CSource *pms,
                  LPCWSTR pName);
#endif
    virtual ~CSourceStream(void);  

protected:

    CSource *m_pFilter;	

    
    
    
    
    
    
    

    
    
    virtual HRESULT FillBuffer(IMediaSample *pSamp) PURE;

    
    
    
    virtual HRESULT OnThreadCreate(void) {return NOERROR;};
    virtual HRESULT OnThreadDestroy(void) {return NOERROR;};
    virtual HRESULT OnThreadStartPlay(void) {return NOERROR;};

    
    
    

    HRESULT Active(void);    
    HRESULT Inactive(void);  

public:
    
    enum Command {CMD_INIT, CMD_PAUSE, CMD_RUN, CMD_STOP, CMD_EXIT};
    HRESULT Init(void) { return CallWorker(CMD_INIT); }
    HRESULT Exit(void) { return CallWorker(CMD_EXIT); }
    HRESULT Run(void) { return CallWorker(CMD_RUN); }
    HRESULT Pause(void) { return CallWorker(CMD_PAUSE); }
    HRESULT Stop(void) { return CallWorker(CMD_STOP); }

protected:
    Command GetRequest(void) { return (Command) CAMThread::GetRequest(); }
    BOOL    CheckRequest(Command *pCom) { return CAMThread::CheckRequest( (DWORD *) pCom); }

    
    virtual DWORD ThreadProc(void);  		

    virtual HRESULT DoBufferProcessingLoop(void);    

    
    
    

    
    virtual HRESULT CheckMediaType(const CMediaType *pMediaType);
    virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);  

    
    
    
    
    virtual HRESULT GetMediaType(CMediaType *pMediaType) {return E_UNEXPECTED;}

    STDMETHODIMP QueryId(
        LPWSTR * Id
    );
};

#endif 

