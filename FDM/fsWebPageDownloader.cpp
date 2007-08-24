/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "fsWebPageDownloader.h"
#include "DownloadsWnd.h"
#include "inetutil.h"
#include "ShedulerWnd.h"

extern CShedulerWnd *_pwndScheduler;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDownloadsWnd *_pwndDownloads;        

fsWebPageDownloader::fsWebPageDownloader()
{
	m_pfnEvents = NULL;
	ReadDefaultWPDS (&m_wpds);
	InitializeCriticalSection (&m_cs_Done_Redir_Events);
	m_nMaxID = 0;
	m_bStopped = FALSE;
	m_bIsDeleting = FALSE;
	m_bWasShutdownMsg = FALSE;
}

fsWebPageDownloader::~fsWebPageDownloader()
{
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		SAFE_DELETE (m_vConfs [i].wp->pvUnpLinks);
		SAFE_DELETE (m_vConfs [i].wp->pvUrls);
	}

	for (i = 0; i < (size_t)m_wpds.vIgnoreList.size (); i++)
		delete m_wpds.vIgnoreList [i];

	DeleteCriticalSection (&m_cs_Done_Redir_Events);
}

BOOL fsWebPageDownloader::Create(LPCSTR pszPageURL, BOOL bAutoStart, fsSchedule *task)
{
	if (AddPage (NULL, pszPageURL, WPDPT_PAGE, TRUE, bAutoStart, task))
	{
		_Snds.Event (SME_DOWNLOADADDED);
		return TRUE;
	}

	return FALSE;
}

fsDLWebPage* fsWebPageDownloader::AddPage(fs::ListTree <fsDLWebPage> *root, LPCSTR pszPageURL, vmsWPDPageType enPageType, BOOL bSetCTReq, BOOL bAutoStart, fsSchedule *task)
{
	fsDLWebPage wp;
	vmsDownloadSmartPtr dld;
	Download_CreateInstance (dld);

	if (m_bIsDeleting)
		return NULL;

	if (enPageType != WPDPT_PAGE)
		bSetCTReq = FALSE;

	LPSTR pszWA; 
	
	if (fsIsAnchorInUrl (pszPageURL, &pszWA))
	{
		
		wp.strURL = pszWA;
		delete [] pszWA;
	}
	else
		wp.strURL = pszPageURL;

	if (bSetCTReq)
		dld->dwFlags |= DLD_CTREQ_HTML;
	else if (enPageType == WPDPT_FILE)
		dld->dwFlags |= DLD_NOTIFYBEFOREDOWNLOAD;

	
	if (IR_SUCCESS != dld->pMgr->GetDownloadMgr ()->CreateByUrl (wp.strURL, TRUE))
		return NULL;

	
	fsDownload_NetworkProperties *dnp = dld->pMgr->GetDownloadMgr ()->GetDNP ();
	if (*dnp->pszUserName == 0 && m_wpds.strUserName.Length ())
	{
		SAFE_DELETE_ARRAY (dnp->pszUserName);
		SAFE_DELETE_ARRAY (dnp->pszPassword);

		fsnew (dnp->pszUserName, char, m_wpds.strUserName.Length () + 1);
		fsnew (dnp->pszPassword, char, m_wpds.strPassword.Length () + 1);
		strcpy (dnp->pszUserName, m_wpds.strUserName);
		strcpy (dnp->pszPassword, m_wpds.strPassword);
	}

	if (root == NULL)	
		m_strStartServer = dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszServerName;	

	
	char szAddPath [10000];
	*szAddPath = 0;	

	
	if ((m_wpds.dwFlags & WPDF_KEEPFOLDERSTRUCTURE) && *dnp->pszPathName)
	{
		

		char szFilePath [10000];
		char szPath [10000];

		
		fsFilePathFromUrlPath (dnp->pszPathName+1, dnp->enProtocol == NP_FTP,
			TRUE, szFilePath, sizeof (szFilePath));

		
		fsGetPath (szFilePath, szPath);
		fsPathToGoodPath (szPath);

		
		if (strcmp (szPath, "\\") == 0)
			*szPath = 0;	

		
		
		
		lstrcpy (szAddPath, m_strStartServer);
		lstrcat (szAddPath, "\\");
		lstrcat (szAddPath, szPath);
	}

	
	fsnew (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName, char, m_wpds.strFolderSaveTo.Length () + strlen (szAddPath) + 1);
	LPSTR pszFile = dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName;
	lstrcpy (pszFile, m_wpds.strFolderSaveTo);	
	lstrcat (pszFile, szAddPath);		

	ApplySettingsToDld (dld);
	dld->bAutoStart = m_bStopped ? FALSE : bAutoStart;
	if (root != NULL)
		dld->dwFlags |= DLD_DONTPUTTOHISTORY;
	dld->dwFlags |= DLD_NOTIFICATIONS_LL;

	if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
	{
		
		if (FALSE == dld->pMgr->GetDownloadMgr ()->InitFile (TRUE, enPageType == WPDPT_PAGE && m_wpds.bSavePagesUnderHTM ? "htm" : NULL))
			return NULL;
	}

	wp.bState = 0;
	wp.nID = ++m_nMaxID;
	fsnew1 (wp.pvUnpLinks, fs::list <_WP_UnprocessedLinks>);
	fsnew1 (wp.pvUrls, fs::list <fsString>);

	wp.dld = dld;

	
	return AddWebPage (&wp, root, task);
}

void fsWebPageDownloader::ApplySettingsToDld(vmsDownloadSmartPtr dld)
{
	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);

	dld->pGroup = m_wpds.pDLGroup;
	dld->bAutoStart = m_bStopped == FALSE;
	
	*dld->pMgr->GetDownloadMgr ()->GetDP ()->pszAdditionalExt = 0;
	
	dld->pMgr->GetDownloadMgr ()->GetDP ()->enAER = AER_RENAME;
	if (m_wpds.dwFlags & WPDF_DELCOMPLETEDDLDS)
		dld->dwFlags |= DLD_DELETEWHENDONE;
	dld->dwFlags |= DLD_NOAUTOLAUNCH | DLD_DONTSHOWDIALOG;
	dld->pMgr->GetDownloadMgr ()->GetDP ()->dwFlags &= ~DPF_GENERATEDESCFILE;
	dld->dwFlags |= DLD_USEDBYHTMLSPIDER;
	dld->pMgr->GetDownloadMgr ()->GetDP ()->bCheckIntegrityWhenDone = FALSE;
}

