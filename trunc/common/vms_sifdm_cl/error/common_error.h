/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "../config.h"

namespace common_error {

enum common_error	{
	unknown_error = 1,
	unexpected,
	operation_resume_failure
};

class category_impl : 
	public std::error_category
{
public:
	virtual const char* name() const override
	{
		return "common";
	}

	virtual std::string message (int ev) const override
	{
		return "common unknown error";
	}

	virtual std::error_condition default_error_condition (int ev) const override
	{
		switch (ev)
		{
		case unexpected:
			return std::errc::owner_dead; 

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

inline std::error_code make_error_code (common_error e)
{
	return std::error_code (
		static_cast <int> (e), category ());
}

inline std::error_condition make_error_condition (common_error e)
{
	return std::error_condition (
		static_cast <int> (e), category ());
}

} 

namespace std
{
	template <>
	struct is_error_code_enum <common_error::common_error>
		: public true_type {};
}