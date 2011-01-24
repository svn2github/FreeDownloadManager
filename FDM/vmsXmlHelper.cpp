/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "fdmapp.h"
#include "vmsXmlHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsXmlHelper::vmsXmlHelper()
{

}

vmsXmlHelper::~vmsXmlHelper()
{

}

VARIANT_BOOL vmsXmlHelper::LoadXML(IXMLDOMDocument *pXML, LPCSTR pszXml)
{
	TCHAR tszTmpFile [MAX_PATH];
	TCHAR tszTmpPath [MAX_PATH];
	GetTempPath (MAX_PATH, tszTmpPath);
	GetTempFileName (tszTmpPath, _T ("tmp"), 0, tszTmpFile);
	TCHAR tszTmpFile1 [MAX_PATH];
	_tcscpy (tszTmpFile1, tszTmpFile);
	_tcscpy (tszTmpFile1+_tcslen (tszTmpFile1) - 3, _T ("xml"));
	DeleteFile (tszTmpFile);
	
	HANDLE hFile = CreateFile (tszTmpFile1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dw;
	WriteFile (hFile, pszXml, strlen (pszXml), &dw, NULL);
	CloseHandle (hFile);

	VARIANT_BOOL b;
	pXML->load (COleVariant (tszTmpFile1), &b);

	DeleteFile (tszTmpFile1);

	return b;
}

std::string vmsXmlHelper::toUtf8(const tstring &tstr, bool bRemoveBadChars)
{
	CString str = tstr.c_str ();

	if (bRemoveBadChars)
	{
		for (int i = 0; i < str.GetLength (); i++)
		{
			if (str [i] >= ' ')
				continue;
			char ch = str [i];
			if (ch == '\t' || ch == '\r' || ch == '\n')
				continue;
			str.Delete (i--);
		}
	}

	str.Replace (_T ("&"), _T ("&amp;"));
	str.Replace (_T ("<"), _T ("&lt;"));
	str.Replace (_T ("\""), _T ("&quot;"));
	
	wchar_t *pwsz = new wchar_t [str.GetLength () + 1];
	
	#if defined (UNICODE) || defined (_UNICODE)
		wcscpy (pwsz, str);
	#else
		MultiByteToWideChar (CP_ACP, 0, str, -1, pwsz, str.GetLength () + 1);
	#endif
	
	LPSTR psz = new char [2 * str.GetLength () + 1];
	WideCharToMultiByte (CP_UTF8, 0, pwsz, -1, psz, 2 * str.GetLength () + 1, NULL, NULL);
	
	std::string str2 = psz;
	
	delete [] pwsz;
	delete [] psz;
	
	return str2;
}

tstring vmsXmlHelper::FromUtf8(LPCSTR psz)
{
	wchar_t *pwsz = new wchar_t [strlen (psz) + 1];
	
	MultiByteToWideChar (CP_UTF8, 0, psz, -1, pwsz, strlen (psz) + 1);
	
	USES_CONVERSION;
	tstring tstr = W2T (pwsz);
	
	delete [] pwsz;
	
	return tstr;
}

std::string vmsXmlHelper::toUtf8noEncode(const tstring &tstr)
{
	wchar_t *pwsz = new wchar_t [tstr.length () + 1];
	
#if defined (UNICODE) || defined (_UNICODE)
	wcscpy (pwsz, tstr.c_str ());
#else
	MultiByteToWideChar (CP_ACP, 0, tstr.c_str (), -1, pwsz, tstr.length () + 1);
#endif
	
	LPSTR psz = new char [2 * tstr.length () + 1];
	WideCharToMultiByte (CP_UTF8, 0, pwsz, -1, psz, 2 * tstr.length () + 1, NULL, NULL);
	
	std::string str2 = psz;
	
	delete [] pwsz;
	delete [] psz;
	
	return str2;
}
