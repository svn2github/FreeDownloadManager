/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_UNARY_H
#define AVCODEC_UNARY_H

#include "get_bits.h"


static inline int get_unary(GetBitContext *gb, int stop, int len)
{
    int i;

    for(i = 0; i < len && get_bits1(gb) != stop; i++);
    return i;
}


static inline int get_unary_0_33(GetBitContext *gb)
{
    return get_unary(gb, 0, 33);
}

static inline int get_unary_0_9(GetBitContext *gb)
{
    return get_unary(gb, 0, 9);
}

#endif 
