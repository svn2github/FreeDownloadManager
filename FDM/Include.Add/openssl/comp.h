/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    


#ifndef HEADER_COMP_H
#define HEADER_COMP_H

#include <openssl/crypto.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct comp_ctx_st COMP_CTX;

typedef struct comp_method_st
	{
	int type;		
	const char *name;	
	int (*init)(COMP_CTX *ctx);
	void (*finish)(COMP_CTX *ctx);
	int (*compress)(COMP_CTX *ctx,
			unsigned char *out, unsigned int olen,
			unsigned char *in, unsigned int ilen);
	int (*expand)(COMP_CTX *ctx,
		      unsigned char *out, unsigned int olen,
		      unsigned char *in, unsigned int ilen);
	
	long (*ctrl)(void);
	long (*callback_ctrl)(void);
	} COMP_METHOD;

struct comp_ctx_st
	{
	COMP_METHOD *meth;
	unsigned long compress_in;
	unsigned long compress_out;
	unsigned long expand_in;
	unsigned long expand_out;

	CRYPTO_EX_DATA	ex_data;
	}; 

COMP_CTX *COMP_CTX_new(COMP_METHOD *meth);
void COMP_CTX_free(COMP_CTX *ctx);
int COMP_compress_block(COMP_CTX *ctx, unsigned char *out, int olen,
	unsigned char *in, int ilen);
int COMP_expand_block(COMP_CTX *ctx, unsigned char *out, int olen,
	unsigned char *in, int ilen);
COMP_METHOD *COMP_rle(void );
COMP_METHOD *COMP_zlib(void );  

void ERR_load_COMP_strings(void);      

#ifdef  __cplusplus
}
#endif
#endif
