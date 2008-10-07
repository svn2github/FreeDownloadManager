/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __QSSL_H
#define __QSSL_H   

#include "urldata.h"

int Curl_qsossl_init(void);
void Curl_qsossl_cleanup(void);
CURLcode Curl_qsossl_connect(struct connectdata * conn, int sockindex);
void Curl_qsossl_close(struct connectdata *conn, int sockindex);
int Curl_qsossl_close_all(struct SessionHandle * data);
int Curl_qsossl_shutdown(struct connectdata * conn, int sockindex);

ssize_t Curl_qsossl_send(struct connectdata * conn,
                         int sockindex,
                         const void * mem,
                         size_t len);
ssize_t Curl_qsossl_recv(struct connectdata * conn, 
                         int num,                   
                         char * buf,                
                         size_t buffersize,         
                         bool * wouldblock);

size_t Curl_qsossl_version(char * buffer, size_t size);
int Curl_qsossl_check_cxn(struct connectdata * cxn);

#endif
