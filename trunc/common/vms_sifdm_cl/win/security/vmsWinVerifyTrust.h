/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include <softpub.h>
#include <wintrust.h>

#pragma comment (lib, "wintrust")
#pragma comment (lib, "crypt32.lib")

class vmsWinVerifyTrust
{
public:
	static bool VerifyFileSignature (const std::wstring& wstrFile)
	{
		WINTRUST_FILE_INFO wtfi = {0};
		wtfi.cbStruct = sizeof (wtfi);
		wtfi.pcwszFilePath = wstrFile.c_str ();

		WINTRUST_DATA wtd = {0};
		wtd.cbStruct = sizeof (wtd);
		wtd.dwUIChoice = WTD_UI_NONE;
		wtd.fdwRevocationChecks = WTD_REVOKE_WHOLECHAIN; 
		wtd.dwUnionChoice = WTD_CHOICE_FILE;
		wtd.pFile = &wtfi;

		GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;

		LONG lStatus = WinVerifyTrust (NULL, &WVTPolicyGUID, &wtd);

		return lStatus == ERROR_SUCCESS;
	}

	static bool GetSubjectName (const std::wstring& wstrFile, std::wstring &wstrSubjectName)
	{
		wstrSubjectName.clear ();

		DWORD dwEncoding, dwContentType, dwFormatType;
		HCERTSTORE hStore = NULL;
		HCRYPTMSG hMsg = NULL;
		PCCERT_CONTEXT pCertContext = NULL;
		std::wstring wstrSubjectNameTmp;
		CERT_INFO CertInfo = {0};
		std::vector <BYTE> vbSignerInfo;
		bool bResult = false;

		if (!CryptQueryObject (CERT_QUERY_OBJECT_FILE, wstrFile.c_str (), CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED, 
			CERT_QUERY_FORMAT_FLAG_BINARY, 0, &dwEncoding, &dwContentType, &dwFormatType, &hStore, &hMsg, NULL))
			goto _lExit;

		DWORD dwLen = 0;
		if (!CryptMsgGetParam (hMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL, &dwLen))
			goto _lExit;

		vbSignerInfo.resize (dwLen);
		if (!CryptMsgGetParam (hMsg, CMSG_SIGNER_INFO_PARAM, 0, &vbSignerInfo.front (), &dwLen))
			goto _lExit;

		PCMSG_SIGNER_INFO pSignerInfo = reinterpret_cast <PCMSG_SIGNER_INFO> (&vbSignerInfo.front ());

		CertInfo.Issuer = pSignerInfo->Issuer;
		CertInfo.SerialNumber = pSignerInfo->SerialNumber;

		pCertContext = CertFindCertificateInStore (hStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			0, CERT_FIND_SUBJECT_CERT, (void*)&CertInfo, NULL);
		if (!pCertContext)
			goto _lExit;

		dwLen = CertGetNameString (pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
		if (!dwLen)
			goto _lExit;

		wstrSubjectNameTmp.resize (dwLen);
		if (!CertGetNameString (pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, &wstrSubjectNameTmp.front (), dwLen))
			goto _lExit;
		wstrSubjectNameTmp.resize (dwLen - 1); 

		wstrSubjectName.swap (wstrSubjectNameTmp);
		bResult = true;

_lExit:
		if (pCertContext)
			CertFreeCertificateContext (pCertContext);
		if (hStore)
			CertCloseStore (hStore, 0);
		if (hMsg)
			CryptMsgClose (hMsg);

		return bResult;
	}
};
