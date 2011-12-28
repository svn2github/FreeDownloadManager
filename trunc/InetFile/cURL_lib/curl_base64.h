/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CURL_BASE64_H
#define __CURL_BASE64_H

size_t Curl_base64_encode(struct SessionHandle *data,
                          const char *input, size_t size, char **str);
size_t Curl_base64_decode(const char *source, unsigned char **outptr);
#endif
