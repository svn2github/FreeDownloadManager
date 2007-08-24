/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LIB_CONFIG_WIN32_H
#define __LIB_CONFIG_WIN32_H               

#define HAVE_ASSERT_H 1          

#define HAVE_FCNTL_H 1    

#define HAVE_IO_H 1 

#define HAVE_MALLOC_H 1             

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

#define HAVE_WINSOCK2_H 1 

#define HAVE_WS2TCPIP_H 1     

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

#define HAVE_VPRINTF 1     

#define in_addr_t unsigned long       

#if (defined(__WATCOMC__) && (__WATCOMC__ >= 1240)) || defined(__POCC__)
#else
#define ssize_t int
#endif        

#define SIZEOF_LONG_DOUBLE 16    

#ifdef SIZEOF_CURL_OFF_T
#undef SIZEOF_CURL_OFF_T
#endif  

#if defined(__BORLANDC__) || defined(__POCC__)
#define SIZEOF_CURL_OFF_T 4
#else
#define SIZEOF_CURL_OFF_T 8
#endif     

#define HAVE_STRUCT_SOCKADDR_STORAGE 1               

#define DL_LDAP_FILE "wldap32.dll"     

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1 

#undef OS
#define OS "i386-pc-win32" 

#define PACKAGE "curl" 

#endif 
