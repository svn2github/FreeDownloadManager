/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef HEADER_CURL_ADDRINFO_H
#define HEADER_CURL_ADDRINFO_H


#include "setup.h"

#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif

#ifdef  VMS
#  include <in.h>
#  include <inet.h>
#  include <stdlib.h>
#endif




struct Curl_addrinfo {
  int                   ai_flags;
  int                   ai_family;
  int                   ai_socktype;
  int                   ai_protocol;
  socklen_t             ai_addrlen;   
  char                 *ai_canonname;
  struct sockaddr      *ai_addr;
  struct Curl_addrinfo *ai_next;
};
typedef struct Curl_addrinfo Curl_addrinfo;

void
Curl_freeaddrinfo(Curl_addrinfo *cahead);

#ifdef HAVE_GETADDRINFO
int
Curl_getaddrinfo_ex(const char *nodename,
                    const char *servname,
                    const struct addrinfo *hints,
                    Curl_addrinfo **result);
#endif

Curl_addrinfo *
Curl_he2ai(const struct hostent *he, int port);

Curl_addrinfo *
Curl_ip2addr(int af, const void *inaddr, const char *hostname, int port);

#if defined(CURLDEBUG) && defined(HAVE_FREEADDRINFO)
void
curl_dofreeaddrinfo(struct addrinfo *freethis,
                    int line, const char *source);
#endif

#if defined(CURLDEBUG) && defined(HAVE_GETADDRINFO)
int
curl_dogetaddrinfo(const char *hostname,
                   const char *service,
                   const struct addrinfo *hints,
                   struct addrinfo **result,
                   int line, const char *source);
#endif

#endif 
