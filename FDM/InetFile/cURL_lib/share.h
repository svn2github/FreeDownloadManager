/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_SHARE_H
#define __CURL_SHARE_H  

#include "setup.h"
#include <curl/curl.h>
#include "cookie.h" 

#ifdef __SALFORDC__
#define CURL_VOLATILE
#else
#define CURL_VOLATILE volatile
#endif 

struct Curl_share {
  unsigned int specifier;
  CURL_VOLATILE unsigned int dirty;

  curl_lock_function lockfunc;
  curl_unlock_function unlockfunc;
  void *clientdata;

  struct curl_hash *hostcache;
  struct CookieInfo *cookies;
};

CURLSHcode Curl_share_lock (struct SessionHandle *, curl_lock_data,
                            curl_lock_access);
CURLSHcode Curl_share_unlock (struct SessionHandle *, curl_lock_data);

#endif 
