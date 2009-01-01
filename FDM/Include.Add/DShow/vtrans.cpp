/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>
#include <measure.h>  

CVideoTransformFilter::CVideoTransformFilter
    ( TCHAR *pName, LPUNKNOWN pUnk, REFCLSID clsid)
    : CTransformFilter(pName, pUnk, clsid)
    , m_itrLate(0)
    , m_nKeyFramePeriod(0)      
    , m_nFramesSinceKeyFrame(0)
    , m_bSkipping(FALSE)
    , m_tDecodeStart(0)
    , m_itrAvgDecode(300000)    
    , m_bQualityChanged(FALSE)
{
#ifdef PERF
    RegisterPerfId();
#endif 
}  

CVideoTransformFilter::~CVideoTransformFilter()
{
  
}      

HRESULT CVideoTransformFilter::StartStreaming()
{
    m_itrLate = 0;
    m_nKeyFramePeriod = 0;       
    m_nFramesSinceKeyFrame = 0;
    m_bSkipping = FALSE;
    m_tDecodeStart = 0;
    m_itrAvgDecode = 300000;     
    m_bQualityChanged = FALSE;
    m_bSampleSkipped = FALSE;
    return NOERROR;
}      

HRESULT CVideoTransformFilter::EndFlush()
{
    {
        
        CAutoLock lck(&m_csReceive);

        
        
        
        
        
        CVideoTransformFilter::StartStreaming();
    }
    return CTransformFilter::EndFlush();
}  

HRESULT CVideoTransformFilter::AbortPlayback(HRESULT hr)
{
    NotifyEvent(EC_ERRORABORT, hr, 0);
    m_pOutput->DeliverEndOfStream();
    return hr;
}                                      

