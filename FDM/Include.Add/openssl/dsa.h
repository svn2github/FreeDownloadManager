/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

    

#ifndef HEADER_DSA_H
#define HEADER_DSA_H

#include <openssl/e_os2.h>

#ifdef OPENSSL_NO_DSA
#error DSA is disabled.
#endif

#ifndef OPENSSL_NO_BIO
#include <openssl/bio.h>
#endif
#include <openssl/crypto.h>
#include <openssl/ossl_typ.h>

#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#ifndef OPENSSL_NO_DH
# include <openssl/dh.h>
#endif
#endif

#ifndef OPENSSL_DSA_MAX_MODULUS_BITS
# define OPENSSL_DSA_MAX_MODULUS_BITS	10000
#endif

#define DSA_FLAG_CACHE_MONT_P	0x01
#define DSA_FLAG_NO_EXP_CONSTTIME       0x02 

#ifdef  __cplusplus
extern "C" {
#endif    

typedef struct DSA_SIG_st
	{
	BIGNUM *r;
	BIGNUM *s;
	} DSA_SIG;

struct dsa_method
	{
	const char *name;
	DSA_SIG * (*dsa_do_sign)(const unsigned char *dgst, int dlen, DSA *dsa);
	int (*dsa_sign_setup)(DSA *dsa, BN_CTX *ctx_in, BIGNUM **kinvp,
								BIGNUM **rp);
	int (*dsa_do_verify)(const unsigned char *dgst, int dgst_len,
							DSA_SIG *sig, DSA *dsa);
	int (*dsa_mod_exp)(DSA *dsa, BIGNUM *rr, BIGNUM *a1, BIGNUM *p1,
			BIGNUM *a2, BIGNUM *p2, BIGNUM *m, BN_CTX *ctx,
			BN_MONT_CTX *in_mont);
	int (*bn_mod_exp)(DSA *dsa, BIGNUM *r, BIGNUM *a, const BIGNUM *p,
				const BIGNUM *m, BN_CTX *ctx,
				BN_MONT_CTX *m_ctx); 
	int (*init)(DSA *dsa);
	int (*finish)(DSA *dsa);
	int flags;
	char *app_data;
	
	int (*dsa_paramgen)(DSA *dsa, int bits,
			unsigned char *seed, int seed_len,
			int *counter_ret, unsigned long *h_ret,
			BN_GENCB *cb);
	
	int (*dsa_keygen)(DSA *dsa);
	};

struct dsa_st
	{
	
	int pad;
	long version;
	int write_params;
	BIGNUM *p;
	BIGNUM *q;	
	BIGNUM *g;

	BIGNUM *pub_key;  
	BIGNUM *priv_key; 

	BIGNUM *kinv;	
	BIGNUM *r;	

	int flags;
	
	BN_MONT_CTX *method_mont_p;
	int references;
	CRYPTO_EX_DATA ex_data;
	const DSA_METHOD *meth;
	
	ENGINE *engine;
	};

#define DSAparams_dup(x) ASN1_dup_of_const(DSA,i2d_DSAparams,d2i_DSAparams,x)
#define d2i_DSAparams_fp(fp,x) (DSA *)ASN1_d2i_fp((char *(*)())DSA_new, \
		(char *(*)())d2i_DSAparams,(fp),(unsigned char **)(x))
#define i2d_DSAparams_fp(fp,x) ASN1_i2d_fp(i2d_DSAparams,(fp), \
		(unsigned char *)(x))
#define d2i_DSAparams_bio(bp,x) ASN1_d2i_bio_of(DSA,DSA_new,d2i_DSAparams,bp,x)
#define i2d_DSAparams_bio(bp,x) ASN1_i2d_bio_of_const(DSA,i2d_DSAparams,bp,x) 

DSA_SIG * DSA_SIG_new(void);
void	DSA_SIG_free(DSA_SIG *a);
int	i2d_DSA_SIG(const DSA_SIG *a, unsigned char **pp);
DSA_SIG * d2i_DSA_SIG(DSA_SIG **v, const unsigned char **pp, long length);

