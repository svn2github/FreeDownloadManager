/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVDEVICE_ALSA_AUDIO_H
#define AVDEVICE_ALSA_AUDIO_H

#include <alsa/asoundlib.h>
#include "config.h"
#include "libavformat/avformat.h"



#if HAVE_BIGENDIAN
#define DEFAULT_CODEC_ID CODEC_ID_PCM_S16BE
#else
#define DEFAULT_CODEC_ID CODEC_ID_PCM_S16LE
#endif

typedef struct {
    snd_pcm_t *h;
    int frame_size;  
    int period_size; 
} AlsaData;


int ff_alsa_open(AVFormatContext *s, snd_pcm_stream_t mode,
                 unsigned int *sample_rate,
                 int channels, enum CodecID *codec_id);


int ff_alsa_close(AVFormatContext *s1);


int ff_alsa_xrun_recover(AVFormatContext *s1, int err);

#endif 
