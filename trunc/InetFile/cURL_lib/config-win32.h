/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __LIB_CONFIG_WIN32_H
#define __LIB_CONFIG_WIN32_H













#define HAVE_ASSERT_H 1








#define HAVE_FCNTL_H 1





#define HAVE_IO_H 1


#ifndef __SALFORDC__
#define NEED_MALLOC_H 1
#endif








#ifndef __SALFORDC__
#define HAVE_PROCESS_H 1
#endif


#define HAVE_SIGNAL_H 1








#define HAVE_STDLIB_H 1














#define HAVE_SYS_STAT_H 1





#define HAVE_SYS_TYPES_H 1


#ifndef __BORLANDC__
#define HAVE_SYS_UTIME_H 1
#endif








#define HAVE_TIME_H 1


#if defined(__MINGW32__) || defined(__WATCOMC__) || defined(__LCC__) || \
    defined(__POCC__)
#define HAVE_UNISTD_H 1
#endif


#define HAVE_WINDOWS_H 1


#define HAVE_WINSOCK_H 1

#ifndef __SALFORDC__

#define HAVE_WINSOCK2_H 1


#define HAVE_WS2TCPIP_H 1
#endif






#define HAVE_SIG_ATOMIC_T 1


#define STDC_HEADERS 1









#define HAVE_CLOSESOCKET 1





#define HAVE_GETHOSTBYADDR 1


#define HAVE_GETHOSTNAME 1





#define HAVE_GETSERVBYNAME 1


#define HAVE_GETPROTOBYNAME





#define HAVE_INET_ADDR 1


#define HAVE_IOCTLSOCKET 1


#define HAVE_PERROR 1


#define HAVE_RAND_SCREEN 1


#define HAVE_RAND_STATUS 1


#define HAVE_CRYPTO_CLEANUP_ALL_EX_DATA 1


#define HAVE_SELECT 1


#define HAVE_SETVBUF 1


#define HAVE_SOCKET 1





#define HAVE_STRDUP 1


#define HAVE_STRFTIME 1


#define HAVE_STRICMP 1





#define HAVE_STRNICMP 1


#define HAVE_STRSTR 1


#if defined(__MINGW32__) || defined(__WATCOMC__) || defined(__POCC__)
#define HAVE_STRTOLL 1
#endif








#ifndef __BORLANDC__
#define HAVE_UTIME 1
#endif


#define GETNAMEINFO_QUAL_ARG1 const


#define GETNAMEINFO_TYPE_ARG1 struct sockaddr *


#define GETNAMEINFO_TYPE_ARG2 socklen_t


#define GETNAMEINFO_TYPE_ARG46 DWORD


#define GETNAMEINFO_TYPE_ARG7 int


#define HAVE_RECV 1


#define RECV_TYPE_ARG1 SOCKET


#define RECV_TYPE_ARG2 char *


#define RECV_TYPE_ARG3 int


#define RECV_TYPE_ARG4 int


#define RECV_TYPE_RETV int


#define HAVE_RECVFROM 1


#define RECVFROM_TYPE_ARG1 SOCKET


#define RECVFROM_TYPE_ARG2 char


#define RECVFROM_TYPE_ARG3 int


#define RECVFROM_TYPE_ARG4 int


#define RECVFROM_TYPE_ARG5 struct sockaddr


#define RECVFROM_TYPE_ARG6 int


#define RECVFROM_TYPE_RETV int


#define HAVE_SEND 1


#define SEND_TYPE_ARG1 SOCKET


#define SEND_QUAL_ARG2 const


#define SEND_TYPE_ARG2 char *


#define SEND_TYPE_ARG3 int


#define SEND_TYPE_ARG4 int


#define SEND_TYPE_RETV int






#define in_addr_t unsigned long


#define RETSIGTYPE void

#ifndef _SSIZE_T_DEFINED
#if (defined(__WATCOMC__) && (__WATCOMC__ >= 1240)) || defined(__POCC__) || \
    defined(__MINGW32__)
#elif defined(_WIN64)
#define ssize_t __int64
#else
#define ssize_t int
#endif
#define _SSIZE_T_DEFINED
#endif






#define SIZEOF_LONG_DOUBLE 16









#ifndef __SALFORDC__
#define HAVE_STRUCT_SOCKADDR_STORAGE 1
#endif


#define HAVE_STRUCT_TIMEVAL 1












#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#define HAVE_VARIADIC_MACROS_C99 1
#endif


#if defined(__MINGW32__) || defined(__WATCOMC__)
#define HAVE_LONGLONG 1
#endif


#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif



#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  ifndef _USE_32BIT_TIME_T
#    define SIZEOF_TIME_T 8
#  else
#    define SIZEOF_TIME_T 4
#  endif
#endif



#if defined(_MSC_VER) && (_MSC_VER >= 1500)
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0501
#  endif
#  ifndef WINVER
#    define WINVER 0x0501
#  endif
#  if (_WIN32_WINNT < 0x0501) || (WINVER < 0x0501)
#    error VS2008 does not support Windows build targets prior to WinXP
#  endif
#endif



#if defined(HAVE_WS2TCPIP_H)
#  if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0501)
#    define HAVE_FREEADDRINFO 1
#    define HAVE_GETADDRINFO  1
#    define HAVE_GETNAMEINFO  1
#  elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#    define HAVE_FREEADDRINFO 1
#    define HAVE_GETADDRINFO  1
#    define HAVE_GETNAMEINFO  1
#  endif
#endif





#if defined(_MSC_VER) && !defined(_WIN32_WCE)
#  if (_MSC_VER >= 900) && (_INTEGRAL_MAX_BITS >= 64)
#    define USE_WIN32_LARGE_FILES
#  else
#    define USE_WIN32_SMALL_FILES
#  endif
#endif

#if defined(__MINGW32__) && !defined(USE_WIN32_LARGE_FILES)
#  define USE_WIN32_LARGE_FILES
#endif

#if !defined(USE_WIN32_LARGE_FILES) && !defined(USE_WIN32_SMALL_FILES)
#  define USE_WIN32_SMALL_FILES
#endif





#if defined(CURL_HAS_NOVELL_LDAPSDK) || defined(CURL_HAS_MOZILLA_LDAPSDK)
#undef CURL_LDAP_HYBRID
#undef CURL_LDAP_WIN
#define HAVE_LDAP_SSL_H 1
#define HAVE_LDAP_URL_PARSE 1
#elif defined(CURL_HAS_OPENLDAP_LDAPSDK)
#undef CURL_LDAP_HYBRID
#undef CURL_LDAP_WIN
#define HAVE_LDAP_URL_PARSE 1
#else
#undef CURL_LDAP_HYBRID
#undef HAVE_LDAP_URL_PARSE
#define CURL_LDAP_WIN 1
#endif






#undef OS
#if defined(_M_IX86) || defined(__i386__) 
#define OS "i386-pc-win32"
#elif defined(_M_IA64) 
#define OS "ia64-pc-win32"
#elif defined(_M_X64) 
#define OS "amd64-pc-win32"
#else
#define OS "unknown-pc-win32"
#endif


#define PACKAGE "curl"


#endif 
