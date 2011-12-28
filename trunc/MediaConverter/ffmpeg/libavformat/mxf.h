/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_MXF_H
#define AVFORMAT_MXF_H

#include "libavcodec/avcodec.h"
#include <stdint.h>

typedef uint8_t UID[16];

enum MXFMetadataSetType {
    AnyType,
    MaterialPackage,
    SourcePackage,
    SourceClip,
    TimecodeComponent,
    Sequence,
    MultipleDescriptor,
    Descriptor,
    Track,
    CryptoContext,
    Preface,
    Identification,
    ContentStorage,
    SubDescriptor,
    IndexTableSegment,
    EssenceContainerData,
    TypeBottom,
};

typedef struct {
    UID key;
    int64_t offset;
    uint64_t length;
} KLVPacket;

typedef struct {
    UID uid;
    unsigned matching_len;
    int id;
} MXFCodecUL;

extern const MXFCodecUL ff_mxf_data_definition_uls[];
extern const MXFCodecUL ff_mxf_codec_uls[];

#ifdef DEBUG
#define PRINT_KEY(pc, s, x) dprintf(pc, "%s %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", s, \
                             (x)[0], (x)[1], (x)[2], (x)[3], (x)[4], (x)[5], (x)[6], (x)[7], (x)[8], (x)[9], (x)[10], (x)[11], (x)[12], (x)[13], (x)[14], (x)[15])
#else
#define PRINT_KEY(pc, s, x)
#endif

#endif 
