/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SSLUSE_H
#define __SSLUSE_H   

#include "urldata.h"
CURLcode Curl_ossl_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_ossl_connect_nonblocking(struct connectdata *conn,
                                       int sockindex,
                                       bool *done); 

void Curl_ossl_close(struct connectdata *conn, int sockindex); 

int Curl_ossl_close_all(struct SessionHandle *data); 

CURLcode Curl_ossl_set_engine(struct SessionHandle *data, const char *engine); 

void Curl_ossl_session_free(void *ptr); 

CURLcode Curl_ossl_set_engine_default(struct SessionHandle *data); 

struct curl_slist *Curl_ossl_engines_list(struct SessionHandle *data);

int Curl_ossl_init(void);
void Curl_ossl_cleanup(void);

ssize_t Curl_ossl_send(struct connectdata *conn,
                       int sockindex,
                       const void *mem,
                       size_t len);
ssize_t Curl_ossl_recv(struct connectdata *conn, 
                       int num,                  
                       char *buf,                
                       size_t buffersize,        
                       bool *wouldblock);

size_t Curl_ossl_version(char *buffer, size_t size);
int Curl_ossl_check_cxn(struct connectdata *cxn);
int Curl_ossl_seed(struct SessionHandle *data);

int Curl_ossl_shutdown(struct connectdata *conn, int sockindex);

#endif