DSA_SIG * DSA_do_sign(const unsigned char *dgst,int dlen,DSA *dsa);
int	DSA_do_verify(const unsigned char *dgst,int dgst_len,
		      DSA_SIG *sig,DSA *dsa);

const DSA_METHOD *DSA_OpenSSL(void);

void	DSA_set_default_method(const DSA_METHOD *);
const DSA_METHOD *DSA_get_default_method(void);
int	DSA_set_method(DSA *dsa, const DSA_METHOD *);

DSA *	DSA_new(void);
DSA *	DSA_new_method(ENGINE *engine);
void	DSA_free (DSA *r);

int	DSA_up_ref(DSA *r);
int	DSA_size(const DSA *);
	
int	DSA_sign_setup( DSA *dsa,BN_CTX *ctx_in,BIGNUM **kinvp,BIGNUM **rp);
int	DSA_sign(int type,const unsigned char *dgst,int dlen,
		unsigned char *sig, unsigned int *siglen, DSA *dsa);
int	DSA_verify(int type,const unsigned char *dgst,int dgst_len,
		const unsigned char *sigbuf, int siglen, DSA *dsa);
int DSA_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	     CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int DSA_set_ex_data(DSA *d, int idx, void *arg);
void *DSA_get_ex_data(DSA *d, int idx);

DSA *	d2i_DSAPublicKey(DSA **a, const unsigned char **pp, long length);
DSA *	d2i_DSAPrivateKey(DSA **a, const unsigned char **pp, long length);
DSA * 	d2i_DSAparams(DSA **a, const unsigned char **pp, long length); 

#ifndef OPENSSL_NO_DEPRECATED
DSA *	DSA_generate_parameters(int bits,
		unsigned char *seed,int seed_len,
		int *counter_ret, unsigned long *h_ret,void
		(*callback)(int, int, void *),void *cb_arg);
#endif  

int	DSA_generate_parameters_ex(DSA *dsa, int bits,
		unsigned char *seed,int seed_len,
		int *counter_ret, unsigned long *h_ret, BN_GENCB *cb);

int	DSA_generate_key(DSA *a);
int	i2d_DSAPublicKey(const DSA *a, unsigned char **pp);
int 	i2d_DSAPrivateKey(const DSA *a, unsigned char **pp);
int	i2d_DSAparams(const DSA *a,unsigned char **pp);

#ifndef OPENSSL_NO_BIO
int	DSAparams_print(BIO *bp, const DSA *x);
int	DSA_print(BIO *bp, const DSA *x, int off);
#endif
#ifndef OPENSSL_NO_FP_API
int	DSAparams_print_fp(FILE *fp, const DSA *x);
int	DSA_print_fp(FILE *bp, const DSA *x, int off);
#endif

#define DSS_prime_checks 50

#define DSA_is_prime(n, callback, cb_arg) \
	BN_is_prime(n, DSS_prime_checks, callback, NULL, cb_arg)

#ifndef OPENSSL_NO_DH

DH *DSA_dup_DH(const DSA *r);
#endif  

void ERR_load_DSA_strings(void);   

#define DSA_F_D2I_DSA_SIG				 110
#define DSA_F_DSAPARAMS_PRINT				 100
#define DSA_F_DSAPARAMS_PRINT_FP			 101
#define DSA_F_DSA_DO_SIGN				 112
#define DSA_F_DSA_DO_VERIFY				 113
#define DSA_F_DSA_NEW_METHOD				 103
#define DSA_F_DSA_PRINT					 104
#define DSA_F_DSA_PRINT_FP				 105
#define DSA_F_DSA_SIGN					 106
#define DSA_F_DSA_SIGN_SETUP				 107
#define DSA_F_DSA_SIG_NEW				 109
#define DSA_F_DSA_VERIFY				 108
#define DSA_F_I2D_DSA_SIG				 111
#define DSA_F_SIG_CB					 114 

#define DSA_R_BAD_Q_VALUE				 102
#define DSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE		 100
#define DSA_R_MISSING_PARAMETERS			 101
#define DSA_R_MODULUS_TOO_LARGE				 103

#ifdef  __cplusplus
}
#endif
#endif
