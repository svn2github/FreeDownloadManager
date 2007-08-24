/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#include <string>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

namespace libtorrent
{
	std::string unescape_string(std::string const& s)
	{
		std::string ret;
		for (std::string::const_iterator i = s.begin(); i != s.end(); ++i)
		{
			if(*i == '+')
			{
				ret += ' ';
			}
			else if (*i != '%')
			{
				ret += *i;
			}
			else
			{
				++i;
				if (i == s.end())
					throw std::runtime_error("invalid escaped string");

				int high;
				if(*i >= '0' && *i <= '9') high = *i - '0';
				else if(*i >= 'A' && *i <= 'F') high = *i + 10 - 'A';
				else if(*i >= 'a' && *i <= 'f') high = *i + 10 - 'a';
				else throw std::runtime_error("invalid escaped string");

				++i;
				if (i == s.end())
					throw std::runtime_error("invalid escaped string");

				int low;
				if(*i >= '0' && *i <= '9') low = *i - '0';
				else if(*i >= 'A' && *i <= 'F') low = *i + 10 - 'A';
				else if(*i >= 'a' && *i <= 'f') low = *i + 10 - 'a';
				else throw std::runtime_error("invalid escaped string");

				ret += char(high * 16 + low);
			}
		}
		return ret;
	} 

	std::string escape_string(const char* str, int len)
	{
		assert(str != 0);
		assert(len >= 0);
		
		
		

		static const char unreserved_chars[] = "-_.!~*()"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
			"0123456789";

		std::stringstream ret;
		ret << std::hex << std::setfill('0');
		for (int i = 0; i < len; ++i)
		{
			if (std::count(
					unreserved_chars
					, unreserved_chars+sizeof(unreserved_chars)-1
					, *str))
			{
				ret << *str;
			}
			else
			{
				ret << '%'
					<< std::setw(2)
					<< (int)static_cast<unsigned char>(*str);
			}
			++str;
		}
		return ret.str();
	}
	
	std::string escape_path(const char* str, int len)
	{
		assert(str != 0);
		assert(len >= 0);
		static const char unreserved_chars[] = "/-_.!~*()"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
			"0123456789";

		std::stringstream ret;
		ret << std::hex << std::setfill('0');
		for (int i = 0; i < len; ++i)
		{
			if (std::count(
					unreserved_chars
					, unreserved_chars+sizeof(unreserved_chars)-1
					, *str))
			{
				ret << *str;
			}
			else
			{
				ret << '%'
					<< std::setw(2)
					<< (int)static_cast<unsigned char>(*str);
			}
			++str;
		}
		return ret.str();
	}
}
