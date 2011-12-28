/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "Utils.h"
#include "MemHandleGuard.h"

bool ComErrToSysErrCode(HRESULT hr, DWORD& dwErr)
{
	if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) {
		
		dwErr = HRESULT_CODE(hr);
		return true;
	}

	if (hr == S_OK) {
		dwErr = HRESULT_CODE(hr);
		return true;
	}

	
	return false;

}

void FormatMessageByErrCode(CString& sMsg, DWORD dwErr, bool& bFailedToRetreive)
{
	LPVOID lpMsgBuf = 0;
	DWORD dwFlag = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS;

	CMemHandleGuard mhgMemGuard(lpMsgBuf, bFailedToRetreive);

	if(::FormatMessage(dwFlag, NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 0, NULL) == 0) {
	
			bFailedToRetreive = true;
			return;
	}

	sMsg = (LPCTSTR)lpMsgBuf;
}

void appendDiagnostics(CString&sMsg, const CString& sDiagnostics)
{
	CString sTmp;
	CString sFmt = LS (L_SYS_DIAGNOSTICS_FMT);
	sTmp.Format((LPCTSTR)sFmt, (LPCTSTR)sDiagnostics);
	sMsg += " ";
	sMsg += sTmp;
}
