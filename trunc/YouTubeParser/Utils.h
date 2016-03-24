/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef UTILS_H
#define UTILS_H
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "tstring.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

template<class T>
class CUtf8ToTStrBufferStorage
{
public:
	CUtf8ToTStrBufferStorage() {}
	virtual ~CUtf8ToTStrBufferStorage()
	{
		size_t szIndex = 0;
		for (szIndex = 0; szIndex < m_strings.size(); ++szIndex) {
			delete m_strings[szIndex];
		}
	}
	void Add(T* t)
	{
		m_strings.push_back(t);
	}
	T* Get()
	{
		return m_strings.back();
	}

protected:
	std::vector<T*> m_strings;
};

TCHAR* Utf8ToTStrHelper(const char* szSrc, CUtf8ToTStrBufferStorage<TCHAR>& utsbsStorage);

#define USES_CONVERSION_FROM_UTF8 CUtf8ToTStrBufferStorage<TCHAR> utsbsUtf8ToTStrBufferStorage
#define U2T(x) Utf8ToTStrHelper(x, utsbsUtf8ToTStrBufferStorage)

#define array_length(array) (sizeof (array) / sizeof (array)[0])

tstring HTMLDecode( const tstring& s );

bool hasEnding (tstring const &fullString, tstring const &ending);
void print_tree(const boost::property_tree::wptree& pt, int level);
void print_tree(const boost::property_tree::wptree& pt);

#endif