/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsSitesMgr.h"
#include "inetutil.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsSitesMgr::fsSitesMgr()
{
	m_pfnEvents = NULL;
}

fsSitesMgr::~fsSitesMgr()
{

}

int fsSitesMgr::AddSite(fsSiteInfo &site)
{
	m_vSites.add (site);
	Event (SM_SITEADDED, &m_vSites [m_vSites.size () - 1]);
	return m_vSites.size () - 1;
}

fsSiteInfo* fsSitesMgr::GetSite(int iIndex)
{
	return &m_vSites [iIndex];
}

int fsSitesMgr::FindSite(LPCSTR pszName, DWORD dwValidFor, BOOL bAllReq)
{
	for (int i = 0; i < m_vSites.size (); i++)
	{
		fsSiteInfo &site = m_vSites [i];
		if (fsIsServersEqual (site.strName, pszName, site.dwValidFor & SITE_VALIDFOR_SUBDOMAINS))
		{
			if (bAllReq)
			{
				
				if ((site.dwValidFor & dwValidFor) == dwValidFor)
					return i;
			}
			else
			{
				
				
				if (site.dwValidFor & dwValidFor)
					return i;
			}
		}
	}

	return -1;
}

fsSiteInfo* fsSitesMgr::FindSite2(LPCSTR pszSite, DWORD dwValidFor, BOOL bAllReq)
{
	int iIndex = FindSite (pszSite, dwValidFor, bAllReq);
	
	if (iIndex == -1)
		return NULL;

	return &m_vSites [iIndex];
}

void fsSitesMgr::DeleteSite(fsSiteInfo *pSite)
{
	int iIndex = FindSite (pSite->strName, pSite->dwValidFor);
	if (iIndex == -1)
		return;
	Event (SM_SITEDELETED, &m_vSites [iIndex]);
	m_vSites.del (iIndex);
}

void fsSitesMgr::DeleteAllTempSites()
{
	m_vSites.lock ();

	for (int i = m_vSites.size () - 1; i >= 0; i--)
	{
		if (m_vSites [i].bTemp)
		{
			Event (SM_SITEDELETED, &m_vSites [i]);
			m_vSites.del (i);
		}
	}

	m_vSites.unlock ();
}

int fsSitesMgr::GetSiteCount()
{
	return m_vSites.size ();
}

void fsSitesMgr::SetEventsFunc(fntSitesMgrEvents pfn, LPVOID lpParam)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lpParam;
}

void fsSitesMgr::Event(fsSitesMgrEvent ev, fsSiteInfo* site)
{
	if (m_pfnEvents)
		m_pfnEvents (ev, site, m_lpEventsParam);
}

BOOL fsSitesMgr::SaveToFile(HANDLE hFile)
{
	m_vSites.lock ();

	int cPermSites = GetSiteCount () - GetTempSiteCount ();

	DWORD dw;

	fsSitesFileHdr hdr;

	if (!WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		m_vSites.unlock ();
		return FALSE;
	}

	if (!WriteFile (hFile, &cPermSites, sizeof (cPermSites), &dw, NULL))
	{
		m_vSites.unlock ();
		return FALSE;
	}

	BOOL bMaySavePwds = _App.SM_KeepPasswords () && 
		_App.SM_DontSavePwdsToDisk () == FALSE;
	
	for (int i = 0; i < GetSiteCount (); i++)
	{
		fsSiteInfo *site = GetSite (i);

		if (site->bTemp)
			continue;

		if (!fsSaveStrToFile (site->strName, hFile))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!fsSaveStrToFile (bMaySavePwds ? site->strPassword : NULL, hFile))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!fsSaveStrToFile (bMaySavePwds ? site->strUser : NULL, hFile))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		UINT nGrpId = site->pGroup ? site->pGroup->nId : (UINT)-1;
		if (!WriteFile (hFile, &nGrpId, sizeof (UINT), &dw, NULL))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!WriteFile (hFile, &site->dwFtpFlags, sizeof (site->dwFtpFlags), &dw, NULL))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!WriteFile (hFile, &site->cConnsNow, sizeof (site->cConnsNow), &dw, NULL))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!WriteFile (hFile, &site->cMaxConns, sizeof (site->cMaxConns), &dw, NULL))
		{
			m_vSites.unlock ();
			return FALSE;
		}

		if (!WriteFile (hFile, &site->dwValidFor, sizeof (site->dwValidFor), &dw, NULL))
		{
			m_vSites.unlock ();
			return FALSE;
		}
	}

	m_vSites.unlock ();
	return TRUE;	
}

