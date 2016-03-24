/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

inline std::wstring fileNameFromHttpUrl (const std::wstring& wstrUrl)
{
	assert (!wstrUrl.empty ());
	if (wstrUrl.empty ())
		return std::wstring ();

	size_t nPos1 = wstrUrl.find ('?');
	size_t nPos2 = wstrUrl.find ('#');

	nPos2 = min (nPos1, nPos2);
	if (nPos2 == std::wstring::npos)
		nPos2 = wstrUrl.length ();

	nPos1 = nPos2 - 1;

	while (nPos1 > 0 && wstrUrl [nPos1] != '\\' && wstrUrl [nPos1] != '/')
		nPos1--;

	if (nPos1 == wstrUrl.length () - 1)
		return std::wstring ();

	return std::wstring (wstrUrl.begin () + nPos1 + 1, wstrUrl.begin () + nPos2);
}

inline std::wstring urlPathFromHttpUrl (const std::wstring& wstrUrl)
{
	assert (!wstrUrl.empty ());
	if (wstrUrl.empty ())
		return std::wstring ();

	size_t nPos1 = wstrUrl.find ('?');
	size_t nPos2 = wstrUrl.find ('#');

	nPos2 = min (nPos1, nPos2);
	if (nPos2 == std::wstring::npos)
		nPos2 = wstrUrl.length ();

	nPos1 = nPos2 - 1;

	while (nPos1 > 0 && wstrUrl [nPos1] != '\\' && wstrUrl [nPos1] != '/')
		nPos1--;

	assert (nPos1);
	if (!nPos1)
		return std::wstring ();

	return std::wstring (wstrUrl.begin (), wstrUrl.begin () + nPos1 + 1);
}

inline std::wstring domainFromHttpUrl (const std::wstring& wstrUrl)
{
	auto pszUrl = wstrUrl.c_str ();
	auto pszS = wcsstr (pszUrl, L"://");
	if (pszS == NULL)
		pszS = wcsstr (pszUrl, L":\\\\");
	if (pszS == NULL)
		pszS = pszUrl;
	else
		pszS += 3;
	auto pszE = pszS;
	while (*pszE && *pszE != '\\' && *pszE != '/')
		pszE++;

	wchar_t sz [1000];
	if (pszE - pszS > _countof (sz))
		return L"";

	assert (!"tested");
	wcsncpy (sz, pszS, pszE - pszS);
	sz [pszE - pszS] = 0;
	return sz;
}