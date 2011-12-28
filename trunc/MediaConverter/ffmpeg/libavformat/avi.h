/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_AVI_H
#define AVFORMAT_AVI_H

#include "metadata.h"

#define AVIF_HASINDEX           0x00000010        
#define AVIF_MUSTUSEINDEX       0x00000020
#define AVIF_ISINTERLEAVED      0x00000100
#define AVIF_TRUSTCKTYPE        0x00000800        
#define AVIF_WASCAPTUREFILE     0x00010000
#define AVIF_COPYRIGHTED        0x00020000

#define AVI_MAX_RIFF_SIZE       0x40000000LL
#define AVI_MASTER_INDEX_SIZE   256


#define AVIIF_INDEX             0x10

extern const AVMetadataConv ff_avi_metadata_conv[];


extern const char ff_avi_tags[][5];

#endif 
