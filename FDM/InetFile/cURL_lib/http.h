/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_H
#define __HTTP_H 

#ifndef CURL_DISABLE_HTTP
bool Curl_compareheader(char *headerline,     
                        const char *header,   
                        const char *content);  

CURLcode Curl_proxyCONNECT(struct connectdata *conn,
                           int tunnelsocket,
                           char *hostname, int remote_port); 

CURLcode Curl_http(struct connectdata *conn, bool *done);
CURLcode Curl_http_done(struct connectdata *, CURLcode);
CURLcode Curl_http_connect(struct connectdata *conn, bool *done); 

void Curl_httpchunk_init(struct connectdata *conn);
CHUNKcode Curl_httpchunk_read(struct connectdata *conn, char *datap,
                              ssize_t length, ssize_t *wrote); 

void Curl_http_auth_stage(struct SessionHandle *data, int stage);
CURLcode Curl_http_input_auth(struct connectdata *conn,
                              int httpcode, char *header);
CURLcode Curl_http_auth_act(struct connectdata *conn);

int Curl_http_should_fail(struct connectdata *conn); 

#define CURLAUTH_PICKNONE (1<<30)  

#ifndef MAX_INITIAL_POST_SIZE
#define MAX_INITIAL_POST_SIZE 1024
#endif

#endif
#endif
