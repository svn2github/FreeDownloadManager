/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSTRAYICONMGR_H__0CFF722C_1D89_4E91_9892_4EEAE26390A1__INCLUDED_)
#define AFX_FSTRAYICONMGR_H__0CFF722C_1D89_4E91_9892_4EEAE26390A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class fsTrayIconMgr
{
public:
	LPCTSTR getTip () const;
	void setTip (LPCTSTR pszTip);
	
	
	
	
	
	
	BOOL Create (HWND hWnd, UINT *pIcons, UINT cIcons, LPCTSTR pszTooltip, UINT uCallbackMsg);
	
	void TestIcon();
	
	BOOL ShowIcon (int iIndex);
	
	void Remove();
	
	
	
	
	BOOL ShowBalloon (LPCTSTR pszInfo, LPCTSTR pszInfoTitle, DWORD dwNiifIcon = NIIF_INFO);

	fsTrayIconMgr();
	virtual ~fsTrayIconMgr();

protected:
	int m_iCurIconIndex;	
	fsString m_strTip;		
	UINT m_uCallbackMsg;
	UINT m_nID;
	
	void LoadIcons (UINT* pIcons, UINT cIcons);
	
	BOOL InitializeTrayIcon ();

	UINT m_cIcons;
	HICON *m_pIcons;
	HWND m_hWnd;
};

#endif 
