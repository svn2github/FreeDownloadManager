/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "YouTubeParser.h"
class vmsYouTubeParserDll
{
public:
	vmsYouTubeParserDll(const std::wstring& wstrDll)
	{
		m_dll = LoadLibraryW (wstrDll.c_str ());
		assert (m_dll);
	}

	~vmsYouTubeParserDll(void)
	{
		FreeLibrary (m_dll);
	}

	std::unique_ptr<YouTubeParser> createYouTubeParser (std::wstring url)
	{
		if (!m_dll)
			return nullptr;
		typedef std::unique_ptr<YouTubeParser> (*FNYTD)(std::wstring&);
		FNYTD pfn = (FNYTD) GetProcAddress (m_dll, "createYouTubeParser");
		assert (pfn);
		return pfn ? pfn (url) : nullptr;
	}

protected:
	HMODULE m_dll;
};