DWORD fsWebPageDownloader::_DldEvents(fsDownload* dld, enum fsDownloadsMgrEvent ev, LPVOID lp)
{
	fsWebPageDownloader* pThis = (fsWebPageDownloader*) lp;

	try {

	switch (ev)
	{
		case DME_DOWNLOADSTOPPEDORDONE:
			if (pThis->m_bWasShutdownMsg == FALSE)
			{
				if (pThis->IsDone ())
				{
					pThis->m_bWasShutdownMsg = TRUE;
					pThis->Event (WPDE_DONE);
				}
				else if (pThis->IsRunning () == FALSE)
				{
					pThis->m_bWasShutdownMsg = TRUE;
					pThis->Event (WPDE_STOPPED);
				}
			}
			break;
	
		case DME_DOWNLOADEREVENTRECEIVED:
			if (dld->pMgr->IsRunning () == FALSE && dld->pMgr->IsDone ())
			{
				
				EnterCriticalSection (&pThis->m_cs_Done_Redir_Events);
				
				try { 
					pThis->OnWPDownloadDone (dld);
				}
				catch (...) {}
				LeaveCriticalSection (&pThis->m_cs_Done_Redir_Events);
			}
			break;

		case DME_DOWNLOADWILLBEDELETED:
		{
			fsDLWebPage* wp = pThis->FindWebPage (dld);
	
			try {
			if (wp)
			{
				wp->strFile = wp->dld->pMgr->get_OutputFilePathName (); 
				if (dld->pMgr->IsDone () == FALSE)
					wp->bState |= WPSTATE_DLDWASDELETED;	
				
				pThis->Event (WPDE_DLDWILLBEDELETED, (int)(fsDownload*) dld);
				wp->dld = NULL;	
			}
			} catch (...) {}
			dld = NULL;
		}
		break;

		case DME_DLDRESTORED:
		{
			fsDLWebPage* wp = pThis->FindWebPage (dld);
			if (wp == NULL)
				return FALSE;

			pThis->WebPage_FindDownload (wp);
			wp->bState &= ~WPSTATE_DLDWASDELETED;

			wp->dld->pfnDownloadEventsFunc = _DldEvents;
			wp->dld->lpEventsParam = pThis;

			pThis->Event (WPDE_DLDRESTORED, (int) dld);

			return TRUE; 
		}

		case DME_DLDWILLBEFULLYDELETED:
		{
			fsDLWebPage* wp = pThis->FindWebPage (dld);
	
			try {
			if (wp)
			{
				try {
					if (dld->pMgr->IsDone () == FALSE)
						pThis->CorrectUnpUrls (wp, NULL);
				}catch (...) {}

				wp->strFile = wp->dld->pMgr->get_OutputFilePathName (); 
				wp->uDldId = UINT (-1);	
				wp->dld = NULL;	
				
				SAFE_DELETE (wp->pvUnpLinks);
			}
			} catch (...) {}
			dld = NULL;
		}
		break;

		case DME_DOWNLOADWASDELETEDFROMLIST:
			dld = NULL;
			break;

		case DME_REDIRECTED:
			
			EnterCriticalSection (&pThis->m_cs_Done_Redir_Events);
			try {
				pThis->OnDldRedirected (dld);
			}catch (...) {}
			LeaveCriticalSection (&pThis->m_cs_Done_Redir_Events);
			dld = NULL;
			break;

		case DME_BEFOREDOWNLOADING:
			return pThis->OnCheckFileExtIsOK (dld);
	}

	}
	catch (...) {}

	if (dld)
		pThis->Event (WPDE_DLDEVENTRECEIVED, (int) dld);

	return TRUE;
}

fsDLWebPage* fsWebPageDownloader::AddWebPage(fsDLWebPage *wp, fs::ListTree <fsDLWebPage> *root, fsSchedule *task)
{
	if (m_bIsDeleting)
		return NULL;

	
	fs::ListTree <fsDLWebPage>* wptree = &m_pages;
	
	fsDLWebPage *wpadded = NULL;

	wp->dld->pfnDownloadEventsFunc = _DldEvents;
	wp->dld->lpEventsParam = this;

	
	_pwndDownloads->CreateDownload (wp->dld, task, TRUE);

	
	wp->uDldId = wp->dld->nID;

	
	
	WebPage_FindDownload (wp);

	if (root)
	{
		if (m_bIsDeleting == FALSE)
		{
			wptree = root->AddLeaf (*wp);
			wpadded = &root->GetLeaf (root->GetLeafCount () - 1)->GetData ();
		}
	}
	else
	{
		m_pages.SetData (*wp);
		wpadded = &m_pages.GetData ();
	}

	if (wpadded)
	{
		_Conformity conf;	
		conf.wptree = wptree;
		conf.wp = wpadded;
		m_vConfs.push_back (conf);
	}

	Event (WPDE_FILEADDED, (int) root);

	return wpadded;
}

void fsWebPageDownloader::WebPage_FindDownload(fsDLWebPage *wp)
{
	wp->dld = _DldsMgr.GetDownloadByID (wp->uDldId);
}

fsDLWebPage* fsWebPageDownloader::FindWebPage(vmsDownloadSmartPtr dld)
{
	t_wptree wptree = FindWebPageTree (dld);
	return wptree ? &wptree->GetData () : NULL;
}

fs::ListTree <fsDLWebPage>* fsWebPageDownloader::FindWebPageTree(vmsDownloadSmartPtr dld)
{
	
	

	for (size_t  i = 0; i < m_vConfs.size (); i++)
	{
		if (m_vConfs [i].wp->uDldId == dld->nID)
			return m_vConfs [i].wptree;
	}

	return NULL;
}

void fsWebPageDownloader::OnWPDownloadDone(vmsDownloadSmartPtr dld)
{
	if (dld == NULL)
		return;

	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);
	if (dld->pMgr->GetDownloadMgr () == NULL)
		return;

	t_wptree wptree = FindWebPageTree (dld);

	if (wptree == NULL)	
		return;

	SAFE_DELETE (wptree->GetData ().pvUnpLinks);

	char szFile [MY_MAX_PATH];
	if (fsFileNameFromUrlPath (dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszPathName, 
			dld->pMgr->GetDownloadMgr ()->GetDNP ()->enProtocol == NP_FTP,
			TRUE, szFile, sizeof (szFile)))
	{
		LPCSTR pszExt = strrchr (szFile, '.');
		if (pszExt == NULL || IsExtInExtsStr (m_wpds.strHTMLExts, pszExt+1) || *szFile == 0)
		{
			

			
			
			dld->pMgr->GetDownloadMgr ()->CloseFile ();

			if (m_wpds.dwFlags & WPDF_DONTSTOREPAGES)
				dld->dwFlags |= DLD_DELETEWHENDONE | DLD_DELETEFILEALWAYS | 
						DLD_DONTPUTTOHISTORY | DLD_DONTPUTTORECYCLE;

			
			
			
			ParseHTMLFile (wptree, wptree->GetDepth () > m_wpds.iDepth);
		}
	}
}

int fsWebPageDownloader::FindConfIndex(vmsDownloadSmartPtr dld)
{
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		if (m_vConfs [i].wp->dld == dld)
			return i;
	}

	return -1;
}

void fsWebPageDownloader::ParseHTMLFile(t_wptree wptree, BOOL bFixUrlsOnly)
{
	fsDLWebPage *wp = &wptree->GetData ();

	
	if (wp->bState & WPSTATE_PAGEPROCESSED)
		return;

	HANDLE hFile;

	ASSERT (wp->dld == NULL || wp->dld->pMgr->GetDownloadMgr () != NULL);
	if (wp->dld != NULL && wp->dld->pMgr->GetDownloadMgr () == NULL)
		return;

	
	do
	{
		hFile = CreateFile (wp->dld ? wp->dld->pMgr->get_OutputFilePathName () : wp->strFile, 
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			
				return;
		}
	}
	while (hFile == INVALID_HANDLE_VALUE);

	

	DWORD dwSize = GetFileSize (hFile, NULL);
	LPSTR pszHTML;
	fsnew (pszHTML, char, int (dwSize*1.5) + 50000 + 1); 

	UINT newlen = 0;
	if (ReadFile (hFile, pszHTML, dwSize, &dwSize, NULL))
	{
		pszHTML [dwSize] = 0;
		
		newlen = ParseHTML (pszHTML, wptree, bFixUrlsOnly);

		if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
		{
			
			SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
			SetEndOfFile (hFile);
			DWORD dw;
			WriteFile (hFile, pszHTML, newlen, &dw, NULL);
		}
	}

	delete [] pszHTML;
	CloseHandle (hFile);

	wp->bState |= WPSTATE_PAGEPROCESSED;
}

