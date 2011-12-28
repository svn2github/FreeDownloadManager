/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_GXF_H
#define AVFORMAT_GXF_H

typedef enum {
    PKT_MAP         = 0xbc,
    PKT_MEDIA       = 0xbf,
    PKT_EOS         = 0xfb,
    PKT_FLT         = 0xfc,
    PKT_UMF         = 0xfd,
} GXFPktType;

typedef enum {
    MAT_NAME        = 0x40,
    MAT_FIRST_FIELD = 0x41,
    MAT_LAST_FIELD  = 0x42,
    MAT_MARK_IN     = 0x43,
    MAT_MARK_OUT    = 0x44,
    MAT_SIZE        = 0x45,
} GXFMatTag;

typedef enum {
    TRACK_NAME      = 0x4c,
    TRACK_AUX       = 0x4d,
    TRACK_VER       = 0x4e,
    TRACK_MPG_AUX   = 0x4f,
    TRACK_FPS       = 0x50,
    TRACK_LINES     = 0x51,
    TRACK_FPF       = 0x52,
} GXFTrackTag;

#endif 
