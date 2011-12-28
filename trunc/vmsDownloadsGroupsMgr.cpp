/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsDownloadsGroupsMgr.h"
#include "mfchelp.h"
#include "misc.h"
#include "fsDownloads_GroupsMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsDownloadsGroupsMgr::vmsDownloadsGroupsMgr()
{
	m_nGrpNextId = 1;
	m_tGroups.CreateInstance ();
	m_bIsGroupsInformationChanged = false;
}

vmsDownloadsGroupsMgr::~vmsDownloadsGroupsMgr()
{

}

void vmsDownloadsGroupsMgr::CreateDefaultGroups()
{
	vmsDownloadsGroupSmartPtr grp;

	

	grp.CreateInstance ();
	grp->strName = LS (L_OTHER);
	if (IS_PORTABLE_MODE)
	{
		grp->strOutFolder = "%sdrive%\\Downloads\\";
	}
	else
	{
		DWORD dw = GetLogicalDrives ();
		DWORD dw2 = 1 << 2;
		BYTE i = 0;
		for (i = 0; i < 24 && (dw & dw2) == 0; i++, dw2 <<= 1)
			;
		if (i == 26)
			i = 0;
		grp->strOutFolder = "A";
		grp->strOutFolder [0] = (char) ('C' + i);
		grp->strOutFolder += ":\\Downloads\\";
	}
	CString strRoot = grp->strOutFolder;
	grp->strExts = "";
	grp->nId = GRP_OTHER_ID;
	Add (grp, NULL, TRUE);

	grp.CreateInstance ();
	grp->strName = "Video";
	grp->strOutFolder = strRoot + "Video\\";
	grp->strExts = GetVideoExts ();
	grp->nId = m_nGrpNextId++;
	Add (grp, NULL, TRUE);
	
	grp.CreateInstance ();
	grp->strName = "Music";
	grp->strOutFolder = strRoot + "Music\\";
	grp->strExts = GetAudioExts ();
	grp->nId = m_nGrpNextId++;
	Add (grp, NULL, TRUE);
	
	grp.CreateInstance ();
	grp->strName = "Software";
	grp->strOutFolder = strRoot + "Software\\";
	grp->strExts = "exe com msi";
	grp->nId = m_nGrpNextId++;
	Add (grp, NULL, TRUE);
}

