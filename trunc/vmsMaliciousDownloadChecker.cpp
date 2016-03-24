/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsMaliciousDownloadChecker.h"
#include "vmsSimpleFileDownloader.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsMaliciousDownloadChecker::vmsMaliciousDownloadChecker()
{
	m_dldr = NULL;
	m_bNeedStop = false;
}

vmsMaliciousDownloadChecker::~vmsMaliciousDownloadChecker()
{

}

fsInternetResult vmsMaliciousDownloadChecker::Check(LPCTSTR pszUrl)
{
	TCHAR szTmpPath [MY_MAX_PATH];
	TCHAR szTmpFile [MY_MAX_PATH];

	m_bNeedStop = false;

	GetTempPath (_countof (szTmpPath), szTmpPath);
	GetTempFileName (szTmpPath, _T("fdm"), 0, szTmpFile);

	
	CString strUrl;
	strUrl.Format (_T("http://fdm.freedownloadmanager.org/fromfdm/url.php?url=%s"), EncodeUrl (pszUrl));

	
	vmsSimpleFileDownloader dldr;
	m_dldr = &dldr;
	if (m_bNeedStop) {
		DeleteFile (szTmpFile);
		return IR_S_FALSE;
	}
	dldr.Download (strUrl, szTmpFile);
	while (dldr.IsRunning ())
		Sleep (50);
	m_dldr = NULL;
	if (dldr.GetLastError ().first != IR_SUCCESS) {
		DeleteFile (szTmpFile);
		return dldr.GetLastError ().first;
	}
	if (m_bNeedStop) {
		DeleteFile (szTmpFile);
		return IR_S_FALSE;
	}

	
	HANDLE hFile = CreateFile (szTmpFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_FLAG_DELETE_ON_CLOSE, NULL);
	ASSERT (hFile != INVALID_HANDLE_VALUE);
	if (hFile == INVALID_HANDLE_VALUE) {
		DeleteFile (szTmpFile);
		return IR_ERROR;
	}

	char szBuf [1000];
	DWORD dwSize = 0;
	ReadFile (hFile, szBuf, sizeof (szBuf), &dwSize, NULL);
	CloseHandle (hFile);

	if (dwSize == 0)
	{
		
		
		m_cOpinions = 0;
		m_cMalOpinions = 0;
		m_fRating = 0;
		m_strVirusCheckResult = _T("");
	}
	else
	{
		
		

		szBuf [dwSize] = 0;

		char szVCR [10000];
		sscanf (szBuf, "%d %f %d %s", &m_cOpinions, &m_fRating, &m_cMalOpinions, szVCR);

		std::wstring sRes;
		AnsiToUni(szBuf, sRes);

		m_strVirusCheckResult = sRes.c_str();
	}

	return IR_SUCCESS;
}

CString vmsMaliciousDownloadChecker::EncodeUrl(LPCTSTR pszUrl)
{
	CString str;

	auto utf8url = stringFromTstring (pszUrl);

	for (auto it = utf8url.begin (); it != utf8url.end (); ++it)
	{
		CHAR c = *it;
		if ((c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9'))
		{
			str += c;
		}
		else
		{
			
			
			TCHAR szHex [10];
			_stprintf_s (szHex, 10, _T("%%%x"), (int)(BYTE)c);
			str += szHex;
		}
	}

	return str;
}

int vmsMaliciousDownloadChecker::get_OpinionTotalCount()
{
	return m_cOpinions;
}

int vmsMaliciousDownloadChecker::get_MaliciousOpinionCount()
{
	return m_cMalOpinions;
}

float vmsMaliciousDownloadChecker::get_AverageRating()
{
	return m_fRating;
}

LPCTSTR vmsMaliciousDownloadChecker::get_VirusCheckResult()
{
	return m_strVirusCheckResult;
}

void vmsMaliciousDownloadChecker::Stop()
{
	m_bNeedStop = true;
	if (m_dldr)
		m_dldr->Stop ();
}
