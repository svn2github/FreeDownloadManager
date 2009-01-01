/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

     

#ifndef	KSSL_H
#define	KSSL_H

#include <openssl/opensslconf.h>

#ifndef OPENSSL_NO_KRB5

#include <stdio.h>
#include <ctype.h>
#include <krb5.h>

#ifdef  __cplusplus
extern "C" {
#endif 

#ifdef KRB5_HEIMDAL
typedef unsigned char krb5_octet;
#define FAR
#else

#ifndef FAR
#define FAR
#endif

#endif  

#ifndef	KRB5SVC
#define KRB5SVC	"host"
#endif

#ifndef	KRB5KEYTAB
#define KRB5KEYTAB	"/etc/krb5.keytab"
#endif

#ifndef KRB5SENDAUTH
#define KRB5SENDAUTH	1
#endif

#ifndef KRB5CHECKAUTH
#define KRB5CHECKAUTH	1
#endif

#ifndef KSSL_CLOCKSKEW
#define	KSSL_CLOCKSKEW	300;
#endif

#define	KSSL_ERR_MAX	255
typedef struct kssl_err_st  {
	int  reason;
	char text[KSSL_ERR_MAX+1];
	} KSSL_ERR;  

typedef struct kssl_ctx_st
        {
                                
	char *service_name;	
	char *service_host;	
	char *client_princ;	
	char *keytab_file;	
	char *cred_cache;	
	krb5_enctype enctype;
	int length;
	krb5_octet FAR *key;
	} KSSL_CTX;

#define	KSSL_CLIENT 	1
#define KSSL_SERVER 	2
#define	KSSL_SERVICE	3
#define	KSSL_KEYTAB 	4

#define KSSL_CTX_OK 	0
#define KSSL_CTX_ERR	1
#define KSSL_NOMEM	2 

krb5_error_code kssl_ctx_setstring(KSSL_CTX *kssl_ctx, int which, char *text);
KSSL_CTX *kssl_ctx_new(void);
KSSL_CTX *kssl_ctx_free(KSSL_CTX *kssl_ctx);
void kssl_ctx_show(KSSL_CTX *kssl_ctx);
krb5_error_code kssl_ctx_setprinc(KSSL_CTX *kssl_ctx, int which,
        krb5_data *realm, krb5_data *entity, int nentities);
krb5_error_code	kssl_cget_tkt(KSSL_CTX *kssl_ctx,  krb5_data **enc_tktp,
        krb5_data *authenp, KSSL_ERR *kssl_err);
krb5_error_code	kssl_sget_tkt(KSSL_CTX *kssl_ctx,  krb5_data *indata,
        krb5_ticket_times *ttimes, KSSL_ERR *kssl_err);
krb5_error_code kssl_ctx_setkey(KSSL_CTX *kssl_ctx, krb5_keyblock *session);
void	kssl_err_set(KSSL_ERR *kssl_err, int reason, char *text);
void kssl_krb5_free_data_contents(krb5_context context, krb5_data *data);
krb5_error_code  kssl_build_principal_2(krb5_context context,
			krb5_principal *princ, int rlen, const char *realm,
			int slen, const char *svc, int hlen, const char *host);
krb5_error_code  kssl_validate_times(krb5_timestamp atime,
					krb5_ticket_times *ttimes);
krb5_error_code  kssl_check_authent(KSSL_CTX *kssl_ctx, krb5_data *authentp,
			            krb5_timestamp *atimep, KSSL_ERR *kssl_err);
unsigned char	*kssl_skip_confound(krb5_enctype enctype, unsigned char *authn);

#ifdef  __cplusplus
}
#endif
#endif	
#endif	
