/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/


#include "setup.h" 

#ifdef HAVE_LIBZ
#define ALL_CONTENT_ENCODINGS "deflate, gzip"
#else
#define ALL_CONTENT_ENCODINGS "identity"
#endif

CURLcode Curl_unencode_deflate_write(struct SessionHandle *data,
                                     struct Curl_transfer_keeper *k,
                                     ssize_t nread);

CURLcode
Curl_unencode_gzip_write(struct SessionHandle *data,
                         struct Curl_transfer_keeper *k,
                         ssize_t nread);
