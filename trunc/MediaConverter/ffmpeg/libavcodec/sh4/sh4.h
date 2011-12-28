/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_SH4_SH4_H
#define AVCODEC_SH4_SH4_H

#ifdef __SH4__
#   define fp_single_enter(fpscr)                               \
    do {                                                        \
        __asm__ volatile ("sts   fpscr, %0     \n\t"            \
                          "and   %1,    %0     \n\t"            \
                          "lds   %0,    fpscr  \n\t"            \
                          : "=&r"(fpscr) : "r"(~(1<<19)));      \
    } while (0)

#   define fp_single_leave(fpscr)                       \
    do {                                                \
        __asm__ volatile ("or    %1,    %0     \n\t"    \
                          "lds   %0,    fpscr  \n\t"    \
                          : "+r"(fpscr) : "r"(1<<19));  \
    } while (0)
#else
#   define fp_single_enter(fpscr) ((void)fpscr)
#   define fp_single_leave(fpscr)
#endif

#endif 
