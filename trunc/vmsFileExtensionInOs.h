/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSFILEEXTENSIONINOS_H__C040E944_2841_4F64_AEA3_6E2766A7D277__INCLUDED_)
#define AFX_VMSFILEEXTENSIONINOS_H__C040E944_2841_4F64_AEA3_6E2766A7D277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsFileExtensionInOs  
{
public:
	
	static BOOL CreateAssociation (LPCTSTR pszExt, LPCTSTR pszProgId);
	
	
	static BOOL SetAssociation(LPCTSTR pszExt, LPCTSTR pszVerb, LPCTSTR pszValue, BOOL appendDot = TRUE);
	static fsString GetAssociation (LPCTSTR pszExt, LPCTSTR pszVerb, BOOL appendDot = TRUE);

	static LONG OpenAssociationKey(CRegKey &key, const tstring& tstrExt, bool bCreateIfDoesNotExist, REGSAM samDesired);

	vmsFileExtensionInOs();
	virtual ~vmsFileExtensionInOs();

protected:
	static BOOL SetAssociation_2(LPCTSTR pszProgId, LPCTSTR pszVerb, LPCTSTR pszValue);
	static fsString GetAssociation_2 (LPCTSTR pszProgId, LPCTSTR pszVerb);
};

#endif 
