/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsDownloaderSecCheckFailureHandler.h"
#include "MyMessageBox.h"
#include "inetutil.h"

vmsDownloaderSecCheckFailureHandler::vmsDownloaderSecCheckFailureHandler (
	std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> errIgnoreList,
	std::shared_ptr <vmsDownloaderSecCheckFailureIgnoreList> errNotIgnoreList) :
	m_errIgnoreList (errIgnoreList),
	m_errNotIgnoreList (errNotIgnoreList)
{
	assert (m_errIgnoreList);
	assert (m_errNotIgnoreList);
}

vmsDownloaderSecCheckFailureHandler::~vmsDownloaderSecCheckFailureHandler ()
{
}

bool vmsDownloaderSecCheckFailureHandler::onSecCheckFailure (
	const std::wstring& host, DWORD what)
{
	std::unique_lock <std::mutex> lock (m_mutex);

	if (m_askingUser)
		m_finishedAskingUser.wait (lock, [this](){return !m_askingUser;});

	const auto errs_not_ignored = what & (~m_errIgnoreList->host_errs_ignore_flags (host));
	if (!errs_not_ignored)
		return true;

	const auto errs_not_ignored_dont_ask = what & m_errNotIgnoreList->host_errs_ignore_flags (host);
	if (errs_not_ignored_dont_ask)
		return false;

	m_askingUser = true;

	lock.unlock ();

	auto ignore_error = askUserAboutCheckFailure (host, errs_not_ignored);

	if (ignore_error.second) 
	{
		if (ignore_error.first)
		{
			m_errIgnoreList->host_errs_ignore_flags_append (
				host, errs_not_ignored);
		}
		else
		{
			m_errNotIgnoreList->host_errs_ignore_flags_append (
				host, errs_not_ignored);
		}
	}

	lock.lock ();
	m_askingUser = false;
	m_finishedAskingUser.notify_all ();

	return ignore_error.first;
}

std::pair <bool, bool> vmsDownloaderSecCheckFailureHandler::askUserAboutCheckFailure (
	const std::wstring& host, DWORD what)
{
	std::wstringstream wss;
	wss << LS (L_SEC_CHECK_FAILED_FOR_HOST) << L" " << host << 
		L"." << std::endl << 
		LS (L_ERR) << L": " << fsSCTToStr (what);
	auto ret = MyMessageBox2 (AfxGetMainWnd (), wss.str ().c_str (), LS (L_SECURITY_CHECK_FAILURE),
		L"", IDI_WARNING, LS (L_IGNORE), LS (L_ABORT));
	return std::make_pair (ret.first == IDC_BTN1, true);
}