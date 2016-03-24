/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "system.h"
#include "orbit_download.h"
#include "CsvParser.h"

TOrbitDownload::TOrbitDownload()
	: bIsComplete(false),
	  uCompletedSize(0),
	  uFileSize(0)
{
}

void parseOrbitDownloadsList(const CStringA& sDldList, OrbitDownloadsArray& arrDownloads)
{
	CString sMsg;
	if (sDldList.IsEmpty())
		return;

	USES_CONVERSION;

	CCsvParser cpParser;
	cpParser.Init(sDldList, "\"", ",");

	CString sContent;
	while (cpParser.ParseNextRecord()) {

		int nFieldNumber = -1;
		CStringA sValue;
		TOrbitDownload tOrbitDownload;
		while (cpParser.GetNextField(sValue)) {
			++nFieldNumber;
			if (nFieldNumber == 0) {
				tOrbitDownload.sPath = CA2T((LPCSTR)sValue);
			}
			if (nFieldNumber == 1) {
				tOrbitDownload.sFile = CA2T((LPCSTR)sValue);
			}
			if (nFieldNumber == 2) {
				sValue.Trim();
				UINT64 uFileSize;
				if (sscanf_s((const char*)sValue, "%I64u", &uFileSize) != 1) {
					sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
					std::string _sMsg = CT2A((LPCTSTR)sMsg);
					throw std::runtime_error(_sMsg.c_str());
				}

				char szCompletedSize[16] = {0,};
				sprintf_s(szCompletedSize, 16, "%I64u", uFileSize);
				if (sValue.CompareNoCase(szCompletedSize) != 0) {
					sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
					std::string _sMsg = CT2A((LPCTSTR)sMsg);
					throw std::runtime_error(_sMsg.c_str());
				}

				tOrbitDownload.uFileSize = uFileSize;

			}
			if (nFieldNumber == 4) {
				tOrbitDownload.sUrl = sValue;
			}
			if (nFieldNumber == 10) {
				sValue.Trim();
				if (!sValue.IsEmpty())
					tOrbitDownload.bIsComplete = true;
			}

			
		}

		if (nFieldNumber < 10) {
			sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
			std::string _sMsg = CT2A((LPCTSTR)sMsg);
			throw std::runtime_error(_sMsg.c_str());
		}

		if (tOrbitDownload.bIsComplete)
			arrDownloads.Add(tOrbitDownload);

	}

	
	int nFieldNumber = -1;
	CStringA sValue;
	TOrbitDownload tOrbitDownload;
	while (cpParser.GetNextField(sValue)) {
		++nFieldNumber;
		if (nFieldNumber == 0) {
			tOrbitDownload.sPath = CA2T((LPCSTR)sValue);
		}
		if (nFieldNumber == 1) {
			tOrbitDownload.sFile = CA2T((LPCSTR)sValue);
		}
		if (nFieldNumber == 2) {
			sValue.Trim();
			UINT64 uFileSize;
			if (sscanf_s((const char*)sValue, "%I64u", &uFileSize) != 1) {
				sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
				std::string _sMsg = CT2A((LPCTSTR)sMsg);
				throw std::runtime_error(_sMsg.c_str());
			}

			char szCompletedSize[16] = {0,};
			sprintf_s(szCompletedSize, 16, "%I64u", uFileSize);
			if (sValue.CompareNoCase(szCompletedSize) != 0) {
				sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
				std::string _sMsg = CT2A((LPCTSTR)sMsg);
				throw std::runtime_error(_sMsg.c_str());
			}

			tOrbitDownload.uFileSize = uFileSize;

		}
		if (nFieldNumber == 4) {
			tOrbitDownload.sUrl = sValue;
		}

		if (nFieldNumber == 10) {
			sValue.Trim();
			if (!sValue.IsEmpty())
				tOrbitDownload.bIsComplete = true;
		}

		
	}

	if (nFieldNumber < 10) {
		sMsg = LS (L_CANT_PARSE_ORBIT_DOWNLOAD_LIST);
		std::string _sMsg = CT2A((LPCTSTR)sMsg);
		throw std::runtime_error(_sMsg.c_str());
	}

	if (tOrbitDownload.bIsComplete)
		arrDownloads.Add(tOrbitDownload);
}