int fsSitesMgr::GetTempSiteCount()
{
	int c = 0;

	for (int i = m_vSites.size () - 1; i >= 0; i--)
		if (m_vSites [i].bTemp)
			c++;

	return c;
}

BOOL fsSitesMgr::LoadFromFile(HANDLE hFile)
{
	int cSites;

	DWORD dw;
	BOOL bOldVer = FALSE;

	m_vSites.clear ();

	fsSitesFileHdr hdr;
	if (!ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		bOldVer = TRUE;
	else
	{
		if (strcmp (hdr.szSig, SITESFILE_SIG))
		{
			bOldVer = TRUE;
			SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
		}
		else if (hdr.wVer > SITESFILE_CURRENT_VERSION)
			return FALSE;
	}

	if (!ReadFile (hFile, &cSites, sizeof (cSites), &dw, NULL) || dw != sizeof (cSites))
		return FALSE;
	
	for (int i = 0; i < cSites; i++)
	{
		fsSiteInfo site;

		if (bOldVer)
		{
			fsSiteInfo_v1 sitev1;
			if (!ReadFile (hFile, &sitev1, sizeof (fsSiteInfo_v1), &dw, NULL) || dw != sizeof (fsSiteInfo_v1))
				return FALSE;

			site.dwFtpFlags = sitev1.bFtpPassive;
			site.bTemp = sitev1.bTemp;
			site.cConnsNow = sitev1.cConnsNow;
			site.cMaxConns = sitev1.cMaxConns;
			site.dwValidFor = SITE_VALIDFOR_HTTP|SITE_VALIDFOR_HTTPS|SITE_VALIDFOR_FTP|SITE_VALIDFOR_SUBDOMAINS;
		}
		else
		{
			if (hdr.wVer < 3)
			{
				fsSiteInfo_v2 sitev2;

				if (!ReadFile (hFile, &sitev2, sizeof (sitev2), &dw, NULL) || dw != sizeof (sitev2))
					return FALSE;

				if (!fsReadStrFromFile (&sitev2.strGroup.pszString, hFile))
					return FALSE;

				site.dwFtpFlags = sitev2.dwFtpFlags;
				site.bTemp = sitev2.bTemp;
				site.cConnsNow = sitev2.cConnsNow;
				site.cMaxConns = sitev2.cMaxConns;
				site.dwValidFor = sitev2.dwValidFor;
			}
		}

		if (!fsReadStrFromFile (&site.strName.pszString, hFile))
			return FALSE;

		if (!fsReadStrFromFile (&site.strPassword.pszString, hFile))
			return FALSE;

		if (!fsReadStrFromFile (&site.strUser.pszString, hFile))
			return FALSE;

		if (hdr.wVer >= 3)
		{
			UINT nId;
			if (!ReadFile (hFile, &nId, sizeof (nId), &dw, NULL))
				return FALSE;
			site.pGroup = nId != (UINT)-1 ? _DldsGrps.FindGroup (nId) : NULL;

			if (!ReadFile (hFile, &site.dwFtpFlags, sizeof (site.dwFtpFlags), &dw, NULL))
				return FALSE;

			if (!ReadFile (hFile, &site.cConnsNow, sizeof (site.cConnsNow), &dw, NULL))
				return FALSE;

			if (!ReadFile (hFile, &site.cMaxConns, sizeof (site.cMaxConns), &dw, NULL))
				return FALSE;

			if (!ReadFile (hFile, &site.dwValidFor, sizeof (site.dwValidFor), &dw, NULL))
				return FALSE;
		}

		site.cConnsNow = 0;
		site.bTemp = FALSE;

		AddSite (site);
	}

	return TRUE;	
}

void fsSitesMgr::SiteUpdated(fsSiteInfo *pSite)
{
	Event (SM_SITEUPDATED, pSite);
}

void fsSitesMgr::DeleteAllPasswords()
{
	for (int i = m_vSites.size () - 1; i >= 0; i--)
	{
		fsSiteInfo* site = &m_vSites [i];
		site->strUser = NULL;
		site->strPassword = NULL;
		SiteUpdated (site);
	}
}

void fsSitesMgr::CheckGroups()
{
	std::vector <fsSiteInfo*> v;

	m_vSites.lock ();

	for (int i = m_vSites.size () - 1; i >= 0; i--)
	{
		fsSiteInfo *site = &m_vSites [i];
		if (site->pGroup != NULL && site->pGroup->bAboutToBeDeleted)
		{
			site->pGroup = NULL;
			v.push_back (site);
		}
	}

	m_vSites.unlock ();

	for (size_t j = 0; j < v.size (); j++)
		SiteUpdated (v [j]);
}
