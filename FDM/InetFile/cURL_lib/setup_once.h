/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SETUP_ONCE_H
#define __SETUP_ONCE_H       

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#else
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#endif

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#endif

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif   

#ifndef HAVE_STRUCT_TIMEVAL
struct timeval {
 long tv_sec;
 long tv_usec;
};
#endif   

#ifdef HAVE_MSG_NOSIGNAL
#define SEND_4TH_ARG MSG_NOSIGNAL
#else
#define SEND_4TH_ARG 0
#endif   

#ifdef HAVE_RECV
#if !defined(RECV_TYPE_ARG1) || \
    !defined(RECV_TYPE_ARG2) || \
    !defined(RECV_TYPE_ARG3) || \
    !defined(RECV_TYPE_ARG4) || \
    !defined(RECV_TYPE_RETV)
  
  Error Missing_definition_of_return_and_arguments_types_of_recv
  
#else
#define sread(x,y,z) (ssize_t)recv((RECV_TYPE_ARG1)(x), \
                                   (RECV_TYPE_ARG2)(y), \
                                   (RECV_TYPE_ARG3)(z), \
                                   (RECV_TYPE_ARG4)(0))
#endif
#else 
#ifndef sread
  
  Error Missing_definition_of_macro_sread
  
#endif
#endif 

#ifdef HAVE_SEND
#if !defined(SEND_TYPE_ARG1) || \
    !defined(SEND_QUAL_ARG2) || \
    !defined(SEND_TYPE_ARG2) || \
    !defined(SEND_TYPE_ARG3) || \
    !defined(SEND_TYPE_ARG4) || \
    !defined(SEND_TYPE_RETV)
  
  Error Missing_definition_of_return_and_arguments_types_of_send
  
#else
#define swrite(x,y,z) (ssize_t)send((SEND_TYPE_ARG1)(x), \
                                    (SEND_TYPE_ARG2)(y), \
                                    (SEND_TYPE_ARG3)(z), \
                                    (SEND_TYPE_ARG4)(SEND_4TH_ARG))
#endif
#else 
#ifndef swrite
  
  Error Missing_definition_of_macro_swrite
  
#endif
#endif    

#define ISSPACE(x)  (isspace((int)  ((unsigned char)x)))
#define ISDIGIT(x)  (isdigit((int)  ((unsigned char)x)))
#define ISALNUM(x)  (isalnum((int)  ((unsigned char)x)))
#define ISXDIGIT(x) (isxdigit((int) ((unsigned char)x)))
#define ISGRAPH(x)  (isgraph((int)  ((unsigned char)x)))
#define ISALPHA(x)  (isalpha((int)  ((unsigned char)x)))
#define ISPRINT(x)  (isprint((int)  ((unsigned char)x)))
#define ISUPPER(x)  (isupper((int)  ((unsigned char)x)))
#define ISLOWER(x)  (islower((int)  ((unsigned char)x)))

#define ISBLANK(x)  (int)((((unsigned char)x) == ' ') || \
                          (((unsigned char)x) == '\t'))   

#ifndef HAVE_BOOL_T
typedef unsigned char bool;
#define HAVE_BOOL_T
#endif   

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif   

#ifndef HAVE_SIG_ATOMIC_T
typedef int sig_atomic_t;
#define HAVE_SIG_ATOMIC_T
#endif   

#ifdef HAVE_SIG_ATOMIC_T_VOLATILE
#define SIG_ATOMIC_T static sig_atomic_t
#else
#define SIG_ATOMIC_T static volatile sig_atomic_t
#endif   

#ifndef RETSIGTYPE
#define RETSIGTYPE void
#endif   

#ifdef CURLDEBUG
#define DEBUGF(x) x
#else
#define DEBUGF(x) do { } while (0)
#endif   

#if defined(CURLDEBUG) && defined(HAVE_ASSERT_H)
#define DEBUGASSERT(x) assert(x)
#else
#define DEBUGASSERT(x) do { } while (0)
#endif   

#ifdef USE_WINSOCK
#define SOCKERRNO         ((int)WSAGetLastError())
#define SET_SOCKERRNO(x)  (WSASetLastError((int)(x)))
#else
#define SOCKERRNO         (errno)
#define SET_SOCKERRNO(x)  (errno = (x))
#endif   

#ifdef WIN32
#define ERRNO         ((int)GetLastError())
#define SET_ERRNO(x)  (SetLastError((DWORD)(x)))
#else
#define ERRNO         (errno)
#define SET_ERRNO(x)  (errno = (x))
#endif   

