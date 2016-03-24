/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsOptionalModulesList
{
public:
	vmsOptionalModulesList(void)
	{
	}

	virtual ~vmsOptionalModulesList(void)
	{
	}

public:
	struct module_info
	{
		std::wstring download_url;
	};

	typedef std::map <std::wstring, std::shared_ptr <module_info>> modules_list_t;
	typedef std::vector <std::wstring> modules_names_t;

public:
	void register_module (const std::wstring &name, 
		std::shared_ptr <module_info> info)
	{
		m_modules [name] = info;
	}

	const modules_list_t& modules () const
	{
		return m_modules;
	}

	void modules_names (modules_names_t &result) const
	{
		result.clear ();
		for (auto it = m_modules.begin (); it != m_modules.end (); ++it)
			result.push_back (it->first);
	}

protected:
	modules_list_t m_modules;
};

