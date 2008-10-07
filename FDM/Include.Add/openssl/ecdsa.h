/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
  

#ifndef HEADER_ECDSA_H
#define HEADER_ECDSA_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_ECDSA
#error ECDSA is disabled.
#endif

#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ECDSA_SIG_st
	{
	BIGNUM *r;
	BIGNUM *s;
	} ECDSA_SIG; 

ECDSA_SIG *ECDSA_SIG_new(void); 

void	  ECDSA_SIG_free(ECDSA_SIG *a); 

int	  i2d_ECDSA_SIG(const ECDSA_SIG *a, unsigned char **pp); 

ECDSA_SIG *d2i_ECDSA_SIG(ECDSA_SIG **v, const unsigned char **pp, long len); 

ECDSA_SIG *ECDSA_do_sign(const unsigned char *dgst,int dgst_len,EC_KEY *eckey); 

ECDSA_SIG *ECDSA_do_sign_ex(const unsigned char *dgst, int dgstlen, 
		const BIGNUM *kinv, const BIGNUM *rp, EC_KEY *eckey); 

int	  ECDSA_do_verify(const unsigned char *dgst, int dgst_len,
		const ECDSA_SIG *sig, EC_KEY* eckey);

const ECDSA_METHOD *ECDSA_OpenSSL(void); 

void	  ECDSA_set_default_method(const ECDSA_METHOD *meth); 

const ECDSA_METHOD *ECDSA_get_default_method(void); 

int 	  ECDSA_set_method(EC_KEY *eckey, const ECDSA_METHOD *meth); 

int	  ECDSA_size(const EC_KEY *eckey); 

int 	  ECDSA_sign_setup(EC_KEY *eckey, BN_CTX *ctx, BIGNUM **kinv, 
		BIGNUM **rp); 

int	  ECDSA_sign(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, EC_KEY *eckey);  

int	  ECDSA_sign_ex(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, const BIGNUM *kinv,
		const BIGNUM *rp, EC_KEY *eckey); 

int 	  ECDSA_verify(int type, const unsigned char *dgst, int dgstlen, 
		const unsigned char *sig, int siglen, EC_KEY *eckey); 

int 	  ECDSA_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new 
		*new_func, CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int 	  ECDSA_set_ex_data(EC_KEY *d, int idx, void *arg);
void 	  *ECDSA_get_ex_data(EC_KEY *d, int idx);   

void ERR_load_ECDSA_strings(void);   

#define ECDSA_F_ECDSA_DATA_NEW_METHOD			 100
#define ECDSA_F_ECDSA_DO_SIGN				 101
#define ECDSA_F_ECDSA_DO_VERIFY				 102
#define ECDSA_F_ECDSA_SIGN_SETUP			 103 

#define ECDSA_R_BAD_SIGNATURE				 100
#define ECDSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE		 101
#define ECDSA_R_ERR_EC_LIB				 102
#define ECDSA_R_MISSING_PARAMETERS			 103
#define ECDSA_R_NEED_NEW_SETUP_VALUES			 106
#define ECDSA_R_RANDOM_NUMBER_GENERATION_FAILED		 104
#define ECDSA_R_SIGNATURE_MALLOC_FAILED			 105

#ifdef  __cplusplus
}
#endif
#endif
