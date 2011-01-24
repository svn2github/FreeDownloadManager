//
// Analogue of fsInternetFile class, but uses cURL library instead of 
// WinInet library. It's used for download files larger than 2GB because
// WinInet can't download such files.
//

#if !defined(AFX_FSINTERNETFILE2_H__2F47FA5E_F9B6_4C4C_B002_66C62E904353__INCLUDED_)
#define AFX_FSINTERNETFILE2_H__2F47FA5E_F9B6_4C4C_B002_66C62E904353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <limits.h>
#include "fsinet.h"	// Added by ClassView
#include <array.h>
#include "fsInternetFile.h"	// Added by ClassView

class fsInternetFile2 : public fsSpeaking 
{
public:
	void setInterface (LPCSTR psz);
	void setUseFtpAsciiMode (bool bUse);
	UINT64 GetFileSize();
	LPCSTR get_SuggestedFileName();
	fsResumeSupportType IsResumeSupported();
	BOOL GetLastModifiedDate(FILETIME *pTime);
	LPCSTR get_ContentType();
	// see fsInternetFile...
	void set_PostData (LPCSTR psz);
	void set_Proxy (LPCSTR pszProxy, LPCSTR pszUser, LPCSTR pszPwd);
	void set_Auth (LPCSTR pszUser, LPCSTR pszPwd);
	void set_Cookie (LPCSTR psz);
	void set_Referer (LPCSTR psz);
	void set_UseHttp11 (BOOL bUse);
	void set_UserAgent (LPCSTR psz);
	void set_ResumeFrom (UINT64 uStart);
	void StopDownloading();
	fsInternetResult Read(LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead);
	fsInternetResult StartDownloading ();
	// set URL for cURL handle
	void set_URL (LPCSTR pszURL);
	fsInternetResult Initialize();
	fsInternetFile2();
	virtual ~fsInternetFile2();

protected:
	fsResumeSupportType m_enRST;
	UINT64 m_uStartPos;
	BOOL m_bDoPause;
	UINT64 m_uFileSize;
	fsString m_strSuggestedFileName;
	fsString m_strHttpHeader;
	static size_t _WriteHeader(void *ptr, size_t size, size_t nmemb, void *stream);
	long m_fileTime;
	void ExtractFileInfoFromResponse();
	// this function will be called by cURL when this lib received data
	size_t OnWriteData (LPBYTE ptr, size_t size);
	// this struct we'll use to let know OnWriteData func that we are ready to get
	// some data
	struct fsNeedToRead {
		vms::array <BYTE> abData;	// read here
		UINT nDataLeft;				// amount of data requested
	} m_ReadReq;	// read request
	// events using by read data function
	HANDLE m_hevReadDataReq, m_hevReadDataDone;
	static fsInternetResult CURLcodeToIR (int code);
	fsInternetResult m_irLastError;
	// this thread will download data
	static DWORD WINAPI _threadDownload (LPVOID lp);
	// this will be called by cURL on data receive
	static size_t _WriteData (void *ptr, size_t size, size_t nmemb, void *stream);
	// cURL handle
	void* m_curl;
	// is downloading?
	bool m_bDownloading;
	// was some data received or an error occurred?
	bool m_bAnswrFromServRcvd;
	// set to true in order to stop downloading
	bool m_bNeedStop;
	fsString m_strContentType;
};

#endif // !defined(AFX_FSINTERNETFILE2_H__2F47FA5E_F9B6_4C4C_B002_66C62E904353__INCLUDED_)
