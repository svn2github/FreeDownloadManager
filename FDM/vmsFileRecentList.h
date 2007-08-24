/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSFILERECENTLIST_H__A4B7CD08_47AB_4114_8532_B8287C88932C__INCLUDED_)
#define AFX_VMSFILERECENTLIST_H__A4B7CD08_47AB_4114_8532_B8287C88932C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsFileRecentList  
{
public:
	
	int get_Count () const;
	
	LPCSTR get_FilePathName(int nIndex) const;
	
	LPCSTR get_FileDispName (int nIndex) const;
	
	void Clear();
	
	BOOL Load (HANDLE hFile);
	BOOL Save (HANDLE hFile);
	
	void Add (LPCSTR pszFileDispName, LPCSTR pszFilePathName);

	vmsFileRecentList();
	virtual ~vmsFileRecentList();

protected:
	struct _inc_FileInfo
	{
		fsString strPathName;
		fsString strDispName;
	};
	
	std::vector <_inc_FileInfo> m_vList;
	
	int m_nMaxEntries;
};

#endif 
