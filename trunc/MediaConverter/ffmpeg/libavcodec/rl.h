/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_RL_H
#define AVCODEC_RL_H

#include <stdint.h>
#include "get_bits.h"


#define MAX_RUN    64
#define MAX_LEVEL  64


typedef struct RLTable {
    int n;                         
    int last;                      
    const uint16_t (*table_vlc)[2];
    const int8_t *table_run;
    const int8_t *table_level;
    uint8_t *index_run[2];         
    int8_t *max_level[2];          
    int8_t *max_run[2];            
    VLC vlc;                       
    RL_VLC_ELEM *rl_vlc[32];       
} RLTable;


void init_rl(RLTable *rl, uint8_t static_store[2][2*MAX_RUN + MAX_LEVEL + 3]);
void init_vlc_rl(RLTable *rl);

#define INIT_VLC_RL(rl, static_size)\
{\
    int q;\
    static RL_VLC_ELEM rl_vlc_table[32][static_size];\
    INIT_VLC_STATIC(&rl.vlc, 9, rl.n + 1,\
             &rl.table_vlc[0][1], 4, 2,\
             &rl.table_vlc[0][0], 4, 2, static_size);\
\
    if(!rl.rl_vlc[0]){\
        for(q=0; q<32; q++)\
            rl.rl_vlc[q]= rl_vlc_table[q];\
\
        init_vlc_rl(&rl);\
    }\
}

static inline int get_rl_index(const RLTable *rl, int last, int run, int level)
{
    int index;
    index = rl->index_run[last][run];
    if (index >= rl->n)
        return rl->n;
    if (level > rl->max_level[last][run])
        return rl->n;
    return index + level - 1;
}

#endif 
