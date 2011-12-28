/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_OGGDEC_H
#define AVFORMAT_OGGDEC_H

#include "avformat.h"
#include "metadata.h"

struct ogg_codec {
    const int8_t *magic;
    uint8_t magicsize;
    const int8_t *name;
    
    int (*header)(AVFormatContext *, int);
    int (*packet)(AVFormatContext *, int);
    
    uint64_t (*gptopts)(AVFormatContext *, int, uint64_t, int64_t *dts);
    
    int granule_is_start;
};

struct ogg_stream {
    uint8_t *buf;
    unsigned int bufsize;
    unsigned int bufpos;
    unsigned int pstart;
    unsigned int psize;
    unsigned int pflags;
    unsigned int pduration;
    uint32_t serial;
    uint64_t granule;
    int64_t lastpts;
    int64_t lastdts;
    int64_t sync_pos;   
    int64_t page_pos;   
    int flags;
    const struct ogg_codec *codec;
    int header;
    int nsegs, segp;
    uint8_t segments[255];
    int incomplete; 
    int page_end;   
    int keyframe_seek;
    void *private;
};

struct ogg_state {
    uint64_t pos;
    int curidx;
    struct ogg_state *next;
    int nstreams;
    struct ogg_stream streams[1];
};

struct ogg {
    struct ogg_stream *streams;
    int nstreams;
    int headers;
    int curidx;
    struct ogg_state *state;
};

#define OGG_FLAG_CONT 1
#define OGG_FLAG_BOS  2
#define OGG_FLAG_EOS  4

extern const struct ogg_codec ff_dirac_codec;
extern const struct ogg_codec ff_flac_codec;
extern const struct ogg_codec ff_ogm_audio_codec;
extern const struct ogg_codec ff_ogm_old_codec;
extern const struct ogg_codec ff_ogm_text_codec;
extern const struct ogg_codec ff_ogm_video_codec;
extern const struct ogg_codec ff_old_dirac_codec;
extern const struct ogg_codec ff_old_flac_codec;
extern const struct ogg_codec ff_skeleton_codec;
extern const struct ogg_codec ff_speex_codec;
extern const struct ogg_codec ff_theora_codec;
extern const struct ogg_codec ff_vorbis_codec;

int ff_vorbis_comment(AVFormatContext *ms, AVMetadata **m, const uint8_t *buf, int size);

static inline int
ogg_find_stream (struct ogg * ogg, int serial)
{
    int i;

    for (i = 0; i < ogg->nstreams; i++)
        if (ogg->streams[i].serial == serial)
            return i;

    return -1;
}

static inline uint64_t
ogg_gptopts (AVFormatContext * s, int i, uint64_t gp, int64_t *dts)
{
    struct ogg *ogg = s->priv_data;
    struct ogg_stream *os = ogg->streams + i;
    uint64_t pts = AV_NOPTS_VALUE;

    if(os->codec && os->codec->gptopts){
        pts = os->codec->gptopts(s, i, gp, dts);
    } else {
        pts = gp;
        if (dts)
            *dts = pts;
    }

    return pts;
}

#endif 
