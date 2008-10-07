/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __COOKIE_H
#define __COOKIE_H 

#include <stdio.h>
#if defined(WIN32)
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#endif

#include <curl/curl.h>

struct Cookie {
  struct Cookie *next; 
  char *name;        
  char *value;       
  char *path;         
  char *domain;      
  curl_off_t expires;  
  char *expirestr;   
  bool tailmatch;    

  
  char *version;     
  char *maxage;      

  bool secure;       
  bool livecookie;   
  bool httponly;     
};

struct CookieInfo {
  
  struct Cookie *cookies;

  char *filename;  
  bool running;    
  long numcookies; 
  bool newsession; 
}; 

#define MAX_COOKIE_LINE 5000
#define MAX_COOKIE_LINE_TXT "4999" 

#define MAX_NAME 1024
#define MAX_NAME_TXT "1023"

struct SessionHandle; 

struct Cookie *Curl_cookie_add(struct SessionHandle *data,
                               struct CookieInfo *, bool header, char *lineptr,
                               const char *domain, const char *path);

struct CookieInfo *Curl_cookie_init(struct SessionHandle *data,
                                    const char *, struct CookieInfo *, bool);
struct Cookie *Curl_cookie_getlist(struct CookieInfo *, const char *,
		                   const char *, bool);
void Curl_cookie_freelist(struct Cookie *cookies, bool cookiestoo);
void Curl_cookie_clearall(struct CookieInfo *cookies);
void Curl_cookie_clearsess(struct CookieInfo *cookies);
void Curl_cookie_cleanup(struct CookieInfo *);
int Curl_cookie_output(struct CookieInfo *, const char *);

#if defined(CURL_DISABLE_HTTP) || defined(CURL_DISABLE_COOKIES)
#define Curl_cookie_list(x) NULL
#define Curl_cookie_loadfiles(x) do { } while (0)
#else
struct curl_slist *Curl_cookie_list(struct SessionHandle *data);
void Curl_cookie_loadfiles(struct SessionHandle *data);
#endif

#endif
