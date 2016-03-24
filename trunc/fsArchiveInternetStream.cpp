/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fsArchiveInternetStream.h"
#include "fsInternetDownloader.h"
#include "inetutil.h"
#include "vmsInternetSession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace fsArchive;

fsArchiveInternetStream::fsArchiveInternetStream()
{
	m_bNeedStop = FALSE;
	m_bOpened = FALSE;
	m_irLastErr = IR_SUCCESS;
	m_cMaxRetries = 1;
}

fsArchiveInternetStream::~fsArchiveInternetStream()
{

}

fsInternetResult fsArchiveInternetStream::Open(fsInternetDownloader *dldr, UINT64 uStartPos)
{
	m_dldr = dldr;
	m_bNeedStop = FALSE;

	for (UINT i = 0; i < m_cMaxRetries; i++)
	{
		Close ();
		m_irLastErr = Open_imp (uStartPos);
		
		if (m_bNeedStop)
		{
			m_irLastErr = IR_S_FALSE;
			break;
		}

		if (m_irLastErr == IR_SUCCESS)
			break;
	}
	
	return m_irLastErr;
}

int fsArchiveInternetStream::Read(LPVOID pBuffer, int cBytes)
{
	DWORD cRead = 0;
	DWORD cToRead;
	int c0Reads = 0;

	while (cRead < (DWORD)cBytes)
	{
		if (m_bNeedStop)
			return -1;
		
		cToRead = cBytes - cRead;
		m_irLastErr = m_file.Read (LPBYTE (pBuffer) + cRead, cToRead, &cToRead);
		if (m_irLastErr != IR_SUCCESS)
		{
			m_dwLastError = ASE_NOMOREDATA;
			return -1;
		}

		if (cToRead)
			c0Reads = 0;
		else 
			c0Reads++;

		cRead += cToRead;
		m_uCurPos += cToRead;
		
		if (c0Reads > 100)
		{
			m_dwLastError = ASE_NOMOREDATA;
			return -1;
		}
	}
	
	return cBytes;
}

int fsArchiveInternetStream::Write(LPVOID , int )
{
	return -1;
}

BOOL fsArchiveInternetStream::Seek(UINT64 uDistance, fsSeekType enType)
{
	Close ();

	UINT64 posTo;

	switch (enType)
	{
		case ST_CURRENT:
			posTo = m_uCurPos + uDistance;
			break;

		case ST_BEGIN:
			posTo = uDistance;
			break;

		default:
			return FALSE;
	}

	if (IR_SUCCESS != (m_irLastErr = Open (m_dldr, posTo)))
		return FALSE;

	return TRUE;
}

void fsArchiveInternetStream::Close()
{
	m_file.CloseHandle ();
	m_bOpened = FALSE;
}

void fsArchiveInternetStream::Stop()
{
	m_bNeedStop = TRUE;
	Close ();
}

fsInternetResult fsArchiveInternetStream::Open_imp(UINT64 uStartPos)
{
	fsInternetResult ir;
	fsDownload_NetworkProperties* dnp = m_dldr->DNP ();

	if (m_bOpened == FALSE)
	{
		vmsInternetSession* pSession = new vmsInternetSession;
		ir  = pSession->Create (dnp->pszAgent, dnp->enAccType, dnp->pszProxyName, dnp->enProtocol);
		if (ir != IR_SUCCESS)
		{
			delete pSession;
			return ir;
		}
	
		m_dldr->ApplyProxySettings (pSession, dnp, m_dldr->Get_Timeout ());

		ir = m_file.Initialize (pSession, TRUE);
		if (ir != IR_SUCCESS)
		{
			delete pSession;
			return ir;
		}
	}

	m_dldr->ApplyProperties (&m_file, dnp);
	m_file.FtpSetTransferType (FTT_BINARY);
	
	LPCTSTR pszHost = m_bOpened ? NULL : dnp->pszServerName;
	if (dnp->enProtocol != NP_FTP)
		pszHost = dnp->pszServerName;

	ir = m_file.Open (fsNPToScheme (dnp->enProtocol), pszHost,
				dnp->pszUserName, dnp->pszPassword, dnp->uServerPort, 
				dnp->pszPathName, uStartPos);

	if (m_bNeedStop)
		return IR_S_FALSE;

	if (ir != IR_SUCCESS)
	{
		if (ir == IR_NEEDREDIRECT)
		{
			
			
			
			if (m_dldr->GetNumberOfSections ())
				return IR_SERVERUNKERROR;

			LPCTSTR pszUrlTo = m_file.GetLastError ();

			fsURL url;
			LPTSTR pszUrl = new TCHAR [10000];
			DWORD dwLen = 10000/sizeof(TCHAR);

			if (url.Crack (pszUrlTo) != IR_SUCCESS) 
			{
				LPTSTR pszUrlPath = new TCHAR [10000];

				if (*pszUrlTo == 0)
					_tcscpy (pszUrlPath, _T("/"));
				else if (*pszUrlTo != _T('/') && *pszUrlTo != _T('\\'))
				{
					fsPathFromUrlPath (dnp->pszPathName, dnp->enProtocol == NP_FTP, FALSE, pszUrlPath, 10000);

					if (pszUrlPath [lstrlen (pszUrlPath)-1] != _T('\\') &&
							pszUrlPath [lstrlen (pszUrlPath)-1] != _T('/'))
						lstrcat (pszUrlPath, _T("\\"));
					
					_tcscat (pszUrlPath, pszUrlTo);
				}
				else
					_tcscat (pszUrlPath, pszUrlTo);

				url.Create (fsNPToScheme (dnp->enProtocol), dnp->pszServerName, dnp->uServerPort, 
					dnp->pszUserName, dnp->pszPassword, pszUrlPath, pszUrl, &dwLen);

				delete [] pszUrlPath;
			}
			else
				_tcscpy (pszUrl, pszUrlTo);	

			LPTSTR pszUser = new TCHAR [10000], pszPassword = new TCHAR [10000];	
			if (dnp->pszUserName)
				_tcscpy (pszUser, dnp->pszUserName);
			else
				*pszUser = 0;

			if (dnp->pszPassword)
				_tcscpy (pszPassword, dnp->pszPassword);
			else
				*pszPassword = 0;

			
			ir = fsDNP_ApplyUrl (dnp, pszUrl);
			m_dldr->setDirty();

			delete [] pszUrl;

			if (ir != IR_SUCCESS)
				return ir;

			if (dnp->pszUserName == NULL || *dnp->pszUserName == 0)
			{
				SAFE_DELETE_ARRAY (dnp->pszUserName);
				dnp->pszUserName = new TCHAR [_tcslen (pszUser) + 1];
				_tcscpy (dnp->pszUserName, pszUser);

				SAFE_DELETE_ARRAY (dnp->pszPassword);
				dnp->pszPassword = new TCHAR [_tcslen (pszPassword) + 1];
				_tcscpy (dnp->pszPassword, pszPassword);
				m_dldr->setDirty();
			}

			delete [] pszUser; delete [] pszPassword;

			if (m_bNeedStop == FALSE)
				return Open (m_dldr, uStartPos);
			else
				return IR_S_FALSE;
		}

		return ir;
	}

	m_uCurPos = uStartPos;
	m_bOpened = TRUE;
	return IR_SUCCESS;
}

fsInternetResult fsArchiveInternetStream::GetLastNetworkErr()
{
	return m_irLastErr;
}

void fsArchiveInternetStream::Set_MaxRetriesCount(UINT cMax)
{
	m_cMaxRetries = cMax;
}
