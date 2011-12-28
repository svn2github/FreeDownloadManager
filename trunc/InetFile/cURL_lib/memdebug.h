/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifdef CURLDEBUG
#ifndef _CURL_MEDEBUG_H
#define _CURL_MEDEBUG_H




#include "setup.h"

#include <curl/curl.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <stdio.h>
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#define logfile curl_debuglogfile

extern FILE *logfile;


CURL_EXTERN void *curl_domalloc(size_t size, int line, const char *source);
CURL_EXTERN void *curl_docalloc(size_t elements, size_t size, int line, const char *source);
CURL_EXTERN void *curl_dorealloc(void *ptr, size_t size, int line, const char *source);
CURL_EXTERN void curl_dofree(void *ptr, int line, const char *source);
CURL_EXTERN char *curl_dostrdup(const char *str, int line, const char *source);
CURL_EXTERN void curl_memdebug(const char *logname);
CURL_EXTERN void curl_memlimit(long limit);


CURL_EXTERN int curl_socket(int domain, int type, int protocol, int line , const char *);
CURL_EXTERN int curl_sclose(int sockfd, int, const char *source);
CURL_EXTERN int curl_accept(int s, void *addr, void *addrlen,
                            int line, const char *source);


CURL_EXTERN FILE *curl_fopen(const char *file, const char *mode, int line,
                             const char *source);
#ifdef HAVE_FDOPEN
CURL_EXTERN FILE *curl_fdopen(int filedes, const char *mode, int line,
                              const char *source);
#endif
CURL_EXTERN int curl_fclose(FILE *file, int line, const char *source);

#ifndef MEMDEBUG_NODEFINES


#undef strdup
#define strdup(ptr) curl_dostrdup(ptr, __LINE__, __FILE__)
#define malloc(size) curl_domalloc(size, __LINE__, __FILE__)
#define calloc(nbelem,size) curl_docalloc(nbelem, size, __LINE__, __FILE__)
#define realloc(ptr,size) curl_dorealloc(ptr, size, __LINE__, __FILE__)
#define free(ptr) curl_dofree(ptr, __LINE__, __FILE__)

#define socket(domain,type,protocol)\
 curl_socket(domain,type,protocol,__LINE__,__FILE__)
#undef accept 
#define accept(sock,addr,len)\
 curl_accept(sock,addr,len,__LINE__,__FILE__)

#ifdef HAVE_GETADDRINFO
#if defined(getaddrinfo) && defined(__osf__)

#define ogetaddrinfo(host,serv,hint,res) \
  curl_dogetaddrinfo(host,serv,hint,res,__LINE__,__FILE__)
#else
#undef getaddrinfo
#define getaddrinfo(host,serv,hint,res) \
  curl_dogetaddrinfo(host,serv,hint,res,__LINE__,__FILE__)
#endif
#endif 

#ifdef HAVE_GETNAMEINFO
#undef getnameinfo
#define getnameinfo(sa,salen,host,hostlen,serv,servlen,flags) \
  curl_dogetnameinfo(sa,salen,host,hostlen,serv,servlen,flags, __LINE__, \
  __FILE__)
#endif 

#ifdef HAVE_FREEADDRINFO
#undef freeaddrinfo
#define freeaddrinfo(data) \
  curl_dofreeaddrinfo(data,__LINE__,__FILE__)
#endif 


#undef sclose
#define sclose(sockfd) curl_sclose(sockfd,__LINE__,__FILE__)

#undef closesocket
#define closesocket(sockfd) curl_sclose(sockfd,__LINE__,__FILE__)

#undef fopen
#define fopen(file,mode) curl_fopen(file,mode,__LINE__,__FILE__)
#undef fdopen
#define fdopen(file,mode) curl_fdopen(file,mode,__LINE__,__FILE__)
#define fclose(file) curl_fclose(file,__LINE__,__FILE__)

#endif 

#endif 
#endif 
