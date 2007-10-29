/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __URL_H
#define __URL_H 

#include <stdarg.h>   

CURLcode Curl_open(struct SessionHandle **curl);
CURLcode Curl_setopt(struct SessionHandle *data, CURLoption option,
                     va_list arg);
CURLcode Curl_close(struct SessionHandle *data); 
CURLcode Curl_connect(struct SessionHandle *, struct connectdata **,
                      bool *async, bool *protocol_connect);
CURLcode Curl_async_resolved(struct connectdata *conn,
                             bool *protocol_connect);
CURLcode Curl_do(struct connectdata **, bool *done);
CURLcode Curl_do_more(struct connectdata *);
CURLcode Curl_done(struct connectdata **, CURLcode, bool premature);
CURLcode Curl_disconnect(struct connectdata *);
CURLcode Curl_protocol_connect(struct connectdata *conn, bool *done);
CURLcode Curl_protocol_connecting(struct connectdata *conn, bool *done);
CURLcode Curl_protocol_doing(struct connectdata *conn, bool *done);
void Curl_safefree(void *ptr); 

struct conncache *Curl_mk_connc(int type, long amount);

void Curl_rm_connc(struct conncache *c);

CURLcode Curl_ch_connc(struct SessionHandle *data,
                       struct conncache *c,
                       long newamount);

int Curl_protocol_getsock(struct connectdata *conn,
                          curl_socket_t *socks,
                          int numsocks);
int Curl_doing_getsock(struct connectdata *conn,
                       curl_socket_t *socks,
                       int numsocks);

CURLcode Curl_addHandleToPipeline(struct SessionHandle *handle,
                                  struct curl_llist *pipe);
int Curl_removeHandleFromPipeline(struct SessionHandle *handle,
                                  struct curl_llist *pipe);
bool Curl_isHandleAtHead(struct SessionHandle *handle,
                         struct curl_llist *pipe);

void Curl_close_connections(struct SessionHandle *data);

#if 0
CURLcode Curl_protocol_fdset(struct connectdata *conn,
                             fd_set *read_fd_set,
                             fd_set *write_fd_set,
                             int *max_fdp);
CURLcode Curl_doing_fdset(struct connectdata *conn,
                          fd_set *read_fd_set,
                          fd_set *write_fd_set,
                          int *max_fdp);
#endif

#endif
