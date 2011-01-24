/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSCRITICALSECTION_H__3776F731_26E6_4128_85DB_7AD04E283414__INCLUDED_)
#define AFX_VMSCRITICALSECTION_H__3776F731_26E6_4128_85DB_7AD04E283414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsCriticalSection : protected CRITICAL_SECTION  
{
public:
	void Unlock();
	void Lock();
	
	vmsCriticalSection();
	virtual ~vmsCriticalSection();

};

#endif 
