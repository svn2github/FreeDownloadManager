/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsSerializationInputStreamBinding
{
public:
	vmsSerializationInputStreamBinding ()
	{

	}

	virtual ~vmsSerializationInputStreamBinding ()
	{

	}

	
	virtual bool BindToStream (std::shared_ptr <std::istream> spStream)
	{
		m_spBoundStream = spStream;
		return true;
	}
	
protected:
	std::shared_ptr <std::istream> m_spBoundStream;
};

class vmsSerializationOutputStreamBinding
{
public:
	vmsSerializationOutputStreamBinding () : m_bFlushedToBoundStream (false)
	{

	}

	virtual ~vmsSerializationOutputStreamBinding ()
	{

	}

	virtual bool BindToStream (std::shared_ptr <std::ostream> spStream)
	{
		m_spBoundStream = spStream;
		m_bFlushedToBoundStream = false;
		return true;
	}

	
	virtual bool Flush ()
	{
		m_bFlushedToBoundStream = true;
		return true;
	}

protected:
	std::shared_ptr <std::ostream> m_spBoundStream;
	bool m_bFlushedToBoundStream;
};