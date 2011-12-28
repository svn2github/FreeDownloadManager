/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __LIB_CURL_SETUP_H
#define __LIB_CURL_SETUP_H




#if (defined(_WIN32) || defined(__WIN32__)) && !defined(WIN32) && !defined(__SYMBIAN32__)
#define WIN32
#endif



#ifdef HAVE_CONFIG_H
#include "config.h"
#else

#ifdef _WIN32_WCE
#include "config-win32ce.h"
#else
#ifdef WIN32
#include "config-win32.h"
#endif
#endif

#if defined(macintosh) && defined(__MRC__)
#  include "config-mac.h"
#endif

#ifdef __AMIGA__
#include "amigaos.h"
#endif

#ifdef __SYMBIAN32__
#include "config-symbian.h"
#endif

#ifdef __OS400__
#  include "config-os400.h"
#endif

#ifdef TPF
#include "config-tpf.h" 

#define select(a,b,c,d,e) tpf_select_libcurl(a,b,c,d,e)
#endif

#endif 












#ifdef NEED_REENTRANT
#  ifndef _REENTRANT
#    define _REENTRANT
#  endif
#endif








#include <curl/curlbuild.h>



#include <curl/curlrules.h>



#ifdef SIZEOF_CURL_OFF_T
#  error "SIZEOF_CURL_OFF_T shall not be defined!"
   Error Compilation_aborted_SIZEOF_CURL_OFF_T_shall_not_be_defined
#endif



#ifdef FORMAT_OFF_T
#  error "FORMAT_OFF_T shall not be defined before this point!"
   Error Compilation_aborted_FORMAT_OFF_T_already_defined
#endif

#ifdef FORMAT_OFF_TU
#  error "FORMAT_OFF_TU shall not be defined before this point!"
   Error Compilation_aborted_FORMAT_OFF_TU_already_defined
#endif

#if (CURL_SIZEOF_CURL_OFF_T > CURL_SIZEOF_LONG)
#  define FORMAT_OFF_T  "lld"
#  define FORMAT_OFF_TU "llu"
#else
#  define FORMAT_OFF_T  "ld"
#  define FORMAT_OFF_TU "lu"
#endif



#ifdef HTTP_ONLY
#  define CURL_DISABLE_TFTP
#  define CURL_DISABLE_FTP
#  define CURL_DISABLE_LDAP
#  define CURL_DISABLE_TELNET
#  define CURL_DISABLE_DICT
#  define CURL_DISABLE_FILE
#endif








#ifdef __OS400__
#  include "setup-os400.h"
#endif



#ifdef HAVE_WINDOWS_H
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#  ifdef HAVE_WINSOCK2_H
#    include <winsock2.h>
#    ifdef HAVE_WS2TCPIP_H
#       include <ws2tcpip.h>
#    endif
#  else
#    ifdef HAVE_WINSOCK_H
#      include <winsock.h>
#    endif
#  endif
#endif



#undef USE_WINSOCK

#ifdef HAVE_WINSOCK2_H
#  define USE_WINSOCK 2
#else
#  ifdef HAVE_WINSOCK_H
#    define USE_WINSOCK 1
#  endif
#endif

#ifdef HAVE_EXTRA_STRICMP_H
#  include <extra/stricmp.h>
#endif

#ifdef HAVE_EXTRA_STRDUP_H
#  include <extra/strdup.h>
#endif

#ifdef TPF
#  include <strings.h>    
#  include <string.h>     
#  include <stdlib.h>     
#  include <sys/socket.h> 
#  include <netdb.h>      
#  include <tpf/sysapi.h> 
#endif

#include <stdio.h>
#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif
#include <errno.h>

#ifdef __TANDEM 
#include <floss.h>
#endif

#ifndef STDC_HEADERS 
#include <curl/stdcheaders.h>
#endif


#ifdef __POCC__
  #include <sys/types.h>
  #include <unistd.h>
  #if (__POCC__ <= 400)
  #define sys_nerr EILSEQ  
  #define EINTR    -1      
  #endif
#endif


#ifdef __SALFORDC__
  #pragma suppress 353             
  #pragma suppress 593             
  #pragma suppress 61              
  #pragma suppress 106             
  #include <clib.h>
#endif



#ifdef USE_WIN32_LARGE_FILES
#  include <io.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  define lseek(fdes,offset,whence)  _lseeki64(fdes, offset, whence)
#  define fstat(fdes,stp)            _fstati64(fdes, stp)
#  define stat(fname,stp)            _stati64(fname, stp)
#  define struct_stat                struct _stati64
#endif



