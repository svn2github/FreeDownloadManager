/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSUPLOADSDLLCALLERSETTINGS_H__45BF1D8A_6AF4_4958_80D8_EAEA14634588__INCLUDED_)
#define AFX_VMSUPLOADSDLLCALLERSETTINGS_H__45BF1D8A_6AF4_4958_80D8_EAEA14634588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "Uploader\CoreDll\dll.h"

class vmsUploadsDllCallerSettings : public vmsUploadsDll::vmsDllCallerSettings
{
public:
	BOOL GetSettingsByName (LPCTSTR pszName, LPVOID pData, DWORD dwDataSize);
	int FirefoxSettings_Proxy_Port (LPCTSTR pszProtocol);
	LPCTSTR FirefoxSettings_Proxy_Addr (LPCTSTR pszProtocol);
	int FirefoxSettings_Proxy_Type();
	BOOL ShowSizesInBytes();
	LPCTSTR HttpProxy_UserName();
	LPCTSTR HttpProxy_Password();
	LPCTSTR HttpProxy_Name();
	LPCTSTR HttpsProxy_UserName();
	LPCTSTR HttpsProxy_Password();
	LPCTSTR HttpsProxy_Name();
	LPCTSTR FtpProxy_UserName();
	LPCTSTR FtpProxy_Password();
	LPCTSTR FtpProxy_Name();
	LPCTSTR FtpAsciiExts();
	LPCTSTR HttpAgent();
	BOOL UseCookie();
	int FtpTransferType();
	BOOL UseHttp11();
	DWORD FtpFlags();
	virtual int InternetAccessType();
	vmsUploadsDllCallerSettings();
	virtual ~vmsUploadsDllCallerSettings();

};

#endif 
