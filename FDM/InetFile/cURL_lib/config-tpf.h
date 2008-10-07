/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LIBCONFIGTPF_H
#define __LIBCONFIGTPF_H                                                                          

#define HAVE_ARPA_INET_H 1    

#define HAVE_ASSERT_H 1 

#define HAVE_BASENAME 1     

#define HAVE_CRYPTO_CLEANUP_ALL_EX_DATA 1  

#define HAVE_CRYPTO_H 1  

#define HAVE_DES_H 1     

#define HAVE_ENGINE_LOAD_BUILTIN_ENGINES 1 

#define HAVE_ERRNO_H 1  

#define HAVE_ERR_H 1 

#define HAVE_FCNTL_H 1  

#define HAVE_FIONBIO 1  

#define HAVE_FORK 1 

#define HAVE_FTRUNCATE 1    

#define HAVE_GETEUID 1 

#define HAVE_GETHOSTBYADDR 1 

#define HAVE_GETHOSTBYNAME 1                      

#define HAVE_GETPWUID 1    

#define HAVE_GETTIMEOFDAY 1  

#define HAVE_GLIBC_STRERROR_R 1 

#define HAVE_GMTIME_R 1             

#define HAVE_ICONV 1          

#define HAVE_INET_ADDR 1 

#define HAVE_INET_NTOA 1          

#define HAVE_INTTYPES_H 1                                   

#define HAVE_LIBSSL 1    

#define HAVE_LIMITS_H 1 

#define HAVE_LL 1 

#define HAVE_LOCALE_H 1 

#define HAVE_LOCALTIME_R 1 

#define HAVE_LONGLONG 1    

#define HAVE_MEMORY_H 1 

#define HAVE_NETDB_H 1 

#define HAVE_NETINET_IN_H 1    

#define HAVE_NET_IF_H 1           

#define HAVE_OPENSSL_CRYPTO_H 1  

#define HAVE_OPENSSL_ENGINE_H 1  

#define HAVE_OPENSSL_ERR_H 1  

#define HAVE_OPENSSL_PEM_H 1  

#define HAVE_OPENSSL_PKCS12_H 1  

#define HAVE_OPENSSL_RSA_H 1  

#define HAVE_OPENSSL_SSL_H 1  

#define HAVE_OPENSSL_X509_H 1     

#define HAVE_PEM_H 1 

#define HAVE_PERROR 1 

#define HAVE_PIPE 1          

#define HAVE_PWD_H 1  

#define HAVE_RAND_EGD 1     

#define HAVE_RAND_STATUS 1  

#define HAVE_RSA_H 1 

#define HAVE_SELECT 1 

#define HAVE_SETJMP_H 1 

#define HAVE_SETLOCALE 1 

#define HAVE_SETRLIMIT 1    

#define HAVE_SIGACTION 1    

#define HAVE_SIGNAL 1 

#define HAVE_SIGNAL_H 1 

#define HAVE_SIG_ATOMIC_T 1       

#define HAVE_SOCKET 1        

#define HAVE_SSL_H 1 

#define HAVE_STDINT_H 1 

#define HAVE_STDLIB_H 1 

#define HAVE_STRCASECMP 1    

#define HAVE_STRDUP 1 

#define HAVE_STRERROR_R 1  

#define HAVE_STRICMP 1 

#define HAVE_STRINGS_H 1 

#define HAVE_STRING_H 1       

#define HAVE_STRSTR 1 

#define HAVE_STRTOK_R 1 

#define HAVE_STRTOLL 1    

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

#define HAVE_UNISTD_H 1 

#define HAVE_UTIME 1 

#define HAVE_UTIME_H 1                         

#define OS "s390x-ibm-tpf" 

#define PACKAGE "curl" 

#define PACKAGE_BUGREPORT "a suitable curl mailing list => http://curl.haxx.se/mail/" 

#define PACKAGE_NAME "curl" 

#define PACKAGE_STRING "curl -" 

#define PACKAGE_TARNAME "curl" 

#define PACKAGE_VERSION "-"    

#define RETSIGTYPE void 

#define SELECT_TYPE_ARG1 int 

#define SELECT_TYPE_ARG234 (fd_set *) 

#define SELECT_TYPE_ARG5 (struct timeval *) 

#define SIZEOF_CURL_OFF_T 8 

#define SIZEOF_LONG 8 

#define SIZEOF_SIZE_T 8 

#define SIZEOF_TIME_T 8 

#define STDC_HEADERS 1 

#define TIME_WITH_SYS_TIME 1                   

#define VERSION "not-used"    

#ifndef _ALL_SOURCE

#endif                                           

#define HAVE_RECV 1 

#define RECV_TYPE_ARG1 int 

#define RECV_TYPE_ARG2 char * 

#define RECV_TYPE_ARG3 int 

#define RECV_TYPE_ARG4 int 

#define RECV_TYPE_RETV int 

#define HAVE_SEND 1 

#define SEND_TYPE_ARG1 int 

#define SEND_QUAL_ARG2 const 

#define SEND_TYPE_ARG2 char * 

#define SEND_TYPE_ARG3 int 

#define SEND_TYPE_ARG4 int 

#define SEND_TYPE_RETV int

#define CURL_DOES_CONVERSIONS
#ifndef CURL_ICONV_CODESET_OF_HOST
#define CURL_ICONV_CODESET_OF_HOST "IBM-1047"
#endif    

#include <strings.h>    
#include <string.h>     
#include <stdlib.h>     
#include <sys/socket.h> 
#include <netdb.h>      
#include <tpf/sysapi.h> 

#endif 
