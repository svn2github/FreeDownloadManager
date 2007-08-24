/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                

#include <stdio.h>
#include <stdlib.h>
#include "MD5.h"  

static unsigned char MD5_PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};  

#define MD5_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD5_G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define MD5_H(x, y, z) ((x) ^ (y) ^ (z))
#define MD5_I(x, y, z) ((y) ^ ((x) | (~z)))  

#ifndef ROTATE_LEFT
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#endif    

#define MD5_FF(a, b, c, d, x, s, ac) {(a) += MD5_F ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_GG(a, b, c, d, x, s, ac) {(a) += MD5_G ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_HH(a, b, c, d, x, s, ac) {(a) += MD5_H ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
#define MD5_II(a, b, c, d, x, s, ac) {(a) += MD5_I ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }  

#define MD5_S11 7  
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5  
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4  
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6  
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21  

static void MD5_Transform (UINT4 *buf, UINT4 *in)
{
	UINT4 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	
	MD5_FF ( a, b, c, d, in[ 0], MD5_S11, (UINT4) 3614090360u); 
	MD5_FF ( d, a, b, c, in[ 1], MD5_S12, (UINT4) 3905402710u); 
	MD5_FF ( c, d, a, b, in[ 2], MD5_S13, (UINT4)  606105819u); 
	MD5_FF ( b, c, d, a, in[ 3], MD5_S14, (UINT4) 3250441966u); 
	MD5_FF ( a, b, c, d, in[ 4], MD5_S11, (UINT4) 4118548399u); 
	MD5_FF ( d, a, b, c, in[ 5], MD5_S12, (UINT4) 1200080426u); 
	MD5_FF ( c, d, a, b, in[ 6], MD5_S13, (UINT4) 2821735955u); 
	MD5_FF ( b, c, d, a, in[ 7], MD5_S14, (UINT4) 4249261313u); 
	MD5_FF ( a, b, c, d, in[ 8], MD5_S11, (UINT4) 1770035416u); 
	MD5_FF ( d, a, b, c, in[ 9], MD5_S12, (UINT4) 2336552879u); 
	MD5_FF ( c, d, a, b, in[10], MD5_S13, (UINT4) 4294925233u); 
	MD5_FF ( b, c, d, a, in[11], MD5_S14, (UINT4) 2304563134u); 
	MD5_FF ( a, b, c, d, in[12], MD5_S11, (UINT4) 1804603682u); 
	MD5_FF ( d, a, b, c, in[13], MD5_S12, (UINT4) 4254626195u); 
	MD5_FF ( c, d, a, b, in[14], MD5_S13, (UINT4) 2792965006u); 
	MD5_FF ( b, c, d, a, in[15], MD5_S14, (UINT4) 1236535329u); 

	
	MD5_GG ( a, b, c, d, in[ 1], MD5_S21, (UINT4) 4129170786u); 
	MD5_GG ( d, a, b, c, in[ 6], MD5_S22, (UINT4) 3225465664u); 
	MD5_GG ( c, d, a, b, in[11], MD5_S23, (UINT4)  643717713u); 
	MD5_GG ( b, c, d, a, in[ 0], MD5_S24, (UINT4) 3921069994u); 
	MD5_GG ( a, b, c, d, in[ 5], MD5_S21, (UINT4) 3593408605u); 
	MD5_GG ( d, a, b, c, in[10], MD5_S22, (UINT4)   38016083u); 
	MD5_GG ( c, d, a, b, in[15], MD5_S23, (UINT4) 3634488961u); 
	MD5_GG ( b, c, d, a, in[ 4], MD5_S24, (UINT4) 3889429448u); 
	MD5_GG ( a, b, c, d, in[ 9], MD5_S21, (UINT4)  568446438u); 
	MD5_GG ( d, a, b, c, in[14], MD5_S22, (UINT4) 3275163606u); 
	MD5_GG ( c, d, a, b, in[ 3], MD5_S23, (UINT4) 4107603335u); 
	MD5_GG ( b, c, d, a, in[ 8], MD5_S24, (UINT4) 1163531501u); 
	MD5_GG ( a, b, c, d, in[13], MD5_S21, (UINT4) 2850285829u); 
	MD5_GG ( d, a, b, c, in[ 2], MD5_S22, (UINT4) 4243563512u); 
	MD5_GG ( c, d, a, b, in[ 7], MD5_S23, (UINT4) 1735328473u); 
	MD5_GG ( b, c, d, a, in[12], MD5_S24, (UINT4) 2368359562u); 

	
	MD5_HH ( a, b, c, d, in[ 5], MD5_S31, (UINT4) 4294588738u); 
	MD5_HH ( d, a, b, c, in[ 8], MD5_S32, (UINT4) 2272392833u); 
	MD5_HH ( c, d, a, b, in[11], MD5_S33, (UINT4) 1839030562u); 
	MD5_HH ( b, c, d, a, in[14], MD5_S34, (UINT4) 4259657740u); 
	MD5_HH ( a, b, c, d, in[ 1], MD5_S31, (UINT4) 2763975236u); 
	MD5_HH ( d, a, b, c, in[ 4], MD5_S32, (UINT4) 1272893353u); 
	MD5_HH ( c, d, a, b, in[ 7], MD5_S33, (UINT4) 4139469664u); 
	MD5_HH ( b, c, d, a, in[10], MD5_S34, (UINT4) 3200236656u); 
	MD5_HH ( a, b, c, d, in[13], MD5_S31, (UINT4)  681279174u); 
	MD5_HH ( d, a, b, c, in[ 0], MD5_S32, (UINT4) 3936430074u); 
	MD5_HH ( c, d, a, b, in[ 3], MD5_S33, (UINT4) 3572445317u); 
	MD5_HH ( b, c, d, a, in[ 6], MD5_S34, (UINT4)   76029189u); 
	MD5_HH ( a, b, c, d, in[ 9], MD5_S31, (UINT4) 3654602809u); 
	MD5_HH ( d, a, b, c, in[12], MD5_S32, (UINT4) 3873151461u); 
	MD5_HH ( c, d, a, b, in[15], MD5_S33, (UINT4)  530742520u); 
	MD5_HH ( b, c, d, a, in[ 2], MD5_S34, (UINT4) 3299628645u); 

	
	MD5_II ( a, b, c, d, in[ 0], MD5_S41, (UINT4) 4096336452u); 
	MD5_II ( d, a, b, c, in[ 7], MD5_S42, (UINT4) 1126891415u); 
	MD5_II ( c, d, a, b, in[14], MD5_S43, (UINT4) 2878612391u); 
	MD5_II ( b, c, d, a, in[ 5], MD5_S44, (UINT4) 4237533241u); 
	MD5_II ( a, b, c, d, in[12], MD5_S41, (UINT4) 1700485571u); 
	MD5_II ( d, a, b, c, in[ 3], MD5_S42, (UINT4) 2399980690u); 
	MD5_II ( c, d, a, b, in[10], MD5_S43, (UINT4) 4293915773u); 
	MD5_II ( b, c, d, a, in[ 1], MD5_S44, (UINT4) 2240044497u); 
	MD5_II ( a, b, c, d, in[ 8], MD5_S41, (UINT4) 1873313359u); 
	MD5_II ( d, a, b, c, in[15], MD5_S42, (UINT4) 4264355552u); 
	MD5_II ( c, d, a, b, in[ 6], MD5_S43, (UINT4) 2734768916u); 
	MD5_II ( b, c, d, a, in[13], MD5_S44, (UINT4) 1309151649u); 
	MD5_II ( a, b, c, d, in[ 4], MD5_S41, (UINT4) 4149444226u); 
	MD5_II ( d, a, b, c, in[11], MD5_S42, (UINT4) 3174756917u); 
	MD5_II ( c, d, a, b, in[ 2], MD5_S43, (UINT4)  718787259u); 
	MD5_II ( b, c, d, a, in[ 9], MD5_S44, (UINT4) 3951481745u); 

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}  

