/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __SELECT_H
#define __SELECT_H


#include "setup.h"

#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#elif defined(HAVE_POLL_H)
#include <poll.h>
#endif



#if defined(USE_WINSOCK) && (USE_WINSOCK > 1) && \
    defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600)
#  undef  HAVE_POLL
#  define HAVE_POLL 1
#  undef  HAVE_POLL_FINE
#  define HAVE_POLL_FINE 1
#  define poll(x,y,z) WSAPoll((x),(y),(z))
#  if defined(_MSC_VER) && defined(POLLRDNORM)
#    undef  POLLPRI
#    define POLLPRI POLLRDBAND
#    define HAVE_STRUCT_POLLFD 1
#  endif
#endif



#if !defined(HAVE_STRUCT_POLLFD) && \
    !defined(HAVE_SYS_POLL_H) && \
    !defined(HAVE_POLL_H)

#define POLLIN      0x01
#define POLLPRI     0x02
#define POLLOUT     0x04
#define POLLERR     0x08
#define POLLHUP     0x10
#define POLLNVAL    0x20

struct pollfd
{
    curl_socket_t fd;
    short   events;
    short   revents;
};

#endif

#ifndef POLLRDNORM
#define POLLRDNORM POLLIN
#endif

#ifndef POLLWRNORM
#define POLLWRNORM POLLOUT
#endif

#ifndef POLLRDBAND
#define POLLRDBAND POLLPRI
#endif

int Curl_socket_ready(curl_socket_t readfd, curl_socket_t writefd,
                      int timeout_ms);

int Curl_poll(struct pollfd ufds[], unsigned int nfds, int timeout_ms);

#ifdef TPF
int tpf_select_libcurl(int maxfds, fd_set* reads, fd_set* writes,
                       fd_set* excepts, struct timeval* tv);
#endif

#endif 
