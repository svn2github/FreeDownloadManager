/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsDownloaderSecCheckFailureHandler
{
public:
	vmsDownloaderSecCheckFailureHandler (
		std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> errIgnoreList,
		std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> errNotIgnoreList);
	~vmsDownloaderSecCheckFailureHandler ();

protected:
	std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> m_errIgnoreList;
	std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> m_errNotIgnoreList;
	bool m_askingUser = false;
	std::condition_variable m_finishedAskingUser;
	std::mutex m_mutex;

public:
	
	bool onSecCheckFailure (const std::wstring& host, DWORD what);

protected:
	std::pair <bool, bool> askUserAboutCheckFailure (const std::wstring& host, DWORD what);
};

