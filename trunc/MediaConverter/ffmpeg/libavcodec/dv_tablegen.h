/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef DV_TABLEGEN_H
#define DV_TABLEGEN_H

#include <stdint.h>
#include "dv_vlc_data.h"

#if CONFIG_SMALL
#define DV_VLC_MAP_RUN_SIZE 15
#define DV_VLC_MAP_LEV_SIZE 23
#else
#define DV_VLC_MAP_RUN_SIZE  64
#define DV_VLC_MAP_LEV_SIZE 512 
#endif


struct dv_vlc_pair {
   uint32_t vlc;
   uint32_t size;
};

#if CONFIG_HARDCODED_TABLES
#define dv_vlc_map_tableinit()
#include "libavcodec/dv_tables.h"
#else
static struct dv_vlc_pair dv_vlc_map[DV_VLC_MAP_RUN_SIZE][DV_VLC_MAP_LEV_SIZE];

static void dv_vlc_map_tableinit(void)
{
    int i, j;
    for (i = 0; i < NB_DV_VLC - 1; i++) {
       if (dv_vlc_run[i] >= DV_VLC_MAP_RUN_SIZE)
           continue;
#if CONFIG_SMALL
       if (dv_vlc_level[i] >= DV_VLC_MAP_LEV_SIZE)
           continue;
#endif

       if (dv_vlc_map[dv_vlc_run[i]][dv_vlc_level[i]].size != 0)
           continue;

       dv_vlc_map[dv_vlc_run[i]][dv_vlc_level[i]].vlc  =
           dv_vlc_bits[i] << (!!dv_vlc_level[i]);
       dv_vlc_map[dv_vlc_run[i]][dv_vlc_level[i]].size =
           dv_vlc_len[i] + (!!dv_vlc_level[i]);
    }
    for (i = 0; i < DV_VLC_MAP_RUN_SIZE; i++) {
#if CONFIG_SMALL
       for (j = 1; j < DV_VLC_MAP_LEV_SIZE; j++) {
          if (dv_vlc_map[i][j].size == 0) {
              dv_vlc_map[i][j].vlc = dv_vlc_map[0][j].vlc |
                        (dv_vlc_map[i-1][0].vlc << (dv_vlc_map[0][j].size));
              dv_vlc_map[i][j].size = dv_vlc_map[i-1][0].size +
                                      dv_vlc_map[0][j].size;
          }
       }
#else
       for (j = 1; j < DV_VLC_MAP_LEV_SIZE/2; j++) {
          if (dv_vlc_map[i][j].size == 0) {
              dv_vlc_map[i][j].vlc = dv_vlc_map[0][j].vlc |
                        (dv_vlc_map[i-1][0].vlc << (dv_vlc_map[0][j].size));
              dv_vlc_map[i][j].size = dv_vlc_map[i-1][0].size +
                                      dv_vlc_map[0][j].size;
          }
          dv_vlc_map[i][((uint16_t)(-j))&0x1ff].vlc =
                                        dv_vlc_map[i][j].vlc | 1;
          dv_vlc_map[i][((uint16_t)(-j))&0x1ff].size =
                                        dv_vlc_map[i][j].size;
       }
#endif
    }
}
#endif 

#endif 
