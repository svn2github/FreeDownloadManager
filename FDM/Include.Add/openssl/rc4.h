/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_RC4_H
#define HEADER_RC4_H

#include <openssl/opensslconf.h> 
#ifdef OPENSSL_NO_RC4
#error RC4 is disabled.
#endif

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct rc4_key_st
	{
	RC4_INT x,y;
	RC4_INT data[256];
	} RC4_KEY;

 
const char *RC4_options(void);
void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data);
void RC4(RC4_KEY *key, unsigned long len, const unsigned char *indata,
		unsigned char *outdata);

#ifdef  __cplusplus
}
#endif

#endif
