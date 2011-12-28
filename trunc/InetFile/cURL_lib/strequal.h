/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __STREQUAL_H
#define __STREQUAL_H


#include <curl/curl.h>

#define strequal(a,b) curl_strequal(a,b)
#define strnequal(a,b,c) curl_strnequal(a,b,c)

#ifndef HAVE_STRLCAT
#define strlcat(x,y,z) Curl_strlcat(x,y,z)
#endif
size_t strlcat(char *dst, const char *src, size_t siz);

#endif
