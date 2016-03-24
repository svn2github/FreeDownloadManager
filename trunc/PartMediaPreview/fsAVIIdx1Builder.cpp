/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#define WINVER 0x501
#define _WIN32_IE 0x501
#define _WIN32_WINNT 0x501

#include <memory>
#include "Utils.h"
#include "fsAVIIdx1Builder.h"
#include <fsString.h>
#include "../system.h"

fsAVIIdx1Builder::fsAVIIdx1Builder()
{

}

fsAVIIdx1Builder::~fsAVIIdx1Builder()
{

}

inline BOOL Read (HANDLE hFile, LPVOID pBuf, DWORD dwSize)
{
	return ReadFile (hFile, pBuf, dwSize, &dwSize, NULL);
}

inline BOOL Read (HANDLE hFile, LPTSTR pStr, DWORD dwSize)
{
	pStr [dwSize] = 0;

	std::auto_ptr<char> apchStrGuard( new char[dwSize * sizeof(TCHAR)] );
	char* szStr = apchStrGuard.get();
	if (szStr == 0) 
		return  FALSE;
	memset(szStr, 0, dwSize);

	if (!ReadFile (hFile, szStr, dwSize, &dwSize, NULL))
		return FALSE;

#ifdef UNICODE
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, szStr, -1, 0, 0);
	if (nLen + 1 > dwSize)
		return FALSE;
	::MultiByteToWideChar(CP_ACP, 0, szStr, -1, pStr, nLen);
#else
	strcpy_s(pStr, dwSize, szStr);
#endif

}

inline BOOL Write (HANDLE hFile, LPVOID pBuf, DWORD dwSize)
{
	return WriteFile (hFile, pBuf, dwSize, &dwSize, NULL);
}

inline void Seek (HANDLE hFile, DWORD dw)
{
	SetFilePointer (hFile, dw, NULL, FILE_BEGIN);
}

inline DWORD GetFilePos (HANDLE hFile)
{
	return SetFilePointer (hFile, 0, NULL, FILE_CURRENT);
}

inline DWORD GetFileSize (HANDLE hFile)
{
	return ::GetFileSize (hFile, NULL);
}

typedef struct {
    DWORD  ckid;
    DWORD  dwFlags;
    DWORD  dwChunkOffset;
    DWORD  dwChunkLength;
} AVIINDEXENTRY;

BOOL fsAVIIdx1Builder::BuildIdx1(HANDLE in, HANDLE out, UINT64 uInMaxPos)
{
	fsString strChunk;
	DWORD dw = 0;

	if (uInMaxPos == _UI64_MAX)
	{
		DWORD dw1, dw2;
		dw1 = GetFileSize (in, &dw2);
		uInMaxPos = UINT64 (dw1) | UINT64 (dw2) << 32;
	}

	strChunk.alloc (4);
	
	UINT64 uPos = 16;
	fsSetFilePointer (in, uPos, FILE_BEGIN);

	
	Read (in, &dw, 4);
	uPos += 4+dw;

	fsSetFilePointer (in, uPos, FILE_BEGIN);

	do
	{
		
		Read (in, strChunk.pszString, 4);
		Read (in, &dw, 4);
		uPos += 8;

		if (strChunk == _T("JUNK") || strChunk == _T("JUNQ"))
		{
			uPos += dw;
			fsSetFilePointer (in, uPos, FILE_BEGIN);
			continue;
		}

		if (strChunk != _T("LIST"))
			return FALSE; 

		m_uIdx1Start = uPos + dw;

		Read (in, strChunk, 4);
		uPos += 4;

		if (strChunk != _T("movi"))
		{
			uPos += dw-4;
			fsSetFilePointer (in, uPos, FILE_BEGIN);
			continue;
		}
	}
	while (strChunk != _T("movi"));

	Write (out, _T("idx1"), 4);
	Write (out, &dw, 4); 

	DWORD dwOffset = 4;

	do
	{
		for (int i = 0; i < 16; i++)
		{
			Read (in, strChunk.pszString, 4);
			if (_tcsicmp (strChunk+2, _T("wb")) == 0 ||
					_tcsicmp (strChunk+2, _T("dc")) == 0 ||
					_tcsicmp (strChunk+2, _T("db")) == 0)
				break;

			fsSetFilePointer (in, ++uPos, FILE_BEGIN);	
			dwOffset++;
		}

		if (_tcsicmp (strChunk+2, _T("wb")) &&
				_tcsicmp (strChunk+2, _T("dc")) &&
				_tcsicmp (strChunk+2, _T("db")))
			return FALSE;

		Read (in, &dw, 4);
		uPos += 8;

		if (uPos + dw > uInMaxPos)
			break;

		char* szChunk = 0;
#ifdef UNICODE
		int nLen = ::WideCharToMultiByte(CP_ACP, 0, strChunk.pszString, -1, 0, 0, 0, 0);
		std::auto_ptr<char> apszChunkGuard( new char[nLen + 1] );
		szChunk = apszChunkGuard.get();
		if (szChunk == 0)
			return FALSE;
		memset(szChunk, 0, nLen + 1);
		::WideCharToMultiByte(CP_ACP, 0, strChunk.pszString, -1, szChunk, nLen, 0, 0);
#else
		szChunk = strChunk.pszString;
#endif

		AVIINDEXENTRY e;
		e.ckid = *((LPDWORD)(LPSTR)szChunk);
		e.dwChunkLength = dw;
		e.dwChunkOffset = dwOffset;
		e.dwFlags = 0;
		if (strChunk == _T("LIST"))
			e.dwFlags |= 0x1; 

		Write (out, &e, sizeof (e));

		uPos += dw;
		dwOffset += 8 + dw;
		fsSetFilePointer (in, uPos, FILE_BEGIN);
	}
	while (uPos < m_uIdx1Start);

	dw = GetFileSize (out) - 8;
	Seek (out, 4);
	Write (out, &dw, 4);

	return TRUE;
}

UINT64 fsAVIIdx1Builder::Get_Idx1StartPosition()
{
	return m_uIdx1Start;
}
