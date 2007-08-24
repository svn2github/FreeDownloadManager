/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SENDF_H
#define __SENDF_H 

CURLcode Curl_sendf(curl_socket_t sockfd, struct connectdata *,
                    const char *fmt, ...);
void Curl_infof(struct SessionHandle *, const char *fmt, ...);
void Curl_failf(struct SessionHandle *, const char *fmt, ...);

#if defined(CURL_DISABLE_VERBOSE_STRINGS)
#if defined(__GNUC__)

#define infof(x...) 
#else  

#define infof (void)
#endif
#else
#define infof Curl_infof
#endif
#define failf Curl_failf

#define CLIENTWRITE_BODY   1
#define CLIENTWRITE_HEADER 2
#define CLIENTWRITE_BOTH   (CLIENTWRITE_BODY|CLIENTWRITE_HEADER)

CURLcode Curl_client_write(struct SessionHandle *data, int type, char *ptr,
                           size_t len); 

int Curl_read(struct connectdata *conn, curl_socket_t sockfd,
              char *buf, size_t buffersize,
              ssize_t *n);

CURLcode Curl_write(struct connectdata *conn,
                    curl_socket_t sockfd,
                    void *mem, size_t len,
                    ssize_t *written); 

int Curl_debug(struct SessionHandle *handle, curl_infotype type,
               char *data, size_t size,
               struct connectdata *conn); 

#endif
