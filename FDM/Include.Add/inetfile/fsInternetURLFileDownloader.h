/*********************************************************************************

  Class fsInternetURLFileDownloader.

  Simple class for downloading small files into the memory buffer.

*********************************************************************************/

#if !defined(AFX_FSINTERNETURLFILEDOWNLOADER_H__2BB778E5_7CEF_48DC_93FA_6AC802EA0222__INCLUDED_)
#define AFX_FSINTERNETURLFILEDOWNLOADER_H__2BB778E5_7CEF_48DC_93FA_6AC802EA0222__INCLUDED_

#include "fsInternetURLFile.h"	// Added by ClassView
#include "fsinet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum fsInternetURLFileDownloaderEvent
{
	UFDE_CONNECTING,
	UFDE_DOWNLOADING,
	UFDE_DONE
};

// prototype of function that may receive events from object of this class
typedef void (*fntInternetURLFileDownloaderEvents)(fsInternetURLFileDownloaderEvent ev, LPVOID lp);

class fsInternetURLFileDownloader  
{
public:
	UINT Get_FileSize();
	void SetEventFunc (fntInternetURLFileDownloaderEvents pfn, LPVOID lpParam);
	void Free_FileBuffer();
	LPBYTE Get_FileBuffer();
        // get internet file object we used to download file
	// use it to retreive the size of file and other info
	fsInternetURLFile* Get_File();
	void Abort();
	fsInternetResult Initialize(fsInternetSession *pSession);
	fsInternetResult Download (LPCSTR pszUrl);
	fsInternetURLFileDownloader();
	virtual ~fsInternetURLFileDownloader();

protected:
	void Event (fsInternetURLFileDownloaderEvent ev);
	LPBYTE m_pBuffer;
	UINT m_uFileSize;
	BOOL m_bAbort;
	fsInternetURLFile m_file;

	fntInternetURLFileDownloaderEvents m_pfnEvents;
	LPVOID m_lpEvParam;
};

#endif // !defined(AFX_FSINTERNETURLFILEDOWNLOADER_H__2BB778E5_7CEF_48DC_93FA_6AC802EA0222__INCLUDED_)
