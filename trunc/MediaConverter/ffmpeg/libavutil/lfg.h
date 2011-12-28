/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_LFG_H
#define AVUTIL_LFG_H

typedef struct {
    unsigned int state[64];
    int index;
} AVLFG;

void av_lfg_init(AVLFG *c, unsigned int seed);


static inline unsigned int av_lfg_get(AVLFG *c){
    c->state[c->index & 63] = c->state[(c->index-24) & 63] + c->state[(c->index-55) & 63];
    return c->state[c->index++ & 63];
}


static inline unsigned int av_mlfg_get(AVLFG *c){
    unsigned int a= c->state[(c->index-55) & 63];
    unsigned int b= c->state[(c->index-24) & 63];
    return c->state[c->index++ & 63] = 2*a*b+a+b;
}


void av_bmg_get(AVLFG *lfg, double out[2]);

#endif 