void MD5Init (MD5_CTX *mdContext, unsigned long pseudoRandomNumber)
{
	mdContext->i[0] = mdContext->i[1] = (UINT4)0;

	
	mdContext->buf[0] = (UINT4)0x67452301 + (pseudoRandomNumber * 11);
	mdContext->buf[1] = (UINT4)0xefcdab89 + (pseudoRandomNumber * 71);
	mdContext->buf[2] = (UINT4)0x98badcfe + (pseudoRandomNumber * 37);
	mdContext->buf[3] = (UINT4)0x10325476 + (pseudoRandomNumber * 97);
}

void MD5Update (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
	UINT4 in[16];
	int mdi = 0;
	unsigned int i = 0, ii = 0;

	
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	
	if ((mdContext->i[0] + ((UINT4)inLen << 3)) < mdContext->i[0])
		mdContext->i[1]++;
	mdContext->i[0] += ((UINT4)inLen << 3);
	mdContext->i[1] += ((UINT4)inLen >> 29);

	while (inLen--)
	{
		
		mdContext->in[mdi++] = *inBuf++;

		
		if (mdi == 0x40)
		{
			for (i = 0, ii = 0; i < 16; i++, ii += 4)
				in[i] = (((UINT4)mdContext->in[ii+3]) << 24) |
					(((UINT4)mdContext->in[ii+2]) << 16) |
					(((UINT4)mdContext->in[ii+1]) << 8) |
					((UINT4)mdContext->in[ii]);

			MD5_Transform (mdContext->buf, in);
			mdi = 0;
		}
	}
}

void MD5Final (MD5_CTX *mdContext)
{
	UINT4 in[16];
	int mdi = 0;
	unsigned int i = 0, ii = 0, padLen = 0;

	
	in[14] = mdContext->i[0];
	in[15] = mdContext->i[1];

	
	mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

	
	padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	MD5Update (mdContext, MD5_PADDING, padLen);

	
	for (i = 0, ii = 0; i < 14; i++, ii += 4)
		in[i] = (((UINT4)mdContext->in[ii+3]) << 24) |
			(((UINT4)mdContext->in[ii+2]) << 16) |
			(((UINT4)mdContext->in[ii+1]) <<  8) |
			((UINT4)mdContext->in[ii]);
	MD5_Transform (mdContext->buf, in);

	
	for (i = 0, ii = 0; i < 4; i++, ii += 4)
	{
		mdContext->digest[ii]   = (unsigned char)( mdContext->buf[i]        & 0xFF);
		mdContext->digest[ii+1] = (unsigned char)((mdContext->buf[i] >>  8) & 0xFF);
		mdContext->digest[ii+2] = (unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
		mdContext->digest[ii+3] = (unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
	}
}    

int md5file ( char *fn , unsigned long seed , MD5_CTX *mdContext )
{

	unsigned long size = 0;
	FILE *f = fopen ( fn , "rb" ) ;
	if ( f == NULL ) return 0 ;		

	MD5Init ( mdContext , seed ) ;

	char buf[2048] ;
	unsigned int trb = 0 ;
	for (;;) {
		int rb = fread ( buf , 1 , 2048 , f ) ;
		if ( size > 0 && rb + trb > size ) rb = size - trb ;
		trb += rb ;
		MD5Update ( mdContext , (unsigned char *) buf , rb ) ;
		if ( rb < 2048 || ( size > 0 && trb >= size ) ) break ;
	}
	fclose ( f ) ;
	MD5Final ( mdContext ) ;

	return trb ;
}