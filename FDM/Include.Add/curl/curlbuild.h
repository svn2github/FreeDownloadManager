/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_CURLBUILD_H
#define __CURL_CURLBUILD_H                 

#ifdef CURL_SIZEOF_LONG
#  error "CURL_SIZEOF_LONG shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_LONG_already_defined
#endif

#ifdef CURL_TYPEOF_CURL_OFF_T
#  error "CURL_TYPEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_TYPEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_T
#  error "CURL_FORMAT_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_TU
#  error "CURL_FORMAT_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_TU_already_defined
#endif

#ifdef CURL_FORMAT_OFF_T
#  error "CURL_FORMAT_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_OFF_T_already_defined
#endif

#ifdef CURL_SIZEOF_CURL_OFF_T
#  error "CURL_SIZEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_T
#  error "CURL_SUFFIX_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_TU
#  error "CURL_SUFFIX_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_TU_already_defined
#endif    

#if defined(__DJGPP__) || defined(__GO32__)
#  if defined(__DJGPP__) && (__DJGPP__ > 1)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  endif

#elif defined(__SALFORDC__)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  long
#  define CURL_FORMAT_CURL_OFF_T  "ld"
#  define CURL_FORMAT_CURL_OFF_TU "lu"
#  define CURL_FORMAT_OFF_T       "%ld"
#  define CURL_SIZEOF_CURL_OFF_T  4
#  define CURL_SUFFIX_CURL_OFF_T  L
#  define CURL_SUFFIX_CURL_OFF_TU UL

#elif defined(__BORLANDC__)
#  if (__BORLANDC__ < 0x520)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  __int64
#    define CURL_FORMAT_CURL_OFF_T  "I64d"
#    define CURL_FORMAT_CURL_OFF_TU "I64u"
#    define CURL_FORMAT_OFF_T       "%I64d"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  i64
#    define CURL_SUFFIX_CURL_OFF_TU ui64
#  endif

#elif defined(__TURBOC__)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  long
#  define CURL_FORMAT_CURL_OFF_T  "ld"
#  define CURL_FORMAT_CURL_OFF_TU "lu"
#  define CURL_FORMAT_OFF_T       "%ld"
#  define CURL_SIZEOF_CURL_OFF_T  4
#  define CURL_SUFFIX_CURL_OFF_T  L
#  define CURL_SUFFIX_CURL_OFF_TU UL

#elif defined(__WATCOMC__)
#  if defined(__386__)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  __int64
#    define CURL_FORMAT_CURL_OFF_T  "I64d"
#    define CURL_FORMAT_CURL_OFF_TU "I64u"
#    define CURL_FORMAT_OFF_T       "%I64d"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  i64
#    define CURL_SUFFIX_CURL_OFF_TU ui64
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  endif

#elif defined(__POCC__)
#  if (__POCC__ < 280)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  elif defined(_MSC_VER)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  __int64
#    define CURL_FORMAT_CURL_OFF_T  "I64d"
#    define CURL_FORMAT_CURL_OFF_TU "I64u"
#    define CURL_FORMAT_OFF_T       "%I64d"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  i64
#    define CURL_SUFFIX_CURL_OFF_TU ui64
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  endif

#elif defined(__LCC__)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  long
#  define CURL_FORMAT_CURL_OFF_T  "ld"
#  define CURL_FORMAT_CURL_OFF_TU "lu"
#  define CURL_FORMAT_OFF_T       "%ld"
#  define CURL_SIZEOF_CURL_OFF_T  4
#  define CURL_SUFFIX_CURL_OFF_T  L
#  define CURL_SUFFIX_CURL_OFF_TU UL

#elif defined(__SYMBIAN32__)
#  if defined(__EABI__)  
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  elif defined(__CW32__)
#    pragma longlong on
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  elif defined(__VC32__)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  __int64
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  endif

#elif defined(__MWERKS__)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  long long
#  define CURL_FORMAT_CURL_OFF_T  "lld"
#  define CURL_FORMAT_CURL_OFF_TU "llu"
#  define CURL_FORMAT_OFF_T       "%lld"
#  define CURL_SIZEOF_CURL_OFF_T  8
#  define CURL_SUFFIX_CURL_OFF_T  LL
#  define CURL_SUFFIX_CURL_OFF_TU ULL

#elif defined(_WIN32_WCE)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  __int64
#  define CURL_FORMAT_CURL_OFF_T  "I64d"
#  define CURL_FORMAT_CURL_OFF_TU "I64u"
#  define CURL_FORMAT_OFF_T       "%I64d"
#  define CURL_SIZEOF_CURL_OFF_T  8
#  define CURL_SUFFIX_CURL_OFF_T  i64
#  define CURL_SUFFIX_CURL_OFF_TU ui64

#elif defined(__MINGW32__)
#  define CURL_SIZEOF_LONG        4
#  define CURL_TYPEOF_CURL_OFF_T  long long
#  define CURL_FORMAT_CURL_OFF_T  "I64d"
#  define CURL_FORMAT_CURL_OFF_TU "I64u"
#  define CURL_FORMAT_OFF_T       "%I64d"
#  define CURL_SIZEOF_CURL_OFF_T  8
#  define CURL_SUFFIX_CURL_OFF_T  LL
#  define CURL_SUFFIX_CURL_OFF_TU ULL

#elif defined(__VMS)
#  if defined(__alpha) || defined(__ia64)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  endif

