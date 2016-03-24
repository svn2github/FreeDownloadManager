/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsSimpleIpcItem.h"
class vmsSimpleIpcItemSender
{
public:
	enum 
	{
		send_crlf	= 1,
		send_data_len_as_string	= 1 << 1,
		send_uri_encoded		= 1 << 2,
		send_base64_encoded		= 1 << 3,
	};
public:
	vmsSimpleIpcItemSender ()
	{
	}

	virtual ~vmsSimpleIpcItemSender ()
	{
	}

	void set_flags (uint32_t flags, bool set = true)
	{
		if (set)
			m_flags |= flags;
		else
			m_flags &= ~flags;
	}

	bool send (const vmsSimpleIpcItem *item)
	{
		auto content = item->content ();
		if (m_flags & send_base64_encoded)
			content = base64::encode (content);
		else if (m_flags & send_uri_encoded)
			content = UriEncode (content);
		uint32_t size = (uint32_t)content.size ();
		if (m_flags & send_data_len_as_string)
		{
			std::stringstream ss;
			ss << size << " ";
			auto s = ss.str ();
			if (s.length () != fwrite (&s.front (), 1, s.length (), stdout))
				return false;
		}
		else
		{
			char sz [4];
			sz [0] = size & 0xFF;
			sz [1] = (size >> 8) & 0xFF;
			sz [2] = (size >> 16) & 0xFF;
			sz [3] = (size >> 24) & 0xFF;
			if (4 != fwrite (sz, 1, 4, stdout))
				return false;
		}		
		if (size != fwrite (&content.front (), 1, size, stdout))
			return false;
		if (m_flags & send_crlf)
		{
			if (2 != fwrite ("\r\n", 1, 2, stdout))
				return false;
		}
		fflush (stdout);
		return true;
	}

protected:
	uint32_t m_flags = 0;
};

