/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "Utils.h"
#include "MemHandleGuard.h"

void UniToAnsi(const std::wstring& sSrc, std::string& sTar)
{

	int nLen = ::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), -1, 0, 0, 0, 0);
	std::auto_ptr<CHAR> pchStrGuard( new CHAR[nLen+1] );
	CHAR* szTar = pchStrGuard.get();
	if (szTar == 0) {
		throw std::exception("Out of memory");
	}
	memset(szTar, 0, sizeof(CHAR) * (nLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), sSrc.length(), szTar, sSrc.length(), 0, 0);
	sTar = szTar;

}

void AnsiToUni(const std::string& sSrc, std::wstring& sTar)
{

	int nLen = ::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), -1, 0, 0);
	std::auto_ptr<WCHAR> pwchTarGuard( new WCHAR[nLen+1] );
	WCHAR* wszTar = pwchTarGuard.get();
	if (wszTar == 0) {
		throw std::exception("Out of memory");
	}
	memset(wszTar, 0, sizeof(WCHAR) * (nLen + 1));
	::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), sSrc.length(), wszTar, sSrc.length());
	sTar = wszTar;

}

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

void CopyString(LPTSTR* tszDest, LPCSTR szSrc)
{
	ASSERT(tszDest != 0);

	if (tszDest == 0)
		throw std::exception("Unkown error");

	if (szSrc == 0)
		return;

#ifdef UNICODE
	int nLen = MultiByteToWideChar(CP_ACP, 0, szSrc, -1, 0, 0);
#else
	int nLen = strlen(szSrc);
#endif

	std::auto_ptr<TCHAR> aptszDestGuard( new TCHAR[nLen+1] );
	TCHAR* _tszDest = aptszDestGuard.get();
	if (_tszDest == 0)
		throw std::exception("Out of memory");
	memset(_tszDest, 0, (nLen+1) * sizeof(TCHAR));

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, szSrc, -1, _tszDest, nLen);
#else
	strcpy(tszDest, szSrc);
#endif
	*tszDest = aptszDestGuard.release();
}

FILE* FopenForRead(const tstring& sStr)
{
	FILE* pfFile = _wfopen(sStr.c_str(), _T("r"));
	if (pfFile == 0)
		throw std::exception("Cannot open a file.");

	return pfFile;
}

void Fclose(FILE* pfFile)
{
	if (pfFile != 0) {
		fclose(pfFile);
	}
}

bool Fgets(FILE* pfFile, std::string& sStr)
{
	sStr = "";

	while (!feof(pfFile)) {

		char c = (char)fgetc(pfFile);

		if (ferror(pfFile))
			throw std::exception("Cannot read a file");

		if (c == '\r') {
			continue;
		}

		if (c == '\n') {
			break;
		}

		sStr += c;
	}

	if (feof(pfFile))
		return false;
	else
		return true;
}

void putStrToBuffer(LPCTSTR tszStr, LPBYTE& pbtCurrentPos, LPBYTE pbtBuffer, DWORD dwBufferSizeSize, DWORD* pdwSizeReuiqred)
{

	int nLen = -1;
	int nBufferSize = 0;
	if (tszStr) {
		nLen = _tcslen(tszStr);
		nBufferSize = sizeof(TCHAR) * nLen;
	}

	if (pbtBuffer == NULL) {
		if (pdwSizeReuiqred) {
			*pdwSizeReuiqred += sizeof(nLen) + nBufferSize;
		}
		return;
	}

	CHECK_BUFFER_BOUNDS(dwBufferSizeSize, sizeof(nLen));
	CopyMemory(pbtCurrentPos, &nLen, sizeof(nLen));
	pbtCurrentPos += sizeof(nLen);

	if (tszStr) {
		CHECK_BUFFER_BOUNDS(dwBufferSizeSize, nBufferSize);
		CopyMemory(pbtCurrentPos, tszStr, nBufferSize);
		pbtCurrentPos += nBufferSize;
	}
}

bool getStrFromBuffer_ANSI_to_UNICODE(LPTSTR* ptszStr, LPBYTE& pbtCurrentPos, LPBYTE pbtBuffer, DWORD dwBufferSizeSize)
{
	USES_CONVERSION;
	LPSTR pszANSI = nullptr;
	if (!getStrFromBuffer (&pszANSI, pbtCurrentPos, pbtBuffer, dwBufferSizeSize))
		return false;
	if (!pszANSI)
	{
		*ptszStr = nullptr;
		return true;
	}
	tstring unicode = CA2CT (pszANSI);
	delete [] pszANSI;
	*ptszStr = new TCHAR [unicode.length () + 1];
	_tcscpy (*ptszStr, unicode.c_str ());
	return true;
}

std::string fromUnicode(const std::wstring &wstr, UINT codePage)
{
    if( wstr.empty() ) return std::string();
    int size_needed = WideCharToMultiByte(codePage, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (codePage, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring toUnicode(const std::string &str, UINT codePage)
{
    if( str.empty() ) return std::wstring();
    int size_needed = MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar                  (codePage, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string loadStringFromBufferA( LPBYTE& pB, DWORD dw ){
	if ( dw == 0 )
		return std::string();
	LPSTR szStr;
	fsnew (szStr, CHAR, dw+1);
	CopyMemory (szStr, pB, dw * sizeof(CHAR));	
	szStr[dw] = 0;				
	pB += dw * sizeof(CHAR);
	std::string res = szStr;
	delete szStr;
	return res;
}

std::wstring loadStringFromBufferW( LPBYTE& pB, DWORD dw ){
	if ( dw == 0 )
		return std::wstring();
	LPWSTR szStr;
	fsnew (szStr, WCHAR, dw+1);
	CopyMemory (szStr, pB, dw * sizeof(WCHAR));	
	szStr[dw] = 0;				
	pB += dw * sizeof(WCHAR);
	std::wstring res = szStr;
	delete szStr;
	return res;
}

tstring loadStringFromBuffer( LPBYTE& pB, DWORD dw ){
#ifdef UNICODE
	return loadStringFromBufferW (pB, dw);
#else
	return loadStringFromBufferA (pB, dw);
#endif
}