UINT fsWebPageDownloader::ParseHTML(LPCSTR pszHTML, t_wptree wptree, BOOL bFixUrlsOnly)
{
	if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) && bFixUrlsOnly)
		return 0;

	fsHTMLParser html;

	html.SetKillDupes (FALSE);

	
	html.ParseHTML ((LPSTR)pszHTML);

	

	
	char szBaseURL [10000];
	LPCSTR pszBaseURL = html.Get_BaseURL ();
	if (*pszBaseURL == 0)
		pszBaseURL = wptree->GetData ().strURL;
	else
	{
		fsURL url;
		if (url.Crack (pszBaseURL) != IR_SUCCESS)
		{
			lstrcpy (szBaseURL, "http://");	
			lstrcat (szBaseURL, pszBaseURL);
			if (url.Crack (szBaseURL) == IR_SUCCESS)
			{
				if (szBaseURL [strlen (szBaseURL)-1] != '/' && szBaseURL [strlen (szBaseURL)-1] != '\\')
					lstrcat (szBaseURL, "/");

				pszBaseURL = szBaseURL;
			}
			else
				pszBaseURL = wptree->GetData ().strURL;
		}
		else
		{
			if (pszBaseURL [strlen (pszBaseURL)-1] != '/' && pszBaseURL [strlen (pszBaseURL)-1] != '\\')
			{
				lstrcpy (szBaseURL, pszBaseURL);
				lstrcat (szBaseURL, "/");
				pszBaseURL = szBaseURL;
			}
		}
	}

	BOOL bAdded = FALSE;
	int cAdded;

	
	cAdded = ParseHTMLFrameUrls (html, wptree, bFixUrlsOnly, pszBaseURL);
	bAdded = bAdded || cAdded != 0;

	
	cAdded = ParseHTMLUrls (html, wptree, bFixUrlsOnly, pszBaseURL);
	bAdded = bAdded || cAdded != 0;

	
	if (m_wpds.bDownloadStyles)
	{
		cAdded = ParseHTMLLinkUrls (html, wptree, bFixUrlsOnly, pszBaseURL);
		bAdded = bAdded || cAdded != 0;
	}

	
	if (m_wpds.bDownloadImages)
	{
		cAdded = ParseHTMLImages (html, wptree, bFixUrlsOnly, pszBaseURL);
		bAdded = bAdded || cAdded != 0;
	}

	if (*html.Get_BaseURL ())
		html.RemoveBaseTag ();

	if (bAdded)
		_Snds.Event (SME_DOWNLOADADDED);

	return html.GetHTMLLength ();
}

