/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_STRERROR_H
#define __CURL_STRERROR_H 

#include "urldata.h"

const char *Curl_strerror (struct connectdata *conn, int err);

#ifdef USE_LIBIDN
const char *Curl_idn_strerror (struct connectdata *conn, int err);
#endif

#endif
