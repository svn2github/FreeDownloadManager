/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CURL_MULTI_H
#define __CURL_MULTI_H




#include "curl.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef void CURLM;

typedef enum {
  CURLM_CALL_MULTI_PERFORM = -1, 
  CURLM_OK,
  CURLM_BAD_HANDLE,      
  CURLM_BAD_EASY_HANDLE, 
  CURLM_OUT_OF_MEMORY,   
  CURLM_INTERNAL_ERROR,  
  CURLM_BAD_SOCKET,      
  CURLM_UNKNOWN_OPTION,  
  CURLM_LAST
} CURLMcode;


#define CURLM_CALL_MULTI_SOCKET CURLM_CALL_MULTI_PERFORM

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


#define CURL_POLL_NONE   0
#define CURL_POLL_IN     1
#define CURL_POLL_OUT    2
#define CURL_POLL_INOUT  3
#define CURL_POLL_REMOVE 4

#define CURL_SOCKET_TIMEOUT CURL_SOCKET_BAD

#define CURL_CSELECT_IN   0x01
#define CURL_CSELECT_OUT  0x02
#define CURL_CSELECT_ERR  0x04

typedef int (*curl_socket_callback)(CURL *easy,      
                                    curl_socket_t s, 
                                    int what,        
                                    void *userp,     
                                    void *socketp);  

typedef int (*curl_multi_timer_callback)(CURLM *multi,    
                                         long timeout_ms, 
                                         void *userp);    

CURL_EXTERN CURLMcode curl_multi_socket(CURLM *multi_handle, curl_socket_t s,
                                        int *running_handles);

CURL_EXTERN CURLMcode curl_multi_socket_action(CURLM *multi_handle,
                                               curl_socket_t s,
                                               int ev_bitmask,
                                               int *running_handles);

CURL_EXTERN CURLMcode curl_multi_socket_all(CURLM *multi_handle,
                                            int *running_handles);

#ifndef CURL_ALLOW_OLD_MULTI_SOCKET

#define curl_multi_socket(x,y,z) curl_multi_socket_action(x,y,0,z)
#endif


CURL_EXTERN CURLMcode curl_multi_timeout(CURLM *multi_handle,
                                         long *milliseconds);

#undef CINIT 

#ifdef CURL_ISOCPP
#define CINIT(name,type,num) CURLMOPT_ ## name = CURLOPTTYPE_ ## type + num
#else

#define LONG          CURLOPTTYPE_LONG
#define OBJECTPOINT   CURLOPTTYPE_OBJECTPOINT
#define FUNCTIONPOINT CURLOPTTYPE_FUNCTIONPOINT
#define OFF_T         CURLOPTTYPE_OFF_T
#define CINIT(name,type,number) CURLMOPT_name = type + number
#endif

typedef enum {
  
  CINIT(SOCKETFUNCTION, FUNCTIONPOINT, 1),

  
  CINIT(SOCKETDATA, OBJECTPOINT, 2),

    
  CINIT(PIPELINING, LONG, 3),

   
  CINIT(TIMERFUNCTION, FUNCTIONPOINT, 4),

  
  CINIT(TIMERDATA, OBJECTPOINT, 5),

  
  CINIT(MAXCONNECTS, LONG, 6),

  CURLMOPT_LASTENTRY 
} CURLMoption;



CURL_EXTERN CURLMcode curl_multi_setopt(CURLM *multi_handle,
                                        CURLMoption option, ...);



CURL_EXTERN CURLMcode curl_multi_assign(CURLM *multi_handle,
                                        curl_socket_t sockfd, void *sockp);

#ifdef __cplusplus
} 
#endif

#endif