int fsWebPageDownloader::ParseHTMLUrls(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL)
{
	int cAdded = 0;

	
	for (int i = 0; i < parser.GetUrlCount (); i ++)
	{
		LPCSTR pszUrl = parser.GetUrl (i);

		
		if (*pszUrl == '#')
			continue;	

		LPSTR pszFullUrl;
		
		fsUrlToFullUrl (pszBaseURL, pszUrl, &pszFullUrl);
		if (pszFullUrl == NULL)
			continue;

		fsURL url;
		if (IR_SUCCESS != url.Crack (pszFullUrl, TRUE))
		{
			delete [] pszFullUrl;
			continue;
		}

		if (*url.GetHostName () == 0)
		{
			delete [] pszFullUrl;
			continue;
		}

		LPCSTR pszPath = url.GetPath ();

		
		char szFile [MY_MAX_PATH];
		fsFileNameFromUrlPath (pszPath, url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFile, sizeof (szFile));

		
		LPCSTR pszExt = strrchr (szFile, '.');

		BOOL bExt = FALSE;		
		BOOL bWebPage = FALSE;	

		if (pszExt++)
		{
			bExt = IsExtInExtsStr (m_wpds.strExts, pszExt);
			bWebPage = IsExtInExtsStr (m_wpds.strHTMLExts, pszExt);
		}
		else
		{
			bWebPage = TRUE; 
		}

		if (*pszPath == 0 || pszPath [strlen (pszPath)-1] == '/' || pszPath [strlen (pszPath)-1] == '\\')
			bWebPage = TRUE;

		if (url.GetInternetScheme () != INTERNET_SCHEME_HTTP && url.GetInternetScheme () != INTERNET_SCHEME_HTTPS)
			bWebPage = FALSE;

		
		BOOL bSetCTReq = bWebPage ? TRUE : FALSE;

		if (m_wpds.bDownloadFiles == FALSE)
		{
			if (bWebPage == FALSE)
			{
				
				parser.ReplaceUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
			cAdded++;
		}
		else
			bSetCTReq = FALSE;
		
		if (m_wpds.enExtsType != WPDET_OFF)
		{
			if ( (bExt && m_wpds.enExtsType == WPDET_NOTDOWNLOAD) ||
				  (bExt == FALSE && m_wpds.enExtsType == WPDET_DOWNLOAD) )
			{
				if (bWebPage == FALSE)
				{
					
					parser.ReplaceUrl (i, pszFullUrl);
					delete [] pszFullUrl;
					continue;
				}
			}
		}

		
		if ( (m_wpds.bNotAllFiles && bWebPage == FALSE) || 
			  (m_wpds.bNotAllPages && bWebPage)  )
		{
			
			if (fsIsServersEqual (url.GetHostName (), m_strStartServer) == FALSE)
			{
				
				parser.ReplaceUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
		}

		if (IsURLShouldBeIgnored (url))
		{
			parser.ReplaceUrl (i, pszFullUrl);
			SAFE_DELETE_ARRAY (pszFullUrl);
			continue;
		}
		
		LPSTR pszWA = NULL, pszFA = NULL; 

		if (bWebPage && bFixUrlsOnly == FALSE)
		{
			if (CrackUrl (pszFullUrl, &pszWA, &pszFA))
			{
				delete [] pszFullUrl;
				pszFullUrl = pszWA;
			}
		}

		fsDLWebPage *wp = FindWebPage (pszFullUrl);

		if (wp == NULL && bFixUrlsOnly == FALSE)
		{
			
			wp = AddPage (wptree, pszFullUrl, bWebPage ? WPDPT_PAGE : WPDPT_FILE, bSetCTReq);

			if (wp == NULL)
			{
				SAFE_DELETE_ARRAY (pszFA);
				parser.ReplaceUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
		}

		if (wp && wp->pvUnpLinks)
		{
			
			
			

			_WP_UnprocessedLinks unplink;
			unplink.nWPIDWhere = wptree->GetData ().nID;	
			unplink.nParserUrl = i;			
			unplink.lt = WPLT_A;			
			wp->pvUnpLinks->add (unplink);
		}

		if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
		{
			if (wp)
			{
				
				GetFileForReplace (&wptree->GetData (), wp, szFile);
				if (pszFA)	
					strcat (szFile, strchr (pszFA, '#'));	
				
				
				parser.ReplaceUrl (i, szFile);	
			}
			else
			{
				parser.ReplaceUrl (i, pszFullUrl);
			}
		}

		delete [] pszFullUrl;
	}

	return cAdded;
}

fsDLWebPage* fsWebPageDownloader::FindWebPage(LPCSTR pszFullUrl)
{
	fsURL url1, url2;

	if (IR_SUCCESS != url1.Crack (pszFullUrl))
		return NULL;

	
	for (size_t i = 0; i < m_vConfs.size (); i ++)
	{
		fsDLWebPage *wp = m_vConfs [i].wp;

		

		if (IsUrlsEqual (url1, wp->strURL))
			return wp;

		
		for (int i = 0; i < wp->pvUrls->size (); i++)
			if (IsUrlsEqual (url1, wp->pvUrls->at (i)))
				return wp;
	}

	return NULL;	
}

int fsWebPageDownloader::ParseHTMLImages(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL)
{
	

	int cAdded = 0;

	for (int i = 0; i < parser.GetImageCount (); i ++)
	{
		LPCSTR pszUrl = parser.GetImage (i);

		LPSTR pszFullUrl;
		fsUrlToFullUrl (pszBaseURL, pszUrl, &pszFullUrl);
		if (pszFullUrl == NULL)
			continue;

		fsURL url;
		if (IR_SUCCESS != url.Crack (pszFullUrl, TRUE))
		{
			delete [] pszFullUrl;
			continue;
		}

		if (*url.GetHostName () == 0)
		{
			delete [] pszFullUrl;
			continue;
		}

		char szFile [MY_MAX_PATH];
		fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFile, sizeof (szFile));
		LPCSTR pszExt = strrchr (szFile, '.');
		BOOL bExt = FALSE;
		if (pszExt++)
			bExt = IsExtInExtsStr (m_wpds.strImgsExts, pszExt);

		if (m_wpds.enImgsExtsType != WPDET_OFF)
		{
			if ( (bExt && m_wpds.enImgsExtsType == WPDET_NOTDOWNLOAD) ||
				  (bExt == FALSE && m_wpds.enImgsExtsType == WPDET_DOWNLOAD) )
			{
				parser.ReplaceImage (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
		}

		if (m_wpds.bNotAllImages)
		{
			int iUrl = parser.GetImageLinkTo (i);
			if (iUrl != -1)
			{
				fsURL url;
				LPCSTR pszUrlTo = parser.GetUrl (iUrl);
				LPSTR pszFullUrlTo;
				fsUrlToFullUrl (wptree->GetData ().strURL, pszUrlTo, &pszFullUrlTo);

				if (pszFullUrlTo)
				{
					url.Crack (pszFullUrlTo);
					delete [] pszFullUrlTo;

					if (fsIsServersEqual (url.GetHostName (), m_strStartServer) == FALSE)
					{
						parser.ReplaceImage (i, pszFullUrl);
						delete [] pszFullUrl;
						continue;
					}
				}
			}
		}

		fsDLWebPage *wp = FindWebPage (pszFullUrl);

		if (wp == NULL && bFixUrlsOnly == FALSE)
		{
			wp = AddPage (wptree, pszFullUrl, WPDPT_IMAGE);
			if (wp == NULL)
			{
				parser.ReplaceImage (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
			cAdded++;
		}

		if (wp && wp->pvUnpLinks)
		{
			_WP_UnprocessedLinks unplink;
			unplink.nWPIDWhere = wptree->GetData ().nID;
			unplink.nParserUrl = i;
			unplink.lt = WPLT_IMG;
			wp->pvUnpLinks->add (unplink);
		}

		if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
		{
			if (wp)
			{
				GetFileForReplace (&wptree->GetData (), wp, szFile);
				parser.ReplaceImage (i, szFile);
			}
			else
			{
				parser.ReplaceImage (i, pszFullUrl);
			}
		}

		delete [] pszFullUrl;
	}

	return cAdded;
}

LPCSTR fsWebPageDownloader::GetStartURL()
{
	return m_pages.GetData ().strURL;
}

float fsWebPageDownloader::GetPercentDone()
{
	float fDone = 0;	

	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		fsDLWebPage *wp = m_vConfs [i].wp;

		if (wp->dld && (wp->bState & WPSTATE_DLDWASDELETED) == 0 && wp->dld->pMgr->IsDone () == FALSE)
		{
			fDone += wp->dld->pMgr->GetPercentDone ();
			ASSERT (fDone >= 0);
		}
		else
			fDone += 100;
	}

	ASSERT (fDone >= 0);

	return fDone / m_vConfs.size ();	
}

int fsWebPageDownloader::GetFileCount()
{
	return m_vConfs.size ();
}

int fsWebPageDownloader::GetDoneFileCount()
{
	int cDone = 0;

	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		fsDLWebPage *wp = m_vConfs [i].wp;

		if (wp->dld && (wp->bState & WPSTATE_DLDWASDELETED) == 0)
		{
			if (wp->dld->pMgr->IsDone ())
				cDone ++;
		}
		else
			cDone ++;
	}

	return cDone;
}

BOOL fsWebPageDownloader::IsRunning()
{
	try {
	
	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		if (m_vConfs [i].wp->dld && (m_vConfs [i].wp->bState & WPSTATE_DLDWASDELETED) == 0 && 
				m_vConfs [i].wp->dld->pMgr->IsRunning ())
			return TRUE;
	}

	}catch (...) {Sleep (0); return IsRunning ();}

	return FALSE;
}

BOOL fsWebPageDownloader::IsDone()
{
	return GetDoneFileCount () == GetFileCount ();
}

BOOL fsWebPageDownloader::IsScheduled()
{
	if (_pwndScheduler == NULL)
		return FALSE;

	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		if (m_vConfs [i].wp->dld && (m_vConfs [i].wp->bState & WPSTATE_DLDWASDELETED) == 0 && 
			  _pwndScheduler->GetMgr ()->IsDownloadScheduled (m_vConfs [i].wp->dld))
			return TRUE;
	}

	return FALSE;
}

BOOL fsWebPageDownloader::IsOnAutoStart()
{
	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;

		if (dld && (m_vConfs [i].wp->bState & WPSTATE_DLDWASDELETED) == 0 && 
				dld->bAutoStart && dld->pMgr->IsDone () == FALSE)
			return TRUE;
	}

	return FALSE;
}

BOOL fsWebPageDownloader::IsDownloading()
{
	for (int i = m_vConfs.size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld != NULL)
		{
			if ((m_vConfs [i].wp->bState & WPSTATE_DLDWASDELETED) == 0 && 
				 dld->pMgr->IsDownloading ())
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL fsWebPageDownloader::CrackUrl(LPCSTR pszFullUrl, LPSTR *ppszUrlWA, LPSTR *ppszFA)
{
	LPCSTR pszA;	

	
	if (fsIsAnchorInUrl (pszFullUrl, ppszUrlWA, &pszA) == FALSE)
		return FALSE;

	

	if (ppszUrlWA && ppszFA)
	{
		

		char szFile [10000];	
        *szFile = 0;
		fsFileNameFromUrlPath (*ppszUrlWA, FALSE, FALSE, szFile, sizeof (szFile));		

		fsnew (*ppszFA, char, strlen (szFile) + strlen (pszA) + 1);

		strcpy (*ppszFA, szFile);
		strcat (*ppszFA, pszA);
	}

	return TRUE;
}

fsWPDSettings* fsWebPageDownloader::GetWDPS()
{
	return &m_wpds;
}

void fsWebPageDownloader::ReadDefaultWPDS(fsWPDSettings *wpds)
{
	wpds->strHTMLExts = _App.Spider_HTMLExts ();
	wpds->bNotAllFiles = _App.Spider_NotAllFiles ();
	wpds->bNotAllImages = _App.Spider_NotAllImages ();
	wpds->bNotAllPages = _App.Spider_NotAllPages ();
	wpds->bDownloadFiles = _App.Spider_DownloadFiles ();
	wpds->bDownloadImages = _App.Spider_DownloadImages ();
	wpds->iDepth = _App.Spider_Depth ();
	wpds->strExts = _App.Spider_Exts ();
	wpds->enExtsType = _App.Spider_ExtsType ();
	wpds->strImgsExts = _App.Spider_ImgExts ();
	wpds->enImgsExtsType = _App.Spider_ImgExtsType ();
	wpds->bDownloadStyles = _App.Spider_DownloadStyles ();
	wpds->bSavePagesUnderHTM = _App.Spider_SavePagesUnderHTM ();
	wpds->dwFlags = WPDF_KEEPFOLDERSTRUCTURE | WPDF_DELCOMPLETEDDLDS;
}

int fsWebPageDownloader::ParseHTMLLinkUrls(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL)
{
	
	int cAdded = 0;

	for (int i = 0; i < parser.GetLinkUrlCount (); i ++)
	{
		LPCSTR pszUrl = parser.GetLinkUrl (i);
		fsLinkRelType lrt = parser.GetLinkUrlRelType (i);

		if (lrt != LRT_STYLESHEET)
			continue;

		LPSTR pszFullUrl;
		fsUrlToFullUrl (pszBaseURL, pszUrl, &pszFullUrl);
		if (pszFullUrl == NULL)
			continue;

		fsURL url;
		url.Crack (pszFullUrl);

		if (*url.GetHostName () == 0)
		{
			delete [] pszFullUrl;
			continue;
		}

		char szFile [MY_MAX_PATH];
		fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFile, sizeof (szFile));
		
		fsDLWebPage *wp = FindWebPage (pszFullUrl);

		if (wp == NULL && bFixUrlsOnly == NULL)
		{
			wp = AddPage (wptree, pszFullUrl, WPDPT_CSS);
			if (wp == NULL)
			{
				parser.ReplaceLinkUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
			cAdded++;
		}
		
		if (wp && wp->pvUnpLinks)
		{
			_WP_UnprocessedLinks unplink;
			unplink.nWPIDWhere = wptree->GetData ().nID;
			unplink.nParserUrl = i;
			unplink.lt = WPLT_STYLESHEET;
			wp->pvUnpLinks->add (unplink);
		}

		if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
		{
			if (wp)
			{
				
				GetFileForReplace (&wptree->GetData (), wp, szFile);
				parser.ReplaceLinkUrl (i, szFile);
			}
			else
			{
				parser.ReplaceLinkUrl (i, pszFullUrl);
			}
		}

		delete [] pszFullUrl;
	}

	return cAdded;
}

int fsWebPageDownloader::GetDownloadCount()
{
	return m_vConfs.size ();
}

vmsDownloadSmartPtr fsWebPageDownloader::GetDownload(int iIndex)
{
	return m_vConfs [iIndex].wp->dld;
}

void fsWebPageDownloader::StartDownloading()
{
	m_bStopped = FALSE;
	m_bWasShutdownMsg = FALSE;

	DLDS_LIST vDlds;
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld == NULL || dld->pMgr->IsDone ())
			continue;
		vDlds.push_back (dld);
	}

	_DldsMgr.StartDownloads (vDlds, TRUE);
}

void fsWebPageDownloader::StopDownloading()
{
	m_bStopped = TRUE;

	DLDS_LIST vDlds;

	size_t sizeWas = m_vConfs.size ();

	for (size_t i = 0; i < sizeWas; i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld == NULL)
			continue;
		vDlds.push_back (dld);
	}

	if (vDlds.size () == 0)
		return;

	_DldsMgr.StopDownloads (vDlds, TRUE);

	for (i = 0; i < vDlds.size (); i++)
		_pwndDownloads->UpdateDownload (vDlds [i]);

	if ((size_t)m_vConfs.size () > sizeWas)
		StopDownloading ();
}

