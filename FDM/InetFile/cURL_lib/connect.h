/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CONNECT_H
#define __CONNECT_H 

int Curl_nonblock(curl_socket_t sockfd,    
                  int nonblock   );

CURLcode Curl_is_connected(struct connectdata *conn,
                           int sockindex,
                           bool *connected);

CURLcode Curl_connecthost(struct connectdata *conn,
                          struct Curl_dns_entry *host, 
                          curl_socket_t *sockconn, 
                          Curl_addrinfo **addr, 
                          bool *connected 
                          );

int Curl_ourerrno(void);

CURLcode Curl_store_ip_addr(struct connectdata *conn);

#define DEFAULT_CONNECT_TIMEOUT 300000 

#endif
