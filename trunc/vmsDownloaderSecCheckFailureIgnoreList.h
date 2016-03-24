/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsDownloaderSecCheckFailureIgnoreList :
	public vmsThreadSafe4
{
public:
	vmsDownloaderSecCheckFailureIgnoreList ()
	{
	}

	~vmsDownloaderSecCheckFailureIgnoreList ()
	{
	}

	DWORD host_errs_ignore_flags (const std::wstring &host) const
	{
		vmsThreadSafe4Scope;
		auto it = m_hosts.find (host);
		return it != m_hosts.end () ? it->second : 0;
	}

	void host_errs_ignore_flags_append (const std::wstring &host,
		DWORD flags)
	{
		vmsThreadSafe4Scope;
		auto it = m_hosts.find (host);
		if (it != m_hosts.end ())
			it->second |= flags;
		else 
			m_hosts.insert (std::make_pair (host, flags));
	}

protected:
	std::unordered_map <std::wstring, DWORD> m_hosts;
};

