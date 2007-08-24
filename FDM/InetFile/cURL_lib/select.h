/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SELECT_H
#define __SELECT_H 

#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#else

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

#define CSELECT_IN   0x01
#define CSELECT_OUT  0x02
#define CSELECT_ERR  0x04

int Curl_select(curl_socket_t readfd, curl_socket_t writefd, int timeout_ms);

int Curl_poll(struct pollfd ufds[], unsigned int nfds, int timeout_ms); 

#endif
