/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __RAWSTR_H
#define __RAWSTR_H 

#include <curl/curl.h> 

int Curl_raw_equal(const char *first, const char *second);
int Curl_raw_nequal(const char *first, const char *second, size_t max); 

#define checkprefix(a,b)    Curl_raw_nequal(a,b,strlen(a))

#endif
