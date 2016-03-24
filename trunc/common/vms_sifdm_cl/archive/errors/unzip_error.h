#pragma once

namespace unzip_error {

	enum unzip_error	{
		unknown_error = 1,
	};

	class category_impl : 
		public std::error_category
	{
	public:
		virtual const char* name() const override
		{
			return "unzip";
		}

		virtual std::string message (int ev) const override
		{
			return "unzip unknown error";
		}

		virtual std::error_condition default_error_condition (int ev) const override
		{
			return std::error_condition (ev, *this);
			/*switch (ev)
			{
			default:
				return std::error_condition (ev, *this);
			}*/
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

	inline std::error_code make_error_code (unzip_error e)
	{
		return std::error_code (
			static_cast <int> (e), category ());
	}

	inline std::error_condition make_error_condition (unzip_error e)
	{
		return std::error_condition (
			static_cast <int> (e), category ());
	}

} // namespace unzip_error

namespace std
{
	template <>
	struct is_error_code_enum <unzip_error::unzip_error>
		: public true_type {};
}