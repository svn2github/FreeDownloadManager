/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
  

#ifndef HEADER_DH_H
#define HEADER_DH_H

#include <openssl/e_os2.h>

#ifdef OPENSSL_NO_DH
#error DH is disabled.
#endif

#ifndef OPENSSL_NO_BIO
#include <openssl/bio.h>
#endif
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif
	
#ifndef OPENSSL_DH_MAX_MODULUS_BITS
# define OPENSSL_DH_MAX_MODULUS_BITS	10000
#endif

#define DH_FLAG_CACHE_MONT_P     0x01
#define DH_FLAG_NO_EXP_CONSTTIME 0x02 

#ifdef  __cplusplus
extern "C" {
#endif    

struct dh_method
	{
	const char *name;
	
	int (*generate_key)(DH *dh);
	int (*compute_key)(unsigned char *key,const BIGNUM *pub_key,DH *dh);
	int (*bn_mod_exp)(const DH *dh, BIGNUM *r, const BIGNUM *a,
				const BIGNUM *p, const BIGNUM *m, BN_CTX *ctx,
				BN_MONT_CTX *m_ctx); 

	int (*init)(DH *dh);
	int (*finish)(DH *dh);
	int flags;
	char *app_data;
	
	int (*generate_params)(DH *dh, int prime_len, int generator, BN_GENCB *cb);
	};

struct dh_st
	{
	
	int pad;
	int version;
	BIGNUM *p;
	BIGNUM *g;
	long length; 
	BIGNUM *pub_key;	
	BIGNUM *priv_key;	

	int flags;
	BN_MONT_CTX *method_mont_p;
	
	BIGNUM *q;
	BIGNUM *j;
	unsigned char *seed;
	int seedlen;
	BIGNUM *counter;

	int references;
	CRYPTO_EX_DATA ex_data;
	const DH_METHOD *meth;
	ENGINE *engine;
	};

#define DH_GENERATOR_2		2

#define DH_GENERATOR_5		5 

#define DH_CHECK_P_NOT_PRIME		0x01
#define DH_CHECK_P_NOT_SAFE_PRIME	0x02
#define DH_UNABLE_TO_CHECK_GENERATOR	0x04
#define DH_NOT_SUITABLE_GENERATOR	0x08 

#define DH_CHECK_PUBKEY_TOO_SMALL	0x01
#define DH_CHECK_PUBKEY_TOO_LARGE	0x02 

#define DH_CHECK_P_NOT_STRONG_PRIME	DH_CHECK_P_NOT_SAFE_PRIME

#define DHparams_dup(x) ASN1_dup_of_const(DH,i2d_DHparams,d2i_DHparams,x)
#define d2i_DHparams_fp(fp,x) (DH *)ASN1_d2i_fp((char *(*)())DH_new, \
		(char *(*)())d2i_DHparams,(fp),(unsigned char **)(x))
#define i2d_DHparams_fp(fp,x) ASN1_i2d_fp(i2d_DHparams,(fp), \
		(unsigned char *)(x))
#define d2i_DHparams_bio(bp,x) ASN1_d2i_bio_of(DH,DH_new,d2i_DHparams,bp,x)
#define i2d_DHparams_bio(bp,x) ASN1_i2d_bio_of_const(DH,i2d_DHparams,bp,x)

const DH_METHOD *DH_OpenSSL(void);

void DH_set_default_method(const DH_METHOD *meth);
const DH_METHOD *DH_get_default_method(void);
int DH_set_method(DH *dh, const DH_METHOD *meth);
DH *DH_new_method(ENGINE *engine);

DH *	DH_new(void);
void	DH_free(DH *dh);
int	DH_up_ref(DH *dh);
int	DH_size(const DH *dh);
int DH_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	     CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int DH_set_ex_data(DH *d, int idx, void *arg);
void *DH_get_ex_data(DH *d, int idx); 

#ifndef OPENSSL_NO_DEPRECATED
DH *	DH_generate_parameters(int prime_len,int generator,
		void (*callback)(int,int,void *),void *cb_arg);
#endif  

int	DH_generate_parameters_ex(DH *dh, int prime_len,int generator, BN_GENCB *cb);

int	DH_check(const DH *dh,int *codes);
int	DH_check_pub_key(const DH *dh,const BIGNUM *pub_key, int *codes);
int	DH_generate_key(DH *dh);
int	DH_compute_key(unsigned char *key,const BIGNUM *pub_key,DH *dh);
DH *	d2i_DHparams(DH **a,const unsigned char **pp, long length);
int	i2d_DHparams(const DH *a,unsigned char **pp);
#ifndef OPENSSL_NO_FP_API
int	DHparams_print_fp(FILE *fp, const DH *x);
#endif
#ifndef OPENSSL_NO_BIO
int	DHparams_print(BIO *bp, const DH *x);
#else
int	DHparams_print(char *bp, const DH *x);
#endif  

void ERR_load_DH_strings(void);   

#define DH_F_COMPUTE_KEY				 102
#define DH_F_DHPARAMS_PRINT				 100
#define DH_F_DHPARAMS_PRINT_FP				 101
#define DH_F_DH_BUILTIN_GENPARAMS			 106
#define DH_F_DH_NEW_METHOD				 105
#define DH_F_GENERATE_KEY				 103
#define DH_F_GENERATE_PARAMETERS			 104 

#define DH_R_BAD_GENERATOR				 101
#define DH_R_INVALID_PUBKEY				 102
#define DH_R_MODULUS_TOO_LARGE				 103
#define DH_R_NO_PRIVATE_VALUE				 100

#ifdef  __cplusplus
}
#endif
#endif
