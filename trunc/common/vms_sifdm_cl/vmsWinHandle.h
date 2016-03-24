/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsWinHandle
{
public:
	typedef std::shared_ptr <vmsWinHandle> tSP;

public:
	vmsWinHandle (HANDLE h = NULL) : 
	  m_handle (h) {}

	~vmsWinHandle () 
	{
		Close ();
	}

	void Close () 
	{
		if (m_handle)
		{
			CloseHandle (m_handle);
			m_handle = NULL;
		}
	}

	HANDLE getHandle () const 
	{
		return m_handle;
	}

	void setHandle (HANDLE h)
	{
		Close ();
		m_handle = h;
	}

	HANDLE detach ()
	{
		HANDLE h = m_handle;
		m_handle = NULL;
		return h;
	}

	operator HANDLE () const {return m_handle;}

public:
	HANDLE m_handle;

private:
	vmsWinHandle (const vmsWinHandle&);
	const vmsWinHandle& operator = (const vmsWinHandle&);
};