HRESULT CVideoTransformFilter::Receive(IMediaSample *pSample)
{
    
    
    
    
    
    
    
    
    
    

    ASSERT(CritCheckIn(&m_csReceive));
    AM_MEDIA_TYPE *pmtOut, *pmt;
#ifdef DEBUG
    FOURCCMap fccOut;
#endif
    HRESULT hr;
    ASSERT(pSample);
    IMediaSample * pOutSample;

    
    ASSERT (m_pOutput != NULL) ;

    
    
    
    
    

#define rcS1 ((VIDEOINFOHEADER *)(pmt->pbFormat))->rcSource
#define rcT1 ((VIDEOINFOHEADER *)(pmt->pbFormat))->rcTarget

    pSample->GetMediaType(&pmt);
    if (pmt != NULL && pmt->pbFormat != NULL) {

	
	ASSERT(!IsEqualGUID(pmt->majortype, GUID_NULL));
#ifdef DEBUG
        fccOut.SetFOURCC(&pmt->subtype);
	LONG lCompression = HEADER(pmt->pbFormat)->biCompression;
	LONG lBitCount = HEADER(pmt->pbFormat)->biBitCount;
	LONG lStride = (HEADER(pmt->pbFormat)->biWidth * lBitCount + 7) / 8;
	lStride = (lStride + 3) & ~3;
        DbgLog((LOG_TRACE,3,TEXT("*Changing input type on the fly to")));
        DbgLog((LOG_TRACE,3,TEXT("FourCC: %lx Compression: %lx BitCount: %ld"),
		fccOut.GetFOURCC(), lCompression, lBitCount));
        DbgLog((LOG_TRACE,3,TEXT("biHeight: %ld rcDst: (%ld, %ld, %ld, %ld)"),
		HEADER(pmt->pbFormat)->biHeight,
		rcT1.left, rcT1.top, rcT1.right, rcT1.bottom));
        DbgLog((LOG_TRACE,3,TEXT("rcSrc: (%ld, %ld, %ld, %ld) Stride: %ld"),
		rcS1.left, rcS1.top, rcS1.right, rcS1.bottom,
		lStride));
#endif

	
	
	

	StopStreaming();
	m_pInput->CurrentMediaType() = *pmt;
	DeleteMediaType(pmt);
	
	hr = StartStreaming();
	if (FAILED(hr)) {
	    return AbortPlayback(hr);
	}
    }

    
    

    if (ShouldSkipFrame(pSample)) {
        MSR_NOTE(m_idSkip);
        m_bSampleSkipped = TRUE;
        return NOERROR;
    }

    
    hr = InitializeOutputSample(pSample, &pOutSample);

    if (FAILED(hr)) {
        return hr;
    }

    m_bSampleSkipped = FALSE;

    
    

#define rcS ((VIDEOINFOHEADER *)(pmtOut->pbFormat))->rcSource
#define rcT ((VIDEOINFOHEADER *)(pmtOut->pbFormat))->rcTarget

    pOutSample->GetMediaType(&pmtOut);
    if (pmtOut != NULL && pmtOut->pbFormat != NULL) {

	
	ASSERT(!IsEqualGUID(pmtOut->majortype, GUID_NULL));
#ifdef DEBUG
        fccOut.SetFOURCC(&pmtOut->subtype);
	LONG lCompression = HEADER(pmtOut->pbFormat)->biCompression;
	LONG lBitCount = HEADER(pmtOut->pbFormat)->biBitCount;
	LONG lStride = (HEADER(pmtOut->pbFormat)->biWidth * lBitCount + 7) / 8;
	lStride = (lStride + 3) & ~3;
        DbgLog((LOG_TRACE,3,TEXT("*Changing output type on the fly to")));
        DbgLog((LOG_TRACE,3,TEXT("FourCC: %lx Compression: %lx BitCount: %ld"),
		fccOut.GetFOURCC(), lCompression, lBitCount));
        DbgLog((LOG_TRACE,3,TEXT("biHeight: %ld rcDst: (%ld, %ld, %ld, %ld)"),
		HEADER(pmtOut->pbFormat)->biHeight,
		rcT.left, rcT.top, rcT.right, rcT.bottom));
        DbgLog((LOG_TRACE,3,TEXT("rcSrc: (%ld, %ld, %ld, %ld) Stride: %ld"),
		rcS.left, rcS.top, rcS.right, rcS.bottom,
		lStride));
#endif

	
	
	

	StopStreaming();
	m_pOutput->CurrentMediaType() = *pmtOut;
	DeleteMediaType(pmtOut);
	hr = StartStreaming();

	if (SUCCEEDED(hr)) {
 	    
	    
	    
            DbgLog((LOG_TRACE,3,TEXT("Output format change means we must wait for a keyframe")));
	    m_nWaitForKey = 30;

	
	} else {

            
            
            
            pOutSample->Release();
	    AbortPlayback(hr);
            return hr;
	}
    }

    
    if (pSample->IsDiscontinuity() == S_OK) {
        DbgLog((LOG_TRACE,3,TEXT("Non-key discontinuity - wait for keyframe")));
	m_nWaitForKey = 30;
    }

    

    if (SUCCEEDED(hr)) {
        m_tDecodeStart = timeGetTime();
        MSR_START(m_idTransform);

        
        hr = Transform(pSample, pOutSample);

        
        MSR_STOP(m_idTransform);
        m_tDecodeStart = timeGetTime()-m_tDecodeStart;
        m_itrAvgDecode = m_tDecodeStart*(10000/16) + 15*(m_itrAvgDecode/16);

        
        if (m_nWaitForKey)
            m_nWaitForKey--;
        if (m_nWaitForKey && pSample->IsSyncPoint() == S_OK)
	    m_nWaitForKey = FALSE;

        
        if (m_nWaitForKey && hr == NOERROR) {
            DbgLog((LOG_TRACE,3,TEXT("still waiting for a keyframe")));
	    hr = S_FALSE;
	}
    }

    if (FAILED(hr)) {
        DbgLog((LOG_TRACE,1,TEXT("Error from video transform")));
    } else {
        
        
        
        
        

        if (hr == NOERROR) {
    	    hr = m_pOutput->Deliver(pOutSample);
        } else {
            
            
            
            
            if (S_FALSE == hr) {

                
                
                
                
                pOutSample->Release();
                m_bSampleSkipped = TRUE;
                if (!m_bQualityChanged) {
                    m_bQualityChanged = TRUE;
                    NotifyEvent(EC_QUALITY_CHANGE,0,0);
                }
                return NOERROR;
            }
        }
    }

    
    
    pOutSample->Release();
    ASSERT(CritCheckIn(&m_csReceive));

    return hr;
}    

