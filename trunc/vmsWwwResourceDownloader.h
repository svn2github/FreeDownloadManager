/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsResourceDownloader.h"
#include "vmsSimpleFileDownloader.h"
class vmsWwwResourceDownloader :
	public vmsResourceDownloader
{
public:
	virtual bool Download (const std::wstring &url, std::wstring &out_path,
		vmsOperationProgressCallback *callback, vmsError &err) override
	{
		err.clear ();

		vmsSimpleFileDownloader sfd;
		auto result = sfd.Download (url.c_str (), out_path.c_str ());
		if (result != IR_SUCCESS)
		{
			err.error = common_error::unknown_error; 
			return false;
		}

		while (sfd.IsRunning ())
		{
			if (callback)
			{
				if (!callback->onOperationProgressPercentage (
					sfd.getProgressInPercentage ()))
				{
					sfd.Stop ();
					while (sfd.IsRunning ())
						Sleep (20);
				}
			}

			Sleep (20);
		}

		if (IR_SUCCESS != sfd.GetLastError ().first)
		{
			err.error = common_error::unknown_error; 
			return false;
		}

		out_path = sfd.out_file_name ();

		return true;
	}
};

class vmsWwwResourceDownloaderFactory : 
	public vmsResourceDownloaderFactory
{
public:
	virtual bool supported_protocol (const std::wstring& protocol) override
	{
		return protocol == L"http" || protocol == L"https" || 
			protocol == L"ftp";
	}

	virtual std::unique_ptr <vmsResourceDownloader> create_downloader (
		const std::wstring& protocol) override
	{
		std::unique_ptr <vmsResourceDownloader> result;
		assert (supported_protocol (protocol));
		if (!supported_protocol (protocol))
			return result;
		result.reset (new vmsWwwResourceDownloader ());
		return std::move (result);
	}
};

