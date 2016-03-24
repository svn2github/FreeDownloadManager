/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#define LS(i)	_LngMgr.GetString (i)
#define LSNP(i) _LngMgr.GetStringNP (i)

enum text_file_encoding
{
	tfe_unknown,
	tfe_utf8,
	tfe_utf16,
	tfe_utf16le
};

inline bool get_text_file_encoding (const tstring& name, text_file_encoding& result)
{
	HANDLE file = CreateFile (name.c_str (), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		return false;

	BYTE abc [3];
	DWORD dw;
	bool ok = ReadFile (file, abc, 3, &dw, nullptr) != FALSE;

	CloseHandle (file);

	if (!ok)
		return false;

	if (abc[0] == 0xEF && abc [1] == 0xBB && abc [2] == 0xBF)
		result = tfe_utf8;

	else if (abc [0] == 0xFE && abc [1] == 0xFF)
		result = tfe_utf16;

	else if (abc [0] == 0xFF && abc [1] == 0xFE)
		result = tfe_utf16le;

	else
		result = tfe_unknown;

	return true;
}

inline FILE* open_text_file (const tstring& name)
{
	text_file_encoding tfe;
	if (!get_text_file_encoding (name, tfe))
		return nullptr;

	static std::map <text_file_encoding, tstring> enc_map;
	if (enc_map.empty ())
	{
		enc_map [tfe_utf8] = _T("UTF-8");
		enc_map [tfe_utf16] = _T("UNICODE");
		enc_map [tfe_utf16le] = _T("UTF-16LE");
	}

	tstringstream tss;
	tss << _T ("rt");
	if (tfe != tfe_unknown)
		tss << _T (", ccs=") << enc_map [tfe];

	FILE *f;
	errno_t err = _tfopen_s (&f, name.c_str (), tss.str ().c_str ());
	if (err)
		return nullptr;

	return f;
}

struct fsLngFileInfo
{
	tstring strFileName;		
	tstring strFileNameWoExt;	
	tstring strLngName;			
};

class scoped_override_locale
{
public:
	scoped_override_locale (int category, LPCTSTR locale) :
	  m_category (category)
	{
		m_old_locale = _tsetlocale (category, locale);
	}

	~scoped_override_locale()
	{
		_tsetlocale (m_category, m_old_locale);
	}

protected:
	int m_category;
	LPCTSTR m_old_locale;
};

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

#define LNG_COMMENT_CHAR	';'

class fsLangMgr  
{
public:
	LPCTSTR GetLngFileNameWoExt (int iIndex)
	{
		if (iIndex < 0 || iIndex >= (int)m_vLngFiles.size ())
			return _T("eng");
		return m_vLngFiles [iIndex].strFileNameWoExt.c_str ();
	}

	
	BOOL Initialize (tstring lng_folder = tstring ())
	{
		scoped_win_ansi_locale_helper win_ansi;

		if (m_builtinLngResId)
			LoadBuiltInLngStrings ();

		WIN32_FIND_DATA wfd;

		m_strLngFolder = lng_folder.empty () ? language_folder () : lng_folder;
		m_strLngFolder += '\\';

		tstring strMask = m_strLngFolder;
		strMask += _T("*.lng");

		HANDLE hFind = FindFirstFile (strMask.c_str (), &wfd);
		BOOL bFind = hFind != INVALID_HANDLE_VALUE;

		while (bFind)
		{
			try 
			{
				fsLngFileInfo info;
				info.strFileName = wfd.cFileName;
				info.strFileNameWoExt = wfd.cFileName;
				assert (info.strFileNameWoExt.length () > 4);
				info.strFileNameWoExt.erase (info.strFileNameWoExt.end ()-4, info.strFileNameWoExt.end ());

				auto file = open_text_file ((m_strLngFolder + info.strFileName).c_str ());
				if (file)
				{
					wchar_t wsz [5000] = L"";
					
					while (fgetws (wsz, _countof (wsz), file))
					{
						tstring str = wsz;
						if (str.back () == '\n')
							str.erase (str.end () - 1);
						if (!str.empty () && str.front () != LNG_COMMENT_CHAR)
						{
							info.strLngName = str;							
							AddLngFileInfo (info);
							break;
						}
					}
					fclose (file);
				}
			}
			catch (...)
			{
				assert (!"expected");
			}

			bFind = FindNextFile (hFind, &wfd);
		}

		if (hFind != INVALID_HANDLE_VALUE)
			FindClose (hFind);

		BOOL ok = m_vLngFiles.size () != 0;

		if (ok && m_vBuiltInStrings.empty ())
		{
			int index = FindLngByName (_T ("English"));
			if (index != -1 && LoadLng (index))
				m_vBuiltInStrings = m_vStrings;
		}

		return ok;
	}

	
	int GetLngCount()
	{
		return m_vLngFiles.size ();
	}

	
	LPCTSTR GetLngName (int iIndex)
	{
		static TCHAR szBuiltIn [] = _T("Built In");
		if (iIndex < 0 || iIndex >= (int)m_vLngFiles.size ())
			return szBuiltIn;
		return m_vLngFiles [iIndex].strLngName.c_str ();
	}

	
	int FindLngByName (LPCTSTR pszName)
	{
		for (size_t i = 0; i < m_vLngFiles.size (); i++)
		{
			if (m_vLngFiles [i].strLngName == pszName || 
				lstrcmpi (m_vLngFiles [i].strFileName.c_str (), pszName) == 0)
			{
				return (int)i;
			}
		}

		return -1;
	}

	
	BOOL LoadLng (int iIndex)
	{
		scoped_win_ansi_locale_helper win_ansi;

		try 
		{
			auto file = open_text_file (m_strLngFolder + m_vLngFiles [iIndex].strFileName);
			if (!file)
				return FALSE;

			wchar_t wsz [5000] = L"";

			
			do
			{
				if (!fgetws (wsz, _countof (wsz), file))
				{
					fclose (file);
					return FALSE;
				}

				tstring str = wsz;
				if (str.back () == '\n')
					str.erase (str.end () - 1);

				if (!str.empty () && str.front () != LNG_COMMENT_CHAR)
					break;
			}
			while (TRUE);

			m_vStrings.clear ();

			
			while (fgetws (wsz, _countof (wsz), file))
			{
				tstring str = wsz;
				if (str.back () == '\n')
					str.erase (str.end () - 1);

				if (str.empty () || str.front () == LNG_COMMENT_CHAR)
					continue; 

				PreprocessLanguageString (str);

				m_vStrings.push_back (str);
			}

			fclose (file);

			m_iCurLng = iIndex;

			return m_vStrings.size () != 0;
		}
		catch (...)
		{
			assert (!"expected");
			return FALSE;
		}
	}

	
	int GetStringCount()
	{
		return (int)m_vStrings.size ();
	}

	
	int GetCurLng()
	{
		return m_iCurLng;
	}

	
	LPCTSTR GetString (int iIndex)
	{
		static TCHAR szNull [1] = {0};

		if (iIndex < (int)m_vStrings.size ()) 
			return m_vStrings [iIndex].c_str ();
		else if (iIndex < (int)m_vBuiltInStrings.size ()) 
			return m_vBuiltInStrings [iIndex].c_str ();
		else
			return szNull;
	}

	
	tstring GetStringNP (int iIndex)
	{
		tstring str = GetString (iIndex);
		tstringReplace (str, _T ("&"), _T (""));
		return str;
	}

	fsLangMgr (LPCTSTR builtinLngResId = nullptr, 
		LPCTSTR builtinLngResType = nullptr) :
		m_iCurLng (-1),
		m_builtinLngResId (builtinLngResId),
		m_builtinLngResType (builtinLngResType)
	{

	}
	
	virtual ~fsLangMgr()
	{

	}

protected:
	virtual void PreprocessLanguageString (tstring& str)
	{
		tstringReplace (str, _T("\\n"), _T("\n"));
	}

	
	void LoadBuiltInLngStrings()
	{
		if (!m_builtinLngResId)
			return;

		HGLOBAL hResource;
		HRSRC hRes;
		LPBYTE pbRes;

		hRes = FindResource (NULL, m_builtinLngResId, m_builtinLngResType);
		if (hRes == NULL)
			return;

		hResource = LoadResource (NULL, hRes);
		pbRes = (LPBYTE) LockResource (hResource);
		LPBYTE pbResEnd = pbRes + SizeofResource (NULL, hRes);

		LPCSTR psz = (LPCSTR) pbRes;
		LPCSTR pszE = (LPCSTR) pbResEnd;
		bool bLngNamePassed = false;

		while (psz < pszE) 
		{
			tstring str;
			while (psz < pszE && *psz != '\n' && *psz != '\r')
				str += *psz++;
			while (psz < pszE && (*psz == '\n' || *psz == '\r'))
				psz++;

			if (str.empty () || str.front () == LNG_COMMENT_CHAR)
				continue;

			if (bLngNamePassed == false) {
				bLngNamePassed = true;
				continue;
			}

			PreprocessLanguageString (str);
			m_vBuiltInStrings.push_back (str);
		}
	}

	
	
	void AddLngFileInfo (fsLngFileInfo& info)
	{
		for (auto it = m_vLngFiles.begin (); it != m_vLngFiles.end (); ++it)
		{
			if (_tcsicmp (it->strLngName.c_str (), info.strLngName.c_str ()) > 0)
			{
				m_vLngFiles.insert (it, info);
				return;
			}
		}

		m_vLngFiles.push_back (info);
	}

	tstring language_folder ()
	{
		MEMORY_BASIC_INFORMATION mbi;
		auto module = VirtualQuery (&open_text_file, &mbi, sizeof (mbi)) != 0 ? (HMODULE)mbi.AllocationBase : NULL;
		TCHAR tsz [MAX_PATH] = _T ("");
		GetModuleFileName (module, tsz, MAX_PATH);
		auto path_e = _tcsrchr (tsz, '\\');
		assert (path_e);
		if (path_e)
			path_e [1] = 0;
		_tcscat (tsz, _T("Language"));
		return tsz;
	}

protected:
	int m_iCurLng;	
	std::vector <fsLngFileInfo> m_vLngFiles;	
	std::vector <tstring> m_vStrings;		
	std::vector <tstring> m_vBuiltInStrings;	
	tstring m_strLngFolder;
	LPCTSTR m_builtinLngResId, m_builtinLngResType;
};

