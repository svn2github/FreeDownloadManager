/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsThreadSafe
{
public:
	typedef std::shared_ptr <vmsCriticalSectionAutoLock> tAutoLock;
	virtual void Lock (vmsCriticalSectionAutoLock &csal) {csal.Attach (m_csObject);}
	virtual tAutoLock Lock () {return tAutoLock (new vmsCriticalSectionAutoLock (m_csObject));}
	vmsThreadSafe () {}
protected:
	mutable vmsCriticalSection m_csObject;
private:
	vmsThreadSafe& operator = (vmsThreadSafe&);
	vmsThreadSafe (const vmsThreadSafe&);
};

#define vmsTHREAD_SAFE_SCOPE vmsAUTOLOCKSECTION(m_csObject)
#define vmsTHREAD_SAFE_SCOPE_UNLOCK vmsAUTOLOCKSECTION_UNLOCK(m_csObject)
#define vmsTHREAD_SAFE_SCOPE_LOCK vmsAUTOLOCKSECTION_LOCK(m_csObject)

#define vmsTHREAD_SAFE_SCOPE2(baseClassName) vmsAUTOLOCKSECTION2(baseClassName, m_csObject)
#define vmsTHREAD_SAFE_SCOPE2_UNLOCK(baseClassName) vmsAUTOLOCKSECTION2_UNLOCK(baseClassName, m_csObject)
#define vmsTHREAD_SAFE_SCOPE2_LOCK(baseClassName) vmsAUTOLOCKSECTION2_LOCK(baseClassName, m_csObject)

