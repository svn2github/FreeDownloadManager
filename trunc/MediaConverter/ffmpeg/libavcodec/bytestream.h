/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_BYTESTREAM_H
#define AVCODEC_BYTESTREAM_H

#include <string.h>
#include "libavutil/common.h"
#include "libavutil/intreadwrite.h"

#define DEF_T(type, name, bytes, read, write)                             \
static av_always_inline type bytestream_get_ ## name(const uint8_t **b){\
    (*b) += bytes;\
    return read(*b - bytes);\
}\
static av_always_inline void bytestream_put_ ##name(uint8_t **b, const type value){\
    write(*b, value);\
    (*b) += bytes;\
}

#define DEF(name, bytes, read, write) \
    DEF_T(unsigned int, name, bytes, read, write)
#define DEF64(name, bytes, read, write) \
    DEF_T(uint64_t, name, bytes, read, write)

DEF64(le64, 8, AV_RL64, AV_WL64)
DEF  (le32, 4, AV_RL32, AV_WL32)
DEF  (le24, 3, AV_RL24, AV_WL24)
DEF  (le16, 2, AV_RL16, AV_WL16)
DEF64(be64, 8, AV_RB64, AV_WB64)
DEF  (be32, 4, AV_RB32, AV_WB32)
DEF  (be24, 3, AV_RB24, AV_WB24)
DEF  (be16, 2, AV_RB16, AV_WB16)
DEF  (byte, 1, AV_RB8 , AV_WB8 )

#undef DEF
#undef DEF64
#undef DEF_T

static av_always_inline unsigned int bytestream_get_buffer(const uint8_t **b, uint8_t *dst, unsigned int size)
{
    memcpy(dst, *b, size);
    (*b) += size;
    return size;
}

static av_always_inline void bytestream_put_buffer(uint8_t **b, const uint8_t *src, unsigned int size)
{
    memcpy(*b, src, size);
    (*b) += size;
}

#endif 