BOOL CVideoTransformFilter::ShouldSkipFrame( IMediaSample * pIn)
{
    REFERENCE_TIME trStart, trStopAt;
    HRESULT hr = pIn->GetTime(&trStart, &trStopAt);

    
    if (hr != S_OK)
	return FALSE;

    int itrFrame = (int)(trStopAt - trStart);  

    if(S_OK==pIn->IsSyncPoint()) {
        MSR_INTEGER(m_idFrameType, 1);
        if ( m_nKeyFramePeriod < m_nFramesSinceKeyFrame ) {
            
            m_nKeyFramePeriod = m_nFramesSinceKeyFrame;
        }
        m_nFramesSinceKeyFrame = 0;
        m_bSkipping = FALSE;
    } else {
        MSR_INTEGER(m_idFrameType, 2);
        if (  m_nFramesSinceKeyFrame>m_nKeyFramePeriod
           && m_nKeyFramePeriod>0
           ) {
            
            
            m_nKeyFramePeriod = m_nFramesSinceKeyFrame;
        }
    }  

    
    
    
    
    
    if (m_itrAvgDecode*4>itrFrame) {

        
        
        if ( m_itrLate > itrFrame ) {

            
            
            
            

            
            
            
            
            

            
            
            if (m_nKeyFramePeriod>0) {
                
                
                
                
                int it = (itrFrame/10000)
                         * (m_nKeyFramePeriod-m_nFramesSinceKeyFrame -  1);
                MSR_INTEGER(m_idTimeTillKey, it);

                
#ifdef VTRANSPERF
                MSR_INTEGER(0, itrFrame);
                MSR_INTEGER(0, m_nFramesSinceKeyFrame);
                MSR_INTEGER(0, m_nKeyFramePeriod);
#endif
                if (m_itrLate/10000 > it) {
                    m_bSkipping = TRUE;
                    
                    
                } else {
#ifdef VTRANSPERF
                    MSR_INTEGER(0, 777770);  
#endif
                }
            } else {
#ifdef VTRANSPERF
                MSR_INTEGER(0, 777771);  
#endif
            }
        } else {
#ifdef VTRANSPERF
            MSR_INTEGER(0, 777772);  
            MSR_INTEGER(0, m_itrLate);
            MSR_INTEGER(0, itrFrame);
#endif
        }
    } else {
#ifdef VTRANSPERF
        MSR_INTEGER(0, 777773);  
        MSR_INTEGER(0, m_itrAvgDecode);
        MSR_INTEGER(0, itrFrame);
#endif
    }

    ++m_nFramesSinceKeyFrame;

    if (m_bSkipping) {
        
        
        
        
        
        
        
        m_itrLate = m_itrLate - itrFrame;
    }

    MSR_INTEGER(m_idLate, (int)m_itrLate/10000 ); 
    if (m_bSkipping) {
        if (!m_bQualityChanged) {
            m_bQualityChanged = TRUE;
            NotifyEvent(EC_QUALITY_CHANGE,0,0);
        }
    }
    return m_bSkipping;
}  

HRESULT CVideoTransformFilter::AlterQuality(Quality q)
{
    
    
    if (m_itrLate>300000000) {
        
        m_itrLate = 300000000;
    } else {
        m_itrLate = (int)q.Late;
    }
    

    
    
    
    
    
    

    return E_FAIL;     

}      

#pragma warning(disable:4514)

