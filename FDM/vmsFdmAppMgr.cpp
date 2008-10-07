/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsFdmAppMgr.h"
#include "FdmApp.h"        

vmsFdmAppMgr::vmsFdmAppMgr()
{

}

vmsFdmAppMgr::~vmsFdmAppMgr()
{

}

BOOL vmsFdmAppMgr::IsBtInstalled()
{
	return GetFileAttributes (((CFdmApp*)AfxGetApp ())->m_strAppPath + "fdmbtsupp.dll") != DWORD (-1);
}

BOOL vmsFdmAppMgr::IsMediaFeaturesInstalled()
{
	return GetFileAttributes (((CFdmApp*)AfxGetApp ())->m_strAppPath + "mediaconverter.dll") != DWORD (-1);
}

void vmsFdmAppMgr::ShowInstallMediaFeaturesMessage()
{
	MessageBox (NULL, LS (L_INST_MEDIA_FEATURES), "Free Download Manager", 0);
}

void vmsFdmAppMgr::ShowInstallBtMessage()
{
	MessageBox (NULL, LS (L_INST_BT_SUPPORT), "Free Download Manager", 0);
}

LPCSTR vmsFdmAppMgr::getBuildNumber()
{
	static std::string str;
	if (!str.empty ())
		return str.c_str ();
	str = getVersion ()->m_appVersion.size () > 2 ? getVersion ()->m_appVersion [2].toString () : "???";
	return str.c_str ();
}

const vmsBinaryFileVersionInfo* vmsFdmAppMgr::getVersion()
{
	static vmsBinaryFileVersionInfo bfvi;
	if (bfvi.m_appVersion.size () == 0)
	{
		TCHAR tsz [MAX_PATH];
		GetModuleFileName (NULL, tsz, MAX_PATH);
		bfvi.Retrieve (tsz);
	}
	return &bfvi;
}

LPCSTR vmsFdmAppMgr::getAppVersionedName()
{
	static std::string str;
	if (str.empty ())
	{
		str = getAppName ();
		str += ' ';
		str += getVersion ()->m_strProductVersion;
	}
	return str.c_str ();
}

LPCSTR vmsFdmAppMgr::getAppAgentName()
{
	static std::string str;
	if (str.empty ())
	{
		str = "FDM ";
		char sz [100];
		str += itoa (getVersion ()->m_appVersion [0].dwVal, sz, 10);
		str += ".x"; 
	}
	return str.c_str ();
}

LPCSTR vmsFdmAppMgr::getAppName()
{
	return getVersion ()->m_strProductName.c_str ();
}
