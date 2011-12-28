/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef UTILS_INCLUDED_FILE
#define UTILS_INCLUDED_FILE

bool ComErrToSysErrCode(HRESULT hr, DWORD& dwErr); 

void FormatMessageByErrCode(CString& sMsg, DWORD dwErr, bool& bFailedToRetreive);

void appendDiagnostics(CString&sMsg, const CString& sDiagnostics);

const int g_nGrowBy = 128;

template<class T>
void InitArray(CArray<T, T&>& arrDest)
{
	arrDest.SetSize(0, g_nGrowBy);
}

template<class T>
void InitArray(CArray<T*, T*>& arrDest)
{
	arrDest.SetSize(0, g_nGrowBy);
}

template<class T>
void CleanArray(CArray<T, T&>& arrDest)
{
	arrDest.RemoveAll();
	arrDest.FreeExtra();
}

template<class T>
void CleanArray(CArray<T*, T*>& arrDest)
{
	arrDest.RemoveAll();
	arrDest.FreeExtra();
}

template<class T>
void CopyArray(CArray<T, T&>& arrDest, CArray<T, T&>& arrSrc)
{
	CleanArray(arrDest);

	int i = 0;
	for (i = 0; i <= arrSrc.GetUpperBound(); i++) {
		T& tSrc = arrSrc.ElementAt(i);
		arrDest.Add(tSrc);
	}
}

template<class T>
void CopyArray(CArray<T, T&>& arrDest, const CArray<T, T&>& arrSrc)
{
	CleanArray(arrDest);

	int i = 0;
	for (i = 0; i <= arrSrc.GetUpperBound(); i++) {
		T tSrc = arrSrc.ElementAt(i);
		arrDest.Add(tSrc);
	}
}

#endif
