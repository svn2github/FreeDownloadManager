/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PPC_TYPES_ALTIVEC_H
#define AVCODEC_PPC_TYPES_ALTIVEC_H


#define vec_u8  vector unsigned char
#define vec_s8  vector signed char
#define vec_u16 vector unsigned short
#define vec_s16 vector signed short
#define vec_u32 vector unsigned int
#define vec_s32 vector signed int


#define LOAD_ZERO const vec_u8 zerov = vec_splat_u8( 0 )

#define zero_u8v  (vec_u8)  zerov
#define zero_s8v  (vec_s8)  zerov
#define zero_u16v (vec_u16) zerov
#define zero_s16v (vec_s16) zerov
#define zero_u32v (vec_u32) zerov
#define zero_s32v (vec_s32) zerov

#endif 
