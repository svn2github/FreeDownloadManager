/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "list.h"

#ifndef FDM_DLDR__RAWCODEONLY
#include "DownloadsWnd.h"
#endif

#include "downloadproperties.h"
#include "fsDownloadMgr.h"
#include "system.h"

#ifndef FDM_DLDR__RAWCODEONLY
extern CDownloadsWnd* _pwndDownloads;
#endif

void DrawVerticalFrame (HDC hdc, int xStart, int xEnd, int yStart, int height)
{
	HPEN hLightPen, hHilightPen, hShadowPen, hOldPen;

	hLightPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DLIGHT));
	hHilightPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DHILIGHT));
	hShadowPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DSHADOW));

	int yEnd = yStart + height;

	hOldPen = (HPEN) SelectObject (hdc, hLightPen);
	MoveToEx (hdc, xStart, yStart, NULL);
	LineTo (hdc, xStart, yEnd);

	SelectObject (hdc, hHilightPen);
	MoveToEx (hdc, xStart+1, yStart, NULL);
	LineTo (hdc, xStart+1, yEnd);

	SelectObject (hdc, hShadowPen);
	MoveToEx (hdc, xEnd, yStart, NULL);
	LineTo (hdc, xEnd, yEnd);

	SelectObject (hdc, hOldPen);

	DeleteObject (hLightPen);
	DeleteObject (hHilightPen);
	DeleteObject (hShadowPen);
}

void DrawHorizontalFrame (HDC hdc, int yStart, int yEnd, int xStart, int width)
{
	HPEN hLightPen, hDarkPen, hHilightPen, hShadowPen, hOldPen;

	hLightPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DLIGHT));
	hDarkPen  = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DDKSHADOW));
	hHilightPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DHILIGHT));
	hShadowPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DSHADOW));

	int xEnd = xStart + width;

	hOldPen = (HPEN) SelectObject (hdc, hLightPen);
	MoveToEx (hdc, xStart, yStart, NULL);
	LineTo (hdc, xEnd, yStart);

	SelectObject (hdc, hHilightPen);
	MoveToEx (hdc, xStart, yStart+1, NULL);
	LineTo (hdc, xEnd, yStart+1);

	SelectObject (hdc, hShadowPen);
	MoveToEx (hdc, xStart, yEnd, NULL);
	LineTo (hdc, xEnd, yEnd);

	SelectObject (hdc, hOldPen);

	DeleteObject (hLightPen);
	DeleteObject (hDarkPen);
	DeleteObject (hHilightPen);
	DeleteObject (hShadowPen);
}

void BytesToXBytes (UINT64 uBytes, float* pfXBytes, LPTSTR pszXVal)
{
	LPCTSTR apszX [] = {LS (L_B), LS (L_KB), LS (L_MB), _T("GB"), _T("TB"), _T("PB")};
	int i = 0;
	double dBytes = (double) (INT64)uBytes;

	while (dBytes > 9999)
	{
		dBytes /= 1024;
		i++;
	}

	if (pszXVal)
	{
		if (i >= _countof (apszX))
			_tcscpy (pszXVal, _T("?"));
		else
			_tcscpy (pszXVal, apszX [i]);
	}

	*pfXBytes = (FLOAT) dBytes;
}

#ifndef FDM_DLDR__RAWCODEONLY
CString BytesToString (UINT64 uSize, bool bForceXbytes)
{
	CString str;

	if (bForceXbytes || 
			(_pwndDownloads != NULL && FALSE == _pwndDownloads->IsSizesInBytes ()))
	{
		float val;
		TCHAR szDim [100];
		BytesToXBytes (uSize, &val, szDim);
		str.Format (_T("%.*g %s"), val > 999 ? 4 : 3, val, szDim);
	}
	else
		str = fsBytesToStr (uSize);

	return str;
}
#endif

void SystemTimeToStr (SYSTEMTIME *time, LPTSTR pszDate, LPTSTR pszTime, BOOL bSeconds)
{
	if (pszDate)
		GetDateFormat (LOCALE_USER_DEFAULT, DATE_SHORTDATE, time, NULL, pszDate, 100);
	
	if (pszTime)
	{
		if (bSeconds)
			GetTimeFormat (LOCALE_USER_DEFAULT, 0, time, NULL, pszTime, 100);
		else
			GetTimeFormat (LOCALE_USER_DEFAULT, TIME_NOSECONDS, time, NULL, pszTime, 100);
	}
}