#ifdef USE_WIN32_SMALL_FILES
#  include <io.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  define lseek(fdes,offset,whence)  _lseek(fdes, (long)offset, whence)
#  define fstat(fdes,stp)            _fstat(fdes, stp)
#  define stat(fname,stp)            _stat(fname, stp)
#  define struct_stat                struct _stat
#endif

#ifndef struct_stat
#  define struct_stat struct stat
#endif



#ifndef SIZEOF_OFF_T
#  if defined(__VMS) && (defined(__alpha) || defined(__ia64))
#    if defined(_LARGEFILE)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__OS400__) && defined(__ILEC400__)
#    if defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__MVS__) && defined(__IBMC__)
#    if defined(_LP64) || defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  elif defined(__370__) && defined(__IBMC__)
#    if defined(_LP64) || defined(_LARGE_FILES)
#      define SIZEOF_OFF_T 8
#    endif
#  endif
#  ifndef SIZEOF_OFF_T
#    define SIZEOF_OFF_T 4
#  endif
#endif



#ifdef WIN32

#  if !defined(__CYGWIN__)
#    define sclose(x) closesocket(x)
#  else
#    define sclose(x) close(x)
#  endif

#  define DIR_CHAR      "\\"
#  define DOT_CHAR      "_"

#else 

#  ifdef MSDOS  

#    include <sys/ioctl.h>
#    define sclose(x)         close_s(x)
#    define select(n,r,w,x,t) select_s(n,r,w,x,t)
#    define ioctl(x,y,z) ioctlsocket(x,y,(char *)(z))
#    define IOCTL_3_ARGS
#    include <tcp.h>
#    ifdef word
#      undef word
#    endif
#    ifdef byte
#      undef byte
#    endif

#  else 

#    ifdef __BEOS__
#      define sclose(x) closesocket(x)
#    else 
#      define sclose(x) close(x)
#    endif 

#  endif 

#  ifdef _AMIGASF
#    undef sclose
#    define sclose(x) CloseSocket(x)
#  endif

#  ifdef __minix
     
     extern char * strtok_r(char *s, const char *delim, char **last);
     extern struct tm * gmtime_r(const time_t * const timep, struct tm *tmp);
#  endif

#  define DIR_CHAR      "/"
#  ifndef DOT_CHAR
#    define DOT_CHAR      "."
#  endif

#  ifdef MSDOS
#    undef DOT_CHAR
#    define DOT_CHAR      "_"
#  endif

#  ifndef fileno 
     int fileno( FILE *stream);
#  endif

#endif 

#if defined(WIN32) && !defined(__CYGWIN__) && !defined(USE_ARES) && \
    !defined(__LCC__)  
#ifdef ENABLE_IPV6
#define USE_THREADING_GETADDRINFO
#else
#define USE_THREADING_GETHOSTBYNAME  
#endif
#endif


#if (defined(_MSC_VER) && !defined(__POCC__)) && !defined(_MT) && !defined(USE_ARES)
#undef USE_THREADING_GETADDRINFO
#undef USE_THREADING_GETHOSTBYNAME
#define CURL_NO__BEGINTHREADEX
#endif



#ifdef _MSC_VER
#if !defined(HAVE_WINSOCK2_H) || ((_MSC_VER < 1300) && !defined(IPPROTO_ESP))
#undef HAVE_STRUCT_SOCKADDR_STORAGE
#endif
#endif

#ifdef mpeix
#define IOCTL_3_ARGS
#endif

#ifdef NETWARE
int netware_init(void);
#ifndef __NOVELL_LIBC__
#include <sys/bsdskt.h>
#include <sys/timeval.h>
#endif
#endif

#if defined(HAVE_LIBIDN) && defined(HAVE_TLD_H)

#define USE_LIBIDN
#endif

#ifndef SIZEOF_TIME_T

#define SIZEOF_TIME_T 4
#endif

#define LIBIDN_REQUIRED_VERSION "0.4.1"

#if defined(USE_GNUTLS) || defined(USE_SSLEAY) || defined(USE_NSS) || defined(USE_QSOSSL)
#define USE_SSL    
#endif

#if !defined(CURL_DISABLE_HTTP) && !defined(CURL_DISABLE_NTLM)
#if defined(USE_SSLEAY) || defined(USE_WINDOWS_SSPI)
#define USE_NTLM
#endif
#endif


#if defined(CURL_WANTS_CA_BUNDLE_ENV) && !defined(CURL_CA_BUNDLE)
#define CURL_CA_BUNDLE getenv("CURL_CA_BUNDLE")
#endif



#ifndef __SETUP_ONCE_H
#include "setup_once.h"
#endif

#include "Ws2tcpip.h"
#include "Wspiapi.h"

#endif 
