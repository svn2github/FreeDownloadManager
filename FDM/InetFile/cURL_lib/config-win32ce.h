/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LIB_CONFIG_WIN32CE_H
#define __LIB_CONFIG_WIN32CE_H                     

#define HAVE_FCNTL_H 1    

#define HAVE_IO_H 1 

#define NEED_MALLOC_H 1       

#define HAVE_SIGNAL_H 1       

#define HAVE_STDLIB_H 1 

#define HAVE_PROCESS_H 1             

#define HAVE_SYS_STAT_H 1       

#define HAVE_SYS_UTIME_H 1       

#define HAVE_TIME_H 1 

#if defined(__MINGW32__) || defined(__WATCOMC__) || defined(__LCC__)
#define HAVE_UNISTD_H 1
#endif 

#define HAVE_WINDOWS_H 1 

#define HAVE_WINSOCK_H 1           

#define HAVE_SIG_ATOMIC_T 1 

#define STDC_HEADERS 1        

#define HAVE_CLOSESOCKET 1    

#define HAVE_GETHOSTBYADDR 1 

#define HAVE_GETHOSTNAME 1    

#define HAVE_GETSERVBYNAME 1    

#define HAVE_INET_ADDR 1 

#define HAVE_IOCTLSOCKET 1 

#define HAVE_PERROR 1 

#define HAVE_RAND_SCREEN 1 

#define HAVE_RAND_STATUS 1 

#define HAVE_SELECT 1 

#define HAVE_SETVBUF 1 

#define HAVE_SOCKET 1                   

#define HAVE_STRSTR 1 

#if defined(__MINGW32__) || defined(__WATCOMC__)
#define HAVE_STRTOLL 1
#endif       

#define HAVE_UTIME 1 

#define HAVE_GETNAMEINFO 1 

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

#if (defined(__WATCOMC__) && (__WATCOMC__ >= 1240)) || defined(__POCC__)
#elif defined(_WIN64)
#define ssize_t __int64
#else
#define ssize_t int
#endif     

#define SIZEOF_LONG_DOUBLE 16           

#define HAVE_STRUCT_TIMEVAL 1        

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

#if defined(_MSC_VER) && !defined(_WIN32_WCE)
#  if (_MSC_VER >= 900) && (_INTEGRAL_MAX_BITS >= 64)
#    define USE_WIN32_LARGE_FILES
#  else
#    define USE_WIN32_SMALL_FILES
#  endif
#endif

#if !defined(USE_WIN32_LARGE_FILES) && !defined(USE_WIN32_SMALL_FILES)
#  define USE_WIN32_SMALL_FILES
#endif    

#define CURL_LDAP_WIN 1
#undef CURL_LDAP_HYBRID
#undef HAVE_LDAP_URL_PARSE     

#undef OS
#define OS "i386-pc-win32ce" 

#define PACKAGE "curl"    

#define CURL_DISABLE_FILE 1
#define CURL_DISABLE_TELNET 1
#define CURL_DISABLE_LDAP 1

#define ENOSPC 1
#define ENOMEM 2
#define EAGAIN 3

extern int stat(const char *path,struct stat *buffer );

#endif 
