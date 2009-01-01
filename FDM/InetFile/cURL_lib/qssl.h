/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __QSSL_H
#define __QSSL_H   

#include "urldata.h"

#ifdef USE_QSOSSL
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

#define curlssl_init Curl_qsossl_init
#define curlssl_cleanup Curl_qsossl_cleanup
#define curlssl_connect Curl_qsossl_connect 

#define curlssl_session_free(x)
#define curlssl_close_all Curl_qsossl_close_all
#define curlssl_close Curl_qsossl_close
#define curlssl_shutdown(x,y) Curl_qsossl_shutdown(x,y)
#define curlssl_set_engine(x,y) CURLE_FAILED_INIT
#define curlssl_set_engine_default(x) CURLE_FAILED_INIT
#define curlssl_engines_list(x) NULL
#define curlssl_send Curl_qsossl_send
#define curlssl_recv Curl_qsossl_recv
#define curlssl_version Curl_qsossl_version
#define curlssl_check_cxn(x) Curl_qsossl_check_cxn(x)
#define curlssl_data_pending(x,y) 0
#endif 
#endif
