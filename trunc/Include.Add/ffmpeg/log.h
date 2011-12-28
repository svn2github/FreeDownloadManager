/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef LOG_H
#define LOG_H

#include <stdarg.h>


typedef struct AVCLASS AVClass;
struct AVCLASS {
    const char* class_name;
    const char* (*item_name)(void*); 
    const struct AVOption *option;
};



#if LIBAVUTIL_VERSION_INT < (50<<16)
#define AV_LOG_QUIET -1
#define AV_LOG_FATAL 0
#define AV_LOG_ERROR 0
#define AV_LOG_WARNING 1
#define AV_LOG_INFO 1
#define AV_LOG_VERBOSE 1
#define AV_LOG_DEBUG 2
#else
#define AV_LOG_QUIET    -8


#define AV_LOG_PANIC     0


#define AV_LOG_FATAL     8


#define AV_LOG_ERROR    16


#define AV_LOG_WARNING  24

#define AV_LOG_INFO     32
#define AV_LOG_VERBOSE  40


#define AV_LOG_DEBUG    48
#endif
extern int av_log_level;

#ifdef __GNUC__
extern void av_log(void*, int level, const char *fmt, ...) __attribute__ ((__format__ (__printf__, 3, 4)));
#else
extern void av_log(void*, int level, const char *fmt, ...);
#endif

#if LIBAVUTIL_VERSION_INT < (50<<16)
extern void av_vlog(void*, int level, const char *fmt, va_list);
extern int av_log_get_level(void);
extern void av_log_set_level(int);
extern void av_log_set_callback(void (*)(void*, int, const char*, va_list));
extern void av_log_default_callback(void* ptr, int level, const char* fmt, va_list vl);
#else
extern void (*av_vlog)(void*, int, const char*, va_list);
#endif

#endif 
