/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef HEADER_HMAC_H
#define HEADER_HMAC_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_HMAC
#error HMAC is disabled.
#endif

#include <openssl/evp.h>

#define HMAC_MAX_MD_CBLOCK	128	

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct hmac_ctx_st
	{
	const EVP_MD *md;
	EVP_MD_CTX md_ctx;
	EVP_MD_CTX i_ctx;
	EVP_MD_CTX o_ctx;
	unsigned int key_length;
	unsigned char key[HMAC_MAX_MD_CBLOCK];
	} HMAC_CTX;

#define HMAC_size(e)	(EVP_MD_size((e)->md))


void HMAC_CTX_init(HMAC_CTX *ctx);
void HMAC_CTX_cleanup(HMAC_CTX *ctx);

#define HMAC_cleanup(ctx) HMAC_CTX_cleanup(ctx) 

void HMAC_Init(HMAC_CTX *ctx, const void *key, int len,
	       const EVP_MD *md); 
void HMAC_Init_ex(HMAC_CTX *ctx, const void *key, int len,
		  const EVP_MD *md, ENGINE *impl);
void HMAC_Update(HMAC_CTX *ctx, const unsigned char *data, size_t len);
void HMAC_Final(HMAC_CTX *ctx, unsigned char *md, unsigned int *len);
unsigned char *HMAC(const EVP_MD *evp_md, const void *key, int key_len,
		    const unsigned char *d, size_t n, unsigned char *md,
		    unsigned int *md_len);


#ifdef  __cplusplus
}
#endif

#endif
