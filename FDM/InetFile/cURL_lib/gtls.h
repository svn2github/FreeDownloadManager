/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __GTLS_H
#define __GTLS_H

int Curl_gtls_init(void);
int Curl_gtls_cleanup(void);
CURLcode Curl_gtls_connect(struct connectdata *conn, int sockindex); 

void Curl_gtls_close_all(struct SessionHandle *data);

 
void Curl_gtls_close(struct connectdata *conn, int sockindex); 

ssize_t Curl_gtls_send(struct connectdata *conn, int sockindex,
                       void *mem, size_t len);
ssize_t Curl_gtls_recv(struct connectdata *conn, 
                       int num,                  
                       char *buf,                
                       size_t buffersize,        
                       bool *wouldblock);
void Curl_gtls_session_free(void *ptr);
size_t Curl_gtls_version(char *buffer, size_t size);
int Curl_gtls_shutdown(struct connectdata *conn, int sockindex);

#endif
