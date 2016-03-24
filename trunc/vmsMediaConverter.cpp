/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
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

BOOL vmsMediaConverter::ConvertMedia(LPCTSTR pszSrcFile, LPCTSTR pszDstFile, LPCTSTR pszDstFormat,
						  LPCTSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCTSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel, int *ffmpegErrCode,
						  std::string *ffmpegErrorMessage)
{
	std::string sSrcFile = utf8FromWide(pszSrcFile);
	std::string sDstFile = utf8FromWide(pszDstFile);
	std::string sDstFormat = stringFromTstring(pszDstFormat);
	std::string sAudioCodec = stringFromTstring(pszAudioCodec);
	std::string sVideoCodec = stringFromTstring(pszVideoCodec);

	enum ConvertResult
	{
		CR_OK						= 0,
		CR_INVALID_ARGS				= 1,
		CR_DLL_RELOAD_IS_REQUIRED	= 2,
		CR_FAILED					= 3,
	};

	static vmsCriticalSectionEx _cs;

	if (pbCancel)
	{
		while (_cs.isLocked () && *pbCancel == FALSE)
			Sleep (200);
		if (*pbCancel)
			return FALSE;
	}

	_cs.Lock ();

	Initialize ();

	BOOL bRes = FALSE;
	if (ffmpegErrCode)
		*ffmpegErrCode = 0;
	
	typedef BOOL (*FNCM)(LPCSTR pszSrcFile, LPCSTR pszDstFile, LPCSTR pszDstFormat,
						  LPCSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel);

	typedef int (*FNC)(LPCSTR pszSrcFile, LPCSTR pszDstFile, LPCSTR pszDstFormat,
						  LPCSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight, int nThreads,
						  int* pnProgress, BOOL *pbCancel, int *pnFfmpegResult);

	typedef int (*FNGED)(int, char*, size_t);

	FNC pfn = (FNC) m_dll.GetProcAddress ("Convert");
	if (pfn)
	{
		int nRes = pfn (sSrcFile.c_str(), sDstFile.c_str(), sDstFormat.c_str(), sAudioCodec.c_str(), nAudioChannels,
			nAudioBitrate, nAudioRate, sVideoCodec.c_str(), nVideoBitrate, nVideoFrameRate,
			nVideoFrameWidth, nVideoFrameHeight, 1, pnProgress, pbCancel, ffmpegErrCode);
		if (nRes == CR_DLL_RELOAD_IS_REQUIRED)
			MessageBox (*AfxGetApp ()->m_pMainWnd, _T("Failed to convert media file. Please try restart FDM.\nError: -2.\nPlease report us about this error."), LS (L_ERROR), MB_ICONERROR | MB_SETFOREGROUND);
		bRes = nRes == CR_OK;
		if (pbCancel && *pbCancel)
			bRes = FALSE;
	}
	else 
	{
		FNCM pfn = (FNCM) m_dll.GetProcAddress ("ConvertMediaFile"); 
		if (pfn)
		{
			bRes = pfn (sSrcFile.c_str(), sDstFile.c_str(), sDstFormat.c_str(), sAudioCodec.c_str(), nAudioChannels,
				nAudioBitrate, nAudioRate, sVideoCodec.c_str(), nVideoBitrate, nVideoFrameRate,
				nVideoFrameWidth, nVideoFrameHeight, pnProgress, pbCancel);
		}
	}
	
	if (!bRes && ffmpegErrCode && *ffmpegErrCode && ffmpegErrorMessage &&
		(!pbCancel || !*pbCancel))
	{
		FNGED pfn = (FNGED) m_dll.GetProcAddress ("GetErrorCodeDescription");
		if (pfn)
		{
			char sz [1000] = "";
			pfn (*ffmpegErrCode, sz, sizeof (sz));
			*ffmpegErrorMessage = sz;
		}
	}

	Shutdown ();

	if (!bRes)
		DeleteFile (pszDstFile);

	_cs.Unlock ();

	return bRes;
}

void vmsMediaConverter::Initialize()
{
	InterlockedIncrement (&m_cDllRefs);

	if (m_dll.is_Loaded () == false)
	{
		m_dll.Load (_T("mediaconverter.dll"));
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
