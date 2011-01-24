/********************************************************************************

	Class fsHttpFiles.

	Used to receive file list on HTTP server.

********************************************************************************/

#if !defined(AFX_FSHTTPFILES_H__50AB10C6_642A_4EEE_96AE_6E16668CF6F0__INCLUDED_)
#define AFX_FSHTTPFILES_H__50AB10C6_642A_4EEE_96AE_6E16668CF6F0__INCLUDED_

#include "fsHttpConnection.h"	// Added by ClassView
#include "fsinet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsInternetFiles.h"
#include "fsHttpFile.h"	// Added by ClassView

// Describe list receiving process from a server.
// Using in CALLBACK procedure to notify a class user about progress
// in list building
enum fsHttpFilesEvent
{
	HFE_CONNECTED,			// connected with server
	HFE_FILELISTREAD,		// list readed
	HFE_STARTBUILDLIST,		// list building (get size, change date for each file)
	HFE_FINISHBUILDLIST		// building complete
};
// CALLBACK procedure
typedef void (*fntHttpFilesEventFunc)(class fsHttpFiles* pFiles, fsHttpFilesEvent enEvent, LPVOID lpParam);

class fsHttpFiles : public fsInternetFiles
{
public:
	// Described in fsInternetFiles.h
	// pszPath - path to HTML file
	fsInternetResult GetList (LPCSTR pszPath);
	void Abort();

	// Set CALLBACK procedure
	// lpParam - additional parameter, defined by application.
	// Will send to CALLBACK procedure
	void SetEventFunc (fntHttpFilesEventFunc pfn, LPVOID lpParam);
	// Use HTTPS?
	void UseSecure (BOOL bUse);
	LPCSTR GetLastError();
	// Set server we'll work with
	void SetServer (fsHttpConnection* pServer);
	// Request file info (size, date) while list building ?
	// Can slow down list building on slow connections.
	void RetreiveInfoWhileGettingList (BOOL b);
	fsHttpFiles();
	virtual ~fsHttpFiles();
protected:
	LPCSTR m_pszBaseURL;
	// notify class user about list building process
	void Event (fsHttpFilesEvent enEvent);
	// Form pInfo->strName field depending on file's url
	// pszSomeUrl - file's URL
	void CalcUrl (fsFileInfo* pInfo, LPCSTR pszSomeUrl);
	// Check this file is a directory. If so then set bFolder field into TRUE
	void CheckFolder (fsFileInfo* file);
	// Request additional file info
	// pszUrl - URL or file path
	// pInfo, [out] - where to write received information
	fsInternetResult GetUrlInfo (LPCSTR pszUrl, fsFileInfo *pInfo);
	// Build list by using HTML parser
	fsInternetResult BuildFileList();
	// Download HTML file
	fsInternetResult LoadFile();

	fntHttpFilesEventFunc m_pfnEvents;	// CALLBACK procedure
	LPVOID m_lpEventsParam;				// additional parameter
	BOOL m_bUseSecure;					// use HTTPS?
	fsString m_strFullPath;				// full URL to directory
	BOOL m_bAbort;						// abort list building ?
	BOOL m_bGetInfo;					// request info about files (size, date) while list building ?
	LPSTR m_pszFileBuffer;				// here is downloaded HTML file stored
	UINT64 m_uFileLen;					// HTML file size
	fsHttpFile m_httpFile;				// HTML file is downloading through it
	fsHttpConnection* m_pServer;		// current server

private:
	fsHttpConnection m_hUI_server;		// used for request
	fsHttpFile m_hUI_file;				// file information
};

#endif // !defined(AFX_FSHTTPFILES_H__50AB10C6_642A_4EEE_96AE_6E16668CF6F0__INCLUDED_)
