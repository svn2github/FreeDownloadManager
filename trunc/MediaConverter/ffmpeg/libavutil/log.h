/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_LOG_H
#define AVUTIL_LOG_H

#include <stdarg.h>
#include "avutil.h"


typedef struct {
    
    const char* class_name;

    
    const char* (*item_name)(void* ctx);

    
    const struct AVOption *option;
} AVClass;



#define AV_LOG_QUIET    -8


#define AV_LOG_PANIC     0


#define AV_LOG_FATAL     8


#define AV_LOG_ERROR    16


#define AV_LOG_WARNING  24

#define AV_LOG_INFO     32
#define AV_LOG_VERBOSE  40


#define AV_LOG_DEBUG    48


#ifdef __GNUC__
void av_log(void*, int level, const char *fmt, ...) __attribute__ ((__format__ (__printf__, 3, 4)));
#else
void av_log(void*, int level, const char *fmt, ...);
#endif

void av_vlog(void*, int level, const char *fmt, va_list);
int av_log_get_level(void);
void av_log_set_level(int);
void av_log_set_callback(void (*)(void*, int, const char*, va_list));
void av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl);

#endif 
