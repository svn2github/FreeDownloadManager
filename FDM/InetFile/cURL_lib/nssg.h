/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __NSSG_H
#define __NSSG_H   

#include "urldata.h"
CURLcode Curl_nss_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_nss_connect_nonblocking(struct connectdata *conn,
                                      int sockindex,
                                      bool *done);
void Curl_nss_close(struct connectdata *conn); 

int Curl_nss_close_all(struct SessionHandle *data);

int Curl_nss_init(void);
void Curl_nss_cleanup(void);

int Curl_nss_send(struct connectdata *conn,
                  int sockindex,
                  void *mem,
                  size_t len);
ssize_t Curl_nss_recv(struct connectdata *conn, 
                      int num,                  
                      char *buf,                
                      size_t buffersize,        
                      bool *wouldblock);

size_t Curl_nss_version(char *buffer, size_t size);
int Curl_nss_check_cxn(struct connectdata *cxn);
int Curl_nss_seed(struct SessionHandle *data);

#endif
