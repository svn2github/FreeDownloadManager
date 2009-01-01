/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __PROGRESS_H
#define __PROGRESS_H 

#include "timeval.h" 

typedef enum {
  TIMER_NONE,
  TIMER_NAMELOOKUP,
  TIMER_CONNECT,
  TIMER_APPCONNECT,
  TIMER_PRETRANSFER,
  TIMER_STARTTRANSFER,
  TIMER_POSTRANSFER,
  TIMER_STARTSINGLE,
  TIMER_REDIRECT,
  TIMER_LAST 
} timerid;

void Curl_pgrsDone(struct connectdata *);
void Curl_pgrsStartNow(struct SessionHandle *data);
void Curl_pgrsSetDownloadSize(struct SessionHandle *data, curl_off_t size);
void Curl_pgrsSetUploadSize(struct SessionHandle *data, curl_off_t size);
void Curl_pgrsSetDownloadCounter(struct SessionHandle *data, curl_off_t size);
void Curl_pgrsSetUploadCounter(struct SessionHandle *data, curl_off_t size);
int Curl_pgrsUpdate(struct connectdata *);
void Curl_pgrsResetTimes(struct SessionHandle *data);
void Curl_pgrsTime(struct SessionHandle *data, timerid timer);  

#define LEAST_SIZE_PROGRESS BUFSIZE

#define PROGRESS_DOWNLOAD (1<<0)
#define PROGRESS_UPLOAD   (1<<1)
#define PROGRESS_DOWN_AND_UP (PROGRESS_UPLOAD | PROGRESS_DOWNLOAD)

#define PGRS_SHOW_DL (1<<0)
#define PGRS_SHOW_UL (1<<1)
#define PGRS_DONE_DL (1<<2)
#define PGRS_DONE_UL (1<<3)
#define PGRS_HIDE    (1<<4)
#define PGRS_UL_SIZE_KNOWN (1<<5)
#define PGRS_DL_SIZE_KNOWN (1<<6)

#define PGRS_HEADERS_OUT (1<<7)  

#endif 
