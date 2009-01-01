/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
                             

#define CURL_DISABLE_LDAP 1 

#define CURL_DISABLE_LDAPS 1                               

#define ENABLE_IPV6 1 

#define GETNAMEINFO_QUAL_ARG1 const 

#define GETNAMEINFO_TYPE_ARG1 struct sockaddr * 

#define GETNAMEINFO_TYPE_ARG2 socklen_t 

#define GETNAMEINFO_TYPE_ARG46 size_t 

#define GETNAMEINFO_TYPE_ARG7 int    

#define HAVE_ARPA_INET_H 1    

#define HAVE_ASSERT_H 1                      

#define HAVE_DLFCN_H 1    

#define HAVE_ERRNO_H 1 

#define HAVE_ERR_H 1 

#define HAVE_FCNTL_H 1 

#define HAVE_FIONBIO 1    

#define HAVE_FTRUNCATE 1 

#define HAVE_GETADDRINFO 1 

#define HAVE_GETEUID 1 

#define HAVE_GETHOSTBYADDR 1 

#define HAVE_GETHOSTBYNAME 1             

#define HAVE_GETNAMEINFO 1    

#define HAVE_GETPPID 1 

#define HAVE_GETPROTOBYNAME 1 

#define HAVE_GETPWUID 1    

#define HAVE_GETTIMEOFDAY 1    

#define HAVE_GMTIME_R 1                                        

#define HAVE_INTTYPES_H 1                                                             

#define HAVE_LIMITS_H 1 

#define HAVE_LL 1 

#define HAVE_LOCALE_H 1 

#define HAVE_LOCALTIME_R 1 

#define HAVE_LONGLONG 1    

#define HAVE_MEMORY_H 1    

#define HAVE_NETDB_H 1 

#define HAVE_NETINET_IN_H 1    

#define HAVE_NET_IF_H 1                                  

#define HAVE_O_NONBLOCK 1    

#define HAVE_PERROR 1 

#define HAVE_PIPE 1             

#define HAVE_POSIX_STRERROR_R 1 

#define HAVE_PWD_H 1 

#define HAVE_RAND_EGD 1       

#define HAVE_RECV 1 

#define HAVE_RECVFROM 1    

#define HAVE_SELECT 1 

#define HAVE_SEND 1 

#define HAVE_SETJMP_H 1 

#define HAVE_SETLOCALE 1                   

#define HAVE_SIGNAL_H 1          

#define HAVE_SOCKET 1             

#define HAVE_STDBOOL_H 1 

#define HAVE_STDINT_H 1 

#define HAVE_STDIO_H 1 

#define HAVE_STDLIB_H 1 

#define HAVE_STRCASECMP 1 

#define HAVE_STRCASESTR 1    

#define HAVE_STRDUP 1 

#define HAVE_STRERROR_R 1    

#define HAVE_STRINGS_H 1 

#define HAVE_STRING_H 1 

#define HAVE_STRLCAT 1 

#define HAVE_STRLCPY 1 

#define HAVE_STRSTR 1 

#define HAVE_STRTOK_R 1 

#define HAVE_STRTOLL 1 

#define HAVE_STRUCT_SOCKADDR_STORAGE 1 

#define HAVE_STRUCT_TIMEVAL 1 

#define HAVE_SYS_FILIO_H 1 

#define HAVE_SYS_IOCTL_H 1 

#define HAVE_SYS_PARAM_H 1    

#define HAVE_SYS_RESOURCE_H 1 

#define HAVE_SYS_SELECT_H 1 

#define HAVE_SYS_SOCKET_H 1 

#define HAVE_SYS_SOCKIO_H 1 

#define HAVE_SYS_STAT_H 1 

#define HAVE_SYS_TIME_H 1 

#define HAVE_SYS_TYPES_H 1          

#define HAVE_TIME_H 1       

#define HAVE_UNAME 1 

#define HAVE_UNISTD_H 1 

#define HAVE_UTIME 1 

#define HAVE_UTIME_H 1 

#define HAVE_VARIADIC_MACROS_C99 1                                           

#ifdef __WINS__
#define OS "i386-pc-epoc32"
#elif __MARM__
#define OS "arm-unknown-epoc32"
#else

#define OS "unknown-unknown-epoc32"
#endif                     

#define RECV_TYPE_ARG1 int
#define RECV_TYPE_ARG2 void*
#define RECV_TYPE_ARG3 size_t
#define RECV_TYPE_ARG4 int
#define RECV_TYPE_RETV ssize_t

#define RECVFROM_TYPE_ARG1 int
#define RECVFROM_TYPE_ARG2 void
#define RECVFROM_TYPE_ARG3 size_t
#define RECVFROM_TYPE_ARG4 int
#define RECVFROM_TYPE_ARG5 struct sockaddr
#define RECVFROM_TYPE_ARG6 size_t
#define RECVFROM_TYPE_RETV ssize_t
#define RECVFROM_TYPE_ARG2_IS_VOID 1

#define SEND_TYPE_ARG1 int
#define SEND_QUAL_ARG2 const
#define SEND_TYPE_ARG2 void*
#define SEND_TYPE_ARG3 size_t
#define SEND_TYPE_ARG4 int
#define SEND_TYPE_RETV ssize_t     

#define SELECT_TYPE_ARG1 int 

#define SELECT_TYPE_ARG234 (fd_set *) 

#define SELECT_TYPE_ARG5 (struct timeval *) 

#define SIZEOF_OFF_T 8 

#define SIZEOF_SIZE_T 4 

#define SIZEOF_TIME_T 4 

#define STDC_HEADERS 1 

#define TIME_WITH_SYS_TIME 1                                  

#ifndef _ALL_SOURCE

#endif 

#define _FILE_OFFSET_BITS 64             

#ifndef __cplusplus

#endif           

#ifdef _DEBUG

#endif 

#ifndef __LONG_LONG_SUPPORTED
#define __LONG_LONG_SUPPORTED
#endif 

#ifdef HAVE_LIBZ
#define HAVE_ZLIB_H 1
#endif 

#ifdef USE_SSL
#define USE_OPENSSL 1
#define USE_SSLEAY 1
#endif
