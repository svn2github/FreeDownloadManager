/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
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

	LOG ("cheking URLsToDownload reg key...");

	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
				"Software\\FreeDownloadManager.ORG\\Free Download Manager\\URLsToDownload"))
	{
		LOG ("failed to open key" << nl);
		return;
	}

	LOG (nl);

	fs::list <fsString> vKeys;
	BOOL bZL = _DldsMgr.GetCount () == 0;

	for (int i = 0; TRUE; i++)
	{
		char szKeyName [10000];
		DWORD dwLen = sizeof (szKeyName);

		if (ERROR_SUCCESS != RegEnumKey (key, i, szKeyName, dwLen))
			break;

		CRegKey key2;
		if (ERROR_SUCCESS == key2.Open (key, szKeyName))
		{
			char szUrl [10000];
			DWORD dw = sizeof (szUrl);
			DWORD bSilent = FALSE;
			char szComment [10000] = "";
			DWORD dwCS = sizeof (szComment);
			DWORD bAutoStart = TRUE;
			DWORD bZLR = FALSE;
			DWORD dwForceAutoLaunch = DWCD_NOFORCEAUTOLAUNCH;
			DWORD bSaveToDesktop = FALSE;

			key2.QueryValue (bSilent, "Silent");
			key2.QueryValue (szComment, "Comment", &dwCS);
			key2.QueryValue (bAutoStart, "AutoStart");
			key2.QueryValue (bZLR, "zlr");
			key2.QueryValue (dwForceAutoLaunch, "ForceAutoLaunch");
			key2.QueryValue (bSaveToDesktop, "SaveToDesktop");
			
			if (bZLR == FALSE || bZL)
			if (ERROR_SUCCESS == key2.QueryValue (szUrl, "URL", &dw))
			{
				char szReferer [10000];
				dw = sizeof (szReferer);
				*szReferer = 0;

				key2.QueryValue (szReferer, "Referer", &dw);

				LOG ("another url found" << nl);
				
				fsURL url;
				if (url.Crack (szUrl) == IR_SUCCESS)
				{
					vmsDWCD_AdditionalParameters params;
					params.dwMask = DWCDAP_FLAGS;
					params.dwFlags = bSaveToDesktop ? DWDCDAP_F_SAVETODESKTOP : 0;
					_pwndDownloads->CreateDownload (szUrl, FALSE, szComment, *szReferer ? szReferer : NULL, 
						bSilent, dwForceAutoLaunch, (LPBOOL)&bAutoStart, &params);
				}
				else
					LOG ("bad url" << nl);
	
				LOG ("url processed" << nl);
			}
		}

		vKeys.add (szKeyName);
	}

	LOG ("deleting all URLsToDownload reg key entries...");

	for (i = 0; i < vKeys.size (); i++)
		key.RecurseDeleteKey (vKeys [i]);

	LOG ("done." << nl);
}
