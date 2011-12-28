/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PS2_MMI_H
#define AVCODEC_PS2_MMI_H

#define align16 __attribute__ ((aligned (16)))




#define         lq(base, off, reg)        \
        __asm__ volatile ("lq " #reg ", %0("#base ")" : : "i" (off) )

#define         lq2(mem, reg)        \
        __asm__ volatile ("lq " #reg ", %0" : : "r" (mem))

#define         sq(reg, off, base)        \
        __asm__ volatile ("sq " #reg ", %0("#base ")" : : "i" (off) )



#define         ld3(base, off, reg)        \
        __asm__ volatile (".word %0" : : "i" ( 0xdc000000 | (base<<21) | (reg<<16) | (off)))

#define         ldr3(base, off, reg)        \
        __asm__ volatile (".word %0" : : "i" ( 0x6c000000 | (base<<21) | (reg<<16) | (off)))

#define         ldl3(base, off, reg)        \
        __asm__ volatile (".word %0" : : "i" ( 0x68000000 | (base<<21) | (reg<<16) | (off)))



#define         sd3(reg, off, base)        \
        __asm__ volatile (".word %0" : : "i" ( 0xfc000000 | (base<<21) | (reg<<16) | (off)))

#define         sw(reg, off, base)        \
        __asm__ volatile ("sw " #reg ", " #off "("#base ")")

#define         sq2(reg, mem)        \
        __asm__ volatile ("sq " #reg ", %0" : : "m" (*(mem)))

#define         pinth(rs, rt, rd) \
        __asm__ volatile ("pinth  " #rd ", " #rs ", " #rt )

#define         phmadh(rs, rt, rd) \
        __asm__ volatile ("phmadh " #rd ", " #rs ", " #rt )

#define         pcpyud(rs, rt, rd) \
        __asm__ volatile ("pcpyud " #rd ", " #rs ", " #rt )

#define         pcpyld(rs, rt, rd) \
        __asm__ volatile ("pcpyld " #rd ", " #rs ", " #rt )

#define         pcpyh(rt, rd) \
        __asm__ volatile ("pcpyh  " #rd ", " #rt )

#define         paddw(rs, rt, rd) \
        __asm__ volatile ("paddw  " #rd ", " #rs ", " #rt )

#define         pextlw(rs, rt, rd) \
        __asm__ volatile ("pextlw " #rd ", " #rs ", " #rt )

#define         pextuw(rs, rt, rd) \
        __asm__ volatile ("pextuw " #rd ", " #rs ", " #rt )

#define         pextlh(rs, rt, rd) \
        __asm__ volatile ("pextlh " #rd ", " #rs ", " #rt )

#define         pextuh(rs, rt, rd) \
        __asm__ volatile ("pextuh " #rd ", " #rs ", " #rt )

#define         psubw(rs, rt, rd) \
        __asm__ volatile ("psubw  " #rd ", " #rs ", " #rt )

#define         psraw(rt, sa, rd) \
        __asm__ volatile ("psraw  " #rd ", " #rt ", %0" : : "i"(sa) )

#define         ppach(rs, rt, rd) \
        __asm__ volatile ("ppach  " #rd ", " #rs ", " #rt )

#define         ppacb(rs, rt, rd) \
        __asm__ volatile ("ppacb  " #rd ", " #rs ", " #rt )

#define         prevh(rt, rd) \
        __asm__ volatile ("prevh  " #rd ", " #rt )

#define         pmulth(rs, rt, rd) \
        __asm__ volatile ("pmulth " #rd ", " #rs ", " #rt )

#define         pmaxh(rs, rt, rd) \
        __asm__ volatile ("pmaxh " #rd ", " #rs ", " #rt )

#define         pminh(rs, rt, rd) \
        __asm__ volatile ("pminh " #rd ", " #rs ", " #rt )

#define         pinteh(rs, rt, rd) \
        __asm__ volatile ("pinteh  " #rd ", " #rs ", " #rt )

#define         paddh(rs, rt, rd) \
        __asm__ volatile ("paddh  " #rd ", " #rs ", " #rt )

#define         psubh(rs, rt, rd) \
        __asm__ volatile ("psubh  " #rd ", " #rs ", " #rt )

#define         psrah(rt, sa, rd) \
        __asm__ volatile ("psrah  " #rd ", " #rt ", %0" : : "i"(sa) )

#define         pmfhl_uw(rd) \
        __asm__ volatile ("pmfhl.uw  " #rd)

#define         pextlb(rs, rt, rd) \
        __asm__ volatile ("pextlb  " #rd ", " #rs ", " #rt )

#endif 
