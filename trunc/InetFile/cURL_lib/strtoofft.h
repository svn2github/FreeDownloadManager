/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef _CURL_STRTOOFFT_H
#define _CURL_STRTOOFFT_H


#include "setup.h"



#if (CURL_SIZEOF_CURL_OFF_T > CURL_SIZEOF_LONG)
#  ifdef HAVE_STRTOLL
#    define curlx_strtoofft strtoll
#  else
#    if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_INTEGRAL_MAX_BITS >= 64)
       _CRTIMP __int64 __cdecl _strtoi64(const char *, char **, int);
#      define curlx_strtoofft _strtoi64
#    else
       curl_off_t curlx_strtoll(const char *nptr, char **endptr, int base);
#      define curlx_strtoofft curlx_strtoll
#      define NEED_CURL_STRTOLL 1
#    endif
#  endif
#else
#  define curlx_strtoofft strtol
#endif

#define CURL_LLONG_MAX CURL_OFF_T_C(0x7FFFFFFFFFFFFFFF)
#define CURL_LLONG_MIN (-CURL_LLONG_MAX - CURL_OFF_T_C(1))

#endif

