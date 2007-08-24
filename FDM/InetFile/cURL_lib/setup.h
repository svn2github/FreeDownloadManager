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

#ifdef AMIGA
#include "amigaos.h"
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

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#if !defined(__cplusplus) && !defined(__BEOS__) && !defined(typedef_bool)
typedef unsigned char bool;
#define typedef_bool
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

#if defined(CURLDEBUG) && defined(HAVE_ASSERT_H)
#define curlassert(x) assert(x)
#else

#define curlassert(x)
#endif

#ifdef MSG_NOSIGNAL

#define SEND_4TH_ARG MSG_NOSIGNAL
#define HAVE_MSG_NOSIGNAL 1 
#else
#define SEND_4TH_ARG 0
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

#if !defined(__GNUC__) || defined(__MINGW32__)
#define sclose(x) closesocket(x) 

#define sread(x,y,z) recv(x,y,(int)(z), SEND_4TH_ARG)
#define swrite(x,y,z) (size_t)send(x,y, (int)(z), SEND_4TH_ARG)
#undef HAVE_ALARM
#else
     
#define sclose(x) close(x)
#define sread(x,y,z) recv(x,y,z, SEND_4TH_ARG)
#define swrite(x,y,z) send(x,y,z, SEND_4TH_ARG)
#define HAVE_ALARM
#endif 

#define DIR_CHAR      "\\"
#define DOT_CHAR      "_"

#else 

#ifdef DJGPP
#include <sys/ioctl.h>
#define sclose(x)         close_s(x)
#define sread(x,y,z)      read_s(x,y,z)
#define swrite(x,y,z)     write_s(x,y,z)
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
#define sread(x,y,z) (ssize_t)recv(x,y,z, SEND_4TH_ARG)
#define swrite(x,y,z) (ssize_t)send(x,y,z, SEND_4TH_ARG)
#else 
#define sclose(x) close(x)
#define sread(x,y,z) recv(x,y,z, SEND_4TH_ARG)
#define swrite(x,y,z) send(x,y,z, SEND_4TH_ARG)
#endif 

#define HAVE_ALARM

#endif 

#ifdef _AMIGASF
#undef HAVE_ALARM
#undef sclose
#define sclose(x) CloseSocket(x)
#endif

#define DIR_CHAR      "/"
#ifndef DOT_CHAR
#define DOT_CHAR      "."
#endif

#ifdef DJGPP
#undef DOT_CHAR
#define DOT_CHAR      "_"
#endif

#ifndef fileno 
int fileno( FILE *stream);
#endif

#endif  

#ifdef WIN32
typedef SOCKET curl_socket_t;
#define CURL_SOCKET_BAD INVALID_SOCKET
#else
typedef int curl_socket_t;
#define CURL_SOCKET_BAD -1
#endif

#if defined(ENABLE_IPV6) && defined(USE_ARES)
#error "ares does not yet support IPv6. Disable IPv6 or ares and rebuild"
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

#if defined(USE_GNUTLS) || defined(USE_SSLEAY)
#define USE_SSL    
#endif

#if !defined(CURL_DISABLE_HTTP) && !defined(CURL_DISABLE_NTLM)
#if defined(USE_SSLEAY) || defined(USE_WINDOWS_SSPI)
#define USE_NTLM
#endif
#endif

#ifdef CURLDEBUG
#define DEBUGF(x) x
#else
#define DEBUGF(x)
#endif

#endif 
