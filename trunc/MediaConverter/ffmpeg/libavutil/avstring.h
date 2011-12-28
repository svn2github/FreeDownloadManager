/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_AVSTRING_H
#define AVUTIL_AVSTRING_H

#include <stddef.h>


int av_strstart(const char *str, const char *pfx, const char **ptr);


int av_stristart(const char *str, const char *pfx, const char **ptr);


char *av_stristr(const char *haystack, const char *needle);


size_t av_strlcpy(char *dst, const char *src, size_t size);


size_t av_strlcat(char *dst, const char *src, size_t size);


size_t av_strlcatf(char *dst, size_t size, const char *fmt, ...);


char *av_d2str(double d);

#endif 
