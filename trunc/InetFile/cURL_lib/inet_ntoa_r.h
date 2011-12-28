/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __INET_NTOA_R_H
#define __INET_NTOA_R_H


#include "setup.h"

#ifdef HAVE_INET_NTOA_R_2_ARGS

char *inet_ntoa_r(const struct in_addr in, char buffer[]);

#else

char *inet_ntoa_r(const struct in_addr in, char *buffer, int buflen);

#endif

#endif
