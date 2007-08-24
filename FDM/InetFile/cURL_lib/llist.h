/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LLIST_H
#define __LLIST_H 

#include "setup.h"
#include <stddef.h>

typedef void (*curl_llist_dtor)(void *, void *);

struct curl_llist_element {
  void *ptr;

  struct curl_llist_element *prev;
  struct curl_llist_element *next;
};

struct curl_llist {
  struct curl_llist_element *head;
  struct curl_llist_element *tail;

  curl_llist_dtor dtor;

  size_t size;
};

void Curl_llist_init(struct curl_llist *, curl_llist_dtor);
struct curl_llist *Curl_llist_alloc(curl_llist_dtor);
int Curl_llist_insert_next(struct curl_llist *, struct curl_llist_element *,
                           const void *);
int Curl_llist_insert_prev(struct curl_llist *, struct curl_llist_element *,
                           const void *);
int Curl_llist_remove(struct curl_llist *, struct curl_llist_element *,
                      void *);
int Curl_llist_remove_next(struct curl_llist *, struct curl_llist_element *,
                           void *);
size_t Curl_llist_count(struct curl_llist *);
void Curl_llist_destroy(struct curl_llist *, void *);

#endif
