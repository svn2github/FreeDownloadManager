/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSWEBPAGEDOWNLOADER_H__A6C75579_FDC3_46CB_A771_85AB6875B099__INCLUDED_)
#define AFX_FSWEBPAGEDOWNLOADER_H__A6C75579_FDC3_46CB_A771_85AB6875B099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "fsScheduleMgr.h"
#include "tree.h"  

enum fsWPDExtsType
{
	WPDET_DOWNLOAD,		
	WPDET_NOTDOWNLOAD,	
	WPDET_OFF			
};    

#define WPDF_KEEPFOLDERSTRUCTURE		1

#define WPDF_DONTSTOREPAGES				2

#define WPDF_DELCOMPLETEDDLDS			4  

struct fsWPDSettings
{
	fsString strHTMLExts;	
	BOOL bNotAllPages;		
	BOOL bNotAllFiles;		
	BOOL bNotAllImages;		
	BOOL bDownloadImages;	
	BOOL bDownloadFiles;	
	BOOL bDownloadStyles;	
	int iDepth;				
	fsString strExts;		
	fsWPDExtsType enExtsType;	
	fsString strImgsExts;		
	fsWPDExtsType enImgsExtsType;	
	vmsDownloadsGroupSmartPtr pDLGroup;	
	fsString strFolderSaveTo;	
	int iReserved;			
	fsString strUserName;	
	fsString strPassword;	
	BOOL bSavePagesUnderHTM;	
	
	DWORD dwFlags;
	
	fs::list <struct fsWPDIgnoreListItem*> vIgnoreList;
};  

struct fsWPDIgnoreListItem
{
	fsString strURL;
	DWORD dwFlags;		
};    

#define WPD_ILITEM_SUBFOLDERSALSO	1  

#define WPD_ILITEM_THISPATHONLY		2  

struct fsWPDSettings_v1
{
	fsString strHTMLExts;
	BOOL bNotAllPages; 
	BOOL bNotAllFiles; 
	BOOL bNotAllImages;	
	BOOL bDownloadImages;
	BOOL bDownloadFiles;
	BOOL bDownloadStyles;
	int iDepth;			
	fsString strExts;	
	fsWPDExtsType enExtsType;
	fsString strImgsExts;	
	fsWPDExtsType enImgsExtsType;
	fsString strDLGroup; 
	fsString strFolderSaveTo;
	int iReserved;
};      

#define WPSTATE_DLDWASDELETED	1

#define WPSTATE_PAGEPROCESSED	2    

enum _WP_LinkType
{
	WPLT_A,					
	WPLT_IMG,				
	WPLT_STYLESHEET			
};

struct _WP_UnprocessedLinks
{
	UINT nWPIDWhere;		
	UINT nParserUrl;		
	_WP_LinkType lt;		
};      

struct fsDLWebPage
{
	vmsDownloadSmartPtr dld;	
	UINT uDldId;		
	fsString strURL;	
	fsString strFile;	
	BYTE bState;		
	UINT nID;			
	
	
	
	
	
	fs::list <_WP_UnprocessedLinks> *pvUnpLinks;	
	
	fs::list <fsString> *pvUrls;
};  

typedef fs::ListTree <fsDLWebPage>* t_wptree;  

typedef void (*fntWPDEvents) (class fsWebPageDownloader* dldr, enum fsWPDEvent ev, int info, LPVOID lp);  

enum fsWPDEvent
{
	WPDE_DLDEVENTRECEIVED,		
	WPDE_FILEADDED,				
	WPDE_DLDWILLBEDELETED,		
	WPDE_DLDRESTORED,			
	WPDE_WEBPAGEWILLBEDELETED,	
	WPDE_STOPPED,				
	WPDE_DONE,					
};

enum vmsWPDPageType
{
	WPDPT_PAGE,					
	WPDPT_FILE,					
	WPDPT_IMAGE,				
	WPDPT_CSS,					
};

class fsWebPageDownloader  
{
	friend class fsWebPageDownloadsMgr;
public:
	
	void GetDownloadingSiteName (LPSTR psz);
	
	
	
	
	BOOL Create (LPCSTR pszPageURL, BOOL bAutoStart = TRUE, fsSchedule *task = NULL);
	
	t_wptree GetRootPage();
	
	BOOL Load (HANDLE hFile, WORD wVer);
	
	BOOL Load_OLD (HANDLE hFile, BOOL bOldVer);
	
	BOOL Save (HANDLE hFile);
	
	void DeleteAllDownloads(BOOL bByUser);
	
