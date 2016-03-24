/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

template <class TLockable>
class vmsLockableAutolock
{
public:
	vmsLockableAutolock (const TLockable *pObject) : 
	  m_pObject (pObject)
	{
		if (m_pObject)
			lock ();
	}

	~vmsLockableAutolock ()
	{
		unlock ();
	}

	void lock ()
	{
		if (m_pObject && !m_locked)
		{
			m_pObject->Lock ();
			m_locked = true;
		}
	}

	void unlock ()
	{
		if (m_pObject && m_locked)
		{
			m_pObject->Unlock ();
			m_locked = false;
		}
	}

protected:
	const TLockable *m_pObject;
	bool m_locked = false;

private:
	vmsLockableAutolock (const vmsLockableAutolock&);
	vmsLockableAutolock& operator= (const vmsLockableAutolock&);
};

class vmsLockable
{
public:
	typedef vmsLockableAutolock <vmsLockable> tAutolock;
	typedef std::shared_ptr <tAutolock> tspAutolock;
public:
	
	
	virtual void Lock () const {}
	virtual void Unlock () const {}
	
	virtual tspAutolock LockAuto () const
	{
		return std::make_shared <tAutolock> (this);
	}
};

#define vmsLockable_ImplementDelegatedTo(obj) \
	virtual void Lock () const override {obj.Lock ();} \
	virtual void Unlock () const override {obj.Unlock ();}

#define vmsLockable_ImplementDelegatedTo2(obj) \
	virtual void Lock () const override {obj.lock ();} \
	virtual void Unlock () const override {obj.unlock ();}

#define vmsLockableScope vmsLockableAutolock<vmsLockable> lockObj (this)
#define vmsLockableScopeUnlock lockObj.unlock ()
