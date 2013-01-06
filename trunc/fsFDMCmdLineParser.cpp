/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsFDMCmdLineParser.h"
#include "vmsTorrentExtension.h"
#include "vmsAppMutex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsFDMCmdLineParser::fsFDMCmdLineParser() : 
	m_bRunAsElevatedTasksProcessor (false),
	m_bInstallIeIntegration (false)
{
	m_bAnotherFDMStarted = FALSE;
	m_bForceSilent = FALSE;
	m_bNeedExit = false;
	m_bNeedRegisterServer = m_bNeedUnregisterServer = false;
}

fsFDMCmdLineParser::~fsFDMCmdLineParser()
{

}

void fsFDMCmdLineParser::Parse()
{
	
	extern vmsAppMutex _appMutex;
	m_bAnotherFDMStarted = _appMutex.isAnotherInstanceStartedAlready ();

	if (FALSE == m_parser.Parse ())
		return;

	m_bForceSilent = FALSE;

	CFdmApp* app = (CFdmApp*) AfxGetApp ();

	for (int i = 0; i < m_parser.Get_ParameterCount (); i++)
	{
		LPCSTR pszParam = m_parser.Get_Parameter (i);
		LPCSTR pszValue = m_parser.Get_ParameterValue (i);

		if (strcmp (pszParam, "?") == 0)
		{
			MessageBox (0, "fdm.exe [-fs] [-url=]url1 [-url=]url2 ...\n\n-fs - force silent mode.\nIf url contains spaces it should be in quotes.\n\nExample:\nfdm.exe -fs \"http://site.com/read me.txt\"", "Command line usage", 0);
		}
		else if (stricmp (pszParam, "fs") == 0)
		{
			m_bForceSilent = TRUE;
		}
		else if (stricmp (pszParam, "URL") == 0 || *pszParam == 0)
		{
			fsURL url;
			BOOL bUrl = IR_SUCCESS == url.Crack (pszValue) && pszValue [1] != ':';
			BOOL bTorrent = FALSE;

			if (bUrl == FALSE)
			{
				bTorrent = strlen (pszValue) > 8 && 
					0 == stricmp (pszValue + strlen (pszValue) - 8, ".torrent");
			}

			if (bUrl == FALSE && bTorrent == FALSE)
				continue;

			if (bTorrent)
			{
				static BOOL bBtI = vmsFdmAppMgr::MakeSureBtInstalled ();
				if (bBtI)
					AddTorrentFile (pszValue);
				continue;
			}

			if (m_bAnotherFDMStarted)
			{
				IWGUrlReceiver* pAdd = NULL;
				CoCreateInstance (CLSID_WGUrlReceiver, NULL, CLSCTX_ALL, 
					IID_IWGUrlReceiver, (void**) &pAdd);

				if (pAdd)
				{
					CComBSTR url = pszValue;
					pAdd->put_Url (url);
					if (m_bForceSilent)
						pAdd->put_ForceSilent (TRUE);
					pAdd->AddDownload ();
					pAdd->Release ();
				}
			}
			else
			{
				CFdmApp::_inc_UrlToAdd url;
				url.strUrl = pszValue;
				url.bForceSilent = m_bForceSilent;
				app->m_vUrlsToAdd.add (url);
			}
		}
		else if (stricmp (pszParam, "nostart") == 0)
		{
			m_bNeedExit = true;
		}
		else if (stricmp (pszParam, "assocwithtorrent") == 0)
		{
			bool bAssoc = strcmp (pszValue, "0") != 0;
			if (bAssoc)
			{
				_App.Bittorrent_OldTorrentAssociation (vmsTorrentExtension::GetCurrentAssociation ());
				vmsTorrentExtension::Associate ();
			}
			else
			{
				vmsTorrentExtension::AssociateWith (_App.Bittorrent_OldTorrentAssociation ());
			}
		}
		else if (!stricmp (pszParam, "RegServer"))
		{
			m_bNeedRegisterServer = true;
		}
		else if (!stricmp (pszParam, "UnregServer"))
		{
			m_bNeedUnregisterServer = true;
		}
		else if (!stricmp (pszParam, "runelevated"))
		{
			m_bRunAsElevatedTasksProcessor = true;
		}
		else if (!stricmp (pszParam, "ie"))
		{
			m_bInstallIeIntegration = true;
		}
	}
}

BOOL fsFDMCmdLineParser::is_ForceSilentSpecified()
{
	return m_bForceSilent;
}

void fsFDMCmdLineParser::AddTorrentFile(LPCSTR pszFile)
{
	if (m_bAnotherFDMStarted)
	{
		IFdmTorrentFilesRcvr* pAdd = NULL;
		CoCreateInstance (CLSID_FdmTorrentFilesRcvr, NULL, CLSCTX_ALL, 
			IID_IFdmTorrentFilesRcvr, (void**) &pAdd);

		if (pAdd)
		{
			CComBSTR file = pszFile;
			pAdd->put_ForceSilent (m_bForceSilent);
			pAdd->CreateBtDownloadFromFile (file);
			pAdd->Release ();
		}
	}
	else
	{
		CFdmApp::_inc_UrlToAdd url;
		url.strUrl = pszFile;
		url.bForceSilent = m_bForceSilent;
		((CFdmApp*) AfxGetApp ())->m_vTorrentFilesToAdd.add (url);
	}
}

bool fsFDMCmdLineParser::isNeedRegisterServer(void)
{
	return m_bNeedRegisterServer;
}

bool fsFDMCmdLineParser::isNeedUnregisterServer(void)
{
	return m_bNeedUnregisterServer;
}
