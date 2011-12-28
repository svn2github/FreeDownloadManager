/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __HASH_H
#define __HASH_H


#include "setup.h"

#include <stddef.h>

#include "llist.h"


typedef size_t (*hash_function) (void* key,
                                 size_t key_length,
                                 size_t slots_num);


typedef size_t (*comp_function) (void* key1,
                                 size_t key1_len,
                                 void*key2,
                                 size_t key2_len);

typedef void (*curl_hash_dtor)(void *);

struct curl_hash {
  struct curl_llist **table;

  
  hash_function hash_func;

  
  comp_function comp_func;
  curl_hash_dtor   dtor;
  int slots;
  size_t size;
};

struct curl_hash_element {
  void   *ptr;
  char   *key;
  size_t key_len;
};


int Curl_hash_init(struct curl_hash *h,
                   int slots,
                   hash_function hfunc,
                   comp_function comparator,
                   curl_hash_dtor dtor);

struct curl_hash *Curl_hash_alloc(int slots,
                                  hash_function hfunc,
                                  comp_function comparator,
                                  curl_hash_dtor dtor);

void *Curl_hash_add(struct curl_hash *h, void *key, size_t key_len, void *p);
int Curl_hash_delete(struct curl_hash *h, void *key, size_t key_len);
void *Curl_hash_pick(struct curl_hash *, void * key, size_t key_len);
void Curl_hash_apply(struct curl_hash *h, void *user,
                     void (*cb)(void *user, void *ptr));
int Curl_hash_count(struct curl_hash *h);
void Curl_hash_clean(struct curl_hash *h);
void Curl_hash_clean_with_criterium(struct curl_hash *h, void *user,
                                    int (*comp)(void *, void *));
void Curl_hash_destroy(struct curl_hash *h);

size_t Curl_hash_str(void* key, size_t key_length, size_t slots_num);
size_t Curl_str_key_compare(void*k1, size_t key1_len, void*k2,
                            size_t key2_len);

#endif
