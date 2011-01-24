/*******************************************************************************

	Class fsHttpFile.

	Working with files on FTP server.

*******************************************************************************/

#if !defined(AFX_FSFTPFILE_H__12AF861F_17DB_4F84_8679_7B8EBDCD4D75__INCLUDED_)
#define AFX_FSFTPFILE_H__12AF861F_17DB_4F84_8679_7B8EBDCD4D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>
#include "fsInternetFile.h"
#include "fsinet.h"	// Added by ClassView

// file transmission type
enum fsFtpTransferType
{
	FTT_BINARY,		// binary
	FTT_ASCII,		// text
	FTT_UNKNOWN		// unknown
};

class fsFtpFile : public fsInternetFile  
{
public:
	// write to file (in upload mode)
	fsInternetResult Write (LPBYTE pBuffer, DWORD dwToWrite, DWORD *pdwWritten);
	// if uUploadFileSize != _UI64_MAX,  uStartPos indicates whether to use append mode or not.
	//  uStartPos != 0 - use append mode (if file exists on a server), 
	//  else - upload from the beginning
	fsInternetResult OpenEx (LPCSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize = _UI64_MAX, UINT64 uUploadTotalSize = _UI64_MAX);
	fsFtpTransferType GetTransferType();
	void SetDontUseLIST (BOOL b);
	// ask for a file size (details in fsInternetFile.h)
	virtual fsInternetResult QuerySize (LPCSTR pszFilePath);
	// close file
	virtual void CloseHandle();
	// set file transmission type. (see fsFtpTransferType above)
	fsInternetResult SetTransferType (fsFtpTransferType enType);
	// Read file (see fsInternetFile file)
	fsInternetResult Read (LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead);
	// Open file (see fsInternetFile file)
	fsInternetResult Open (LPCSTR pszFilePath, UINT64 uStartPos);
	fsFtpFile();
	virtual ~fsFtpFile();

protected:
	static BOOL FtpCommand (HINTERNET hConnect, BOOL fExpectResponse, DWORD dwFlags, LPCSTR pszCommand, DWORD_PTR dwContext, HINTERNET* phFtpCommand);
	BOOL Send_LIST (LPSTR pszCmd, LPCSTR pszFile);
	BOOL m_bDontUseLIST;
	// c - digit? (truly, unnecessary function here)
	BOOL IsDigit (char c);
	// connect with a server on a port, received in result of PASV command.
	// calling from Open () function if passive mode is on
	fsInternetResult PASV_ConnectSocket();
	// get file size from a server response on SIZE command
	BOOL ParseSIZE();
	// returns FTP server last error
	fsInternetResult FtpError ();
	// opens a socket and wait for a server response.
	// calling from Open () function in cases then passive mode is off
	// returns number of connected port, 0 for error
	SHORT OpenSocket();

	fsString m_strPORT;	// address+port for a PORT command to a server. Forming in OpenSocket () func
	SOCKET m_sFile;		// date-connection socket
	SOCKET m_sRcv;		// socket opening on waiting for a connection by OpenSocket () functions
	void ReceiveExtError ();	// get extended error from a server
	DWORD m_dwTransferType;	// file transmission type
};

#endif // !defined(AFX_FSFTPFILE_H__12AF861F_17DB_4F84_8679_7B8EBDCD4D75__INCLUDED_)