void FileTimeToStr (FILETIME *time, LPTSTR pszDate, LPTSTR pszTime, BOOL bSeconds)
{
	SYSTEMTIME s;
	FileTimeToSystemTime (time, &s);
	SystemTimeToStr (&s, pszDate, pszTime, bSeconds);
}

BOOL fsErrorToStr (LPTSTR pszErr, DWORD dwMaxSize, DWORD* pdwLastError)
{
	LPCTSTR pszDesc;
	CString str;
	
	*pszErr = 0;

	switch (pdwLastError ? *pdwLastError : GetLastError ())
	{
		case ERROR_FILE_NOT_FOUND:
			pszDesc = LS (L_FILENOTFOUND);
		break;

		case ERROR_PATH_NOT_FOUND:
			pszDesc = LS (L_PATHNOTFOUND);
			break;

		case ERROR_BAD_FORMAT:
			pszDesc = LS (L_BADEXE);
			break;

		case ERROR_SHARING_VIOLATION:
			pszDesc = LS (L_FILEINUSE);
		break;

		case ERROR_ACCESS_DENIED:
			pszDesc = LS (L_ACCDENIED);
			break;

		case ERROR_NOT_READY:
			pszDesc = LS (L_DEVNOTREADY);
			break;

		case ERROR_DISK_FULL:
			pszDesc = LS (L_DISKFULL);
			break;

		case ERROR_INVALID_NAME:
			pszDesc = LS (L_BADFILENAME);
			break;

		case ERROR_ALREADY_EXISTS:
			pszDesc = LS (L_ALREXISTS);
			break;

		case ERROR_DIRECTORY:
			pszDesc = LS (L_INVALIDDIRNAME);
			break;
		
		default:
			str.Format (_T("%s - [%d]"), LS (L_UNKERR), pdwLastError ? *pdwLastError : GetLastError ());
			pszDesc = str;
	}

	if (_tcslen (pszDesc) >= dwMaxSize)
		return FALSE;

	_tcscpy (pszErr, pszDesc);

	return TRUE;
}

void fsGetPath (LPCTSTR pszFile, LPTSTR pszPath)
{
	_tcscpy (pszPath, pszFile);
	
	int len = _tcslen (pszPath) - 1;

	while (len >= 0 && pszPath [len] != _T('\\') && pszPath [len] != _T('/'))
		len--;

	pszPath [len+1] = 0;
}

void fsGetFileName (LPCTSTR pszFilePath, LPTSTR pszFileName)
{
	TCHAR szPath [MY_MAX_PATH];
	fsGetPath (pszFilePath, szPath);
	_tcscpy (pszFileName, pszFilePath + _tcslen (szPath));
}

