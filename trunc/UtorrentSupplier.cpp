/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "UtorrentSupplier.h"
#include "Utils.h"
#include "common/vmsFileUtil.h"
#include "vmsLogger.h"

CUtorrentSupplier::CUtorrentSupplier()
{
}

CUtorrentSupplier::~CUtorrentSupplier()
{
}

bool CUtorrentSupplier::CheckSupplier() const
{
	TCHAR szPath [MAX_PATH];
	return UtorrentInstalled(szPath);
}

void CUtorrentSupplier::Import(informer fnInformer, void* pData, TImportResult& tImportResult) const
{
	try 
	{
		TCHAR szPath [MAX_PATH];
		if(!UtorrentInstalled(szPath)) {
			CString sMsg = LS (L_UTORRENT_SAVEFOLDER_NOTFOUND);
			USES_CONVERSION;
			std::string _sMsg = CT2A(sMsg);
			throw std::runtime_error(_sMsg.c_str());
		}
		ImportUtorrentDownloads(fnInformer, pData, tImportResult, this, szPath);
	}
	catch (CException* pExc) 
	{
		vmsLogger::WriteLog("CUtorrentSupplier::Import exception");
		CString sMsg;

		TCHAR szMsg[1024] = {0,};
		if (!pExc->GetErrorMessage(szMsg, 1024)) {
			szMsg[1023] = 0;
			appendDiagnostics(sMsg, szMsg);
		} else {
			sMsg = "Unkown error";
		}
		pExc->Delete();

		USES_CONVERSION;
		std::string _sMsg = CT2A(sMsg);
		throw std::runtime_error(_sMsg);
	}
}

CString CUtorrentSupplier::GetSupplierName() const
{
	CString sSupplier = LS (L_UTORRENT_MANAGER);
	return sSupplier;
}

void CUtorrentSupplier::appendDiagnostics(CString&sMsg, const CString& sDiagnostics) const
{
	CString sTmp;
	CString sFmt = LS (L_SYS_DIAGNOSTICS_FMT);
	sTmp.Format((LPCTSTR)sFmt, (LPCTSTR)sDiagnostics);
	sMsg += " ";
	sMsg += sTmp;
}

void CUtorrentSupplier::appendErrorCode(CString& sMsg, HRESULT hr) const
{
	CString sTmp;
	CString sFmt = LS (L_ERROR_CODE_FMT);
	sTmp.Format((LPCTSTR)sFmt, hr, hr);
	sMsg += " ";
	sMsg += sTmp;
}

bool CUtorrentSupplier::UtorrentInstalled(LPTSTR szPath)
{
	vmsFDM::vmsFileUtil::GetAppDataPath(_T("uTorrent"), szPath);
	return vmsFDM::vmsFileUtil::DirectoryExists(szPath) != FALSE;
}

void CUtorrentSupplier::ImportUtorrentDownloads(informer fnInformer, void* pData, TImportResult& tImportResult, const CUtorrentSupplier* pUtorrentSupplier, LPTSTR szPath)
{
	CString uTorrentPath = szPath;
	vmsObjectSmartPtr <vmsUTorrentDownloadsDb> spUTorrentDownloads = _BT.CreateUTorrentDownloadsDbObject ();
	assert (spUTorrentDownloads != NULL);
	if (!spUTorrentDownloads)
		return;

	
	CString sFile = uTorrentPath + _T("resume.dat");
	spUTorrentDownloads->ImportUtorrentDownloads((LPCTSTR)sFile);

	tImportResult.nFailure = 0;
	tImportResult.nSuccess = 0;
	tImportResult.nTotal = spUTorrentDownloads->getDownloadCount ();
	tImportResult.nUnprocessed = tImportResult.nTotal;

	if (fnInformer != 0 && pUtorrentSupplier != 0)
		fnInformer(tImportResult.nTotal, tImportResult.nTotal - tImportResult.nUnprocessed, pUtorrentSupplier->GetSupplierName(), pData);
	
	UINT nSize = spUTorrentDownloads->getDownloadCount ();
	for (UINT i=0; i < nSize; i++) {

		CImportResultGuard resultGuard(tImportResult);

		const vmsUTorrentDownloadsDb::Download *dl = spUTorrentDownloads->getDownload (i);

		CString pszFile = uTorrentPath + dl->strTorrentFileName.c_str();
		CString pszTorrentUrl = "file://"; pszTorrentUrl += pszFile;
		CString outputFolder = dl->strOutputPath.c_str();
		_pwndDownloads->CreateBtDownload(pszFile, pszTorrentUrl, TRUE, FALSE, outputFolder);

		resultGuard.Success(true);

		if (fnInformer != 0 && pUtorrentSupplier != 0)
			fnInformer(tImportResult.nTotal, tImportResult.nTotal - tImportResult.nUnprocessed, pUtorrentSupplier->GetSupplierName(), pData);
	}
}
