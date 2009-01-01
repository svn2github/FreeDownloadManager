/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_STRDUP_H
#define __CURL_STRDUP_H

#include "setup.h"

#ifndef HAVE_STRDUP
extern char *curlx_strdup(const char *str);
#endif

#endif

