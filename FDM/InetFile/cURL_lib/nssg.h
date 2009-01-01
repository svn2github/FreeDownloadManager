/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __NSSG_H
#define __NSSG_H 

#ifdef USE_NSS 

#include "urldata.h"
CURLcode Curl_nss_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_nss_connect_nonblocking(struct connectdata *conn,
                                      int sockindex,
                                      bool *done);

void Curl_nss_close(struct connectdata *conn, int sockindex); 

int Curl_nss_close_all(struct SessionHandle *data);

int Curl_nss_init(void);
void Curl_nss_cleanup(void);

int Curl_nss_send(struct connectdata *conn,
                  int sockindex,
                  const void *mem,
                  size_t len);
ssize_t Curl_nss_recv(struct connectdata *conn, 
                      int num,                  
                      char *buf,                
                      size_t buffersize,        
                      bool *wouldblock);

size_t Curl_nss_version(char *buffer, size_t size);
int Curl_nss_check_cxn(struct connectdata *cxn);
int Curl_nss_seed(struct SessionHandle *data); 

#define curlssl_init Curl_nss_init
#define curlssl_cleanup Curl_nss_cleanup
#define curlssl_connect Curl_nss_connect 

#define curlssl_session_free(x)
#define curlssl_close_all Curl_nss_close_all
#define curlssl_close Curl_nss_close

#define curlssl_shutdown(x,y) (x=x, y=y, 1)
#define curlssl_set_engine(x,y) (x=x, y=y, CURLE_FAILED_INIT)
#define curlssl_set_engine_default(x) (x=x, CURLE_FAILED_INIT)
#define curlssl_engines_list(x) (x=x, (struct curl_slist *)NULL)
#define curlssl_send Curl_nss_send
#define curlssl_recv Curl_nss_recv
#define curlssl_version Curl_nss_version
#define curlssl_check_cxn(x) Curl_nss_check_cxn(x)
#define curlssl_data_pending(x,y) (x=x, y=y, 0)

#endif 
#endif
