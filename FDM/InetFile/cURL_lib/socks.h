/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SOCKS_H
#define __SOCKS_H  

CURLcode Curl_SOCKS4(const char *proxy_name,
                     const char *hostname,
                     int remote_port,
                     int sockindex,
                     struct connectdata *conn,
                     bool protocol4a); 

CURLcode Curl_SOCKS5(const char *proxy_name,
                     const char *proxy_password,
                     const char *hostname,
                     int remote_port,
                     int sockindex,
                     struct connectdata *conn);

#endif
