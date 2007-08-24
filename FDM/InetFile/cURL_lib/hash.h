/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HASH_H
#define __HASH_H 

#include "setup.h"

#include <stddef.h>

#include "llist.h"

typedef void (*curl_hash_dtor)(void *);

struct curl_hash {
  struct curl_llist **table;
  curl_hash_dtor   dtor;
  int slots;
  size_t size;
};

struct curl_hash_element {
  void   *ptr;
  char   *key;
  size_t key_len;
}; 

int Curl_hash_init(struct curl_hash *, int, curl_hash_dtor);
struct curl_hash *Curl_hash_alloc(int, curl_hash_dtor);
void *Curl_hash_add(struct curl_hash *, char *, size_t, void *);
int Curl_hash_delete(struct curl_hash *h, char *key, size_t key_len);
void *Curl_hash_pick(struct curl_hash *, char *, size_t);
void Curl_hash_apply(struct curl_hash *h, void *user,
                     void (*cb)(void *user, void *ptr));
int Curl_hash_count(struct curl_hash *h);
void Curl_hash_clean(struct curl_hash *h);
void Curl_hash_clean_with_criterium(struct curl_hash *h, void *user,
                                    int (*comp)(void *, void *));
void Curl_hash_destroy(struct curl_hash *h);

#endif
