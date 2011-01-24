/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/    

#ifndef __COMMON_INTTOSTRING_H
#define __COMMON_INTTOSTRING_H

#include <stddef.h>
#include "Types.h"

void ConvertUInt64ToString(UInt64 value, char *s, UInt32 base = 10);
void ConvertUInt64ToString(UInt64 value, wchar_t *s);

void ConvertInt64ToString(Int64 value, char *s);
void ConvertInt64ToString(Int64 value, wchar_t *s);

#endif
