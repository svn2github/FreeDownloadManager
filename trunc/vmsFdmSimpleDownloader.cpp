/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsFdmSimpleDownloader.h"
#include "fsInternetDownloader.h"
#include "vmsInternetSession.h"
#include "inetutil.h"

vmsFdmSimpleDownloader::vmsFdmSimpleDownloader(void)
{
	ZeroMemory (&m_dp, sizeof (m_dp));
	m_dp.wStructSize = sizeof (m_dp);
	fsDP_BuffersInfo bi;
	fsDP_GetDefaults (&m_dp, &bi, TRUE);

	ZeroMemory (&m_dnp, sizeof (fsDownload_NetworkProperties));
}

vmsFdmSimpleDownloader::~vmsFdmSimpleDownloader(void)
{
	fsDNP_GetByUrl_Free (&m_dnp);
	SAFE_DELETE_ARRAY (m_dp.pszFileName);
	SAFE_DELETE_ARRAY (m_dp.pszAdditionalExt);
	SAFE_DELETE_ARRAY (m_dp.pszCreateExt);
}

fsInternetResult vmsFdmSimpleDownloader::OpenFile (const fsDownload_Properties& dp, 
	fsDownload_NetworkProperties& dnp, UINT64 uStartPos, 
	std::unique_ptr <fsInternetURLFile> &fileResult)
{
	fsInternetResult ir;

	std::unique_ptr <fsInternetURLFile> file (new fsInternetURLFile ());
	std::unique_ptr <vmsInternetSession> pSession (new vmsInternetSession);

	TCHAR szProxy [10000];
	vmsMakeWinInetProxy (dnp.pszProxyName, dnp.enProtocol, dnp.enProtocol, szProxy);
	ir  = pSession->Create (dnp.pszAgent, dnp.enAccType, szProxy, dnp.enProtocol);
	if (ir != IR_SUCCESS)
		return ir;

	fsInternetDownloader::ApplyProxySettings (pSession.get (), &dnp, dp.uTimeout);

	file->Initialize (pSession.release (), TRUE);

	fsInternetDownloader::ApplyProperties (file.get (), &dnp);

	fsInternetSession::AdjustWinInetConnectionLimit ();

	try{
		ir = file->Open (fsNPToScheme (dnp.enProtocol), dnp.pszServerName,
			dnp.pszUserName, dnp.pszPassword, dnp.uServerPort, 
			dnp.pszPathName, uStartPos, dnp.dwFlags & DNPF_IMMEDIATELY_SEND_AUTH_AS_BASIC);
	}
	catch (...) {
		assert ("unexpected exception" &&0); 
		return IR_ERROR;
	}

	if (ir == IR_NEEDREDIRECT)
	{
		std::unique_ptr <fsDownload_NetworkProperties> upRedirectedDnp;
		ir = fsInternetDownloader::ApplyRedirectURL (file.get (), &dnp, upRedirectedDnp);
		if (ir != IR_SUCCESS)
			return ir;
		return OpenFile (dp, upRedirectedDnp ? *upRedirectedDnp.get () : dnp, uStartPos, fileResult);
	}

	if (ir == IR_SUCCESS)
		fileResult.swap (file);

	return ir;
}

fsInternetResult vmsFdmSimpleDownloader::InitializeWithURL (const tstring& url,
	fsDownload_NetworkProperties& dnp)
{
	fsDNP_GetByUrl_Free (&dnp);

	CString strURL = url.c_str ();
	strURL.Replace (_T("&lt;"), _T("<"));
	strURL.Replace (_T("&gt;"), _T(">"));
	strURL.Replace (_T("&amp;"), _T("&"));
	strURL.Replace (_T("&quot;"), _T("\""));

	
	fsDNP_BuffersInfo buffs;
	return fsDNP_GetByUrl (&dnp, &buffs, TRUE, strURL);
}

fsInternetResult vmsFdmSimpleDownloader::Download (const tstring& url, std::ostream& os)
{
	fsInternetResult ir = InitializeWithURL (url, m_dnp);
	if (ir != IR_SUCCESS)
		return ir;

	std::unique_ptr <fsInternetURLFile> file;
	ir = OpenFile (m_dp, m_dnp, 0, file);
	if (ir != IR_SUCCESS)
		return ir;

	assert (file);

	char buff [16*1024];
	DWORD dwRead;
	while ((ir = file->Read ((BYTE*)buff, _countof (buff), &dwRead)) == IR_SUCCESS && dwRead)
		os.write (buff, dwRead);

	return ir != IR_S_FALSE ? ir : IR_SUCCESS;
}