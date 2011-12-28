/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __PULLPIN_H__
#define __PULLPIN_H__

class CPullPin : public CAMThread
{
    IAsyncReader*       m_pReader;
    REFERENCE_TIME      m_tStart;
    REFERENCE_TIME      m_tStop;
    REFERENCE_TIME      m_tDuration;
    BOOL                m_bSync;

    enum ThreadMsg {
	TM_Pause,       
	TM_Start,       
	TM_Exit,        
    };

    ThreadMsg m_State;

    
    DWORD ThreadProc(void);

    
    void Process(void);

    
    void CleanupCancelled(void);

    
    HRESULT PauseThread();

    
    HRESULT StartThread();

    
    HRESULT StopThread();

    
    HRESULT QueueSample(
		REFERENCE_TIME& tCurrent,
		REFERENCE_TIME tAlignStop,
		BOOL bDiscontinuity);

    HRESULT CollectAndDeliver(
		REFERENCE_TIME tStart,
		REFERENCE_TIME tStop);

    HRESULT DeliverSample(
		IMediaSample* pSample,
		REFERENCE_TIME tStart,
		REFERENCE_TIME tStop);

protected:
    IMemAllocator *     m_pAlloc;

public:
    CPullPin();
    virtual ~CPullPin();

    
    
    
    
    
    
    HRESULT Connect(IUnknown* pUnk, IMemAllocator* pAlloc, BOOL bSync);

    
    HRESULT Disconnect();

    
    
    
    
    
    virtual HRESULT DecideAllocator(
		IMemAllocator* pAlloc,
		ALLOCATOR_PROPERTIES * pProps);

    
    
    HRESULT Seek(REFERENCE_TIME tStart, REFERENCE_TIME tStop);

    
    HRESULT Duration(REFERENCE_TIME* ptDuration);

    
    HRESULT Active(void);

    
    HRESULT Inactive(void);

    
    LONGLONG AlignDown(LONGLONG ll, LONG lAlign) {
	
	return ll & ~(lAlign-1);
    };

    LONGLONG AlignUp(LONGLONG ll, LONG lAlign) {
	
	return (ll + (lAlign -1)) & ~(lAlign -1);
    };

    
    
    IAsyncReader* GetReader() {
	m_pReader->AddRef();
	return m_pReader;
    };

    

    
    
    virtual HRESULT Receive(IMediaSample*) PURE;

    
    virtual HRESULT EndOfStream(void) PURE;

    
    
    
    
    virtual void OnError(HRESULT hr) PURE;

    
    virtual HRESULT BeginFlush() PURE;
    virtual HRESULT EndFlush() PURE;

};

#endif 
