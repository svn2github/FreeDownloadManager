/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CURL_CONTENT_ENCODING_H
#define __CURL_CONTENT_ENCODING_H

#include "setup.h"


#ifdef HAVE_LIBZ
#define ALL_CONTENT_ENCODINGS "deflate, gzip"
#else
#define ALL_CONTENT_ENCODINGS "identity"
#endif

CURLcode Curl_unencode_deflate_write(struct connectdata *conn,
                                     struct SingleRequest *req,
                                     ssize_t nread);

CURLcode
Curl_unencode_gzip_write(struct connectdata *conn,
                         struct SingleRequest *k,
                         ssize_t nread);

#endif
