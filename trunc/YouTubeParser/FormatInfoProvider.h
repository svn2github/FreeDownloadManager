/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef FORMATINFOPROVIDERH
#define FORMATINFOPROVIDERH
#include "ValuesProviderBase.h"
#include "FormatInfo.h"
class FormatInfoProvider : public ValuesProviderBase<FormatInfo>{
public:
	std::vector<FormatInfo> getValues();
	~FormatInfoProvider(){};
};
#endif