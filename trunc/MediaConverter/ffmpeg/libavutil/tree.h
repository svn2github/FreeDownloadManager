/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_TREE_H
#define AVUTIL_TREE_H

struct AVTreeNode;
extern const int av_tree_node_size;


void *av_tree_find(const struct AVTreeNode *root, void *key, int (*cmp)(void *key, const void *b), void *next[2]);


void *av_tree_insert(struct AVTreeNode **rootp, void *key, int (*cmp)(void *key, const void *b), struct AVTreeNode **next);
void av_tree_destroy(struct AVTreeNode *t);


void av_tree_enumerate(struct AVTreeNode *t, void *opaque, int (*cmp)(void *opaque, void *elem), int (*enu)(void *opaque, void *elem));


#endif 
