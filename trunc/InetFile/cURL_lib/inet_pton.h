/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __INET_PTON_H
#define __INET_PTON_H


#include "setup.h"

int Curl_inet_pton(int, const char *, void *);

#ifdef HAVE_INET_PTON
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#define Curl_inet_pton(x,y,z) inet_pton(x,y,z)
#endif

#endif 