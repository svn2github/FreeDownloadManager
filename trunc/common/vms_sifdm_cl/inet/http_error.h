/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

namespace http_error {

enum http_error	{
	continue_request		= 100,
	switching_protocols		= 101,
	ok						= 200,
	moved					= 301, 
	redirect				= 302, 
	redirect_method			= 303, 
	redirect_keep_verb		= 307, 
	forbidden				= 403,
	gateway_timeout			= 504,
	version_not_supported	= 505
};

class category_impl : 
	public std::error_category
{
public:
	virtual const char* name() const override
	{
		return "http";
	}

	virtual std::string message (int ev) const override
	{
		return "http unknown error";
	}

	virtual std::error_condition default_error_condition (int ev) const override
	{
		switch (ev)
		{
		case forbidden:
			return std::errc::permission_denied;

		default:
			return std::error_condition (ev, *this);
		}
	}
};

#ifdef VMSCL_IMPL_IMPORT
_declspec (dllimport) const std::error_category& category ();
#else
VMSCL_EXPORTABLE inline const std::error_category& category ()
{
	static category_impl instance;
	return instance;
}
#endif

inline std::error_code make_error_code (http_error e)
{
	return std::error_code (
		static_cast <int> (e), category ());
}

inline std::error_condition make_error_condition (http_error e)
{
	return std::error_condition (
		static_cast <int> (e), category ());
}

} 

namespace std
{
	template <>
	struct is_error_code_enum <http_error::http_error>
		: public true_type {};
}