void fsWebPageDownloader::SetAutoStartDownloading(BOOL b)
{
	if (b)
		m_bStopped = FALSE;
	
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld && dld->pMgr->IsDone () == FALSE)
		{
			dld->bAutoStart = b;
			_pwndDownloads->UpdateDownload (dld);
		}
	}

	_DldsMgr.ProcessDownloads ();
}

void fsWebPageDownloader::SetEventFunc(fntWPDEvents pfn, LPVOID lp)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lp;
}

void fsWebPageDownloader::Event(fsWPDEvent ev, int info)
{
	try {
	if (m_pfnEvents)
		m_pfnEvents (this, ev, info, m_lpEventsParam);
	}
	catch (...) {}
}

void fsWebPageDownloader::DeleteAllDownloads(BOOL bByUser)
{
	DLDS_LIST vDlds;

	m_bIsDeleting = TRUE;
	m_bStopped = TRUE;

	StopDownloading ();

	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld)
			vDlds.push_back (dld);
	}

	if (vDlds.size () == 0)
		return;

	size_t cDeleted = _DldsMgr.DeleteDownloads (vDlds, bByUser, FALSE);

	for (i = 0; i < m_vConfs.size () && cDeleted; i++)
	{
		if (m_vConfs [i].wp->dld)
		{
			m_vConfs [i].wp->dld = NULL;
			cDeleted --;
		}
	}

	if (vDlds.size () == cDeleted)
		DeleteAllDownloads (bByUser); 
}

void fsWebPageDownloader::DetachFromDownloads()
{
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld)
			dld->pfnDownloadEventsFunc = NULL;
	}
}

BOOL fsWebPageDownloader::Save(HANDLE hFile)
{
	DWORD dw;

	if (FALSE == fsSaveStrToFile (m_strStartServer, hFile))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bDownloadFiles, sizeof (m_wpds.bDownloadFiles), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bDownloadImages, sizeof (m_wpds.bDownloadImages), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bDownloadStyles, sizeof (m_wpds.bDownloadStyles), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bNotAllFiles, sizeof (m_wpds.bNotAllFiles), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bNotAllImages, sizeof (m_wpds.bNotAllImages), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bNotAllPages, sizeof (m_wpds.bNotAllPages), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.bSavePagesUnderHTM, sizeof (m_wpds.bSavePagesUnderHTM), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.dwFlags, sizeof (m_wpds.dwFlags), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.enExtsType, sizeof (m_wpds.enExtsType), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.enImgsExtsType, sizeof (m_wpds.enImgsExtsType), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.iDepth, sizeof (m_wpds.iDepth), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.iReserved, sizeof (m_wpds.iReserved), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &m_wpds.pDLGroup->nId, sizeof (m_wpds.pDLGroup->nId), &dw, NULL))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strExts, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strImgsExts, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strFolderSaveTo, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strHTMLExts, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strUserName, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (m_wpds.strPassword, hFile))
		return FALSE;

	
	int cItems = m_wpds.vIgnoreList.size ();
	if (FALSE == WriteFile (hFile, &cItems, sizeof (int), &dw, NULL))
		return FALSE;
	for (int i = 0; i < cItems; i++)
	{
		if (FALSE == fsSaveStrToFile (m_wpds.vIgnoreList [i]->strURL, hFile))
			return FALSE;

		if (FALSE == WriteFile (hFile, &m_wpds.vIgnoreList [i]->dwFlags, sizeof (DWORD),
				&dw, NULL))
			return FALSE;
	}		

	
	return Save (hFile, &m_pages);
}

