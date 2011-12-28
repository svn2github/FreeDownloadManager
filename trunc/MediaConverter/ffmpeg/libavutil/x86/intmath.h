/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_X86_INTMATH_H
#define AVUTIL_X86_INTMATH_H

#define FASTDIV(a,b) \
    ({\
        int ret, dmy;\
        __asm__ volatile(\
            "mull %3"\
            :"=d"(ret), "=a"(dmy)\
            :"1"(a), "g"(ff_inverse[b])\
            );\
        ret;\
    })

#endif 
