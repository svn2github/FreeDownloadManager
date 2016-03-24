/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSINTERNETFILE_H__3693BB45_3388_4234_821D_FE3961082A7C__INCLUDED_)
#define AFX_FSINTERNETFILE_H__3693BB45_3388_4234_821D_FE3961082A7C__INCLUDED_

#include "fsinet.h"	
#include <limits.h>
#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
#include <wininet.h>
#include "fsServerConnection.h"
#include <fsString.h>
#include "fsSpeaking.h"

enum fsResumeSupportType
{
	RST_PRESENT,	
	RST_NONE,		
	RST_UNKNOWN,	
};

class fsInternetFile : public fsSpeaking
{
public:
	
	HINTERNET GetRawHandle();
	
	
	virtual fsInternetResult QuerySize(LPCTSTR pszFilePath);
	
	
	
	
	virtual fsInternetResult Open (LPCTSTR pszFilePath, UINT64 uStartPos) = NULL;
	virtual fsInternetResult OpenEx (LPCTSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize, UINT64 uUploadTotalSize) = NULL;
	
	
	
	
	virtual fsInternetResult Read (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwRead) = NULL;
	virtual fsInternetResult Write (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten) = NULL;
	
	
	
	
	LPCTSTR GetSuggestedFileName ();
	
	fsResumeSupportType IsResumeSupported ();
	
	
	BOOL GetContentType (LPTSTR pszType);
	
	
	
	BOOL GetLastModifiedDate (FILETIME* pTime);
	
	virtual void CloseHandle();
	
	LPCTSTR GetLastError();
	
	virtual UINT64 GetFileSize () {return m_uFileSize;};
	
	
	virtual fsInternetResult Initialize (fsInternetServerConnection *pServer);
	void SetSecurityCheckIgnoreFlags (DWORD flags);
	fsSecurityCheckType get_lastSctFailure () const;
	fsInternetFile();
	virtual ~fsInternetFile();

protected:
	fsResumeSupportType m_enRST;		
	LPTSTR m_pszLastError;		
	fsString m_strContentType;	
	BOOL m_bContentTypeValid;	
	FILETIME m_date;			
	BOOL m_bDateValid;			
	UINT64 m_uFileSize;			
	HINTERNET m_hFile;			
	fsInternetServerConnection *m_pServer;	
	fsString m_strSuggFileName;	
	DWORD m_sctIgnoreFlags = 0;
	fsSecurityCheckType m_lastSctFailure = SCT_NONE;
};

#endif 