BOOL fsWebPageDownloader::Save(HANDLE hFile, t_wptree root)
{
	DWORD dw;

	fsDLWebPage *wp = &root->GetData ();
	
	if (FALSE == WriteFile (hFile, &wp->uDldId, sizeof (wp->uDldId), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &wp->bState, sizeof (wp->bState), &dw, NULL))
		return FALSE;

	if (FALSE == WriteFile (hFile, &wp->nID, sizeof (wp->nID), &dw, NULL))
		return FALSE;

	if (FALSE == fsSaveStrToFile (wp->strFile, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (wp->strURL, hFile))
		return FALSE;

	BYTE cUrls = (BYTE) wp->pvUrls->size ();
	if (FALSE == WriteFile (hFile, &cUrls, sizeof (cUrls), &dw, NULL))
		return FALSE;
	for (int  j = 0; j < cUrls; j++)
	{
		if (FALSE == fsSaveStrToFile (wp->pvUrls->at (j), hFile))
			return FALSE;
	}

	BOOL b = wp->pvUnpLinks && wp->pvUnpLinks->size ();

	if (FALSE == WriteFile (hFile, &b, sizeof (b), &dw, NULL))
		return FALSE;

	if (b)
	{
		if (FALSE == wp->pvUnpLinks->save (hFile))
			return FALSE;
	}
	
	int cLeafs = root->GetLeafCount ();
	if (FALSE == WriteFile (hFile, &cLeafs, sizeof (cLeafs), &dw, NULL))
		return FALSE;

	for (int i = 0; i < cLeafs; i++)
		if (FALSE == Save (hFile, root->GetLeaf (i)))
			return FALSE;

	return TRUE;
}

BOOL fsWebPageDownloader::Load_OLD(HANDLE hFile, BOOL bOldVer)
{
	DWORD dw;

	if (FALSE == fsReadStrFromFile (&m_strStartServer.pszString, hFile))
		return FALSE;

	if (bOldVer)
	{
		fsWPDSettings_v1 wpds1;
		if (FALSE == ReadFile (hFile, &wpds1, sizeof (wpds1), &dw, NULL))
			return FALSE;
		m_wpds.bDownloadFiles = wpds1.bDownloadFiles;
		m_wpds.bDownloadImages = wpds1.bDownloadImages;
		m_wpds.bDownloadStyles = wpds1.bDownloadStyles;
		m_wpds.bNotAllFiles = wpds1.bNotAllFiles;
		m_wpds.bNotAllImages = wpds1.bNotAllImages;
		m_wpds.bNotAllPages = wpds1.bNotAllPages;
		m_wpds.enExtsType = wpds1.enExtsType;
		m_wpds.enImgsExtsType = wpds1.enImgsExtsType;
		m_wpds.iDepth = wpds1.iDepth;
		m_wpds.iReserved = wpds1.iReserved;
		wpds1.strDLGroup.pszString = NULL;
		wpds1.strExts.pszString = NULL;
		wpds1.strFolderSaveTo.pszString = NULL;
		wpds1.strHTMLExts.pszString = NULL;
		wpds1.strImgsExts.pszString = NULL;
	}
	else
	{
		if (FALSE == ReadFile (hFile, &m_wpds, sizeof (m_wpds) - 2*sizeof (BOOL), &dw, NULL))
			return FALSE;

		m_wpds.bSavePagesUnderHTM = FALSE;
		m_wpds.dwFlags = 0;
	}

	fsString str;
	if (FALSE == fsReadStrFromFile (&str.pszString, hFile))
		return FALSE;
	m_wpds.pDLGroup = _DldsGrps.FindGroupByName (str);
	if (m_wpds.pDLGroup == NULL)
		m_wpds.pDLGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);

	if (FALSE == fsReadStrFromFile (&m_wpds.strExts.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strImgsExts.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strFolderSaveTo.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strHTMLExts.pszString, hFile))
		return FALSE;

	if (bOldVer == FALSE)
	{
		if (FALSE == fsReadStrFromFile (&m_wpds.strUserName.pszString, hFile))
			return FALSE;

		if (FALSE == fsReadStrFromFile (&m_wpds.strPassword.pszString, hFile))
			return FALSE;
	}
	else
	{
		m_wpds.strUserName.pszString = NULL;
		m_wpds.strPassword.pszString = NULL;
	}

	m_nMaxID = 0;

	return Load (hFile, &m_pages, 3);
}

BOOL fsWebPageDownloader::Load(HANDLE hFile, t_wptree root, WORD wVer)
{
	DWORD dw;

	fsDLWebPage wp;

	if (FALSE == ReadFile (hFile, &wp.uDldId, sizeof (wp.uDldId), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &wp.bState, sizeof (wp.bState), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &wp.nID, sizeof (wp.nID), &dw, NULL))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&wp.strFile.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&wp.strURL.pszString, hFile))
		return FALSE;

	BYTE cUrls;
	if (FALSE == ReadFile (hFile, &cUrls, sizeof (cUrls), &dw, NULL))
		return FALSE;
	fsnew1 (wp.pvUrls, fs::list <fsString>);
	while (cUrls--)
	{
		fsString strURL;
		if (FALSE == fsReadStrFromFile (&strURL.pszString, hFile))
			return FALSE;
		wp.pvUrls->add (strURL);
	}

	m_nMaxID = max (m_nMaxID, wp.nID);
	
	wp.dld = NULL;
	wp.pvUnpLinks = NULL;

	if (wp.uDldId != UINT (-1))
	{
		WebPage_FindDownload (&wp);

		BOOL b;

		if (wVer > 3)
		{
			if (FALSE == ReadFile (hFile, &b, sizeof (b), &dw, NULL))
				return FALSE;
		}
		else
		{
			b = wp.dld ? wp.dld->pMgr->IsDone () == FALSE : FALSE;
		}
		
		if (b)
		{
			fsnew1 (wp.pvUnpLinks, fs::list <_WP_UnprocessedLinks>);
			if (FALSE == wp.pvUnpLinks->load (hFile))
				return FALSE;
		}

		if (wp.dld)
		{
			wp.dld->pfnDownloadEventsFunc = _DldEvents;
			wp.dld->lpEventsParam = this;
			
			wp.dld->dwFlags |= DLD_USEDBYHTMLSPIDER;
		}
	}

	root->SetData (wp);

	_Conformity conf;
	conf.wp = &root->GetData ();
	conf.wptree = root;
	m_vConfs.push_back (conf);

	int cLeafs;
	if (FALSE == ReadFile (hFile, &cLeafs, sizeof (cLeafs), &dw, NULL))
		return FALSE;

	for (int i = 0; i < cLeafs; i++)
	{
		fsDLWebPage wp;
		if (FALSE == Load (hFile, root->AddLeaf (wp), wVer))
			return FALSE;
	}

	return TRUE;
}

t_wptree fsWebPageDownloader::GetRootPage()
{
	return &m_pages;
}