	void DetachFromDownloads();
	
	void SetEventFunc (fntWPDEvents pfn, LPVOID lp);
	
	void SetAutoStartDownloading (BOOL b);
	
	void StopDownloading();
	
	void StartDownloading();
	
	vmsDownloadSmartPtr GetDownload (int iIndex);
	
	int GetDownloadCount();
	
	static void ReadDefaultWPDS (fsWPDSettings* wpds);
	
	fsWPDSettings* GetWDPS ();
	
	BOOL IsDownloading();
	
	BOOL IsOnAutoStart();
	
	BOOL IsScheduled ();
	
	BOOL IsDone();
	
	BOOL IsRunning();
	
	int GetDoneFileCount();
	
	int GetFileCount();
	
	float GetPercentDone();
	
	LPCSTR GetStartURL ();

	
	fs::ListTree <fsDLWebPage>* FindWebPageTree (vmsDownloadSmartPtr dld);

	fsWebPageDownloader();
	virtual ~fsWebPageDownloader();

protected:
	
	BOOL IsURLShouldBeIgnored (fsURL& url);
	
	int ParseHTMLFrameUrls(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL);
	BOOL m_bWasShutdownMsg; 
	
	void CorrectUnpUrls (fsDLWebPage* wpfrom, fsDLWebPage* wpto);
	
	
	DWORD OnCheckFileExtIsOK (vmsDownloadSmartPtr dld);
	
	
	
	void Load_PerformRollback();
	BOOL m_bIsDeleting;	
	BOOL m_bStopped;	
	
	
	
	
	
	void GetFileForReplace (fsDLWebPage* wpwhere, fsDLWebPage *wpwhat, LPSTR pszFile);
	
	void GetPtrToFile (LPCSTR pszToFile, LPCSTR pszFromFile, LPSTR pszPtr);
	
	void DeleteWebPage (fsDLWebPage *wp);
	
	fsDLWebPage* FindWebPage (UINT nID);
	fsDLWebPage* FindWebPage (vmsDownloadSmartPtr dld);
	fsDLWebPage* FindWebPage (LPCSTR pszFullUrl);
	
	void OnDldRedirected (vmsDownloadSmartPtr dld);
	
	BOOL IsUrlsEqual (fsURL& url1, LPCSTR pszUrl2);
	UINT m_nMaxID;		
	CRITICAL_SECTION m_cs_Done_Redir_Events;
	
	BOOL Load (HANDLE hFile, t_wptree root, WORD wVer);
	BOOL Save (HANDLE hFile, t_wptree root);

	
	void Event (fsWPDEvent ev, int info = 0);
	fntWPDEvents m_pfnEvents;
	LPVOID m_lpEventsParam;

	
	
	
	
	int ParseHTMLLinkUrls(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL);
	
	
	int ParseHTMLUrls (fsHTMLParser& parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL);
	
	
	int ParseHTMLImages(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL);
	
	
	BOOL CrackUrl (LPCSTR pszFullUrl, LPSTR* ppszUrlWA, LPSTR* ppszFA = NULL);
	
	void ParseHTMLFile (t_wptree wptree, BOOL bFixUrlsOnly);
	
	UINT ParseHTML (LPCSTR pszHTML, t_wptree wptree, BOOL bFixUrlsOnly);
	
	int FindConfIndex (vmsDownloadSmartPtr dld);
	
	void OnWPDownloadDone (vmsDownloadSmartPtr dld);
	
	
	void WebPage_FindDownload (fsDLWebPage *wp);
	
	fsDLWebPage* AddPage (fs::ListTree <fsDLWebPage> *root, LPCSTR pszPageURL, vmsWPDPageType enPageType, BOOL bSetCTReq = FALSE, BOOL bAutoStart = TRUE, fsSchedule *task = NULL);
	
	
	fsDLWebPage* AddWebPage (fsDLWebPage *wp, fs::ListTree <fsDLWebPage> *root, fsSchedule *task = NULL);

	
	static DWORD _DldEvents (fsDownload* dld, enum fsDownloadsMgrEvent ev, LPVOID lp);

	
	void ApplySettingsToDld (vmsDownloadSmartPtr dld);

	fsWPDSettings m_wpds;			
	fs::ListTree <fsDLWebPage> m_pages;	
	fsString m_strStartServer;		

	
	struct _Conformity
	{
		fsDLWebPage* wp;						
		fs::ListTree <fsDLWebPage>* wptree;		
	};

	std::vector <_Conformity> m_vConfs;		
};

#endif 
