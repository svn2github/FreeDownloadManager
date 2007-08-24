/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
 

#ifndef _CURL_STRTOK_R_H
#define _CURL_STRTOK_R_H

#include "setup.h"
#include <stddef.h>

#ifndef HAVE_STRTOK_R
char *Curl_strtok_r(char *s, const char *delim, char **last);
#define strtok_r Curl_strtok_r
#else
#include <string.h>
#endif

#endif

