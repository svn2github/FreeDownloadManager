/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef AVCODEC_BFIN_CONFIG_BFIN_H
#define AVCODEC_BFIN_CONFIG_BFIN_H

#include "config.h"

#ifndef DEFUN

#define mL3 .text
#ifndef mL1
#if defined(__FDPIC__) && CONFIG_SRAM
#define mL1 .l1.text
#else
#define mL1 mL3
#endif
#endif

#define DEFUN(fname,where,interface) \
        .section where;              \
        .global _ff_bfin_ ## fname ; \
        .type _ff_bfin_ ## fname, STT_FUNC; \
        .align 8;                    \
        _ff_bfin_ ## fname

#define DEFUN_END(fname) \
        .size _ff_bfin_ ## fname, . - _ff_bfin_ ## fname

#ifdef __FDPIC__
#define RELOC(reg,got,obj) reg = [got + obj@GOT17M4]
#else
#define RELOC(reg,got,obj) reg.L = obj; reg.H = obj
#endif

#endif

#endif 