BOOL fsWebPageDownloader::IsUrlsEqual(fsURL &url1, LPCSTR pszUrl)
{
	fsURL url2;
	if (url2.Crack (pszUrl) == IR_SUCCESS)
	{
		if (url1.GetPort () == url2.GetPort () && url1.GetInternetScheme () == url2.GetInternetScheme () &&
			 fsIsServersEqual(url1.GetHostName (), url2.GetHostName ()))
		{
			
			if (stricmp (url2.GetPath (), url1.GetPath ()) == 0)
				return TRUE;	

			
			
			

			if (*url2.GetPath () == 0 || strcmp (url2.GetPath (), "/")  == 0 || strcmp (url2.GetPath (), "\\")  == 0)
			{
				char szFile [MY_MAX_PATH];
				if (fsFileNameFromUrlPath (url1.GetPath (), url1.GetInternetScheme () == INTERNET_SCHEME_FTP,
						FALSE, szFile, sizeof (szFile)))
				{
					LPCSTR pszPath = url1.GetPath (); 
					if (*pszPath == '\\' || *pszPath == '/') 
						pszPath++;
					
					if (strcmp (szFile, pszPath) == 0)
					{
						char *pszExt = strrchr (szFile, '.');
						if (pszExt)
						{
							
							if (pszExt - szFile == 5 && strncmp (szFile, "index", 5) == 0)
							{
								
								if (IsExtInExtsStr (m_wpds.strHTMLExts, pszExt+1))
									return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

void fsWebPageDownloader::OnDldRedirected(vmsDownloadSmartPtr dld)
{
	fsString strNewUrl = dld->pMgr->get_URL (TRUE);

	
	fsDLWebPage *wp = FindWebPage (strNewUrl);
	if (wp == NULL)
	{
		

		wp = FindWebPage (dld);
		wp->pvUrls->add (wp->strURL);	
		wp->strURL = strNewUrl;	
	}
	else
	{
		
		
		
		

		
		fsDLWebPage* oldwp = FindWebPage (dld);

		if (oldwp == wp)
			return;

		
		CorrectUnpUrls (oldwp, wp);
		

		
		DeleteWebPage (oldwp);
		
		dld->dwFlags |= DLD_DELETEFILEALWAYS;
		_pwndDownloads->DeleteDownload (dld, FALSE);
		
		wp->pvUrls->add (strNewUrl);
	}
}

fsDLWebPage* fsWebPageDownloader::FindWebPage(UINT nID)
{
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		if (m_vConfs [i].wp->nID == nID)
			return m_vConfs [i].wp;
	}

	return NULL;
}

void fsWebPageDownloader::DeleteWebPage(fsDLWebPage *wp)
{
	t_wptree tree = NULL;

	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		if (m_vConfs [i].wp == wp)
		{
			tree = m_vConfs [i].wptree;
			break;
		}
	}

	if (tree == NULL)
		return;

	t_wptree root = tree->GetRoot ();

	Event (WPDE_WEBPAGEWILLBEDELETED, (int) wp);

	m_vConfs.erase (m_vConfs.begin () + i);

	for (i = 0; i < (size_t)root->GetLeafCount (); i++)
	{
		if (root->GetLeaf (i)->GetData ().nID == wp->nID)
		{
			root->DeleteLeaf (i);
			break;
		}
	}
	
	SAFE_DELETE (wp->pvUnpLinks);
	SAFE_DELETE (wp->pvUrls);
}

BOOL fsWebPageDownloader::Load(HANDLE hFile, WORD wVer)
{
	if(wVer < 3)
		return Load_OLD (hFile, wVer < 2);

	DWORD dw;

	if (FALSE == fsReadStrFromFile (&m_strStartServer.pszString, hFile))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bDownloadFiles, sizeof (m_wpds.bDownloadFiles), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bDownloadImages, sizeof (m_wpds.bDownloadImages), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bDownloadStyles, sizeof (m_wpds.bDownloadStyles), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bNotAllFiles, sizeof (m_wpds.bNotAllFiles), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bNotAllImages, sizeof (m_wpds.bNotAllImages), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bNotAllPages, sizeof (m_wpds.bNotAllPages), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.bSavePagesUnderHTM, sizeof (m_wpds.bSavePagesUnderHTM), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.dwFlags, sizeof (m_wpds.dwFlags), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.enExtsType, sizeof (m_wpds.enExtsType), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.enImgsExtsType, sizeof (m_wpds.enImgsExtsType), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.iDepth, sizeof (m_wpds.iDepth), &dw, NULL))
		return FALSE;

	if (FALSE == ReadFile (hFile, &m_wpds.iReserved, sizeof (m_wpds.iReserved), &dw, NULL))
		return FALSE;

	if (wVer < 6)
	{
		fsString str;
		if (FALSE == fsReadStrFromFile (&str.pszString, hFile))
			return FALSE;
		m_wpds.pDLGroup = _DldsGrps.FindGroupByName (str);
	}
	else
	{
		UINT nId;
		if (FALSE == ReadFile (hFile, &nId, sizeof (nId), &dw, NULL))
			return FALSE;
		m_wpds.pDLGroup = _DldsGrps.FindGroup (nId);
	}

	if (m_wpds.pDLGroup == NULL)
		m_wpds.pDLGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);

	if (FALSE == fsReadStrFromFile (&m_wpds.strExts.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strImgsExts.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strFolderSaveTo.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strHTMLExts.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strUserName.pszString, hFile))
		return FALSE;

	if (FALSE == fsReadStrFromFile (&m_wpds.strPassword.pszString, hFile))
		return FALSE;

	if (wVer >= 5)
	{
		
		int cItems;
		ASSERT (m_wpds.vIgnoreList.size () == 0);
		if (FALSE == ReadFile (hFile, &cItems, sizeof (int), &dw, NULL))
			return FALSE;
		for (int i = 0; i < cItems; i++)
		{
			fsWPDIgnoreListItem *item = new fsWPDIgnoreListItem;
			if (FALSE == fsReadStrFromFile (&item->strURL.pszString, hFile))
				return FALSE;

			if (FALSE == ReadFile (hFile, &item->dwFlags, sizeof (DWORD),
					&dw, NULL))
				return FALSE;

			m_wpds.vIgnoreList.add (item);
		}
	}

	m_nMaxID = 0;

	if (FALSE == Load (hFile, &m_pages, wVer))
	{
		Load_PerformRollback ();
		return FALSE;
	}

	return TRUE;
}

#pragma warning (disable:4706)
void fsWebPageDownloader::GetPtrToFile(LPCSTR pszToFile, LPCSTR pszFromFile, LPSTR pszPtr)
{
	
	int cComm = 0;

	char szToPath [10000];	
	char szFromPath [10000];

	fsGetPath (pszToFile, szToPath);
	fsGetPath (pszFromFile, szFromPath);

	int lto = strlen (szToPath);
	int lfrom = strlen (szFromPath);

	LPCSTR pszTo = szToPath;
	LPCSTR pszFrom = szFromPath;

	cComm = min (lto, lfrom);

	while (TRUE)
	{
		if (strnicmp (pszTo, pszFrom, cComm) == 0 && pszTo [cComm-1] == '\\')
			break;	

		cComm--;
		if (cComm < 1)
			return;	
	}

	pszTo += cComm;
	pszFrom += cComm;

	char szFile [10000];
	fsGetFileName (pszToFile, szFile);

	int cUppers = 0;	

	while (pszFrom = strchr (pszFrom+1, '\\'))
		cUppers ++;

	*pszPtr = 0;

	while (cUppers--)
		strcat (pszPtr, "..\\");	

	
	strcat (pszPtr, pszTo);
	strcat (pszPtr, szFile);	
}
#pragma warning (default:4706)

void fsWebPageDownloader::GetFileForReplace(fsDLWebPage *wpwhere, fsDLWebPage *wpwhat, LPSTR pszFile)
{
	fsString strFileWhat = wpwhat->dld ? wpwhat->dld->pMgr->get_OutputFilePathName () : wpwhat->strFile;

	if (m_wpds.dwFlags & WPDF_KEEPFOLDERSTRUCTURE)
	{
		fsString strFileWhere = wpwhere->dld ? wpwhere->dld->pMgr->get_OutputFilePathName () : wpwhere->strFile;
		GetPtrToFile (strFileWhat, strFileWhere, pszFile);

	}
	else
		fsGetFileName (strFileWhat, pszFile);	

	fsPathToGoodUrlPath (pszFile);
}

void fsWebPageDownloader::GetDownloadingSiteName(LPSTR psz)
{
	fsURL url;
	DWORD dw = 10000;
	url.Crack (GetStartURL ());
	url.Create (url.GetInternetScheme (), url.GetHostName (), url.GetPort (), 
		"", "", "", psz, &dw);
}

void fsWebPageDownloader::Load_PerformRollback()
{
	for (size_t i = 0; i < m_vConfs.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vConfs [i].wp->dld;
		if (dld)
		{
			dld->pfnDownloadEventsFunc = NULL;
		}
	}
}

DWORD fsWebPageDownloader::OnCheckFileExtIsOK(vmsDownloadSmartPtr dld)
{
	BOOL bOK = TRUE;

	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);
	if (dld->pMgr->GetDownloadMgr () == NULL)
		return TRUE;

	
	char szFile [MY_MAX_PATH];
	fsFileNameFromUrlPath (dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszPathName, 
		dld->pMgr->GetDownloadMgr ()->GetDNP ()->enProtocol == NP_FTP,
		TRUE, szFile, sizeof (szFile));

	
	LPCSTR pszExt = strrchr (szFile, '.');

	BOOL bExt = FALSE;		

	if (pszExt++)
		bExt = IsExtInExtsStr (m_wpds.strExts, pszExt);

	if (m_wpds.bDownloadFiles == FALSE)
		bOK = FALSE;

	if (m_wpds.enExtsType != WPDET_OFF)
	{
		if ( (bExt && m_wpds.enExtsType == WPDET_NOTDOWNLOAD) ||
			  (bExt == FALSE && m_wpds.enExtsType == WPDET_DOWNLOAD) )
		{
			bOK = FALSE;
		}
	}

	
	if (m_wpds.bNotAllFiles) 
	{
		
		if (fsIsServersEqual (dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszServerName, m_strStartServer) == FALSE)
			bOK = FALSE;
	}

	return bOK;
}

void fsWebPageDownloader::CorrectUnpUrls(fsDLWebPage* wpfrom, fsDLWebPage* wpto)
{
	char szFile [MY_MAX_PATH];

	if (wpfrom == wpto)
		return;

	if (wpfrom->pvUnpLinks == NULL)
		return;

	while (wpfrom->pvUnpLinks->size ())
	{
		
		UINT nWPIDWhere = wpfrom->pvUnpLinks->at (0).nWPIDWhere;
		fsDLWebPage *wpwhere = FindWebPage (nWPIDWhere);

		
		fs::list <UINT> vnUrls;
		fs::list <_WP_LinkType> vLinkTypes;

		for (int j = 0; j < wpfrom->pvUnpLinks->size ();)
		{
			
			
			if (nWPIDWhere == wpfrom->pvUnpLinks->at (j).nWPIDWhere)
			{
				
				vnUrls.add (wpfrom->pvUnpLinks->at (j).nParserUrl);
				vLinkTypes.add (wpfrom->pvUnpLinks->at (j).lt);
				
				wpfrom->pvUnpLinks->del (j);
			}
			else
			{
				
				j++;
			}
		}

		if (wpwhere == NULL)
			continue;

		

		
		HANDLE hFile = CreateFile (wpwhere->dld ? wpwhere->dld->pMgr->get_OutputFilePathName () : wpwhere->strFile, 
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			
			continue;
		}

		DWORD dwSize = GetFileSize (hFile, NULL);
		LPSTR pszHTML;
		fsnew (pszHTML, char, int (dwSize*1.5) + 10000 + 1); 

		if (wpto) 
			GetFileForReplace (wpwhere, wpto, szFile); 
		else
			strcpy (szFile, wpfrom->strURL); 

		UINT newlen = 0;
		if (ReadFile (hFile, pszHTML, dwSize, &dwSize, NULL))
		{
			

			pszHTML [dwSize] = 0;
			fsHTMLParser parser;
			parser.SetKillDupes (FALSE);
			parser.ParseHTML (pszHTML);	

			

			for (j = 0; j < vnUrls.size (); j++)
			{
				

				switch (vLinkTypes [j])
				{
					case WPLT_A:
						parser.ReplaceUrl (vnUrls [j], szFile);
						break;

					case WPLT_IMG:
						parser.ReplaceImage (vnUrls [j], szFile);
						break;

					case WPLT_STYLESHEET:
						parser.ReplaceLinkUrl (vnUrls [j], szFile);
						break;
				}
			}

			

			newlen = parser.GetHTMLLength ();
			SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
			SetEndOfFile (hFile);
			DWORD dw;
			WriteFile (hFile, pszHTML, newlen, &dw, NULL);
		}

		delete [] pszHTML;
		CloseHandle (hFile);
	}
}

int fsWebPageDownloader::ParseHTMLFrameUrls(fsHTMLParser &parser, t_wptree wptree, BOOL bFixUrlsOnly, LPCSTR pszBaseURL)
{
	int cAdded = 0;

	
	for (int i = 0; i < parser.GetFrameUrlCount (); i ++)
	{
		LPCSTR pszUrl = parser.GetFrameUrl (i);

		
		if (*pszUrl == '#')
			continue;	

		LPSTR pszFullUrl;
		
		fsUrlToFullUrl (pszBaseURL, pszUrl, &pszFullUrl);
		if (pszFullUrl == NULL)
			continue;

		fsURL url;
		if (IR_SUCCESS != url.Crack (pszFullUrl, TRUE))
		{
			delete [] pszFullUrl;
			continue;
		}

		if (*url.GetHostName () == 0)
		{
			delete [] pszFullUrl;
			continue;
		}

		
		char szFile [MY_MAX_PATH];
		fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFile, sizeof (szFile));

		
		if (m_wpds.bNotAllPages)
		{
			
			if (fsIsServersEqual (url.GetHostName (), m_strStartServer) == FALSE)
			{
				
				parser.ReplaceFrameUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
		}

		for (int j = 0; j < m_wpds.vIgnoreList.size (); j++)
		{
			fsURL url2;
			url2.Crack (m_wpds.vIgnoreList [j]->strURL);

			if (fsIsServersEqual (url.GetHostName (), url2.GetHostName ()))
			{
				if (_strnicmp (url.GetPath (), url2.GetPath (), 
						lstrlen (url2.GetPath ())) == 0)
				{
					fsURL urlStart;
					urlStart.Crack (GetRootPage ()->GetData ().strURL);

					
					
					
					
					
					
					
					if (FALSE == fsIsServersEqual (urlStart.GetHostName (), url2.GetHostName ()) ||
							_strnicmp (urlStart.GetPath (), url2.GetPath (), 
								lstrlen (url2.GetPath ())))
					{
						
						BOOL bSkip = m_wpds.vIgnoreList [j]->dwFlags & WPD_ILITEM_SUBFOLDERSALSO;
						
						int l = lstrlen (url.GetPath ()), l2 = lstrlen (url2.GetPath ());
						if (bSkip == FALSE)
							bSkip = l == l2;
						
						if (bSkip == FALSE && l > l2)
							bSkip = (url.GetPath ()) [l] == '#';

						if (bSkip)
						{
							parser.ReplaceFrameUrl (i, pszFullUrl);
							SAFE_DELETE_ARRAY (pszFullUrl);
							break;
						}
					}
				}
			}
		}

		if (pszFullUrl == NULL)
			continue;

		LPSTR pszWA = NULL, pszFA = NULL; 

		if (bFixUrlsOnly == FALSE)
		{
			if (CrackUrl (pszFullUrl, &pszWA, &pszFA))
			{
				delete [] pszFullUrl;
				pszFullUrl = pszWA;
			}
		}

		fsDLWebPage *wp = FindWebPage (pszFullUrl);

		if (wp == NULL && bFixUrlsOnly == FALSE)
		{
			
			wp = AddPage (wptree, pszFullUrl, WPDPT_PAGE, TRUE);

			if (wp == NULL)
			{
				SAFE_DELETE_ARRAY (pszFA);
				parser.ReplaceFrameUrl (i, pszFullUrl);
				delete [] pszFullUrl;
				continue;
			}
		}

		if (wp && wp->pvUnpLinks)
		{
			
			

			_WP_UnprocessedLinks unplink;
			unplink.nWPIDWhere = wptree->GetData ().nID;	
			unplink.nParserUrl = i;			
			unplink.lt = WPLT_A;			
			wp->pvUnpLinks->add (unplink);
		}

		if ((m_wpds.dwFlags & WPDF_DONTSTOREPAGES) == 0)
		{
			if (wp)
			{
				
				GetFileForReplace (&wptree->GetData (), wp, szFile);
				if (pszFA)	
					strcat (szFile, strchr (pszFA, '#'));	
				
				
				parser.ReplaceFrameUrl (i, szFile);	
			}
			else
			{
				parser.ReplaceFrameUrl (i, pszFullUrl);
			}
		}

		delete [] pszFullUrl;
	}

	return cAdded;
}

BOOL fsWebPageDownloader::IsURLShouldBeIgnored(fsURL &url)
{
	
	BOOL bOnlyConditionPresent = FALSE;

	for (int j = 0; j < m_wpds.vIgnoreList.size (); j++)
	{
		BOOL bOnlyCondition = m_wpds.vIgnoreList [j]->dwFlags & WPD_ILITEM_THISPATHONLY;
		if (bOnlyCondition)
			bOnlyConditionPresent = TRUE;

		fsURL url2;
		url2.Crack (m_wpds.vIgnoreList [j]->strURL);

		BOOL bSQ = FALSE; 

		if (fsIsServersEqual (url.GetHostName (), url2.GetHostName ()))
		{
			if (_strnicmp (url.GetPath (), url2.GetPath (), 
					lstrlen (url2.GetPath ())) == 0)
			{
				bSQ = m_wpds.vIgnoreList [j]->dwFlags & WPD_ILITEM_SUBFOLDERSALSO;

				
				if (bSQ == FALSE)
				{
					int l = lstrlen (url.GetPath ()), 
						l2 = lstrlen (url2.GetPath ());

					bSQ = l == l2;
					
					
					if (bSQ == FALSE && l > l2)
						bSQ = (url.GetPath ()) [l2] == '#' || (url.GetPath ()) [l2] == '?';
				}
			}
		}

		if (bSQ)
			return bOnlyCondition ? FALSE : TRUE;
	}

	return bOnlyConditionPresent ? TRUE : FALSE;
}
