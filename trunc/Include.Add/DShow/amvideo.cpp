/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>
#include <limits.h>

const DWORD bits555[] = {0x007C00,0x0003E0,0x00001F};
const DWORD bits565[] = {0x00F800,0x0007E0,0x00001F};
const DWORD bits888[] = {0xFF0000,0x00FF00,0x0000FF};

const struct {
    const GUID *pSubtype;
    WORD BitCount;
    CHAR *pName;
    WCHAR *wszName;
} BitCountMap[] =  { &MEDIASUBTYPE_RGB1,        1,   "RGB Monochrome",     L"RGB Monochrome",   
                     &MEDIASUBTYPE_RGB4,        4,   "RGB VGA",            L"RGB VGA",          
                     &MEDIASUBTYPE_RGB8,        8,   "RGB 8",              L"RGB 8",            
                     &MEDIASUBTYPE_RGB565,      16,  "RGB 565 (16 bit)",   L"RGB 565 (16 bit)", 
                     &MEDIASUBTYPE_RGB555,      16,  "RGB 555 (16 bit)",   L"RGB 555 (16 bit)", 
                     &MEDIASUBTYPE_RGB24,       24,  "RGB 24",             L"RGB 24",           
                     &MEDIASUBTYPE_RGB32,       32,  "RGB 32",             L"RGB 32",
                     &MEDIASUBTYPE_ARGB32,    32,  "ARGB 32",             L"ARGB 32",
                     &MEDIASUBTYPE_Overlay,     0,   "Overlay",            L"Overlay",          
                     &GUID_NULL,                0,   "UNKNOWN",            L"UNKNOWN"           
};

STDAPI_(DWORD) GetBitmapSize(const BITMAPINFOHEADER *pHeader)
{
    return DIBSIZE(*pHeader);
}

STDAPI_(const GUID) GetTrueColorType(const BITMAPINFOHEADER *pbmiHeader)
{
    BITMAPINFO *pbmInfo = (BITMAPINFO *) pbmiHeader;
    ASSERT(pbmiHeader->biBitCount == 16);

    

    if (pbmiHeader->biCompression == BI_RGB) {
        return MEDIASUBTYPE_RGB555;
    }

    

    DWORD *pMask = (DWORD *) pbmInfo->bmiColors;
    if (pMask[0] == bits555[0]) {
        if (pMask[1] == bits555[1]) {
            if (pMask[2] == bits555[2]) {
                return MEDIASUBTYPE_RGB555;
            }
        }
    }

    

    pMask = (DWORD *) pbmInfo->bmiColors;
    if (pMask[0] == bits565[0]) {
        if (pMask[1] == bits565[1]) {
            if (pMask[2] == bits565[2]) {
                return MEDIASUBTYPE_RGB565;
            }
        }
    }
    return GUID_NULL;
}

STDAPI_(const GUID) GetBitmapSubtype(const BITMAPINFOHEADER *pbmiHeader)
{
    ASSERT(pbmiHeader);

    

    if (pbmiHeader->biCompression != BI_RGB) {
        if (pbmiHeader->biCompression != BI_BITFIELDS) {
            FOURCCMap FourCCMap(pbmiHeader->biCompression);
            return (const GUID) FourCCMap;
        }
    }

    

    switch(pbmiHeader->biBitCount) {
        case 1    :   return MEDIASUBTYPE_RGB1;
        case 4    :   return MEDIASUBTYPE_RGB4;
        case 8    :   return MEDIASUBTYPE_RGB8;
        case 16   :   return GetTrueColorType(pbmiHeader);
        case 24   :   return MEDIASUBTYPE_RGB24;
        case 32   :   return MEDIASUBTYPE_RGB32;
    }
    return GUID_NULL;
}

STDAPI_(WORD) GetBitCount(const GUID *pSubtype)
{
    ASSERT(pSubtype);
    const GUID *pMediaSubtype;
    INT iPosition = 0;

    
    

    while (TRUE) {
        pMediaSubtype = BitCountMap[iPosition].pSubtype;
        if (IsEqualGUID(*pMediaSubtype,GUID_NULL)) {
            return USHRT_MAX;
        }
        if (IsEqualGUID(*pMediaSubtype,*pSubtype)) {
            return BitCountMap[iPosition].BitCount;
        }
        iPosition++;
    }
}

int LocateSubtype(const GUID *pSubtype)
{
    ASSERT(pSubtype);
    const GUID *pMediaSubtype;
    INT iPosition = 0;

    
    

    while (TRUE) {
        pMediaSubtype = BitCountMap[iPosition].pSubtype;
        if (IsEqualGUID(*pMediaSubtype,*pSubtype) ||
            IsEqualGUID(*pMediaSubtype,GUID_NULL)
            )
        {
            break;
        }
        
        iPosition++;
    }

    return iPosition;
}

STDAPI_(WCHAR *) GetSubtypeNameW(const GUID *pSubtype)
{
    return BitCountMap[LocateSubtype(pSubtype)].wszName;
}

STDAPI_(CHAR *) GetSubtypeNameA(const GUID *pSubtype)
{
    return BitCountMap[LocateSubtype(pSubtype)].pName;
}

#ifndef GetSubtypeName
#error wxutil.h should have defined GetSubtypeName
#endif
#undef GetSubtypeName

STDAPI_(CHAR *) GetSubtypeName(const GUID *pSubtype)
{
    return GetSubtypeNameA(pSubtype);
}

STDAPI_(LONG) GetBitmapFormatSize(const BITMAPINFOHEADER *pHeader)
{
    
    LONG Size = SIZE_PREHEADER + pHeader->biSize;

    ASSERT(pHeader->biSize >= sizeof(BITMAPINFOHEADER));
    
    
    
    
    

    
    ASSERT(pHeader->biBitCount <= iPALETTE || pHeader->biClrUsed == 0);

    if (pHeader->biBitCount <= iPALETTE || pHeader->biClrUsed) {
        LONG Entries = (DWORD) 1 << pHeader->biBitCount;
        if (pHeader->biClrUsed) {
            Entries = pHeader->biClrUsed;
        }
        Size += Entries * sizeof(RGBQUAD);
    }

    
    
    

    if (pHeader->biCompression == BI_BITFIELDS) {
        Size += SIZE_MASKS;
    }

    
    
    
    
    

    return Size;
}

STDAPI_(BOOL) ContainsPalette(const VIDEOINFOHEADER *pVideoInfo)
{
    if (PALETTISED(pVideoInfo) == FALSE) {
        if (pVideoInfo->bmiHeader.biClrUsed == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

STDAPI_(const RGBQUAD *) GetBitmapPalette(const VIDEOINFOHEADER *pVideoInfo)
{
    if (pVideoInfo->bmiHeader.biCompression == BI_BITFIELDS) {
        return TRUECOLOR(pVideoInfo)->bmiColors;
    }
    return COLORS(pVideoInfo);
}
