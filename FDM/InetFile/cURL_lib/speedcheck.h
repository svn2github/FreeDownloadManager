/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SPEEDCHECK_H
#define __SPEEDCHECK_H 

#include "setup.h"

#include "timeval.h"

void Curl_speedinit(struct SessionHandle *data);
CURLcode Curl_speedcheck(struct SessionHandle *data,
                         struct timeval now);

#endif
