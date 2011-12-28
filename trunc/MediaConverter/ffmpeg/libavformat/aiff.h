/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVFORMAT_AIFF_H
#define AVFORMAT_AIFF_H

#include "avformat.h"
#include "riff.h"

static const AVCodecTag ff_codec_aiff_tags[] = {
    { CODEC_ID_PCM_S16BE,    MKTAG('N','O','N','E') },
    { CODEC_ID_PCM_S8,       MKTAG('N','O','N','E') },
    { CODEC_ID_PCM_S24BE,    MKTAG('N','O','N','E') },
    { CODEC_ID_PCM_S32BE,    MKTAG('N','O','N','E') },
    { CODEC_ID_PCM_F32BE,    MKTAG('f','l','3','2') },
    { CODEC_ID_PCM_F64BE,    MKTAG('f','l','6','4') },
    { CODEC_ID_PCM_ALAW,     MKTAG('a','l','a','w') },
    { CODEC_ID_PCM_MULAW,    MKTAG('u','l','a','w') },
    { CODEC_ID_MACE3,        MKTAG('M','A','C','3') },
    { CODEC_ID_MACE6,        MKTAG('M','A','C','6') },
    { CODEC_ID_GSM,          MKTAG('G','S','M',' ') },
    { CODEC_ID_ADPCM_G726,   MKTAG('G','7','2','6') },
    { CODEC_ID_PCM_S16LE,    MKTAG('s','o','w','t') },
    { CODEC_ID_ADPCM_IMA_QT, MKTAG('i','m','a','4') },
    { CODEC_ID_QDM2,         MKTAG('Q','D','M','2') },
    { CODEC_ID_QCELP,        MKTAG('Q','c','l','p') },
    { CODEC_ID_NONE,         0 },
};

#endif 
