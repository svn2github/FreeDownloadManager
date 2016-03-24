/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../../archive/vmsZip.h"
#include "../../inet/wininet/vmsPostRequest.h"
class vmsCrashReporter
{
public:
	vmsCrashReporter(void) {}
	virtual ~vmsCrashReporter(void) {}

	bool SubmitDumpToServer(LPCTSTR ptszServerAddr, LPCTSTR ptszScriptPath, LPCTSTR ptszDumpFileName, LPCSTR pszXml)
	{
		vmsZip zip;

		TCHAR tszTmpFile[MAX_PATH] = _T ("");
		TCHAR tszTmpPath[MAX_PATH] = _T ("");
		GetTempPath (MAX_PATH, tszTmpPath);
		GetTempFileName (tszTmpPath, _T ("tmp"), 0, tszTmpFile);

		if (FALSE == zip.CreateNewZipFile (tszTmpFile))
		{
			DeleteFile (tszTmpFile);
			return false;
		}

		if (!zip.AddFile (ptszDumpFileName, _T ("dump.dmp")) ||
			!zip.AddBuffer (pszXml, (DWORD)strlen (pszXml), _T ("details.xml")))
		{
			zip.CloseZipFile ();
			DeleteFile (tszTmpFile);
			return false;
		}

		zip.CloseZipFile ();

		HANDLE hZip = CreateFile (tszTmpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hZip == INVALID_HANDLE_VALUE)
			return FALSE;
		DWORD dwZipSize = GetFileSize (hZip, NULL);
		std::vector <BYTE> vbZip (dwZipSize);
		ReadFile (hZip, &vbZip[0], dwZipSize, &dwZipSize, NULL);
		assert (vbZip.size () == dwZipSize);
		if (vbZip.size () != dwZipSize)
			return FALSE;
		CloseHandle (hZip);
		DeleteFile (tszTmpFile);

		vmsPostRequest req;

		onInitializePostRequest (req);

		req.AddPart ("zipfile", "report.zip", &vbZip[0], dwZipSize);
		req.AddPart ("Submit", NULL, "Submit", 6);

		return req.SendMultipart (ptszServerAddr, ptszScriptPath) != FALSE;
	}

protected:
	static void GenerateXml (LPCTSTR ptszAppName, LPCTSTR ptszAppComponentName, 
		LPCTSTR ptszVersion, LPCTSTR ptszDescription, 
		LPCTSTR ptszFaultModule, DWORD dwGettingFaultModuleNameError, DWORD_PTR dwpCrashAddr, 
		LPCSTR pszAdditionalXmlData, std::string& strResult)
	{
		strResult = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";

		strResult += "<CrashDumpInfo>\r\n";

		strResult += "<Application Name=\""; strResult += vmsXmlUtil::toUtf8 (ptszAppName);
		strResult += "\" Component=\""; strResult += vmsXmlUtil::toUtf8 (ptszAppComponentName);
		strResult += "\" Version=\""; strResult += vmsXmlUtil::toUtf8 (ptszVersion);
		strResult += "\" Platform=\"";
#ifdef _WIN64
		strResult += "x64";
#else
		strResult += "x32";
#endif
		strResult += "\" />\r\n";

		strResult += "<Description>"; strResult += vmsXmlUtil::toUtf8 (ptszDescription);
		strResult += "</Description>\r\n";

		if (ptszFaultModule)
		{
			strResult += "<FaultModule Name=\""; strResult += vmsXmlUtil::toUtf8 (ptszFaultModule);
			strResult += "\" Address=\"0x";
			char sz[100] = "";
			sprintf_s <100> (sz, "%I64x", (UINT64)dwpCrashAddr);
			strResult += sz;
			if (dwGettingFaultModuleNameError && *ptszFaultModule == 0)
			{
				strResult += "\" GettingNameError=\"";
				strResult += _itoa (dwGettingFaultModuleNameError, sz, 10);
			}
			strResult += "\" />\r\n";
		}

		if (pszAdditionalXmlData && *pszAdditionalXmlData)
			strResult += pszAdditionalXmlData;

		strResult += "</CrashDumpInfo>\r\n";
	}

	virtual void onInitializePostRequest (vmsPostRequest& request) {}
};

