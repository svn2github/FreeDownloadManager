/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsResourceDownloader
{
public:
	virtual bool Download (const std::wstring &url, std::wstring &out_path,
		vmsOperationProgressCallback *callback, vmsError &err) = 0;
	virtual ~vmsResourceDownloader(void) {}
};

class vmsResourceDownloaderFactory
{
public:
	virtual bool supported_protocol (const std::wstring& protocol) = 0;
	virtual std::unique_ptr <vmsResourceDownloader> create_downloader (
		const std::wstring& protocol) = 0;
	virtual ~vmsResourceDownloaderFactory () {}
};

