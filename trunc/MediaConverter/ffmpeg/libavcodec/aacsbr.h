/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_AACSBR_H
#define AVCODEC_AACSBR_H

#include "get_bits.h"
#include "aac.h"
#include "sbr.h"


av_cold void ff_aac_sbr_init(void);

av_cold void ff_aac_sbr_ctx_init(SpectralBandReplication *sbr);

av_cold void ff_aac_sbr_ctx_close(SpectralBandReplication *sbr);

int ff_decode_sbr_extension(AACContext *ac, SpectralBandReplication *sbr,
                            GetBitContext *gb, int crc, int cnt, int id_aac);

void ff_sbr_dequant(AACContext *ac, SpectralBandReplication *sbr, int id_aac);

void ff_sbr_apply(AACContext *ac, SpectralBandReplication *sbr, int ch,
                  const float* in, float* out);

#endif 
