/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SSLGEN_H
#define __SSLGEN_H 

bool Curl_ssl_config_matches(struct ssl_config_data* data,
                             struct ssl_config_data* needle);
bool Curl_clone_ssl_config(struct ssl_config_data* source,
                           struct ssl_config_data* dest);
void Curl_free_ssl_config(struct ssl_config_data* sslc);

int Curl_ssl_init(void);
void Curl_ssl_cleanup(void);
CURLcode Curl_ssl_connect(struct connectdata *conn, int sockindex);
CURLcode Curl_ssl_connect_nonblocking(struct connectdata *conn,
                                      int sockindex,
                                      bool *done);
void Curl_ssl_close(struct connectdata *conn, int sockindex);

void Curl_ssl_close_all(struct SessionHandle *data);
CURLcode Curl_ssl_set_engine(struct SessionHandle *data, const char *engine);

CURLcode Curl_ssl_set_engine_default(struct SessionHandle *data);
ssize_t Curl_ssl_send(struct connectdata *conn,
                      int sockindex,
                      const void *mem,
                      size_t len);
ssize_t Curl_ssl_recv(struct connectdata *conn, 
                      int sockindex,            
                      char *mem,                
                      size_t len);               

CURLcode Curl_ssl_initsessions(struct SessionHandle *, long);

int Curl_ssl_getsessionid(struct connectdata *conn,
                          void **ssl_sessionid,
                          size_t *idsize) ;

CURLcode Curl_ssl_addsessionid(struct connectdata *conn,
                               void *ssl_sessionid,
                               size_t idsize); 

struct curl_slist *Curl_ssl_engines_list(struct SessionHandle *data);

size_t Curl_ssl_version(char *buffer, size_t size);

int Curl_ssl_check_cxn(struct connectdata *conn);

CURLcode Curl_ssl_shutdown(struct connectdata *conn, int sockindex);

bool Curl_ssl_data_pending(const struct connectdata *conn,
                           int connindex);

#if !defined(USE_SSL) && !defined(SSLGEN_C)

#define Curl_ssl_close_all(x)
#endif

#define SSL_SHUTDOWN_TIMEOUT 10000 

#endif
