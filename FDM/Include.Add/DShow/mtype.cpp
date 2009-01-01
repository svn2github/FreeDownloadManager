/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                            

#include <streams.h>
#include <mmreg.h>

CMediaType::~CMediaType(){
    FreeMediaType(*this);
}  

CMediaType::CMediaType()
{
    InitMediaType();
}  

CMediaType::CMediaType(const GUID * type)
{
    InitMediaType();
    majortype = *type;
}      

CMediaType::CMediaType(const AM_MEDIA_TYPE& rt)
{
    CopyMediaType(this, &rt);
}

CMediaType::CMediaType(const CMediaType& rt)
{
    CopyMediaType(this, &rt);
}          

CMediaType&
CMediaType::operator=(const AM_MEDIA_TYPE& rt)
{
    if (&rt != this) {
        FreeMediaType(*this);
        CopyMediaType(this, &rt);
    }
    return *this;
}  

CMediaType&
CMediaType::operator=(const CMediaType& rt)
{
    *this = (AM_MEDIA_TYPE &) rt;
    return *this;
}

BOOL
CMediaType::operator == (const CMediaType& rt) const
{
    
    
    
    
    
    

    return ((IsEqualGUID(majortype,rt.majortype) == TRUE) &&
        (IsEqualGUID(subtype,rt.subtype) == TRUE) &&
        (IsEqualGUID(formattype,rt.formattype) == TRUE) &&
        (cbFormat == rt.cbFormat) &&
        ( (cbFormat == 0) ||
          (memcmp(pbFormat, rt.pbFormat, cbFormat) == 0)));
}  

BOOL
CMediaType::operator != (const CMediaType& rt) const
{
    

    if (*this == rt) {
        return FALSE;
    }
    return TRUE;
}  

BOOL
CMediaType::IsValid() const
{
    return (!IsEqualGUID(majortype,GUID_NULL));
}  

void
CMediaType::SetType(const GUID* ptype)
{
    majortype = *ptype;
}  

void
CMediaType::SetSubtype(const GUID* ptype)
{
    subtype = *ptype;
}  

ULONG
CMediaType::GetSampleSize() const {
    if (IsFixedSize()) {
        return lSampleSize;
    } else {
        return 0;
    }
}  

void
CMediaType::SetSampleSize(ULONG sz) {
    if (sz == 0) {
        SetVariableSize();
    } else {
        bFixedSizeSamples = TRUE;
        lSampleSize = sz;
    }
}  

void
CMediaType::SetVariableSize() {
    bFixedSizeSamples = FALSE;
}  

void
CMediaType::SetTemporalCompression(BOOL bCompressed) {
    bTemporalCompression = bCompressed;
}

BOOL
CMediaType::SetFormat(BYTE * pformat, ULONG cb)
{
    if (NULL == AllocFormatBuffer(cb))
	return(FALSE);

    ASSERT(pbFormat);
    memcpy(pbFormat, pformat, cb);
    return(TRUE);
}            

void
CMediaType::SetFormatType(const GUID *pformattype)
{
    formattype = *pformattype;
}      

void CMediaType::ResetFormatBuffer()
{
    if (cbFormat) {
        CoTaskMemFree((PVOID)pbFormat);
    }
    cbFormat = 0;
    pbFormat = NULL;
}          

BYTE*
CMediaType::AllocFormatBuffer(ULONG length)
{
    ASSERT(length);

    

    if (cbFormat == length) {
        return pbFormat;
    }

    

    BYTE *pNewFormat = (PBYTE)CoTaskMemAlloc(length);
    if (pNewFormat == NULL) {
        if (length <= cbFormat) return pbFormat; 
        return NULL;
    }

    

    if (cbFormat != 0) {
        ASSERT(pbFormat);
        CoTaskMemFree((PVOID)pbFormat);
    }

    cbFormat = length;
    pbFormat = pNewFormat;
    return pbFormat;
}            

BYTE*
CMediaType::ReallocFormatBuffer(ULONG length)
{
    ASSERT(length);

    

    if (cbFormat == length) {
        return pbFormat;
    }

    

    BYTE *pNewFormat = (PBYTE)CoTaskMemAlloc(length);
    if (pNewFormat == NULL) {
        if (length <= cbFormat) return pbFormat; 
        return NULL;
    }

    
    

    if (cbFormat != 0) {
        ASSERT(pbFormat);
        memcpy(pNewFormat,pbFormat,min(length,cbFormat));
        CoTaskMemFree((PVOID)pbFormat);
    }

    cbFormat = length;
    pbFormat = pNewFormat;
    return pNewFormat;
}    

