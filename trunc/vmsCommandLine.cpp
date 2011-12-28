/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsCommandLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsCommandLine::vmsCommandLine()
{

}

vmsCommandLine::~vmsCommandLine()
{

}

bool SearchPath(LPCTSTR ptszFile, LPCTSTR ptszExt, tstring *ptstrResult)
{	
	TCHAR tsz [MAX_PATH] = _T ("");
	if (::SearchPath (NULL, ptszFile, ptszExt, MAX_PATH, tsz, NULL))
	{
		*ptstrResult = tsz;
		return true;
	}
	return false;
}

tstring getLongPathName(LPCTSTR ptsz)
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

void vmsCommandLine::fromString(LPCTSTR ptsz, bool bConvertToLongPath)
{
	Clear ();

	if (ptsz == NULL || *ptsz == 0)
		return;

	TCHAR tszTmp [MAX_PATH] = _T ("");
	if (_tcschr (ptsz, '%'))
	{
		ExpandEnvironmentStrings (ptsz, tszTmp, MAX_PATH);
		ptsz = tszTmp;
	}

	if (*ptsz == '"')
	{
		ptsz++;
		while (*ptsz && *ptsz != '"')
			m_tstrExe += *ptsz++;
		if (*ptsz == 0)
		{
			m_tstrExe = _T ("");
			return;
		}

		ptsz++;
	}
	else
	{
		

		while (*ptsz == ' ')
			ptsz++;

		do 
		{
			while (*ptsz && *ptsz != ' ')
				m_tstrExe += *ptsz++;

			if (m_tstrExe.length () > 1 && m_tstrExe [1] != ':')
			{
				if (SearchPath (m_tstrExe.c_str (), _T (".exe"), &m_tstrExe))
				{
					bConvertToLongPath = false; 
					break;
				}
			}

			if (*ptsz)
			{
				DWORD dw = GetFileAttributes (m_tstrExe.c_str ());
				if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY) == 0)
					break;

				tstring tstr = m_tstrExe; 

				tstr += _T (".exe");
				dw = GetFileAttributes (tstr.c_str ());
				if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY) == 0)
					break;

				if (m_tstrExe.length () > 4 && 
						_tcsicmp (m_tstrExe.c_str ()+m_tstrExe.length ()-4, _T (".exe")) == 0)
					break; 

				while (*ptsz == ' ')
					m_tstrExe += *ptsz++;
			}

		} while (*ptsz);
	}

	if (bConvertToLongPath)
		m_tstrExe = getLongPathName (m_tstrExe.c_str ());

	while (*ptsz == ' ')
		ptsz++;
	
	m_tstrArgs = ptsz ? ptsz : _T ("");
}

tstring vmsCommandLine::toString() const
{
	if (m_tstrArgs.empty ())
		return m_tstrExe;

	if (m_tstrExe.empty ())
		return _T ("");

	TCHAR tsz [32*1024] = _T ("");
	_sntprintf (tsz, 32*1024, _T ("\"%s\" %s"), m_tstrExe.c_str (), m_tstrArgs.c_str ());
	return tsz;
}

bool vmsCommandLine::isEmpty() const
{
	return m_tstrExe.empty ();
}

LPCTSTR vmsCommandLine::getExe() const
{
	return m_tstrExe.c_str ();
}

LPCTSTR vmsCommandLine::getArgs() const
{
	return m_tstrArgs.c_str ();
}

void vmsCommandLine::Clear()
{
	m_tstrExe = m_tstrArgs = _T ("");
}

void vmsCommandLine::setExe(LPCTSTR ptsz)
{
	m_tstrExe = ptsz;
}

void vmsCommandLine::setArgs(LPCTSTR ptsz)
{
	while (*ptsz == ' ')
		ptsz++;
	m_tstrArgs = ptsz;
}

bool vmsCommandLine::is_EXE_CmdLine() const
{
	if (m_tstrExe.empty ())
		return false;
	if (m_tstrExe.length () < 5)
		return false;

	return _tcsicmp (m_tstrExe.c_str () + m_tstrExe.length () - 4, _T (".exe")) == 0;
}

LPCTSTR vmsCommandLine::getExeName() const
{
	LPCTSTR ptsz = _tcsrchr (getExe (), '\\');
	if (!ptsz)
		ptsz = _tcsrchr (getExe (), '/');
	if (ptsz)
		ptsz++;
	return ptsz;
}
