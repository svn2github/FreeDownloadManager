/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_H
#define __HTTP_H 

#ifndef CURL_DISABLE_HTTP

extern const struct Curl_handler Curl_handler_http;

#ifdef USE_SSL
extern const struct Curl_handler Curl_handler_https;
#endif

bool Curl_compareheader(const char *headerline,  
                        const char *header,   
                        const char *content);  

CURLcode Curl_proxyCONNECT(struct connectdata *conn,
                           int tunnelsocket,
                           const char *hostname, unsigned short remote_port); 

CURLcode Curl_http(struct connectdata *conn, bool *done);
CURLcode Curl_http_done(struct connectdata *, CURLcode, bool premature);
CURLcode Curl_http_connect(struct connectdata *conn, bool *done); 

void Curl_httpchunk_init(struct connectdata *conn);
CHUNKcode Curl_httpchunk_read(struct connectdata *conn, char *datap,
                              ssize_t length, ssize_t *wrote); 

void Curl_http_auth_stage(struct SessionHandle *data, int stage);
CURLcode Curl_http_input_auth(struct connectdata *conn,
                              int httpcode, const char *header);
CURLcode Curl_http_auth_act(struct connectdata *conn);

int Curl_http_should_fail(struct connectdata *conn); 

#define CURLAUTH_PICKNONE (1<<30)  

#ifndef MAX_INITIAL_POST_SIZE
#define MAX_INITIAL_POST_SIZE (64*1024)
#endif

#ifndef TINY_INITIAL_POST_SIZE
#define TINY_INITIAL_POST_SIZE 1024
#endif

#endif
#endif
