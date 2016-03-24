/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <boost/thread/lockable_concepts.hpp>

template <class T>
class vmsResourceObjectDestructor
{
public:
	virtual void ReleaseResourceObject (T* obj) = 0;
	virtual ~vmsResourceObjectDestructor () {}
};

template <class T>
class vmsResourceObjectHolder
{
public:
	vmsResourceObjectHolder (T* object, std::weak_ptr <vmsResourceObjectDestructor<T>> destructor) :
		m_object (object), m_destructor (destructor)
	{

	}

	~vmsResourceObjectHolder ()
	{
		auto destructor = m_destructor.lock ();
		if (destructor)
			destructor->ReleaseResourceObject (m_object);
	}

	T* object () {return m_object;}

protected:
	T* m_object;
	std::weak_ptr <vmsResourceObjectDestructor<T>> m_destructor;
};

class vmsResourceObjectsHolderController 
{
public:
	
	
	virtual void waitForResourceNotBusy () = 0;
	
	
	
	virtual void releaseResourceReference () = 0;
	
	
	
	virtual void loadResource (const std::wstring& resourceLocation) = 0;
	virtual ~vmsResourceObjectsHolderController () {}
};

template <class T>
class vmsResourceObjectsHolder : public vmsResourceObjectDestructor <T>,
	public vmsResourceObjectsHolderController,
	public std::enable_shared_from_this <vmsResourceObjectsHolder<T>>
{
public:
	virtual void ReleaseResourceObject (T* obj) override
	{
		boost::mutex::scoped_lock lock (m_mutex);
		unregisterAndReleaseRawResourceObject (obj);
	}

	virtual void waitForResourceNotBusy () override
	{
		boost::unique_lock <boost::mutex> lock (m_mutex);
		while (!m_objects.empty ())
			m_cvResourceNotBusy.wait (lock);
		lock.release ();
	}

protected:
	std::unique_ptr <vmsResourceObjectHolder<T>> registerRawResourceObject (T* obj)
	{
		assert (obj);
		if (!obj)
			return nullptr;
		m_objects.insert (obj);
		return std::unique_ptr <vmsResourceObjectHolder<T>> (
			new vmsResourceObjectHolder<T> (obj, shared_from_this ()));
	}

	void unregisterAndReleaseRawResourceObject (T* obj)
	{
		m_objects.erase (obj);
		releaseRawResourceObject (obj);
		if (m_objects.empty ())
			m_cvResourceNotBusy.notify_all ();
	}

	virtual void releaseRawResourceObject (T* obj) 
	{
		delete obj;
	}

protected:
	std::set <T*> m_objects;
	boost::mutex m_mutex;
	boost::condition_variable m_cvResourceNotBusy;
};