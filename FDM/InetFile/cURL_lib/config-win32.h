/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
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

#define HAVE_INET_NTOA 1 

#define HAVE_IOCTLSOCKET 1 

#define HAVE_PERROR 1 

#define HAVE_RAND_SCREEN 1 

#define HAVE_RAND_STATUS 1 

#define HAVE_SELECT 1 

#define HAVE_SETVBUF 1 

#define HAVE_SOCKET 1    

#define HAVE_STRICMP 1 

#define HAVE_STRDUP 1 

#define HAVE_STRFTIME 1 

#define HAVE_STRSTR 1 

#if defined(__MINGW32__) || defined(__WATCOMC__) || defined(__POCC__)
#define HAVE_STRTOLL 1
#endif       

#ifndef __BORLANDC__
#define HAVE_UTIME 1
#endif 

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

#ifndef HAVE_WS2TCPIP_H
#define socklen_t int
#endif     

#define SIZEOF_LONG_DOUBLE 16    

#ifdef SIZEOF_CURL_OFF_T
#undef SIZEOF_CURL_OFF_T
#endif  

#if defined(__BORLANDC__) || defined(__POCC__) || defined(__SALFORDC__)
#define SIZEOF_CURL_OFF_T 4
#else
#define SIZEOF_CURL_OFF_T 8
#endif     

#ifndef __SALFORDC__
#define HAVE_STRUCT_SOCKADDR_STORAGE 1
#endif 

#define HAVE_STRUCT_TIMEVAL 1           

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#define HAVE_VARIADIC_MACROS_C99 1
#endif        

#define DL_LDAP_FILE "wldap32.dll"     

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1 

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
