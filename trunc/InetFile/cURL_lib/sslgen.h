/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __SSLGEN_H
#define __SSLGEN_H


bool Curl_ssl_config_matches(struct ssl_config_data* data,
                             struct ssl_config_data* needle);
bool Curl_clone_ssl_config(struct ssl_config_data* source,
                           struct ssl_config_data* dest);
void Curl_free_ssl_config(struct ssl_config_data* sslc);

#ifdef USE_SSL
int Curl_ssl_init(void);
void Curl_ssl_cleanup(void);
CURLcode Curl_ssl_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_connect_nonblocking(struct connectdata *conn,
                                      int sockindex,
                                      bool *done);

void Curl_ssl_close_all(struct SessionHandle *data);
void Curl_ssl_close(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_shutdown(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_set_engine(struct SessionHandle *data, const char *engine);

CURLcode Curl_ssl_set_engine_default(struct SessionHandle *data);
struct curl_slist *Curl_ssl_engines_list(struct SessionHandle *data);
ssize_t Curl_ssl_send(struct connectdata *conn,
                      int sockindex,
                      const void *mem,
                      size_t len);
ssize_t Curl_ssl_recv(struct connectdata *conn, 
                      int sockindex,            
                      char *mem,                
                      size_t len);              

CURLcode Curl_ssl_initsessions(struct SessionHandle *, long);
size_t Curl_ssl_version(char *buffer, size_t size);
bool Curl_ssl_data_pending(const struct connectdata *conn,
                           int connindex);
int Curl_ssl_check_cxn(struct connectdata *conn);
void Curl_ssl_free_certinfo(struct SessionHandle *data);




int Curl_ssl_getsessionid(struct connectdata *conn,
                          void **ssl_sessionid,
                          size_t *idsize) ;

CURLcode Curl_ssl_addsessionid(struct connectdata *conn,
                               void *ssl_sessionid,
                               size_t idsize);

#define SSL_SHUTDOWN_TIMEOUT 10000 

#else

#define Curl_ssl_init() 1
#define Curl_ssl_cleanup() do { } while (0)
#define Curl_ssl_connect(x,y) CURLE_FAILED_INIT
#define Curl_ssl_close_all(x)
#define Curl_ssl_close(x,y)
#define Curl_ssl_shutdown(x,y) CURLE_FAILED_INIT
#define Curl_ssl_set_engine(x,y) CURLE_FAILED_INIT
#define Curl_ssl_set_engine_default(x) CURLE_FAILED_INIT
#define Curl_ssl_engines_list(x) NULL
#define Curl_ssl_send(a,b,c,d) -1
#define Curl_ssl_recv(a,b,c,d) -1
#define Curl_ssl_initsessions(x,y) CURLE_OK
#define Curl_ssl_version(x,y) 0
#define Curl_ssl_data_pending(x,y) 0
#define Curl_ssl_check_cxn(x) 0
#define Curl_ssl_free_certinfo(x)

#endif

#endif 
