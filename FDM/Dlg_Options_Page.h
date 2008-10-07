/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_DLG_OPTIONS_PAGE_H__B00A0DA1_8AF4_4293_B6A6_9A25A7734225__INCLUDED_)
#define AFX_DLG_OPTIONS_PAGE_H__B00A0DA1_8AF4_4293_B6A6_9A25A7734225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlg_Options_Page : public CDialog  
{
public:
	virtual CString get_PageTitle () = NULL;
	virtual CString get_PageShortTitle () = NULL;
	virtual BOOL Apply () = NULL;

	LPCSTR get_Template();

	CDlg_Options_Page(UINT u, CWnd* wnd);
	virtual ~CDlg_Options_Page();

protected:
	CWnd* get_ReflectParent();
	int m_nReflectLevel;
	virtual void OnCancel();
	virtual void OnOK ();

	DECLARE_MESSAGE_MAP();
};

#endif 