BOOL fsBuildPathToFile (LPCTSTR pszFileName)
{
	TCHAR szPath [MY_MAX_PATH];
	
	fsGetPath (pszFileName, szPath);

	int len = _tcslen (szPath);
	int start = 0;

	
	if (szPath [0] == _T('\\') && szPath [1] == _T('\\'))
	{
		
		LPCTSTR psz = _tcschr (szPath + 2, _T('\\'));
		
		if (psz)
			psz = _tcschr (psz+1, _T('\\'));
		if (psz)
			psz++;
		if (psz == NULL)
		{
			
			SetLastError (ERROR_PATH_NOT_FOUND);
			return FALSE;
		}
		
		
		start = psz - szPath;
	}
	else
	{
		if (szPath [1] == _T(':'))
			start = 3;
	}

	for (int i = start; i < len; i++)
	{
		if (szPath [i] == _T('\\') || szPath [i] == _T('/'))
		{
			
			
			
			
			
			
	
			TCHAR szPath2 [MY_MAX_PATH];

			CopyMemory (szPath2, szPath, i * sizeof(TCHAR));
			szPath2 [i] = 0;

			if (FALSE == CreateDirectory (szPath2, NULL))	
			{
				if (GetLastError () != ERROR_ALREADY_EXISTS)
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL fsBuildPathToFileW (LPCWSTR pwszFileName)
{
	LPCWSTR pwsz = pwszFileName;

	for (;;)
	{
		pwsz = wcschr (pwsz + 1, '\\');
		if (pwsz == NULL)
			break;
		if (pwsz - pwszFileName == 2)
			continue;
		wchar_t wsz [MY_MAX_PATH];
		wcsncpy (wsz, pwszFileName, pwsz - pwszFileName);
		wsz [pwsz - pwszFileName] = 0;
		CreateDirectoryW (wsz, NULL);
	}

	return TRUE;
}

#ifndef FDM_DLDR__RAWCODEONLY
BOOL DPEntry_IsAllEqual (DLDS_LIST *pv, int offset, int size, BOOL bString)
{
	int cDlds = pv->size ();

	if (cDlds == 0)
		return FALSE;

	LPVOID dp0 = LPBYTE (pv->at (0)->pMgr->GetDownloadMgr ()->GetDP ()) + offset;
	LPVOID dpn;

	for (int i = 1; i < cDlds; i++)
	{
		dpn = LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDP ()) + offset;
		
		if (bString)
		{
			LPCTSTR psz1 = *((LPCTSTR*) dp0);
			LPCTSTR psz2 = *((LPCTSTR*) dpn);

			if (_tcscmp (psz1, psz2))
				return FALSE;
		}
		else
		{
			if (memcmp (dp0, dpn, size))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL DNPEntry_IsAllEqual (DLDS_LIST *pv, int offset, int size, BOOL bString)
{
	int cDlds = pv->size ();

	if (cDlds == 0)
		return FALSE;

	LPVOID dp0 = LPBYTE (pv->at (0)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset;
	LPVOID dpn;

	for (int i = 1; i < cDlds; i++)
	{
		dpn = LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset;
		
		if (bString)
		{
			LPCTSTR psz1 = *((LPCTSTR*) dp0);
			LPCTSTR psz2 = *((LPCTSTR*) dpn);

			if (_tcscmp (psz1, psz2))
				return FALSE;
		}
		else
		{
			if (memcmp (dp0, dpn, size))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL DPEntry_IsAllEqual_BitMask (DLDS_LIST *pv, int offset, DWORD dwBitMask)
{
	int cDlds = pv->size ();

	if (cDlds == 0)
		return FALSE;

	LPDWORD dp0 = (LPDWORD) (LPBYTE (pv->at (0)->pMgr->GetDownloadMgr ()->GetDP ()) + offset);
	LPDWORD dpn;

	for (int i = 1; i < cDlds; i++)
	{
		dpn = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDP ()) + offset);
		
		if ((*dp0 & dwBitMask) != (*dpn & dwBitMask))
			return FALSE;
	}

	return TRUE;
}

void DPEntry_SetValue (DLDS_LIST *pv, int offset, int size, BOOL bString, const void* lpNewVal)
{
	LPCTSTR pszNewVal = (LPCTSTR) lpNewVal;
	int len = 0;

	if (bString)
		len = _tcslen (pszNewVal);

	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPVOID dp = LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDP ()) + offset;
		
		if (bString)
		{
			LPTSTR *ppszVal = (LPTSTR*) dp;
			
			SAFE_DELETE_ARRAY (*ppszVal);

			fsnew (*ppszVal, TCHAR, len+1);
			_tcscpy (*ppszVal, pszNewVal);
		}
		else
		{
			memcpy (dp, lpNewVal, size);
		}
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

void DPEntry_SetValue_BitMask (DLDS_LIST *pv, int offset, DWORD dwMask)
{
	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPDWORD dp = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDP ()) + offset);
		*dp |= dwMask;
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

void DPEntry_UnsetValue_BitMask (DLDS_LIST *pv, int offset, DWORD dwMask)
{
	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPDWORD dp = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDP ()) + offset);
		*dp &= ~dwMask;
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

void DNPEntry_SetValue (DLDS_LIST *pv, int offset, int size, BOOL bString, const void* lpNewVal)
{
	LPCTSTR pszNewVal = (LPCTSTR) lpNewVal;
	int len = 0;

	if (bString)
		len = _tcslen (pszNewVal);

	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPVOID dnp = LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset;
		
		if (bString)
		{
			LPTSTR *ppszVal = (LPTSTR*) dnp;
			
			SAFE_DELETE_ARRAY (*ppszVal);

			fsnew (*ppszVal, TCHAR, len+1);
			_tcscpy (*ppszVal, pszNewVal);
		}
		else
		{
			memcpy (dnp, lpNewVal, size);
		}
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

BOOL DNPEntry_IsAllEqual_BitMask (DLDS_LIST *pv, int offset, DWORD dwBitMask)
{
	int cDlds = pv->size ();

	if (cDlds == 0)
		return FALSE;

	LPDWORD dnp0 = (LPDWORD) (LPBYTE (pv->at (0)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset);
	LPDWORD dnpn;

	for (int i = 1; i < cDlds; i++)
	{
		dnpn = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset);
		
		if ((*dnp0 & dwBitMask) != (*dnpn & dwBitMask))
			return FALSE;
	}

	return TRUE;
}

void DNPEntry_SetValue_BitMask (DLDS_LIST *pv, int offset, DWORD dwMask)
{
	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPDWORD dnp = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset);
		*dnp |= dwMask;
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

void DNPEntry_UnsetValue_BitMask (DLDS_LIST *pv, int offset, DWORD dwMask)
{
	for (int i = pv->size () - 1; i >= 0; i--)
	{
		LPDWORD dnp = (LPDWORD) (LPBYTE (pv->at (i)->pMgr->GetDownloadMgr ()->GetDNP ()) + offset);
		*dnp &= ~dwMask;
		pv->at (i)->pMgr->GetDownloadMgr ()->setDirty();
	}
}

#endif

LPCTSTR strcmp_m (LPCTSTR pszWhere, LPCTSTR pszWhat)
{
	if (*pszWhere == 0)
		return *pszWhat == 0 ? pszWhere : NULL;

	if (*pszWhat == 0)
		return NULL;

	
	if (*pszWhat == _T('*'))
	{
		
		if (pszWhat [1] == 0)
			return pszWhere;
		
		
		
		LPCTSTR psz = strcmp_m (pszWhere, pszWhat+1);
		if (psz)
			return psz;

		
		
		
		return strcmp_m (pszWhere+1, pszWhat);
	}

	

	if (*pszWhat != _T('?'))
	{
		
		if (*pszWhere != *pszWhat)
			return NULL;
	}

	return strcmp_m (pszWhere+1, pszWhat+1) ? pszWhere : NULL;
}

LPCTSTR strcmpi_m (LPCTSTR pszWhere, LPCTSTR pszWhat)
{
	TCHAR *psz1 = new TCHAR [lstrlen (pszWhere) + 1];
	TCHAR *psz2 = new TCHAR [lstrlen (pszWhat) + 1];

	lstrcpy (psz1, pszWhere);
	lstrcpy (psz2, pszWhat);

	CharLower (psz1);
	CharLower (psz2);

	LPCTSTR psz = strcmp_m (psz1, psz2);
	LPCTSTR pszRet = NULL;
	
	if (psz)
		pszRet = pszWhere + (psz - psz1);

	delete [] psz1;
	delete [] psz2;

	return pszRet;
}

BOOL IsExtStrEq (LPCTSTR pszMasked, LPCTSTR psz2)
{
	return strcmpi_m (psz2, pszMasked) != NULL;
}

BOOL IsExtInExtsStr (LPCTSTR pszExts, LPCTSTR pszExt)
{
	if (pszExt == NULL)
		return FALSE;

	int len = lstrlen (pszExts);
	int i = 0;
	TCHAR tszExt [1000]; 

	do
	{
		int j = 0;

		

		while (i < len && pszExts [i] != _T(' '))
			tszExt [j++] = pszExts [i++];

		tszExt [j] = 0;
		i++;

		if (IsExtStrEq (tszExt, pszExt))
			return TRUE;

	} while (i < len);

	return FALSE;
}

BOOL fsReadStringFromFile (HANDLE hFile, fsString &str)
{
	SAFE_DELETE_ARRAY (str.pszString);
	return fsReadStrFromFile (&str.pszString, hFile);
}

typedef ULONGLONG* LPULONGLONG;

DWORD fsGetTimeDelta (FILETIME *t1, FILETIME *t2)
{
	return DWORD ((*LPULONGLONG (t1) - *LPULONGLONG (t2)) / 10000000);
}

DWORD fsGetTimeDelta (const SYSTEMTIME *t1, const SYSTEMTIME *t2)
{
	FILETIME ft1, ft2;
	SystemTimeToFileTime (t1, &ft1);
	SystemTimeToFileTime (t2, &ft2);
	return fsGetTimeDelta (&ft1, &ft2);
}

CString fsTimeInSecondsToStr (DWORD dwAmount)
{
	CString strResult;

	strResult.Format (_T ("%02d:%02d:%02d"), 
		(int)(dwAmount / 3600),				
		(int)((dwAmount % 3600) / 60),		
		(int)(dwAmount % 60));				

	return strResult;
}

#pragma warning (disable:4706)
void fsPathToGoodPath (LPTSTR pszPath)
{
	LPTSTR pszBad = pszPath;

	while (pszBad = _tcschr (pszBad, _T('/')))  
		*pszBad = _T('\\');
}

void fsPathToGoodUrlPath (LPTSTR pszPath)
{
	LPTSTR pszBad = pszPath;

	while (pszBad = _tcschr (pszBad, _T('\\')))
		*pszBad = _T('/');
}
#pragma warning (default:4706)

#ifndef FDM_DLDR__RAWCODEONLY
void fsOpenBuyPage ()
{

#ifdef WGET_AFFILIATE_ID
	#pragma message ("================================ AFFILIATE IS ACTIVE!!! ==========================")
	CString str;
	str.Format (_T("https://www.regnow.com/softsell/nph-softsell.cgi?item=9752-1&affiliate=%d"), WGET_AFFILIATE_ID);
	fsOpenUrlInBrowser (str);
#else
	if (_tcsicmp (_LngMgr.GetLngName (_LngMgr.GetCurLng ()), _T("Russian")))
		fsOpenUrlInBrowser (_T("http://www.freedownloadmanager.org/buy.html"));
	else
		fsOpenUrlInBrowser (_T("http://www.freedownloadmanager.org/buy.html"));
#endif
}

CString fsBytesToStr (UINT64 uBytes)
{
	CString strBytes = " "; 
	strBytes += LS (L_B);
	CString str;
	UINT u;

	do
	{
		 if (uBytes > 1000)
			 u = UINT (uBytes % 1000);
		 else
			 u = 0;

		if (u)
		{
			str.Format (_T(",%d"), u);
			if (str.GetLength () == 2)
				str.Insert (1, _T("00"));
			else if (str.GetLength () == 3)
				str.Insert (1, _T("0"));
		}
		else
		{
			if (uBytes < 1000)
				str.Format (_T("%d"), (UINT)uBytes);
			else
			{
				str = _T(",000");
			}
		}

		uBytes /= 1000;

		strBytes.Insert (0, str);
	}
	while (uBytes);

	return strBytes;
}

CString fsGetGrpOTHEROutFolder ()
{
	return _DldsGrps.FindGroup (GRP_OTHER_ID)->strOutFolder;
}
#endif

void vmsUtf8ToAscii (LPSTR psz)
{
	int len = strlen (psz);
	int nRL = MultiByteToWideChar (CP_UTF8, 0, psz, len, NULL, 0);
	wchar_t *pwsz = new wchar_t [nRL+1]; *pwsz = 0;
	MultiByteToWideChar (CP_UTF8, 0, psz, -1, pwsz, nRL+1);
	WideCharToMultiByte (CP_ACP, 0, pwsz, -1, psz, len+1, NULL, NULL);
	delete [] pwsz;
}

std::wstring vmsUtf8Unicode (LPCSTR psz)
{
	wchar_t wsz [100000] = L"";
	MultiByteToWideChar (CP_UTF8, 0, psz, -1, wsz, 99999);
	return wsz;
}

bool vmsUnicodeToUtf8 (LPCTSTR tszSrc, LPSTR szTar, DWORD* pdwTarBuffLen)
{
	int nUtf8Len = WideCharToMultiByte (CP_UTF8, 0, tszSrc, -1, NULL, 0, 0, 0);
	if (*pdwTarBuffLen < nUtf8Len) {
		*pdwTarBuffLen = nUtf8Len;
		return false;
	}
	WideCharToMultiByte (CP_UTF8, 0, tszSrc, -1, szTar, nUtf8Len, 0, 0);
	return true;
}
