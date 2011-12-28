/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __PROPVARIANTCONVERSIONS_H
#define __PROPVARIANTCONVERSIONS_H

#include "Types.h"
#include "String.h"

bool ConvertFileTimeToString(const FILETIME &ft, char *s, bool includeTime = true, bool includeSeconds = true);
UString ConvertFileTimeToString(const FILETIME &ft, bool includeTime = true, bool includeSeconds = true);
UString ConvertPropVariantToString(const PROPVARIANT &propVariant);
UInt64 ConvertPropVariantToUInt64(const PROPVARIANT &propVariant);

#endif
