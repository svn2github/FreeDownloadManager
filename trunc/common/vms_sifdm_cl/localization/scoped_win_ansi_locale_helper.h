/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "scoped_override_locale.h"
class scoped_win_ansi_locale_helper :
	public scoped_override_locale
{
public:
	scoped_win_ansi_locale_helper () :
		scoped_override_locale (LC_ALL, win_ansi_locale ().c_str ()) {}

protected:
	static tstring win_ansi_locale ()
	{
		tstringstream tss;
		tss << _T (".") << GetACP ();
		return tss.str ();
	}
};