/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsFileExtensionInOs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsFileExtensionInOs::vmsFileExtensionInOs()
{

}

vmsFileExtensionInOs::~vmsFileExtensionInOs()
{

}

fsString vmsFileExtensionInOs::GetAssociation(LPCTSTR pszExt, LPCTSTR pszVerb, BOOL appendDot)
{
	fsString strExt; strExt = appendDot ? _T(".") : _T(""); strExt += pszExt;

	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_CLASSES_ROOT, strExt, KEY_READ))
		return _T("");

	TCHAR sz [1000];
	DWORD dw = sizeof (sz) * sizeof(TCHAR);
	if (ERROR_SUCCESS == key.QueryValue (sz, NULL, &dw) && appendDot == TRUE)
		return GetAssociation_2 (sz, pszVerb);
	else
		return GetAssociation_2 (strExt, pszVerb);
}

fsString vmsFileExtensionInOs::GetAssociation_2(LPCTSTR pszProgId, LPCTSTR pszVerb)
{
	fsString str = pszProgId;
	str += _T("\\shell\\"); str += pszVerb; str += _T("\\command");

	CRegKey key;
	
	if (ERROR_SUCCESS != key.Open (HKEY_CLASSES_ROOT, str, KEY_READ))
		return _T("");

	TCHAR sz [MY_MAX_PATH];
	DWORD dw = sizeof (sz);
	if (ERROR_SUCCESS != key.QueryValue (sz, NULL, &dw))
		return _T("");
	
	return sz;
}

BOOL vmsFileExtensionInOs::SetAssociation(LPCTSTR pszExt, LPCTSTR pszVerb, LPCTSTR pszValue, BOOL appendDot)
{
	fsString strExt; strExt = appendDot ? _T(".") : _T(""); strExt += pszExt;

	LONG lErr;

	CRegKey key;
	if (ERROR_SUCCESS != (lErr=key.Open (HKEY_CLASSES_ROOT, strExt, KEY_READ | KEY_WRITE)))
	{
		if (ERROR_SUCCESS != (lErr=key.Create (HKEY_CLASSES_ROOT, strExt)))
		{
			SetLastError (lErr);
			return FALSE;
		}
	}

	TCHAR sz [1000];
	DWORD dw = sizeof (sz) * sizeof(TCHAR);
	if (ERROR_SUCCESS == key.QueryValue (sz, NULL, &dw) && *sz && appendDot)
		return SetAssociation_2 (sz, pszVerb, pszValue);
	else
		return SetAssociation_2 (strExt, pszVerb, pszValue);
}

BOOL vmsFileExtensionInOs::SetAssociation_2(LPCTSTR pszProgId, LPCTSTR pszVerb, LPCTSTR pszValue)
{
	fsString str = pszProgId;
	str += _T("\\shell\\"); str += pszVerb; str += _T("\\command");

	CRegKey key;
	LONG lErr;
	
	if (ERROR_SUCCESS != (lErr=key.Open (HKEY_CLASSES_ROOT, str, KEY_WRITE)))
	{
		if (ERROR_SUCCESS != (lErr=key.Create (HKEY_CLASSES_ROOT, str)))
		{
			SetLastError (lErr);
			return FALSE;
		}
	}

	lErr = key.SetValue (pszValue, NULL);

	if (lErr != ERROR_SUCCESS)
		SetLastError (lErr);

	return lErr == ERROR_SUCCESS;
}

BOOL vmsFileExtensionInOs::CreateAssociation(LPCTSTR pszExt, LPCTSTR pszProgId)
{
	fsString strExt; strExt = _T("."); strExt += pszExt;

	CRegKey key;

	if (ERROR_SUCCESS != key.Create (HKEY_CLASSES_ROOT, strExt))
		return FALSE;

	if (ERROR_SUCCESS != key.SetValue (pszProgId, NULL))
		return FALSE;

	key.Close ();

	if (ERROR_SUCCESS != key.Create (HKEY_CLASSES_ROOT, pszProgId))
	{
		if (GetLastError () != ERROR_ALREADY_EXISTS)
			return FALSE;
	}

	return TRUE;
}

LONG vmsFileExtensionInOs::OpenAssociationKey(CRegKey &key, const tstring& tstrExt, bool bCreateIfDoesNotExist, REGSAM samDesired)
{
	LONG lResult = key.Open (HKEY_CLASSES_ROOT, tstrExt.c_str (), samDesired);

	if (lResult != ERROR_SUCCESS && bCreateIfDoesNotExist)
		lResult = key.Create (HKEY_CLASSES_ROOT, tstrExt.c_str (), NULL, 0, samDesired);

	return lResult;
}
