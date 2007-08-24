/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __TELNET_H
#define __TELNET_H 

#ifndef CURL_DISABLE_TELNET
CURLcode Curl_telnet(struct connectdata *conn, bool *done);
CURLcode Curl_telnet_done(struct connectdata *conn, CURLcode);
#endif
#endif
