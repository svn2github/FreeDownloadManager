/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __TIMEVAL_H
#define __TIMEVAL_H




#include "setup.h"

struct timeval curlx_tvnow(void);


long curlx_tvdiff(struct timeval t1, struct timeval t2);


double curlx_tvdiff_secs(struct timeval t1, struct timeval t2);

long Curl_tvlong(struct timeval t1);


#define Curl_tvnow() curlx_tvnow()
#define Curl_tvdiff(x,y) curlx_tvdiff(x,y)
#define Curl_tvdiff_secs(x,y) curlx_tvdiff_secs(x,y)

#endif
