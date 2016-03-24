/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSDLLCALLER_H__4906328A_9106_4AEC_B3A4_C37928C3303E__INCLUDED_)
#define AFX_VMSDLLCALLER_H__4906328A_9106_4AEC_B3A4_C37928C3303E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

enum vmsWhichBitmap
{
	BMP_FOR_UPLOADSMENU,
	BMP_FOR_UPLOADSMENU_D,
	BMP_FOR_TASKSLIST,
	BMP_FOR_TASKSLIST_SEL,
	BMP_FOR_LOGLIST,
};

class vmsDllCallerSettings
{
public:
	virtual int    InternetAccessType () = NULL;
	virtual DWORD  FtpFlags () = NULL;
	virtual BOOL   UseHttp11 () = NULL;
	virtual int    FtpTransferType () = NULL;
	virtual BOOL   UseCookie () = NULL;
	virtual LPCTSTR HttpAgent () = NULL;
	virtual LPCTSTR FtpAsciiExts () = NULL;

	virtual LPCTSTR FtpProxy_Name () = NULL;
	virtual LPCTSTR FtpProxy_Password () = NULL;
	virtual LPCTSTR FtpProxy_UserName () = NULL;

	virtual LPCTSTR HttpProxy_Name () = NULL;
	virtual LPCTSTR HttpProxy_Password () = NULL;
	virtual LPCTSTR HttpProxy_UserName () = NULL;

	virtual LPCTSTR HttpsProxy_Name () = NULL;
	virtual LPCTSTR HttpsProxy_Password () = NULL;
	virtual LPCTSTR HttpsProxy_UserName () = NULL;

	virtual BOOL   ShowSizesInBytes () = NULL;

	virtual int    FirefoxSettings_Proxy_Type () = NULL;
	virtual LPCTSTR FirefoxSettings_Proxy_Addr (LPCTSTR pszProtocol) = NULL;
	virtual int    FirefoxSettings_Proxy_Port (LPCTSTR pszProtocol) = NULL;

	
	virtual BOOL   GetSettingsByName (LPCTSTR pszName, LPVOID pData, DWORD dwDataSize) = NULL;
};

class vmsDllPersist
{
public:
	virtual BOOL GetProfileString (LPCTSTR pszSecion, LPCTSTR pszName, LPCTSTR pszDefValue, LPTSTR pszValue, DWORD *pdwValueSize) = NULL;
	virtual void WriteProfileString (LPCTSTR pszSecion, LPCTSTR pszName, LPCTSTR pszValue) = NULL;
	virtual UINT GetProfileInt (LPCTSTR pszSecion, LPCTSTR pszName, UINT nDefValue) = NULL;
	virtual void WriteProfileInt (LPCTSTR pszSecion, LPCTSTR pszName, UINT nValue) = NULL;
	virtual BOOL GetProfileBinary (LPCTSTR pszSecion, LPCTSTR pszName, LPBYTE *ppData, UINT *pnDataSize) = NULL;
	virtual void WriteProfileBinary (LPCTSTR pszSection, LPCTSTR pszName, LPVOID pvData, UINT nDataSize) = NULL;

	virtual void FreeBuffer (LPBYTE pb) = NULL;
};

class vmsDllCallerEx
{
};

class vmsDllCaller  
{
public:
	virtual LPCTSTR GetTranslatedString (int nId) = NULL;
	
	virtual vmsDllCallerSettings* GetSettings () = NULL;
	virtual vmsDllPersist* GetPersist () = NULL;
	
	
	virtual void IRToStr (int ir, TCHAR *sz, UINT cch) = NULL;
	
	virtual HBITMAP GetBitmap (vmsWhichBitmap) = NULL;

	
	
	
	
	
	
	
	
	virtual BOOL OnBeforeUpload (LPSTR pszStopReason, bool* pbNeedStop) = NULL;

	
	
	virtual void GetDataFilePath (LPCTSTR pszName, LPTSTR pszResult) = NULL;

	
	virtual void GetCommonFilesFolder (LPTSTR pszResult) = NULL;

	virtual vmsDllCallerEx* GetExtendedFeatures () = NULL;
};

#endif 
