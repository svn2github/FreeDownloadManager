/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_OS_SUPPORT_H
#define AVFORMAT_OS_SUPPORT_H



#include "config.h"

#if defined(__MINGW32__) && !defined(__MINGW32CE__)
#  include <fcntl.h>
#  define lseek(f,p,w) _lseeki64((f), (p), (w))
#endif 

static inline int is_dos_path(const char *path)
{
#if HAVE_DOS_PATHS
    if (path[0] && path[1] == ':')
        return 1;
#endif
    return 0;
}

#ifdef __BEOS__
#  include <sys/socket.h>
#  include <netinet/in.h>
   
#  include <BeBuild.h>
   
#  if B_BEOS_VERSION <= B_BEOS_VERSION_5
#    include <OS.h>
     
#    define usleep(t)  snooze((bigtime_t)(t))
#  endif
#  ifndef SA_RESTART
#    warning SA_RESTART not implemented; ffserver might misbehave.
#    define SA_RESTART 0
#  endif
#endif

#if CONFIG_NETWORK
#if !HAVE_SOCKLEN_T
typedef int socklen_t;
#endif


#if !HAVE_CLOSESOCKET
#define closesocket close
#endif

#if CONFIG_FFSERVER
#if !HAVE_POLL_H
typedef unsigned long nfds_t;

struct pollfd {
    int fd;
    short events;  
    short revents; 
};


#define POLLIN     0x0001  
#define POLLOUT    0x0002  
#define POLLRDNORM POLLIN
#define POLLWRNORM POLLOUT
#define POLLRDBAND 0x0008  
#define POLLWRBAND 0x0010  
#define POLLPRI    0x0020  


#define POLLERR    0x0004  
#define POLLHUP    0x0080  
#define POLLNVAL   0x1000  


int poll(struct pollfd *fds, nfds_t numfds, int timeout);
#endif 
#endif 
#endif 

#endif 
