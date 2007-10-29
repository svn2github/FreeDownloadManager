/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __MULTIIF_H
#define __MULTIIF_H  

void Curl_expire(struct SessionHandle *data, long milli);

void Curl_multi_rmeasy(void *multi, CURL *data);

bool Curl_multi_canPipeline(struct Curl_multi* multi);
void Curl_multi_handlePipeBreak(struct SessionHandle *data); 

#define GETSOCK_WRITEBITSTART 16

#define GETSOCK_BLANK 0  

#define GETSOCK_WRITESOCK(x) (1 << (GETSOCK_WRITEBITSTART + (x))) 

#define GETSOCK_READSOCK(x) (1 << (x))

#endif 
