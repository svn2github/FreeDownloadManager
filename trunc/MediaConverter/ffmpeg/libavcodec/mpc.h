/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPC_H
#define AVCODEC_MPC_H

#include "libavutil/lfg.h"
#include "avcodec.h"
#include "get_bits.h"
#include "dsputil.h"
#include "mpegaudio.h"

#include "mpcdata.h"

#define BANDS            32
#define SAMPLES_PER_BAND 36
#define MPC_FRAME_SIZE   (BANDS * SAMPLES_PER_BAND)


typedef struct {
    int msf; 
    int res[2];
    int scfi[2];
    int scf_idx[2][3];
    int Q[2];
}Band;

typedef struct {
    DSPContext dsp;
    GetBitContext gb;
    int IS, MSS, gapless;
    int lastframelen;
    int maxbands, last_max_band;
    int last_bits_used;
    int oldDSCF[2][BANDS];
    Band bands[BANDS];
    int Q[2][MPC_FRAME_SIZE];
    int cur_frame, frames;
    uint8_t *bits;
    int buf_size;
    AVLFG rnd;
    int frames_to_skip;
    
    DECLARE_ALIGNED(16, MPA_INT, synth_buf)[MPA_MAX_CHANNELS][512*2];
    int synth_buf_offset[MPA_MAX_CHANNELS];
    DECLARE_ALIGNED(16, int32_t, sb_samples)[MPA_MAX_CHANNELS][36][SBLIMIT];
} MPCContext;

void ff_mpc_init(void);
void ff_mpc_dequantize_and_synth(MPCContext *c, int maxband, void *dst);

#endif 
