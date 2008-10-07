/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_EASY_H
#define __CURL_EASY_H

#ifdef  __cplusplus
extern "C" {
#endif

CURL_EXTERN CURL *curl_easy_init(void);
CURL_EXTERN CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
CURL_EXTERN CURLcode curl_easy_perform(CURL *curl);
CURL_EXTERN void curl_easy_cleanup(CURL *curl); 

CURL_EXTERN CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, ...);  

CURL_EXTERN CURL* curl_easy_duphandle(CURL *curl); 

CURL_EXTERN void curl_easy_reset(CURL *curl); 

CURL_EXTERN CURLcode curl_easy_recv(CURL *curl, void *buffer, size_t buflen,
                                    size_t *n); 

CURL_EXTERN CURLcode curl_easy_send(CURL *curl, const void *buffer,
                                    size_t buflen, size_t *n);

#ifdef  __cplusplus
}
#endif

#endif
