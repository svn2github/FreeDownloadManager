/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __INET_NTOP_H
#define __INET_NTOP_H


#include "setup.h"

char *Curl_inet_ntop(int af, const void *addr, char *buf, size_t size);

#ifdef HAVE_INET_NTOP
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#define Curl_inet_ntop(af,addr,buf,size) inet_ntop(af,addr,buf,size)
#endif

#endif 
