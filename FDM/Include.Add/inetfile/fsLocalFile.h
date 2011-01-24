/**********************************************************************************

	Class fsLocalFile.

	Working with LAN files on Windows stations.

**********************************************************************************/

#if !defined(AFX_FSLOCALFILE_H__1DDB29EB_F880_4374_BF2A_7C3A1A12F898__INCLUDED_)
#define AFX_FSLOCALFILE_H__1DDB29EB_F880_4374_BF2A_7C3A1A12F898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsInternetFile.h"
#include "fsinet.h"	// Added by ClassView
#include "fsInternetSession.h"	// Added by ClassView

class fsLocalFile : public fsInternetFile  
{
public:
	fsInternetResult Write (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten);
	fsInternetResult OpenEx (LPCSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize = _UI64_MAX, UINT64 uUploadTotalSize = _UI64_MAX);
	// All methods are described in fsInternetFile.h
	virtual fsInternetResult QuerySize (LPCSTR pszFilePath);
	virtual void CloseHandle();
	void Initialize (LPCSTR pszServer);
	fsInternetResult Read (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwRead);
	fsInternetResult Open (LPCSTR pszFilePath, UINT64 uStartPos);
	fsLocalFile();
	virtual ~fsLocalFile();

protected:
	// network server of UNC path ("\\server\disk\...")
	fsString m_strServer;
};

#endif // !defined(AFX_FSLOCALFILE_H__1DDB29EB_F880_4374_BF2A_7C3A1A12F898__INCLUDED_)
