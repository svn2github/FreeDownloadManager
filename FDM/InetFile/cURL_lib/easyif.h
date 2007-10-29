/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __EASYIF_H
#define __EASYIF_H  

void Curl_easy_addmulti(struct SessionHandle *data, void *multi);

void Curl_easy_initHandleData(struct SessionHandle *data);

CURLcode Curl_convert_to_network(struct SessionHandle *data,
                                 char *buffer, size_t length);
CURLcode Curl_convert_from_network(struct SessionHandle *data,
                                 char *buffer, size_t length);
CURLcode Curl_convert_from_utf8(struct SessionHandle *data,
                                 char *buffer, size_t length);

#endif 
