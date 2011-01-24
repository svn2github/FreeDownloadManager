/*************************************************************************

	Class fsInternetFile.

	Working with files on server (query size, download, ...). 
	Base abstract class for classes, working with files on HTTP and FTP 
	servers.

*************************************************************************/

#if !defined(AFX_FSINTERNETFILE_H__3693BB45_3388_4234_821D_FE3961082A7C__INCLUDED_)
#define AFX_FSINTERNETFILE_H__3693BB45_3388_4234_821D_FE3961082A7C__INCLUDED_

#include "fsinet.h"	// Added by ClassView
#include <limits.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <wininet.h>
#include "fsServerConnection.h"
#include <fsString.h>
#include "fsSpeaking.h"

// is resume feature supported by the remote server?
enum fsResumeSupportType
{
	RST_PRESENT,	// yes
	RST_NONE,		// no
	RST_UNKNOWN,	// it's unknown (server has not reported about this)
};

class fsInternetFile : public fsSpeaking
{
public:
	// get Wininet handle of file
	HINTERNET GetRawHandle();
	// Query file size from server.
	// pszFilePath - file name with path.
	virtual fsInternetResult QuerySize(LPCSTR pszFilePath);
	// Open file.
	// pszFilePath - file name
	// dwStartPos - start position in a file,
	// used for download resuming
	virtual fsInternetResult Open (LPCSTR pszFilePath, UINT64 uStartPos) = NULL;
	virtual fsInternetResult OpenEx (LPCSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize, UINT64 uUploadTotalSize) = NULL;
	// Read file in a buffer
	// pBuffer - pointer to a buffer in memory
	// dwSize - how much bytes need to read
	// pdwRead, [out] - how much bytes was read
	virtual fsInternetResult Read (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwRead) = NULL;
	virtual fsInternetResult Write (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten) = NULL;
	// Get real file name. HTTP only.
	// I.e. file address can be defined by script (ex. http://site.com/dl.pl?file), 
	// then a special field exits in HTTP header, allowing to notify
	// client about real file name
	LPCSTR GetSuggestedFileName ();
	// Resuming is supported ?
	fsResumeSupportType IsResumeSupported ();
	// Get type of file contents (HTTP only) in pszType variable, [out].
	// Returns FALSE, if a type is unknown.
	BOOL GetContentType (LPSTR pszType);
	// File last change date.
	// pTime, [out] - pointer to FILETIME variable to store date in.
	// Returns FALSE when a date is unknown
	BOOL GetLastModifiedDate (FILETIME* pTime);
	// Close file
	virtual void CloseHandle();
	// Get last server reply
	LPCSTR GetLastError();
	// Get file size
	virtual UINT64 GetFileSize () {return m_uFileSize;};
	// Init object.
	// pServer - pointer to connecting with server object
	virtual fsInternetResult Initialize (fsInternetServerConnection *pServer);
	fsInternetFile();
	virtual ~fsInternetFile();

protected:
	fsResumeSupportType m_enRST;		// resuming is supported ?
	LPSTR m_pszLastError;		// last server reply
	fsString m_strContentType;	// type of file content (HTTP)
	BOOL m_bContentTypeValid;	// content type is unknown ?
	FILETIME m_date;			// file last change date
	BOOL m_bDateValid;			// date is known ?
	UINT64 m_uFileSize;			// file size
	HINTERNET m_hFile;			// file handle
	fsInternetServerConnection *m_pServer;	// pointer to connecting with server object
	fsString m_strSuggFileName;	// real file name. == "", if a server wasn't "say" it.
};

#endif // !defined(AFX_FSINTERNETFILE_H__3693BB45_3388_4234_821D_FE3961082A7C__INCLUDED_)
