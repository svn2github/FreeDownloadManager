/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SPLAY_H
#define __SPLAY_H 

struct Curl_tree {
  struct Curl_tree *smaller; 
  struct Curl_tree *larger;  
  struct Curl_tree *same;    
  struct timeval key;        
  void *payload;             
};

struct Curl_tree *Curl_splay(struct timeval i,
                             struct Curl_tree *t);

struct Curl_tree *Curl_splayinsert(struct timeval key,
                                   struct Curl_tree *t,
                                   struct Curl_tree *newnode);

#if 0
struct Curl_tree *Curl_splayremove(struct timeval key,
                                   struct Curl_tree *t,
                                   struct Curl_tree **removed);
#endif

struct Curl_tree *Curl_splaygetbest(struct timeval key,
                                    struct Curl_tree *t,
                                    struct Curl_tree **removed);

int Curl_splayremovebyaddr(struct Curl_tree *t,
                           struct Curl_tree *removenode,
                           struct Curl_tree **newroot);

#define Curl_splaycomparekeys(i,j) ( ((i.tv_sec)  < (j.tv_sec))  ? -1 : \
                                   ( ((i.tv_sec)  > (j.tv_sec))  ?  1 : \
                                   ( ((i.tv_usec) < (j.tv_usec)) ? -1 : \
                                   ( ((i.tv_usec) > (j.tv_usec)) ?  1 : 0 ))))

#ifdef CURLDEBUG
void Curl_splayprint(struct Curl_tree * t, int d, char output);
#else
#define Curl_splayprint(x,y,z)
#endif

#endif