#ifdef USE_WINSOCK
#undef  EBADF            
#define EBADF            WSAEBADF
#undef  EINTR            
#define EINTR            WSAEINTR
#undef  EINVAL           
#define EINVAL           WSAEINVAL
#define EWOULDBLOCK      WSAEWOULDBLOCK
#define EINPROGRESS      WSAEINPROGRESS
#define EALREADY         WSAEALREADY
#define ENOTSOCK         WSAENOTSOCK
#define EDESTADDRREQ     WSAEDESTADDRREQ
#define EMSGSIZE         WSAEMSGSIZE
#define EPROTOTYPE       WSAEPROTOTYPE
#define ENOPROTOOPT      WSAENOPROTOOPT
#define EPROTONOSUPPORT  WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT  WSAESOCKTNOSUPPORT
#define EOPNOTSUPP       WSAEOPNOTSUPP
#define EPFNOSUPPORT     WSAEPFNOSUPPORT
#define EAFNOSUPPORT     WSAEAFNOSUPPORT
#define EADDRINUSE       WSAEADDRINUSE
#define EADDRNOTAVAIL    WSAEADDRNOTAVAIL
#define ENETDOWN         WSAENETDOWN
#define ENETUNREACH      WSAENETUNREACH
#define ENETRESET        WSAENETRESET
#define ECONNABORTED     WSAECONNABORTED
#define ECONNRESET       WSAECONNRESET
#define ENOBUFS          WSAENOBUFS
#define EISCONN          WSAEISCONN
#define ENOTCONN         WSAENOTCONN
#define ESHUTDOWN        WSAESHUTDOWN
#define ETOOMANYREFS     WSAETOOMANYREFS
#define ETIMEDOUT        WSAETIMEDOUT
#define ECONNREFUSED     WSAECONNREFUSED
#define ELOOP            WSAELOOP
#ifndef ENAMETOOLONG     
#define ENAMETOOLONG     WSAENAMETOOLONG
#endif
#define EHOSTDOWN        WSAEHOSTDOWN
#define EHOSTUNREACH     WSAEHOSTUNREACH
#ifndef ENOTEMPTY        
#define ENOTEMPTY        WSAENOTEMPTY
#endif
#define EPROCLIM         WSAEPROCLIM
#define EUSERS           WSAEUSERS
#define EDQUOT           WSAEDQUOT
#define ESTALE           WSAESTALE
#define EREMOTE          WSAEREMOTE
#endif   

#if defined(VMS) && \
    defined(__INITIAL_POINTER_SIZE) && (__INITIAL_POINTER_SIZE == 64)
#define getpwuid __32_getpwuid
#endif   

#ifdef VMS
#define argv_item_t  __char_ptr32
#else
#define argv_item_t  char *
#endif 

#if defined (__LP64__) && defined(__hpux) && !defined(_XOPEN_SOURCE_EXTENDED)
#include <sys/socket.h> 

inline static int Curl_hp_getsockname(int s, struct sockaddr *name,
                                      socklen_t *namelen)
{
  int rc;
  if(namelen) {
     int len = *namelen;
     rc = getsockname(s, name, &len);
     *namelen = len;
   }
  else
     rc = getsockname(s, name, 0);
  return rc;
}

inline static int Curl_hp_getsockopt(int  s, int level, int optname,
                                     void *optval, socklen_t *optlen)
{
  int rc;
  if(optlen) {
    int len = *optlen;
    rc = getsockopt(s, level, optname, optval, &len);
    *optlen = len;
  }
  else
    rc = getsockopt(s, level, optname, optval, 0);
  return rc;
}

inline static int Curl_hp_accept(int sockfd, struct sockaddr *addr,
                                 socklen_t *addrlen)
{
  int rc;
  if(addrlen) {
     int len = *addrlen;
     rc = accept(sockfd, addr, &len);
     *addrlen = len;
  }
  else
     rc = accept(sockfd, addr, 0);
  return rc;
} 

inline static ssize_t Curl_hp_recvfrom(int s, void *buf, size_t len, int flags,
                                       struct sockaddr *from,
                                       socklen_t *fromlen)
{
  ssize_t rc;
  if(fromlen) {
    int fromlen32 = *fromlen;
    rc = recvfrom(s, buf, len, flags, from, &fromlen32);
    *fromlen = fromlen32;
  }
  else {
    rc = recvfrom(s, buf, len, flags, from, 0);
  }
  return rc;
}

#define getsockname(a,b,c) Curl_hp_getsockname((a),(b),(c))
#define getsockopt(a,b,c,d,e) Curl_hp_getsockopt((a),(b),(c),(d),(e))
#define accept(a,b,c) Curl_hp_accept((a),(b),(c))
#define recvfrom(a,b,c,d,e,f) Curl_hp_recvfrom((a),(b),(c),(d),(e),(f))

#endif 

#endif 

