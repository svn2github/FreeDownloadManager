/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __GTLS_H
#define __GTLS_H


#ifdef USE_GNUTLS

int Curl_gtls_init(void);
int Curl_gtls_cleanup(void);
CURLcode Curl_gtls_connect(struct connectdata *conn, int sockindex);


void Curl_gtls_close_all(struct SessionHandle *data);

 
void Curl_gtls_close(struct connectdata *conn, int sockindex);


ssize_t Curl_gtls_send(struct connectdata *conn, int sockindex,
                       const void *mem, size_t len);
ssize_t Curl_gtls_recv(struct connectdata *conn, 
                       int num,                  
                       char *buf,                
                       size_t buffersize,        
                       bool *wouldblock);
void Curl_gtls_session_free(void *ptr);
size_t Curl_gtls_version(char *buffer, size_t size);
int Curl_gtls_shutdown(struct connectdata *conn, int sockindex);


#define curlssl_init Curl_gtls_init
#define curlssl_cleanup Curl_gtls_cleanup
#define curlssl_connect Curl_gtls_connect
#define curlssl_session_free(x)  Curl_gtls_session_free(x)
#define curlssl_close_all Curl_gtls_close_all
#define curlssl_close Curl_gtls_close
#define curlssl_shutdown(x,y) Curl_gtls_shutdown(x,y)
#define curlssl_set_engine(x,y) (x=x, y=y, CURLE_FAILED_INIT)
#define curlssl_set_engine_default(x) (x=x, CURLE_FAILED_INIT)
#define curlssl_engines_list(x) (x=x, (struct curl_slist *)NULL)
#define curlssl_send Curl_gtls_send
#define curlssl_recv Curl_gtls_recv
#define curlssl_version Curl_gtls_version
#define curlssl_check_cxn(x) (x=x, -1)
#define curlssl_data_pending(x,y) (x=x, y=y, 0)

#endif 
#endif
