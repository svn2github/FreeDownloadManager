/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __FILE_H
#define __FILE_H 

#ifndef CURL_DISABLE_FILE
CURLcode Curl_file(struct connectdata *, bool *done);
CURLcode Curl_file_done(struct connectdata *, CURLcode, bool premature);
CURLcode Curl_file_connect(struct connectdata *);
#endif
#endif
