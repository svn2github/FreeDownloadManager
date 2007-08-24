/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsFdmAppMgr.h"
#include "Data Stretcher.h"        

vmsFdmAppMgr::vmsFdmAppMgr()
{

}

vmsFdmAppMgr::~vmsFdmAppMgr()
{

}

BOOL vmsFdmAppMgr::IsBtInstalled()
{
	return GetFileAttributes (((CDataStretcherApp*)AfxGetApp ())->m_strAppPath + "fdmbtsupp.dll") != DWORD (-1);
}

BOOL vmsFdmAppMgr::IsMediaFeaturesInstalled()
{
	return GetFileAttributes (((CDataStretcherApp*)AfxGetApp ())->m_strAppPath + "mediaconverter.dll") != DWORD (-1);
}

void vmsFdmAppMgr::ShowInstallMediaFeaturesMessage()
{
	MessageBox (NULL, LS (L_INST_MEDIA_FEATURES), "Free Download Manager", 0);
}

void vmsFdmAppMgr::ShowInstallBtMessage()
{
	MessageBox (NULL, LS (L_INST_BT_SUPPORT), "Free Download Manager", 0);
}
