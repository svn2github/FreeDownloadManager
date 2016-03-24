/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <wininet.h>

namespace windows_error {

	enum windows_error	{

	};

	class category_impl : 
		public std::error_category
	{
	public:
		virtual const char* name() const override
		{
			return "windows";
		}

		virtual std::string message (int ev) const override
		{
			return "windows unknown error";
		}

		virtual std::error_condition default_error_condition (int ev) const override
		{
			switch (ev)
			{
			case ERROR_ACCESS_DENIED:
				return std::errc::permission_denied;

			case ERROR_INTERNET_NAME_NOT_RESOLVED:
				return std::errc::address_not_available;

			case ERROR_INTERNET_CANNOT_CONNECT:
				return std::errc::host_unreachable;

			case ERROR_INTERNET_CONNECTION_ABORTED:
				return std::errc::connection_aborted;

			case ERROR_INTERNET_CONNECTION_RESET:
				return std::errc::connection_reset;

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

	inline std::error_code make_error_code (windows_error e)
	{
		return std::error_code (
			static_cast <int> (e), category ());
	}

	inline std::error_condition make_error_condition (windows_error e)
	{
		return std::error_condition (
			static_cast <int> (e), category ());
	}

	inline std::error_code last_error ()
	{
		return make_error_code (static_cast <windows_error> (GetLastError ()));
	}

} 

namespace std
{
	template <>
	struct is_error_code_enum <windows_error::windows_error>
		: public true_type {};
}