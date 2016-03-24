#pragma once

namespace zip_error {

	enum zip_error	{
		unknown_error = 1,
	};

	class category_impl : 
		public std::error_category
	{
	public:
		virtual const char* name() const override
		{
			return "zip";
		}

		virtual std::string message (int ev) const override
		{
			return "zip unknown error";
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

	inline std::error_code make_error_code (zip_error e)
	{
		return std::error_code (
			static_cast <int> (e), category ());
	}

	inline std::error_condition make_error_condition (zip_error e)
	{
		return std::error_condition (
			static_cast <int> (e), category ());
	}

} // namespace zip_error

namespace std
{
	template <>
	struct is_error_code_enum <zip_error::zip_error>
		: public true_type {};
}