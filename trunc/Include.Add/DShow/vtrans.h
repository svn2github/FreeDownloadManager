/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

class CVideoTransformFilter : public CTransformFilter
{
  public:

    CVideoTransformFilter(TCHAR *, LPUNKNOWN, REFCLSID clsid);
    ~CVideoTransformFilter();
    HRESULT EndFlush();

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
#ifdef PERF

    
    
    virtual void RegisterPerfId() {
        m_idSkip        = MSR_REGISTER("Video Transform Skip frame");
        m_idFrameType   = MSR_REGISTER("Video transform frame type");
        m_idLate        = MSR_REGISTER("Video Transform Lateness");
        m_idTimeTillKey = MSR_REGISTER("Video Transform Estd. time to next key");
        CTransformFilter::RegisterPerfId();
    }
#endif

  protected:

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    

    int m_nKeyFramePeriod; 
                           

    int m_nFramesSinceKeyFrame; 
                                

    BOOL m_bSkipping;           

#ifdef PERF
    int m_idFrameType;          
    int m_idSkip;               
    int m_idLate;               
    int m_idTimeTillKey;        
#endif

    virtual HRESULT StartStreaming();

    HRESULT AbortPlayback(HRESULT hr);	

    HRESULT Receive(IMediaSample *pSample);

    HRESULT AlterQuality(Quality q);

    BOOL ShouldSkipFrame(IMediaSample * pIn);

    int m_itrLate;              
                                
    int m_tDecodeStart;         
    int m_itrAvgDecode;         

    BOOL m_bNoSkip;             

    
    
    
    BOOL m_bQualityChanged;

    
    
    int m_nWaitForKey;
};
