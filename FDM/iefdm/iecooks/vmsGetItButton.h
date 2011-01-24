/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSGETITBUTTON_H__A26B4169_6D5A_421F_AD64_34AA3AD12F8D__INCLUDED_)
#define AFX_VMSGETITBUTTON_H__A26B4169_6D5A_421F_AD64_34AA3AD12F8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <commctrl.h>
#include <string>

class vmsGetItButton  
{
public:
	enum ButtonResult
	{
		NONE,
		SUCCEEDED,
		E_SNIFF_MODULE_NOT_FOUND,
		E_FAILED,
		E_NO_FLV_FOUND,
		E_URL_NOT_FOUND,
	};
public:
	HWND getHWND () const;
	HRESULT getHResult () const;
	ButtonResult getResult () const;
	std::string m_strHtmlPageUrl;
	void Create (HWND hwndParent);
	vmsGetItButton();
	virtual ~vmsGetItButton();

protected:
	HRESULT m_hrLast;
	ButtonResult m_enBR;
	void onClick ();
	static HIMAGELIST m_hImgs;
	HWND m_hWnd;
	static LRESULT CALLBACK _TBParentWndProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static void Initialize ();
};

#endif 
