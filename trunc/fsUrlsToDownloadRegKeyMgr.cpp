/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsUrlsToDownloadRegKeyMgr.h"
#include "DownloadsWnd.h"

extern CDownloadsWnd* _pwndDownloads;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsUrlsToDownloadRegKeyMgr::fsUrlsToDownloadRegKeyMgr()
{

}

fsUrlsToDownloadRegKeyMgr::~fsUrlsToDownloadRegKeyMgr()
{

}

void fsUrlsToDownloadRegKeyMgr::CheckKey()
{
	CRegKey key;

	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
				_T("Software\\FreeDownloadManager.ORG\\Free Download Manager\\URLsToDownload")))
		return;

	fs::list <fsString> vKeys;
	BOOL bZL = _DldsMgr.GetCount () == 0;

	int i = 0;
	for (i = 0; TRUE; i++)
	{
		TCHAR szKeyName [10000];
		DWORD dwLen = sizeof (szKeyName);

		if (ERROR_SUCCESS != RegEnumKey (key, i, szKeyName, dwLen))
			break;

		CRegKey key2;
		if (ERROR_SUCCESS == key2.Open (key, szKeyName))
		{
			TCHAR szUrl [10000];
			DWORD dw = _countof (szUrl);
			DWORD bSilent = FALSE;
			TCHAR szComment [10000] = _T("");
			DWORD dwCS = sizeof (szComment);
			DWORD bAutoStart = TRUE;
			DWORD bZLR = FALSE;
			DWORD dwForceAutoLaunch = DWCD_NOFORCEAUTOLAUNCH;
			DWORD bSaveToDesktop = FALSE;

			key2.QueryValue (bSilent, _T("Silent"));
			key2.QueryValue (szComment, _T("Comment"), &dwCS);
			key2.QueryValue (bAutoStart, _T("AutoStart"));
			key2.QueryValue (bZLR, _T("zlr"));
			key2.QueryValue (dwForceAutoLaunch, _T("ForceAutoLaunch"));
			key2.QueryValue (bSaveToDesktop, _T("SaveToDesktop"));
			
			if (bZLR == FALSE || bZL)
			if (ERROR_SUCCESS == key2.QueryValue (szUrl, _T("URL"), &dw))
			{
				TCHAR szReferer [10000];
				dw = sizeof (szReferer);
				*szReferer = 0;

				key2.QueryValue (szReferer, _T("Referer"), &dw);
				
				fsURL url;
				if (url.Crack (szUrl) == IR_SUCCESS)
				{
					vmsDWCD_AdditionalParameters params;
					params.dwMask = DWCDAP_FLAGS;
					params.dwFlags = bSaveToDesktop ? DWDCDAP_F_SAVETODESKTOP : 0;
					_pwndDownloads->CreateDownload (szUrl, FALSE, szComment, *szReferer ? szReferer : NULL, 
						bSilent, dwForceAutoLaunch, (LPBOOL)&bAutoStart, &params);
				}
			}
		}

		vKeys.add (szKeyName);
	}

	for (i = 0; i < vKeys.size (); i++)
		key.RecurseDeleteKey (vKeys [i]);
}
