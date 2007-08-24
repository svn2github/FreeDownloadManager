/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsCriticalSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsCriticalSection::vmsCriticalSection()
{
	::InitializeCriticalSection (this);
}

vmsCriticalSection::~vmsCriticalSection()
{
	::DeleteCriticalSection (this);
}

void vmsCriticalSection::Lock()
{
	::EnterCriticalSection (this);
}

void vmsCriticalSection::Unlock()
{
	::LeaveCriticalSection (this);
}
