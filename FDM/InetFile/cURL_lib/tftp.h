/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __TFTP_H
#define __TFTP_H 

#ifndef CURL_DISABLE_TFTP
CURLcode Curl_tftp_connect(struct connectdata *conn, bool *done);
CURLcode Curl_tftp(struct connectdata *conn, bool *done);
CURLcode Curl_tftp_done(struct connectdata *conn, CURLcode, bool premature);
#endif
#endif
