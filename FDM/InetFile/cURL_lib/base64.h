/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __BASE64_H
#define __BASE64_H

size_t Curl_base64_encode(const char *input, size_t size, char **str);
size_t Curl_base64_decode(const char *source, unsigned char **outptr);
#endif
