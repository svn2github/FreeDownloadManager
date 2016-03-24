/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "Utils.h"
#include "vmsFile.h"

vmsFile::vmsFile()
{
	m_file = NULL;
}

vmsFile::~vmsFile()
{
	close ();
}

bool vmsFile::open(LPCTSTR pszName, LPCTSTR pszMode)
{
	close ();

	std::string sName;
	std::string sNode;

#ifdef UNICODE
	UniToAnsi(std::wstring(pszName), sName);
	UniToAnsi(std::wstring(pszMode), sNode);
#else
	sName = pszName;
	sNode = pszMode;
#endif

	m_file = fopen (sName.c_str(), sNode.c_str());

	return m_file != NULL;
}

void vmsFile::close()
{
	if (m_file)
	{
		fclose (m_file);
		m_file = NULL;
	}
}

bool vmsFile::read(tstring& str)
{
	std::string sTmp;
	char sz [1000] = "";
	sTmp = "";

	for (;;)
	{
		LPCSTR psz = fgets (sz, sizeof (sz) - 1, m_file);

		if (psz == NULL && !feof (m_file))
		{
			sTmp = "";
			return false;
		}

		if (psz == NULL)
			return false;

		sTmp += sz;
		if (strlen (sz) < sizeof (sz) - 1 ||
				sTmp [str.length () - 1] == '\n')
			break;
	};

	if (sTmp [str.length () - 1] == '\n')
		sTmp.erase (sTmp.begin () + sTmp.length () - 1);

#ifdef UNICODE
	AnsiToUni(sTmp, str);
#else
	str = sTmp;
#endif

	return true;
}
