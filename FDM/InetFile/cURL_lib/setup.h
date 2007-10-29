/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LIB_CURL_SETUP_H
#define __LIB_CURL_SETUP_H 

#ifdef HTTP_ONLY
#define CURL_DISABLE_TFTP
#define CURL_DISABLE_FTP
#define CURL_DISABLE_LDAP
#define CURL_DISABLE_TELNET
#define CURL_DISABLE_DICT
#define CURL_DISABLE_FILE
#endif 

#if !defined(WIN32) && defined(__WIN32__)

#define WIN32
#endif

#if !defined(WIN32) && defined(_WIN32)

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

#ifdef macintosh
#include "config-mac.h"
#endif

#ifdef __AMIGA__
#include "amigaos.h"
#endif

#ifdef TPF
#include "config-tpf.h" 

#define select(a,b,c,d,e) tpf_select_libcurl(a,b,c,d,e)
#endif

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

#ifdef HAVE_LONGLONG
#define LONG_LONG long long
#define ENABLE_64BIT
#else
#ifdef _MSC_VER
#define LONG_LONG __int64
#define ENABLE_64BIT
#endif 
#endif 

#ifndef SIZEOF_CURL_OFF_T

#define SIZEOF_CURL_OFF_T 4
#endif 

#if SIZEOF_CURL_OFF_T > 4
#define FORMAT_OFF_T "lld"
#else
#define FORMAT_OFF_T "ld"
#endif 

#ifndef _REENTRANT

#define _REENTRANT
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

#if defined(WIN32) && (SIZEOF_CURL_OFF_T > 4)
#include <sys/stat.h>   
#include <io.h>
#define lseek(x,y,z) _lseeki64(x, y, z)
#define struct_stat struct _stati64
#define stat(file,st) _stati64(file,st)
#define fstat(fd,st) _fstati64(fd,st)
#else
#define struct_stat struct stat
#endif    

#ifdef WIN32

#if !defined(__CYGWIN__)
#define sclose(x) closesocket(x)

#undef HAVE_ALARM
#else
     
#define sclose(x) close(x)
#define HAVE_ALARM
#endif 

#define DIR_CHAR      "\\"
#define DOT_CHAR      "_"

#else 

#ifdef MSDOS  
#include <sys/ioctl.h>
#define sclose(x)         close_s(x)
#define select(n,r,w,x,t) select_s(n,r,w,x,t)
#define ioctl(x,y,z) ioctlsocket(x,y,(char *)(z))
#define IOCTL_3_ARGS
#include <tcp.h>
#ifdef word
#undef word
#endif

#else 

#ifdef __BEOS__
#define sclose(x) closesocket(x)
#else 
#define sclose(x) close(x)
#endif 

#define HAVE_ALARM

#endif 

#ifdef _AMIGASF
#undef HAVE_ALARM
#undef sclose
#define sclose(x) CloseSocket(x)
#endif

#ifdef __minix

extern char * strtok_r(char *s, const char *delim, char **last);
extern struct tm * gmtime_r(const time_t * const timep, struct tm *tmp);
#endif

#define DIR_CHAR      "/"
#ifndef DOT_CHAR
#define DOT_CHAR      "."
#endif

#ifdef MSDOS
#undef DOT_CHAR
#define DOT_CHAR      "_"
#endif

#ifndef fileno 
int fileno( FILE *stream);
#endif

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
#ifndef __NOVELL_LIBC__
#include <sys/bsdskt.h>
#include <sys/timeval.h>
#endif
#undef HAVE_ALARM
#endif

#if defined(HAVE_LIBIDN) && defined(HAVE_TLD_H)

#define USE_LIBIDN
#endif

#ifndef SIZEOF_TIME_T

#define SIZEOF_TIME_T 4
#endif

#define LIBIDN_REQUIRED_VERSION "0.4.1"

#ifdef __UCLIBC__
#define HAVE_INET_NTOA_R_2_ARGS 1
#endif

#if defined(USE_GNUTLS) || defined(USE_SSLEAY) || defined(USE_NSS)
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

#endif 
