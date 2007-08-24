/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_SPIDERWND_H__CB81C87C_5466_4ACB_A194_E982B9B99534__INCLUDED_)
#define AFX_SPIDERWND_H__CB81C87C_5466_4ACB_A194_E982B9B99534__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif   

#include "plugins.h"
#include "fsodmenu.h"
#include "SpiderTasks.h"	
#include "SpiderTaskTree.h"	
#include "WndSplitter.h"	
#include "fsWebPageDownloadsMgr.h"	    

class CSpiderWnd : public CWnd
{
	friend class CSpiderTasks;

public:
	CSpiderWnd();  

public:  

public:  

	
	//{{AFX_VIRTUAL(CSpiderWnd)
	//}}AFX_VIRTUAL  

public:
	static HMENU Plugin_GetViewMenu();
	static HMENU Plugin_GetMainMenu();
	
	static void Plugin_SetLanguage (wgLanguage, HMENU hMenuMain, HMENU);
	static void Plugin_GetPluginNames(LPCSTR *ppszLong, LPCSTR *ppszShort);
	static void Plugin_GetMenuViewItems(wgMenuViewItem **ppItems, int * cItems);
	static void Plugin_GetMenuImages(fsSetImage **ppImages, int *pcImages);
	static void Plugin_GetToolBarInfo(wgTButtonInfo **ppButtons, int *pcButtons);
	static HWND Plugin_CreateMainWindow(HWND hParent);
	

	
	void UpdateTasksIcons();
	
	afx_msg void OnSpiderDl();
	
	void SetupDefSettings();
	
	void SaveAll (BOOL bMsgIfErr = FALSE);
	
	void SavePages(BOOL bMsgIfErr = TRUE);
	fsWebPageDownloadsMgr* GetMgr();
	fsWebPageDownloadsMgr m_mgr;	

	
	
	
	fsWebPageDownloader* AddWebPage (LPCSTR pszStartUrl = NULL, BOOL bReqTopMost = FALSE);
	
	BOOL Create (CWnd *pParent);
	virtual ~CSpiderWnd();

protected:
	
	afx_msg LRESULT OnInsert(WPARAM, LPARAM);
	afx_msg void OnPagesListShowCol (UINT uCmd);
	void UpdatePagesListColMenu(CMenu *menu);
	afx_msg LRESULT OnUpdateMenuView (WPARAM, LPARAM lp);
	void ApplyLanguageToMenuView(CMenu *menu);
	void ApplyLanguage();
	afx_msg LRESULT OnUpdateToolBar (WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateMenu (WPARAM, LPARAM);
	afx_msg LRESULT OnAppQueryExit (WPARAM, LPARAM);
	afx_msg LRESULT OnAppExit (WPARAM, LPARAM);
	

	
	void LoadPages();

	
	static void _DownloaderEvents(fsWebPageDownloader* dldr, fsWPDEvent ev, int info, LPVOID lp);

	
	afx_msg void OnShowTree();
	void ShowTree (BOOL bShow);
	BOOL m_bShowTree;

	CWndSplitter m_wndSplitter;
	CSpiderTaskTree m_wndTaskTree;
	CSpiderTasks m_wndTasks;
	//{{AFX_MSG(CSpiderWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSpiderDelete();
	afx_msg void OnSpiderOpenfolder();
	afx_msg void OnSpiderOpenpage();
	afx_msg void OnSpiderSchedule();
	afx_msg void OnSpiderSettings();
	afx_msg void OnSpiderStartautomatically();
	afx_msg void OnSpiderStartdl();
	afx_msg void OnSpiderStopdl();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};    

//{{AFX_INSERT_LOCATION}//}}  

#endif 
