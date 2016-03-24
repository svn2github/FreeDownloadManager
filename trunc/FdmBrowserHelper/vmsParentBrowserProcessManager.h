/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsParentBrowserProcessManager
{
public:
	vmsParentBrowserProcessManager ();
	~vmsParentBrowserProcessManager ();

public:
	bool injectSniffDllProxy ();
	bool injectSniffDll ();

protected:
	HANDLE m_parentProcess = nullptr;
	HMODULE m_sniffDllProxyModule = nullptr;
	HMODULE m_sniffDllModule = nullptr;
};

