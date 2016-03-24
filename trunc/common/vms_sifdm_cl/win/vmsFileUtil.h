/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsFileUtil  
{
public:
	static bool SearchPath (LPCTSTR ptszFile, LPCTSTR ptszExt, tstring *ptstrResult)
	{
		vmsAUTOLOCKSECTION (m_csSearchPathCache);

		for (size_t i = 0; i < m_searchPathCache.size (); i++)
		{
			if (_tcsicmp (m_searchPathCache [i].tstrExt.c_str (), ptszExt) == 0 &&
				_tcsicmp (m_searchPathCache [i].tstrFile.c_str (), ptszFile) == 0)
			{
				*ptstrResult = m_searchPathCache [i].tstrFilePath;
				return true;
			}
		}

		vmsAUTOLOCKSECTION_UNLOCK (m_csSearchPathCache);

		TCHAR tsz [MAX_PATH] = _T ("");
		if (::SearchPath (NULL, ptszFile, ptszExt, MAX_PATH, tsz, NULL))
		{
			*ptstrResult = tsz;
			return true;
		}
		return false;
	}

	static tstring getLongPathName (LPCTSTR ptsz)
	{
		TCHAR tsz [MAX_PATH] = _T ("");
		if (_tcslen (ptsz) > 2 && ptsz [1] != ':')
		{
			tstring tstr;
			if (SearchPath (ptsz, _T ("exe"), &tstr))
				_tcscpy (tsz, tstr.c_str ());
		}
		if (0 == GetLongPathName (ptsz, tsz, MAX_PATH))
			return ptsz;
		return tsz;
	}

protected:
	struct _inc_SearchPathCacheItem {
		tstring tstrFile;
		tstring tstrExt;
		tstring tstrFilePath;
	};
	static std::vector <_inc_SearchPathCacheItem> m_searchPathCache;
	static vmsCriticalSection m_csSearchPathCache;
};

__declspec (selectany) std::vector <vmsFileUtil::_inc_SearchPathCacheItem> vmsFileUtil::m_searchPathCache;
__declspec (selectany) vmsCriticalSection vmsFileUtil::m_csSearchPathCache;

