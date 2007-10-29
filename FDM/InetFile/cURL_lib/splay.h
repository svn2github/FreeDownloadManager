/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SPLAY_H
#define __SPLAY_H 

struct Curl_tree {
  struct Curl_tree *smaller; 
  struct Curl_tree *larger;  
  struct Curl_tree *same;    
  int key;                   
  void *payload;             
};

struct Curl_tree *Curl_splay(int i, struct Curl_tree *t);
struct Curl_tree *Curl_splayinsert(int key, struct Curl_tree *t,
                                   struct Curl_tree *newnode);
#if 0
struct Curl_tree *Curl_splayremove(int key, struct Curl_tree *t,
                                   struct Curl_tree **removed);
#endif

struct Curl_tree *Curl_splaygetbest(int key, struct Curl_tree *t,
                                    struct Curl_tree **removed);
int Curl_splayremovebyaddr(struct Curl_tree *t,
                           struct Curl_tree *remove,
                           struct Curl_tree **newroot);

#ifdef CURLDEBUG
void Curl_splayprint(struct Curl_tree * t, int d, char output);
#else
#define Curl_splayprint(x,y,z)
#endif

#endif
