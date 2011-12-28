/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __SOCKADDR_H
#define __SOCKADDR_H


#include "setup.h"

#ifdef HAVE_STRUCT_SOCKADDR_STORAGE
struct Curl_sockaddr_storage {
  struct sockaddr_storage buffer;
};
#else
struct Curl_sockaddr_storage {
  char buffer[256];   
};
#endif

#endif 
