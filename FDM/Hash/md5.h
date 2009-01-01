/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

            

#ifndef ___MD5_H___
#define ___MD5_H___  

#ifndef UINT4
typedef unsigned long int UINT4;
#endif  

typedef struct {
	UINT4 i[2];                   
	UINT4 buf[4];                                    
	unsigned char in[64];                              
	unsigned char digest[16];     
} MD5_CTX;

static void MD5_Transform (UINT4 *buf, UINT4 *in);

void MD5Init(MD5_CTX *mdContext, unsigned long pseudoRandomNumber = 0);
void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *mdContext);
int md5file (char *fn , unsigned long seed , MD5_CTX *mdContext) ;

#include "md5.cpp"

#endif 
