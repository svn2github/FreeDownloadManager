/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_CURLRULES_H
#define __CURL_CURLRULES_H         

#ifndef CURL_SIZEOF_LONG
#  error "CURL_SIZEOF_LONG definition is missing!"
   Error Compilation_aborted_CURL_SIZEOF_LONG_is_missing
#endif

#ifndef CURL_TYPEOF_CURL_OFF_T
#  error "CURL_TYPEOF_CURL_OFF_T definition is missing!"
   Error Compilation_aborted_CURL_TYPEOF_CURL_OFF_T_is_missing
#endif

#ifndef CURL_FORMAT_CURL_OFF_T
#  error "CURL_FORMAT_CURL_OFF_T definition is missing!"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_T_is_missing
#endif

#ifndef CURL_FORMAT_CURL_OFF_TU
#  error "CURL_FORMAT_CURL_OFF_TU definition is missing!"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_TU_is_missing
#endif

#ifndef CURL_FORMAT_OFF_T
#  error "CURL_FORMAT_OFF_T definition is missing!"
   Error Compilation_aborted_CURL_FORMAT_OFF_T_is_missing
#endif

#ifndef CURL_SIZEOF_CURL_OFF_T
#  error "CURL_SIZEOF_CURL_OFF_T definition is missing!"
   Error Compilation_aborted_CURL_SIZEOF_CURL_OFF_T_is_missing
#endif

#ifndef CURL_SUFFIX_CURL_OFF_T
#  error "CURL_SUFFIX_CURL_OFF_T definition is missing!"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_T_is_missing
#endif

#ifndef CURL_SUFFIX_CURL_OFF_TU
#  error "CURL_SUFFIX_CURL_OFF_TU definition is missing!"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_TU_is_missing
#endif  

#define CurlchkszEQ(t, s) sizeof(t) == s ? 1 : -1

#define CurlchkszGE(t1, t2) sizeof(t1) >= sizeof(t2) ? 1 : -1  

typedef char
  __curl_rule_01__
    [CurlchkszEQ(long, CURL_SIZEOF_LONG)];  

typedef char
  __curl_rule_02__
    [CurlchkszEQ(curl_off_t, CURL_SIZEOF_CURL_OFF_T)];  

typedef char
  __curl_rule_03__
    [CurlchkszGE(curl_off_t, long)];        

#if defined(__STDC__) || defined(_MSC_VER) || defined(__cplusplus) || \
  defined(__HP_aCC) || defined(__BORLANDC__) || defined(__LCC__) || \
  defined(__POCC__) || defined(__SALFORDC__) || defined(__HIGHC__) || \
  defined(__ILEC400__)
  
#define CURL_ISOCPP
#else
  
#undef CURL_ISOCPP
#endif  

#ifdef CURL_ISOCPP
#  define __CURL_OFF_T_C_HELPER2(Val,Suffix) Val ## Suffix
#else
#  define __CURL_OFF_T_C_HELPER2(Val,Suffix) ValSuffix
#endif
#define __CURL_OFF_T_C_HELPER1(Val,Suffix) __CURL_OFF_T_C_HELPER2(Val,Suffix)
#define CURL_OFF_T_C(Val)  __CURL_OFF_T_C_HELPER1(Val,CURL_SUFFIX_CURL_OFF_T)
#define CURL_OFF_TU_C(Val) __CURL_OFF_T_C_HELPER1(Val,CURL_SUFFIX_CURL_OFF_TU)  

#undef CurlchkszEQ
#undef CurlchkszGE  

#undef CURL_PULL_SYS_TYPES_H
#undef CURL_PULL_STDINT_H
#undef CURL_PULL_INTTYPES_H

#undef CURL_TYPEOF_CURL_OFF_T

#endif 
