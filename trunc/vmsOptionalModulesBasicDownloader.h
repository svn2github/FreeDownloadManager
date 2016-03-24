/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesDownloader.h"
#include "vmsResourceDownloader.h"
class vmsOptionalModulesBasicDownloader :
	public vmsOptionalModulesDownloader
{
public:
	vmsOptionalModulesBasicDownloader(
		std::shared_ptr <vmsResourceDownloaderFactory> rdf,
		const std::wstring &download_def_path) :
		m_rdf (rdf),
		m_download_def_path (download_def_path)
	{
		assert (m_rdf);
		assert (!m_download_def_path.empty ());
		if (m_download_def_path.back () == '\\')
			m_download_def_path.erase (m_download_def_path.end () - 1);
	}

	virtual ~vmsOptionalModulesBasicDownloader(void)
	{
	}

	virtual bool download_module (const std::wstring &url, 
		std::wstring &out_path, vmsOperationProgressCallback *callback,
		vmsError &err) override
	{
		auto n = url.find (':');
		assert (n < url.length ());
		if (n >= url.length ())
			return false;
		std::wstring protocol (url.begin (), url.begin () + n);
		auto rd = m_rdf->create_downloader (protocol);
		assert (rd);
		if (!rd)
			return false;
		if (out_path.empty ())
			out_path = m_download_def_path + L'\\';
		return rd->Download (url, out_path, callback, err);
	}

protected:
	std::shared_ptr <vmsResourceDownloaderFactory> m_rdf;
	std::wstring m_download_def_path;
};