void CMediaType::InitMediaType()
{
    ZeroMemory((PVOID)this, sizeof(*this));
    lSampleSize = 1;
    bFixedSizeSamples = TRUE;
}        

BOOL
CMediaType::IsPartiallySpecified(void) const
{
    if ((majortype == GUID_NULL) ||
        (formattype == GUID_NULL)) {
            return TRUE;
    } else {
        return FALSE;
    }
}

BOOL
CMediaType::MatchesPartial(const CMediaType* ppartial) const
{
    if ((ppartial->majortype != GUID_NULL) &&
        (majortype != ppartial->majortype)) {
            return FALSE;
    }
    if ((ppartial->subtype != GUID_NULL) &&
        (subtype != ppartial->subtype)) {
            return FALSE;
    }

    if (ppartial->formattype != GUID_NULL) {
        
        if (formattype != ppartial->formattype) {
            return FALSE;
        }
        if (cbFormat != ppartial->cbFormat) {
            return FALSE;
        }
        if ((cbFormat != 0) &&
            (memcmp(pbFormat, ppartial->pbFormat, cbFormat) != 0)) {
                return FALSE;
        }
    }

    return TRUE;

}              

void WINAPI DeleteMediaType(AM_MEDIA_TYPE *pmt)
{
    

    if (pmt == NULL) {
        return;
    }

    FreeMediaType(*pmt);
    CoTaskMemFree((PVOID)pmt);
}            

AM_MEDIA_TYPE * WINAPI CreateMediaType(AM_MEDIA_TYPE const *pSrc)
{
    ASSERT(pSrc);

    

    AM_MEDIA_TYPE *pMediaType =
        (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));

    if (pMediaType == NULL) {
        return NULL;
    }
    

    CopyMediaType(pMediaType,pSrc);

    return pMediaType;
}      

void WINAPI CopyMediaType(AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource)
{
    
    
    ASSERT(pmtSource != pmtTarget);
    *pmtTarget = *pmtSource;
    if (pmtSource->cbFormat != 0) {
        ASSERT(pmtSource->pbFormat != NULL);
        pmtTarget->pbFormat = (PBYTE)CoTaskMemAlloc(pmtSource->cbFormat);
        if (pmtTarget->pbFormat == NULL) {
            pmtTarget->cbFormat = 0;
        } else {
            CopyMemory((PVOID)pmtTarget->pbFormat, (PVOID)pmtSource->pbFormat,
                       pmtTarget->cbFormat);
        }
    }
    if (pmtTarget->pUnk != NULL) {
        pmtTarget->pUnk->AddRef();
    }
}    

void WINAPI FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0) {
        CoTaskMemFree((PVOID)mt.pbFormat);

        
        mt.cbFormat = 0;
        mt.pbFormat = NULL;
    }
    if (mt.pUnk != NULL) {
        mt.pUnk->Release();
        mt.pUnk = NULL;
    }
}    

STDAPI CreateAudioMediaType(
    const WAVEFORMATEX *pwfx,
    AM_MEDIA_TYPE *pmt,
    BOOL bSetFormat
)
{
    pmt->majortype            = MEDIATYPE_Audio;
    if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        pmt->subtype = ((PWAVEFORMATEXTENSIBLE)pwfx)->SubFormat;
    } else {
        pmt->subtype              = FOURCCMap(pwfx->wFormatTag);
    }
    pmt->formattype           = FORMAT_WaveFormatEx;
    pmt->bFixedSizeSamples    = TRUE;
    pmt->bTemporalCompression = FALSE;
    pmt->lSampleSize          = pwfx->nBlockAlign;
    pmt->pUnk                 = NULL;
    if (bSetFormat) {
        if (pwfx->wFormatTag == WAVE_FORMAT_PCM) {
            pmt->cbFormat         = sizeof(WAVEFORMATEX);
        } else {
            pmt->cbFormat         = sizeof(WAVEFORMATEX) + pwfx->cbSize;
        }
        pmt->pbFormat             = (PBYTE)CoTaskMemAlloc(pmt->cbFormat);
        if (pmt->pbFormat == NULL) {
            return E_OUTOFMEMORY;
        }
        if (pwfx->wFormatTag == WAVE_FORMAT_PCM) {
            CopyMemory(pmt->pbFormat, pwfx, sizeof(PCMWAVEFORMAT));
            ((WAVEFORMATEX *)pmt->pbFormat)->cbSize = 0;
        } else {
            CopyMemory(pmt->pbFormat, pwfx, pmt->cbFormat);
        }
    }
    return S_OK;
}  

#pragma warning(disable:4514)
