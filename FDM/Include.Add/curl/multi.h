/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_MULTI_H
#define __CURL_MULTI_H 

#if defined(_WIN32) && !defined(WIN32)

#define WIN32 1
#endif

#if defined(WIN32) && !defined(_WIN32_WCE) && !defined(__GNUC__) || \
  defined(__MINGW32__)
#if !(defined(_WINSOCKAPI_) || defined(_WINSOCK_H))

#include <winsock2.h>
#endif
#else 

#if defined(_AIX) || defined(NETWARE) || defined(__NetBSD__)
#include <sys/select.h>
#endif

#ifndef _WIN32_WCE
#include <sys/socket.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#endif 

#include "curl.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef void CURLM;

#ifdef HAVE_CURL_MULTI_SOCKET 

#ifndef curl_socket_typedef

#ifdef WIN32
typedef SOCKET curl_socket_t;
#define CURL_SOCKET_BAD INVALID_SOCKET
#else
typedef int curl_socket_t;
#define CURL_SOCKET_BAD -1
#endif
#define curl_socket_typedef
#endif 

#endif 

typedef enum {
  CURLM_CALL_MULTI_PERFORM = -1, 
  CURLM_OK,
  CURLM_BAD_HANDLE,      
  CURLM_BAD_EASY_HANDLE, 
  CURLM_OUT_OF_MEMORY,   
  CURLM_INTERNAL_ERROR,  
  CURLM_LAST
} CURLMcode;

typedef enum {
  CURLMSG_NONE, 
  CURLMSG_DONE, 
  CURLMSG_LAST 
} CURLMSG;

struct CURLMsg {
  CURLMSG msg;       
  CURL *easy_handle; 
  union {
    void *whatever;    
    CURLcode result;   
  } data;
};
typedef struct CURLMsg CURLMsg; 

CURL_EXTERN CURLM *curl_multi_init(void); 

CURL_EXTERN CURLMcode curl_multi_add_handle(CURLM *multi_handle,
                                            CURL *curl_handle);

 
CURL_EXTERN CURLMcode curl_multi_remove_handle(CURLM *multi_handle,
                                               CURL *curl_handle);

 
CURL_EXTERN CURLMcode curl_multi_fdset(CURLM *multi_handle,
                                       fd_set *read_fd_set,
                                       fd_set *write_fd_set,
                                       fd_set *exc_fd_set,
                                       int *max_fd);

 
CURL_EXTERN CURLMcode curl_multi_perform(CURLM *multi_handle,
                                         int *running_handles);

 
CURL_EXTERN CURLMcode curl_multi_cleanup(CURLM *multi_handle); 

CURL_EXTERN CURLMsg *curl_multi_info_read(CURLM *multi_handle,
                                          int *msgs_in_queue); 

CURL_EXTERN const char *curl_multi_strerror(CURLMcode);

#ifdef HAVE_CURL_MULTI_SOCKET

#define CURL_POLL_NONE   0
#define CURL_POLL_IN     1
#define CURL_POLL_OUT    2
#define CURL_POLL_INOUT  3
#define CURL_POLL_REMOVE 4

#define CURL_EASY_NONE (CURL *)0
#define CURL_EASY_TIMEOUT (CURL *)0
#define CURL_SOCKET_TIMEOUT CURL_SOCKET_BAD

typedef int (*curl_socket_callback)(CURL *easy,      
                                    curl_socket_t s, 
                                    int what,        
                                    void *userp);    

CURLMcode curl_multi_socket(CURLM *multi_handle,
                            curl_socket_t s,
                            CURL *easy,
                            curl_socket_callback callback,
                            void *userp); 

CURLMcode curl_multi_socket_all(CURLM *multi_handle,
                                curl_socket_callback callback,
                                void *userp);  

CURLMcode curl_multi_timeout(CURLM *multi_handle, long *milliseconds);

#endif 

#ifdef __cplusplus
} 
#endif

#endif
