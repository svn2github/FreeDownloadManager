/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsTmpFileName
{
public:
	vmsTmpFileName (LPCTSTR ptszPrefix = _T ("tmp"), LPCTSTR ptszExt = NULL)
	{
		TCHAR tsz [MAX_PATH] = _T ("");
		GetTempPath (_countof (tsz), tsz);
		TCHAR tsz1 [MAX_PATH] = _T ("");
		GetTempFileName (tsz, ptszPrefix, 0, tsz1);
		m_tstrTmpFileName1 = tsz1;
		if (ptszExt && _tcsicmp (ptszExt, _T ("tmp")))
		{
			_tcscpy (tsz1 + _tcslen (tsz1) - 3, ptszExt);
			m_tstrTmpFileName2 = tsz1;
			assert (GetFileAttributes (tsz1) == DWORD (-1));
		}
	}

	~vmsTmpFileName ()
	{
		if (!m_tstrTmpFileName2.empty ())
			DeleteFile (m_tstrTmpFileName2.c_str ());
		if (!m_tstrTmpFileName1.empty ())
			DeleteFile (m_tstrTmpFileName1.c_str ());
	}

	operator LPCTSTR () const {return path_name ().c_str ();}
	const tstring& path_name () const {return m_tstrTmpFileName2.empty () ? m_tstrTmpFileName1 : m_tstrTmpFileName2;} 

protected:
	tstring m_tstrTmpFileName1, m_tstrTmpFileName2;
};

