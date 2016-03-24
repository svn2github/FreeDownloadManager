/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __VMS_CRITICAL_SECTION_
#define __VMS_CRITICAL_SECTION_

class vmsCriticalSection
{
public:
	vmsCriticalSection () {InitializeCriticalSection (&m_cs);}
	virtual ~vmsCriticalSection () {DeleteCriticalSection (&m_cs);}

	operator CRITICAL_SECTION& () {return m_cs;}
	operator LPCRITICAL_SECTION () {return &m_cs;}
	LPCRITICAL_SECTION getCS (){return &m_cs;}

protected:
	CRITICAL_SECTION m_cs;
};

class vmsCriticalSectionAutoLock
{
public:
	vmsCriticalSectionAutoLock (LPCRITICAL_SECTION pcs = NULL) : m_pcs (pcs) {
		if (m_pcs)
			EnterCriticalSection (m_pcs);
	}
	vmsCriticalSectionAutoLock (vmsCriticalSection *pcs) : m_pcs (nullptr) {
		if (pcs)
		{
			m_pcs = *pcs;
			EnterCriticalSection (m_pcs);
		}
	}
	~vmsCriticalSectionAutoLock () {
		if (m_pcs)
			LeaveCriticalSection (m_pcs);
	}
	void Attach (LPCRITICAL_SECTION pcs)
	{
		Unlock ();
		m_pcs = pcs;
		if (m_pcs)
			EnterCriticalSection (m_pcs);
	}
	void Unlock () {
		if (m_pcs)
		{
			LeaveCriticalSection (m_pcs);
			m_pcs = NULL;
		}
	}
protected:
	LPCRITICAL_SECTION m_pcs;
};

#define vmsAUTOLOCKSECTION(sect) vmsCriticalSectionAutoLock csal_##sect (sect)
#define vmsAUTOLOCKSECTION_PTR(sect) vmsCriticalSectionAutoLock csal_##sect (sect)

#define vmsAUTOLOCKSECTION_UNLOCK(sect) csal_##sect.Unlock()
#define vmsAUTOLOCKSECTION_LOCK(sect) csal_##sect.Attach(sect)

#define vmsAUTOLOCKSECTION2(sectOwnerClassName, sect) vmsCriticalSectionAutoLock csal_##sectOwnerClassName_##sect (sectOwnerClassName::sect)
#define vmsAUTOLOCKSECTION2_UNLOCK(sectOwnerClassName, sect) csal_##sectOwnerClassName_##sect.Unlock()
#define vmsAUTOLOCKSECTION2_LOCK(sectOwnerClassName, sect) csal_##sectOwnerClassName_##sect.Attach(sectOwnerClassName::sect)

class vmsCriticalSectionEx : protected vmsCriticalSection
{
public:
	void Lock () 
	{
		::EnterCriticalSection (&m_cs);
		m_cLocks++;
	}
    
	void Unlock ()
    {
      	assert (m_cLocks > 0);
		m_cLocks--;
		::LeaveCriticalSection (&m_cs);
    }
    
	bool isLocked () const 
    {
       	return m_cLocks > 0;
    }

public:
	vmsCriticalSectionEx () {m_cLocks = 0;}
	virtual ~vmsCriticalSectionEx () {}

protected:
	int m_cLocks;
};

#endif