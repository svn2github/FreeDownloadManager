/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VALUESPROVIDERBASEH
#define VALUESPROVIDERBASEH

#include <vector>

template<class T>
class ValuesProviderBase{
public:
	virtual std::vector<T> getValues() = 0;
	virtual ~ValuesProviderBase(){};
};
#endif