BOOL vmsDownloadsGroupsMgr::LoadFromDisk()
{
	fsString strFile = fsGetDataFilePath ("groups.sav");

	if (GetFileAttributes (strFile) == DWORD (-1))
	{	
		fsDownloads_GroupsMgr grps;
		if (FALSE == grps.LoadGroups () || grps.GetCount () == 0)
		{
			CreateDefaultGroups ();
			return TRUE;
		}

		for (int i = 0; i < grps.GetCount (); i++)
		{
			fsDownloadGroup grp;
			grps.GetGroup (&grp, i);

			vmsDownloadsGroupSmartPtr grpNew;
			grpNew.CreateInstance ();
			grpNew->strName = grp.szName;
			grpNew->strOutFolder = grp.szOutFolder;
			grpNew->strExts = grp.szExts;
			grpNew->nId = grp.bOther ? GRP_OTHER_ID : m_nGrpNextId++;
			Add (grpNew, NULL, TRUE);
		}

		return TRUE;
	}

	

	HANDLE hFile = CreateFile (strFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		vmsDownloadsGroupsFileHdr hdr;
		DWORD dw;
		if (ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		{
			if (hdr.wVer == DLDSGRPSFILE_CURRENT_VERSION && 
					lstrcmp (hdr.szSig,	DLDSGRPSFILE_SIG) == 0)
			{
				if (FALSE == ReadFile (hFile, &m_nGrpNextId, sizeof (UINT), &dw, NULL))
					return FALSE;

				LoadGroupsTreeFromFile (hFile, m_tGroups);
			}
		}
		CloseHandle (hFile);
	}

	if (m_tGroups->GetLeafCount () == 0)
		CreateDefaultGroups ();

	return TRUE;
}

BOOL vmsDownloadsGroupsMgr::SaveToDisk()
{
	fsString strFile = fsGetDataFilePath ("groups.sav");

	bool bSaveGroupsInformation = false;
	{
		vmsCriticalSectionAutoLock csalGroupsInformationChangeGuardAutoLock(&m_csGroupsInformationChangeGuard);
		bSaveGroupsInformation = m_bIsGroupsInformationChanged;
		m_bIsGroupsInformationChanged = false;
	}

	if (!bSaveGroupsInformation) {
		return TRUE;
	}

	HANDLE hFile = CreateFile (strFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_HIDDEN, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	vmsDownloadsGroupsFileHdr hdr;
	DWORD dw;
	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	if (FALSE == WriteFile (hFile, &m_nGrpNextId, sizeof (UINT), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	BOOL bOk = SaveGroupsTreeToFile (hFile, m_tGroups);
	CloseHandle (hFile);
	return bOk;
}

PDLDS_GROUPS_TREE vmsDownloadsGroupsMgr::Add(vmsDownloadsGroupSmartPtr grp, vmsDownloadsGroupSmartPtr pParentGroup, BOOL bKeepIdAsIs, bool bDontQueryStoringGroupsInformation)
{
	PDLDS_GROUPS_TREE pParent = pParentGroup == NULL ? m_tGroups : FindGroupInTree (pParentGroup);
	return Add (grp, pParent, bKeepIdAsIs, bDontQueryStoringGroupsInformation);	
}

vmsDownloadsGroupSmartPtr vmsDownloadsGroupsMgr::FindGroup(UINT nId)
{
	for (size_t i = 0; i < m_vGroups.size (); i++)
	{
		if (m_vGroups [i]->GetData ()->nId == nId)
			return m_vGroups [i]->GetData ();
	}

	return NULL;
}

fsString vmsDownloadsGroupsMgr::GetGroupFullName(UINT nId)
{
	vmsDownloadsGroupSmartPtr pGroup = FindGroup (nId);
	if (pGroup == NULL)
		return "";
	PDLDS_GROUPS_TREE p = FindGroupInTree (pGroup);
	fsString strName;

	while (p && p->GetData () != NULL)
	{
		fsString str = p->GetData ()->strName;
		if (strName.IsEmpty ())
		{
			strName = str;
		}
		else
		{
			str += '\\';
			str += strName;
			strName = str;
		}

		p = p->GetRoot ();
	}

	return strName;
}

vmsDownloadsGroupSmartPtr vmsDownloadsGroupsMgr::FindGroupByExt(LPCSTR pszExt)
{
	for (size_t i = 0; i < m_vGroups.size (); i++)
	{
		if (IsExtInExtsStr (m_vGroups [i]->GetData ()->strExts, pszExt))
			return m_vGroups [i]->GetData ();
	}
	
	return NULL;
}

size_t vmsDownloadsGroupsMgr::GetTotalCount()
{
	return m_vGroups.size ();
}

vmsDownloadsGroupSmartPtr vmsDownloadsGroupsMgr::GetGroup(size_t nIndex)
{
	ASSERT (nIndex < m_vGroups.size ());
	return m_vGroups [nIndex]->GetData ();
}

PDLDS_GROUPS_TREE vmsDownloadsGroupsMgr::GetGroupsTree()
{
	return m_tGroups;
}

void vmsDownloadsGroupsMgr::GetGroupWithSubgroups(vmsDownloadsGroupSmartPtr pGroup, std::vector <vmsDownloadsGroupSmartPtr> &v)
{
	PDLDS_GROUPS_TREE pGrp = FindGroupInTree (pGroup);
	if (pGrp != NULL)
	{
		v.push_back (pGrp->GetData ());
		GetSubgroups (pGrp, v);
	}
}

void vmsDownloadsGroupsMgr::GetSubgroups(PDLDS_GROUPS_TREE pGroup, std::vector <vmsDownloadsGroupSmartPtr> &v)
{
	for (int i = 0; i < pGroup->GetLeafCount (); i++)
	{
		v.push_back (pGroup->GetLeaf (i)->GetData ());
		GetSubgroups (pGroup->GetLeaf (i), v);
	}
}

PDLDS_GROUPS_TREE vmsDownloadsGroupsMgr::FindGroupInTree(vmsDownloadsGroupSmartPtr pGroup)
{
	for (size_t i = 0; i < m_vGroups.size (); i++)
	{
		if (pGroup->nId == m_vGroups [i]->GetData ()->nId)
			return m_vGroups [i];
	}

	return NULL;
}

fsString vmsDownloadsGroupsMgr::GetGroupsRootOutFolder()
{
	return FindGroup (GRP_OTHER_ID)->strOutFolder;
}

void vmsDownloadsGroupsMgr::SetGroupsRootOutFolder(LPCSTR psz)
{
	SetGroupsRootOutFolder (GetGroupsTree (), psz);
}

void vmsDownloadsGroupsMgr::SetGroupsRootOutFolder(PDLDS_GROUPS_TREE pRoot, LPCSTR pszFolder)
{
	for (int i = 0; i < pRoot->GetLeafCount (); i++)
	{
		PDLDS_GROUPS_TREE pGroup = pRoot->GetLeaf (i);
		
		fsString str = pszFolder;
		if (str [str.GetLength () - 1] != '\\' && str [str.GetLength () - 1] != '/')
			str += '\\';
		pGroup->GetData ()->strOutFolder = str;

		if (pGroup->GetData ()->nId != GRP_OTHER_ID)
		{
			pGroup->GetData ()->strOutFolder += pGroup->GetData ()->strName;
			pGroup->GetData ()->strOutFolder += '\\';
		}

		QueryStoringGroupsInformation();

		SetGroupsRootOutFolder (pGroup, pGroup->GetData ()->strOutFolder);
	}
}

void vmsDownloadsGroupsMgr::DeleteGroup(vmsDownloadsGroupSmartPtr pGroup)
{
	PDLDS_GROUPS_TREE pGrp = FindGroupInTree (pGroup);
	PDLDS_GROUPS_TREE pRoot = pGrp->GetRoot ();
	for (int i = 0; i < pRoot->GetLeafCount (); i++)
	{
		if (pRoot->GetLeaf (i) == pGrp)
		{
			pRoot->DeleteLeaf (i);
			RebuildGroupsList ();
			QueryStoringGroupsInformation();
			return;
		}
	}
}

vmsDownloadsGroupSmartPtr vmsDownloadsGroupsMgr::FindGroupByName(LPCSTR pszName)
{
	return FindGroupByName (pszName, GetGroupsTree ());
}

BOOL vmsDownloadsGroupsMgr::LoadGroupsTreeFromFile(HANDLE hFile, PDLDS_GROUPS_TREE pRoot)
{
	int cGroups = 0;
	DWORD dw;
	if (FALSE == ReadFile (hFile, &cGroups, sizeof (int), &dw, NULL))
		return FALSE;

	while (cGroups-- > 0)
	{
		vmsDownloadsGroupSmartPtr pGroup;
		pGroup.CreateInstance ();
		if (FALSE == LoadGroupFromFile (hFile, pGroup))
			return FALSE;
		PDLDS_GROUPS_TREE pGroupRoot = Add (pGroup, pRoot, TRUE, true);
		if (FALSE == LoadGroupsTreeFromFile (hFile, pGroupRoot))
			return FALSE;
	}

	return TRUE;
}

BOOL vmsDownloadsGroupsMgr::LoadGroupFromFile(HANDLE hFile, vmsDownloadsGroupSmartPtr pGroup)
{
	DWORD dw;

	if (FALSE == ReadFile (hFile, &pGroup->nId, sizeof (pGroup->nId), &dw, NULL))
		return FALSE;

	if (FALSE == fsReadStringFromFile (hFile, pGroup->strName))
		return FALSE;

	if (FALSE == fsReadStringFromFile (hFile, pGroup->strOutFolder))
		return FALSE;

	if (FALSE == fsReadStringFromFile (hFile, pGroup->strExts))
		return FALSE;

	return TRUE;
}

PDLDS_GROUPS_TREE vmsDownloadsGroupsMgr::Add(vmsDownloadsGroupSmartPtr grp, PDLDS_GROUPS_TREE pParentGroup, BOOL bKeepIdAsIs, bool bDontQueryStoringGroupsInformation)
{
	if (pParentGroup == NULL)
		pParentGroup = GetGroupsTree ();

	if (bKeepIdAsIs == FALSE)
		grp->nId = m_nGrpNextId++;
	else
		m_nGrpNextId = max (m_nGrpNextId, grp->nId + 1);

	grp->cDownloads = 0;
	grp->bAboutToBeDeleted = false;

	PDLDS_GROUPS_TREE pGrp = pParentGroup->AddLeaf (grp);
	m_vGroups.push_back (pGrp);

	if (!bDontQueryStoringGroupsInformation) {
		_DldsGrps.QueryStoringGroupsInformation();
	}

	return pGrp;
}

vmsDownloadsGroupSmartPtr vmsDownloadsGroupsMgr::FindGroupByName(LPCSTR pszName, PDLDS_GROUPS_TREE pRoot)
{
	fsString strName;
	while (*pszName && *pszName != '\\' && *pszName != '/')
		strName += *pszName++;
	if (*pszName)
		pszName++;

	for (int i = 0; i < pRoot->GetLeafCount (); i++)
	{
		if (lstrcmpi (pRoot->GetLeaf (i)->GetData ()->strName, strName) == 0)
		{
			if (*pszName)
				return FindGroupByName (pszName, pRoot->GetLeaf (i));
			return pRoot->GetLeaf (i)->GetData ();
		}
	}

	return NULL;
}

BOOL vmsDownloadsGroupsMgr::SaveGroupsTreeToFile(HANDLE hFile, PDLDS_GROUPS_TREE pRoot)
{
	int cGroups = pRoot->GetLeafCount ();
	DWORD dw;
	if (FALSE == WriteFile (hFile, &cGroups, sizeof (int), &dw, NULL))
		return FALSE;

	for (int i = 0; i < cGroups; i++)
	{
		PDLDS_GROUPS_TREE pGroupTree = pRoot->GetLeaf (i);
		if (FALSE == SaveGroupToFile (hFile, pGroupTree->GetData ()))
			return FALSE;
		if (FALSE == SaveGroupsTreeToFile (hFile, pGroupTree))
			return FALSE;
	}

	return TRUE;
}

BOOL vmsDownloadsGroupsMgr::SaveGroupToFile(HANDLE hFile, vmsDownloadsGroupSmartPtr pGroup)
{
	DWORD dw;

	if (FALSE == WriteFile (hFile, &pGroup->nId, sizeof (pGroup->nId), &dw, NULL))
		return FALSE;

	if (FALSE == fsSaveStrToFile (pGroup->strName, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (pGroup->strOutFolder, hFile))
		return FALSE;

	if (FALSE == fsSaveStrToFile (pGroup->strExts, hFile))
		return FALSE;

	return TRUE;
}

LPCSTR vmsDownloadsGroupsMgr::GetVideoExts()
{
	return "avi mpg mov wmv mpeg vob mpe flv mp4";
}

LPCSTR vmsDownloadsGroupsMgr::GetAudioExts()
{
	return "mp3 wav au ogg aif aiff snd voc aac mid wma";
}

void vmsDownloadsGroupsMgr::RebuildGroupsList()
{
	std::vector <fs::ListTree <vmsDownloadsGroupSmartPtr>::ListTreePtr> v;
	RebuildGroupsList (m_tGroups, v);
	m_vGroups = v;
}

void vmsDownloadsGroupsMgr::RebuildGroupsList(PDLDS_GROUPS_TREE pRoot, std::vector <fs::ListTree <vmsDownloadsGroupSmartPtr>::ListTreePtr> &v)
{
	for (int i = 0; i < pRoot->GetLeafCount (); i++)
	{
		PDLDS_GROUPS_TREE pGroupTree = pRoot->GetLeaf (i);
		v.push_back (pGroupTree);
		RebuildGroupsList (pGroupTree, v);
	}
}

void vmsDownloadsGroupsMgr::QueryStoringGroupsInformation() 
{
	vmsCriticalSectionAutoLock csalGroupsInformationChangeGuard(&m_csGroupsInformationChangeGuard);
	m_bIsGroupsInformationChanged = true;
}
