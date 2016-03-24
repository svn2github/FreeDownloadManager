/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsAutorunMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsAutorunMgr::fsAutorunMgr()
{
	m_hAutoStart = NULL;
}

fsAutorunMgr::~fsAutorunMgr()
{

}

BOOL fsAutorunMgr::SetAutoStart(BOOL bSet)
{
	if (FALSE == OpenAutoStartKey ())
		return FALSE;

	TCHAR szModule [MY_MAX_PATH + 50];
	*szModule = 0;
	BOOL bRet;

	
	GetModuleFileName (NULL, szModule, MY_MAX_PATH);

	if (bSet)
	{
		
		tstringstream tss;
		tss << _T ("\"") << szModule << _T ("\" -autorun");
		auto runstr = tss.str ();
		
		bRet = ERROR_SUCCESS == RegSetValueEx (m_hAutoStart, 
			_T ("Free Download Manager"), 0, 
			REG_SZ, (CONST BYTE*) runstr.c_str (), (runstr.length ()+1) * sizeof (TCHAR));
	}
	else
	{
		bRet = ERROR_SUCCESS == RegDeleteValue (m_hAutoStart, _T ("Free Download Manager"));
	}

	CloseAutoStartKey ();

	return bRet;
}

BOOL fsAutorunMgr::OpenAutoStartKey()
{
	
	if (ERROR_SUCCESS != RegOpenKey (HKEY_CURRENT_USER, 
			_T ("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &m_hAutoStart))
		return FALSE;

	return TRUE;
}

void fsAutorunMgr::CloseAutoStartKey()
{
	RegCloseKey (m_hAutoStart);
}

BOOL fsAutorunMgr::IsAutoStart()
{
	if (FALSE == OpenAutoStartKey ())
		return FALSE;

	DWORD dwType;

	return ERROR_SUCCESS == RegQueryValueEx (m_hAutoStart, _T ("Free Download Manager"), NULL,
		&dwType, NULL, NULL);
}
