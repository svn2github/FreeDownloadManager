/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class inter_module_err_helper
{
public:
	static inter_module_err_helper& instance ()
	{
		static inter_module_err_helper inst;
		return inst;
	}

	inter_module_err_helper ()
	{
		register_error_category (std::generic_category ());
		register_error_category (std::system_category ());
	}

	void register_error_category (const std::error_category& category)
	{
		assert (m_categories.find (category.name ()) == m_categories.end ());
		m_categories [category.name ()] = &category;
	}

	std::error_code normalize_error_code (const std::error_code& err) const
	{
		auto it = m_categories.find (err.category ().name ());
		if (it == m_categories.end ())
			return err;
		return std::error_code (err.value (), *it->second);
	}

	std::error_condition normalize_error_condition (const std::error_condition& err) const
	{
		auto it = m_categories.find (err.category ().name ());
		if (it == m_categories.end ())
			return err;
		return std::error_condition (err.value (), *it->second);
	}

	std::error_condition normalized_condition_from_error (const std::error_code& err) const
	{
		return normalize_error_condition (err.default_error_condition ());
	}

	template <class obj_t>
	bool normalized (const obj_t& obj) const
	{
		auto it = m_categories.find (obj.category ().name ());
		return it == m_categories.end () ||
			it->second == &obj.category ();
	}

protected:
	std::map <std::string, const std::error_category*> m_categories;
};