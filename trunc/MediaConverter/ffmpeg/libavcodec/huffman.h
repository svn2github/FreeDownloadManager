/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_HUFFMAN_H
#define AVCODEC_HUFFMAN_H

#include "avcodec.h"
#include "get_bits.h"

typedef struct {
    int16_t  sym;
    int16_t  n0;
    uint32_t count;
} Node;

#define FF_HUFFMAN_FLAG_HNODE_FIRST 0x01
#define FF_HUFFMAN_FLAG_ZERO_COUNT  0x02

typedef int (*HuffCmp)(const void *va, const void *vb);
int ff_huff_build_tree(AVCodecContext *avctx, VLC *vlc, int nb_codes,
                       Node *nodes, HuffCmp cmp, int flags);

#endif 
