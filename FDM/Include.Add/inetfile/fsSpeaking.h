/******************************************************************************

	Class fsSpeacking.

	Base class for all classes, "talking" with internet servers.
	Used for sending to class user notifications about sending/receiving
	to/from	server messages or commands in type of one or several strings.

******************************************************************************/

#if !defined(AFX_FSSPEAKING_H__F6C6F409_5AA9_4555_8ECF_52ECC50EF0BE__INCLUDED_)
#define AFX_FSSPEAKING_H__F6C6F409_5AA9_4555_8ECF_52ECC50EF0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum fsInetFileDialogDirection	// message/command direction
{
	IFDD_TOSERVER,				// client ---> server
	IFDD_FROMSERVER				// client <--- server
};

// CALLBACK function, by which a class user receive notifications
typedef void (*fntInetFileDialogFunc) (fsInetFileDialogDirection enDir, LPCSTR pszMsg, LPVOID lp1, LPVOID lp2);

class fsSpeaking  
{
public:
	void Mute (BOOL bMute);
	// Set CALLBACK function.
	// pfn - function address
	// lpParam1, lpParam2 - some additional parameters to send in function.
	virtual void SetDialogFunc (fntInetFileDialogFunc pfn, LPVOID lpParam1, LPVOID lpParam2);
	fsSpeaking();
	virtual ~fsSpeaking();

protected:
	BOOL m_bMute;
	// Called by derived class after recieving a response from HTTP server
	void DialogHttpResponse (HINTERNET hInet);
	// Called by derived class after recieving a response from FTP server
	void DialogFtpResponse ();
	// Notify class user about sending/receiving a message
	// enDir - direction (see above)
	// pszMsg - string message
	virtual void Dialog (fsInetFileDialogDirection enDir, LPCSTR pszMsg);

	fntInetFileDialogFunc m_pfnDlgFunc;		// CALLBACK function address
	LPVOID m_lpDlgParam1, m_lpDlgParam2;	// Additional parameters for this function
};

#endif // !defined(AFX_FSSPEAKING_H__F6C6F409_5AA9_4555_8ECF_52ECC50EF0BE__INCLUDED_)
