/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __GETINFO_H
#define __GETINFO_H

CURLcode Curl_getinfo(struct SessionHandle *data, CURLINFO info, ...);
CURLcode Curl_initinfo(struct SessionHandle *data);

#endif
