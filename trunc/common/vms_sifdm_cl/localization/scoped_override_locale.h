/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class scoped_override_locale
{
public:
	scoped_override_locale (int category, LPCTSTR locale) :
		m_category (category)
	{
		m_old_locale = _tsetlocale (category, locale);
	}

	~scoped_override_locale ()
	{
		_tsetlocale (m_category, m_old_locale);
	}

protected:
	int m_category;
	LPCTSTR m_old_locale;
};