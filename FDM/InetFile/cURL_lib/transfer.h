/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __TRANSFER_H
#define __TRANSFER_H

CURLcode Curl_perform(struct SessionHandle *data);
CURLcode Curl_pretransfer(struct SessionHandle *data);
CURLcode Curl_second_connect(struct connectdata *conn);
CURLcode Curl_posttransfer(struct SessionHandle *data);
CURLcode Curl_follow(struct SessionHandle *data, char *newurl, bool retry);
CURLcode Curl_readwrite(struct connectdata *conn, bool *done);
void Curl_single_fdset(struct connectdata *conn,
                       fd_set *read_fd_set,
                       fd_set *write_fd_set,
                       fd_set *exc_fd_set,
                       int *max_fd);
CURLcode Curl_readwrite_init(struct connectdata *conn);
CURLcode Curl_readrewind(struct connectdata *conn);
CURLcode Curl_fillreadbuffer(struct connectdata *conn, int bytes, int *nreadp);
bool Curl_retry_request(struct connectdata *conn, char **url);

CURLcode
Curl_Transfer (struct connectdata *data,
               int sockindex,           
               curl_off_t size,         
               bool getheader,          
               curl_off_t *bytecountp,  
               int writesockindex,      
               curl_off_t *writecountp 
);
#endif
