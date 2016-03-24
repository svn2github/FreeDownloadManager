/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsFFMPEG.h"

#include <boost/interprocess/sync/scoped_lock.hpp>

using namespace boost;
using namespace boost::interprocess;

vmsFFMPEG::vmsFFMPEG() : m_isInitialized(false)
{
}

vmsFFMPEG::~vmsFFMPEG()
{
	if(m_isInitialized)
	{
		m_isInitialized = false;
		m_dll.Free();
	}
}

vmsFFMPEG* vmsFFMPEG::instance()
{
	static vmsFFMPEG* _instance = new vmsFFMPEG;
	return _instance;
}

#define GETPROC(n) if(!(m_##n = m_dll.GetProcAddress(#n))) { m_dll.Free(); return false; }

bool vmsFFMPEG::Init()
{
	scoped_lock<recursive_mutex> lock(m_mtxDLL);

	if(m_isInitialized)
		return true;

	if(!m_dll.Load(_T("MediaConverter.dll")))
		return false;

	if(!(m_launchFFMPEG = m_dll.GetProcAddress("launchFFMPEG")))
	{
		m_dll.Free();
		return false;
	}

	

	return (m_isInitialized = true);
}

bool vmsFFMPEG::Initialized() const
{
	return m_isInitialized;
}

void vmsFFMPEG::LockDLL()
{
	m_mtxDLL.lock();
}

void vmsFFMPEG::UnlockDLL()
{
	m_mtxDLL.unlock();
}

int __cdecl vmsFFMPEG::launchFFMPEG(BOOL* bStop, ...)
{
	scoped_lock<recursive_mutex> lock(m_mtxDLL);
	if(!m_isInitialized) return -1;

	va_list vl;
	va_start(vl, bStop);
	std::vector<char*> args;
	while(char* arg = va_arg(vl, char*))
		args.push_back(arg);
	args.push_back(nullptr);
	va_end(vl);

	int result = ((int (*)(int, char**, bool*))m_launchFFMPEG)(args.size(), args.data(), (bool*)bStop);

	
	m_isInitialized = false;
	m_dll.Free();
	Init();

	return result;
}
