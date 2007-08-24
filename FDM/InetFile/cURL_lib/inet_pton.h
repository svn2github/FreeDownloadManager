/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __INET_PTON_H
#define __INET_PTON_H 

#include "setup.h"

int Curl_inet_pton(int, const char *, void *);

#ifdef HAVE_INET_PTON

#if defined(HAVE_NO_INET_PTON_PROTO)
int inet_pton(int af, const char *src, void *dst);
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#define Curl_inet_pton(x,y,z) inet_pton(x,y,z)
#endif

#endif 
