/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef _CURL_STRTOOFFT_H
#define _CURL_STRTOOFFT_H   

#include "setup.h"
#include <stddef.h>
#include <curl/curl.h>  

#if (SIZEOF_CURL_OFF_T > 4) && (SIZEOF_LONG < 8)
#if HAVE_STRTOLL
#define curlx_strtoofft strtoll
#else  

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#define curlx_strtoofft _strtoi64
#else 
curl_off_t curlx_strtoll(const char *nptr, char **endptr, int base);
#define curlx_strtoofft curlx_strtoll
#define NEED_CURL_STRTOLL
#endif 

#endif 
#else 

#define curlx_strtoofft strtol
#endif

#if defined(_MSC_VER) || defined(__WATCOMC__)
#define CURL_LLONG_MIN 0x8000000000000000i64
#define CURL_LLONG_MAX 0x7FFFFFFFFFFFFFFFi64
#elif defined(HAVE_LL)
#define CURL_LLONG_MIN 0x8000000000000000LL
#define CURL_LLONG_MAX 0x7FFFFFFFFFFFFFFFLL
#else
#define CURL_LLONG_MIN 0x8000000000000000L
#define CURL_LLONG_MAX 0x7FFFFFFFFFFFFFFFL
#endif

#endif