#elif defined(__OS400__)
#  if defined(__ILEC400__)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  endif

#elif defined(__MVS__)
#  if defined(__IBMC__) || defined(__IBMCPP__)
#    if defined(_ILP32)
#      define CURL_SIZEOF_LONG        4
#    elif defined(_LP64)
#      define CURL_SIZEOF_LONG        8
#    endif
#    if defined(_LONG_LONG)
#      define CURL_TYPEOF_CURL_OFF_T  long long
#      define CURL_FORMAT_CURL_OFF_T  "lld"
#      define CURL_FORMAT_CURL_OFF_TU "llu"
#      define CURL_FORMAT_OFF_T       "%lld"
#      define CURL_SIZEOF_CURL_OFF_T  8
#      define CURL_SUFFIX_CURL_OFF_T  LL
#      define CURL_SUFFIX_CURL_OFF_TU ULL
#    elif defined(_LP64)
#      define CURL_TYPEOF_CURL_OFF_T  long
#      define CURL_FORMAT_CURL_OFF_T  "ld"
#      define CURL_FORMAT_CURL_OFF_TU "lu"
#      define CURL_FORMAT_OFF_T       "%ld"
#      define CURL_SIZEOF_CURL_OFF_T  8
#      define CURL_SUFFIX_CURL_OFF_T  L
#      define CURL_SUFFIX_CURL_OFF_TU UL
#    else
#      define CURL_TYPEOF_CURL_OFF_T  long
#      define CURL_FORMAT_CURL_OFF_T  "ld"
#      define CURL_FORMAT_CURL_OFF_TU "lu"
#      define CURL_FORMAT_OFF_T       "%ld"
#      define CURL_SIZEOF_CURL_OFF_T  4
#      define CURL_SUFFIX_CURL_OFF_T  L
#      define CURL_SUFFIX_CURL_OFF_TU UL
#    endif
#  endif

#elif defined(__370__)
#  if defined(__IBMC__) || defined(__IBMCPP__)
#    if defined(_ILP32)
#      define CURL_SIZEOF_LONG        4
#    elif defined(_LP64)
#      define CURL_SIZEOF_LONG        8
#    endif
#    if defined(_LONG_LONG)
#      define CURL_TYPEOF_CURL_OFF_T  long long
#      define CURL_FORMAT_CURL_OFF_T  "lld"
#      define CURL_FORMAT_CURL_OFF_TU "llu"
#      define CURL_FORMAT_OFF_T       "%lld"
#      define CURL_SIZEOF_CURL_OFF_T  8
#      define CURL_SUFFIX_CURL_OFF_T  LL
#      define CURL_SUFFIX_CURL_OFF_TU ULL
#    elif defined(_LP64)
#      define CURL_TYPEOF_CURL_OFF_T  long
#      define CURL_FORMAT_CURL_OFF_T  "ld"
#      define CURL_FORMAT_CURL_OFF_TU "lu"
#      define CURL_FORMAT_OFF_T       "%ld"
#      define CURL_SIZEOF_CURL_OFF_T  8
#      define CURL_SUFFIX_CURL_OFF_T  L
#      define CURL_SUFFIX_CURL_OFF_TU UL
#    else
#      define CURL_TYPEOF_CURL_OFF_T  long
#      define CURL_FORMAT_CURL_OFF_T  "ld"
#      define CURL_FORMAT_CURL_OFF_TU "lu"
#      define CURL_FORMAT_OFF_T       "%ld"
#      define CURL_SIZEOF_CURL_OFF_T  4
#      define CURL_SUFFIX_CURL_OFF_T  L
#      define CURL_SUFFIX_CURL_OFF_TU UL
#    endif
#  endif    

#elif defined(_MSC_VER)
#  if (_MSC_VER >= 900) && (_INTEGRAL_MAX_BITS >= 64)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  __int64
#    define CURL_FORMAT_CURL_OFF_T  "I64d"
#    define CURL_FORMAT_CURL_OFF_TU "I64u"
#    define CURL_FORMAT_OFF_T       "%I64d"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  i64
#    define CURL_SUFFIX_CURL_OFF_TU ui64
#  else
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  4
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  endif    

#elif defined(__GNUC__)
#  if defined(__i386__)
#    define CURL_SIZEOF_LONG        4
#    define CURL_TYPEOF_CURL_OFF_T  long long
#    define CURL_FORMAT_CURL_OFF_T  "lld"
#    define CURL_FORMAT_CURL_OFF_TU "llu"
#    define CURL_FORMAT_OFF_T       "%lld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  LL
#    define CURL_SUFFIX_CURL_OFF_TU ULL
#  elif defined(__x86_64__)
#    define CURL_SIZEOF_LONG        8
#    define CURL_TYPEOF_CURL_OFF_T  long
#    define CURL_FORMAT_CURL_OFF_T  "ld"
#    define CURL_FORMAT_CURL_OFF_TU "lu"
#    define CURL_FORMAT_OFF_T       "%ld"
#    define CURL_SIZEOF_CURL_OFF_T  8
#    define CURL_SUFFIX_CURL_OFF_T  L
#    define CURL_SUFFIX_CURL_OFF_TU UL
#  endif

#else
#  error "Unknown non-configure build target!"
   Error Compilation_aborted_Unknown_non_configure_build_target
#endif  

#ifdef CURL_TYPEOF_CURL_OFF_T
  typedef CURL_TYPEOF_CURL_OFF_T curl_off_t;
#endif

#endif 
