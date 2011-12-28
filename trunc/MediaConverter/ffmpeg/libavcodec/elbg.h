/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ELBG_H
#define AVCODEC_ELBG_H

#include "libavutil/lfg.h"


void ff_do_elbg(int *points, int dim, int numpoints, int *codebook,
                int numCB, int num_steps, int *closest_cb,
                AVLFG *rand_state);


void ff_init_elbg(int *points, int dim, int numpoints, int *codebook,
                  int numCB, int num_steps, int *closest_cb,
                  AVLFG *rand_state);

#endif 
