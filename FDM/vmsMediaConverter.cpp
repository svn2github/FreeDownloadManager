/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsMediaConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsDLL vmsMediaConverter::m_dll;
long vmsMediaConverter::m_cDllRefs = 0;

vmsMediaConverter::vmsMediaConverter()
{
	
}

vmsMediaConverter::~vmsMediaConverter()
{

}

BOOL vmsMediaConverter::ConvertMedia(LPCSTR pszSrcFile, LPCSTR pszDstFile, LPCSTR pszDstFormat,
						  LPCSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel)
{
	Initialize ();
	
	typedef BOOL (*FNCM)(LPCSTR pszSrcFile, LPCSTR pszDstFile, LPCSTR pszDstFormat,
						  LPCSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel);

	FNCM pfn = (FNCM) m_dll.GetProcAddress ("ConvertMediaFile");

	BOOL bRes = FALSE;

	if (pfn)
	{
		bRes = pfn (pszSrcFile, pszDstFile, pszDstFormat, pszAudioCodec, nAudioChannels,
			nAudioBitrate, nAudioRate, pszVideoCodec, nVideoBitrate, nVideoFrameRate,
			nVideoFrameWidth, nVideoFrameHeight, pnProgress, pbCancel);
	}

	Shutdown ();

	return bRes;
}

void vmsMediaConverter::Initialize()
{
	InterlockedIncrement (&m_cDllRefs);

	if (m_dll.is_Loaded () == false)
	{
		m_dll.Load ("mediaconverter.dll");
		typedef void (*FNI)();
		FNI pfn = (FNI) m_dll.GetProcAddress ("Initialize");
		if (pfn)
			pfn ();
	}
}

void vmsMediaConverter::Shutdown()
{
	InterlockedDecrement (&m_cDllRefs);

	if (m_cDllRefs == 0)
	{
		typedef void (*FNS)();
		FNS pfn = (FNS) m_dll.GetProcAddress ("Shutdown");
		if (pfn)
			pfn ();
		m_dll.Free ();
	}
}
