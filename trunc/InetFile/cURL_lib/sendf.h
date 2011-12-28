/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __SENDF_H
#define __SENDF_H


#include "setup.h"

CURLcode Curl_sendf(curl_socket_t sockfd, struct connectdata *,
                    const char *fmt, ...);
void Curl_infof(struct SessionHandle *, const char *fmt, ...);
void Curl_failf(struct SessionHandle *, const char *fmt, ...);

#if defined(CURL_DISABLE_VERBOSE_STRINGS)

#if defined(HAVE_VARIADIC_MACROS_C99)
#define infof(...)  do { } while (0)
#elif defined(HAVE_VARIADIC_MACROS_GCC)
#define infof(x...)  do { } while (0)
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

CURLcode Curl_client_write(struct connectdata *conn, int type, char *ptr,
                           size_t len);

void Curl_read_rewind(struct connectdata *conn,
                      size_t extraBytesRead);


int Curl_read_plain(curl_socket_t sockfd,
                    char *buf,
                    size_t bytesfromsocket,
                    ssize_t *n);


int Curl_read(struct connectdata *conn, curl_socket_t sockfd,
              char *buf, size_t buffersize,
              ssize_t *n);

CURLcode Curl_write(struct connectdata *conn,
                    curl_socket_t sockfd,
                    const void *mem, size_t len,
                    ssize_t *written);


CURLcode Curl_write_plain(struct connectdata *conn,
                          curl_socket_t sockfd,
                          const void *mem, size_t len,
                          ssize_t *written);


int Curl_debug(struct SessionHandle *handle, curl_infotype type,
               char *data, size_t size,
               struct connectdata *conn);


#endif 
