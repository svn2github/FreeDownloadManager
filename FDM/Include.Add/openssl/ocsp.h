/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
      

#ifndef HEADER_OCSP_H
#define HEADER_OCSP_H

#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/safestack.h>

#ifdef  __cplusplus
extern "C" {
#endif  

#define OCSP_DEFAULT_NONCE_LENGTH	16

#define OCSP_NOCERTS			0x1
#define OCSP_NOINTERN			0x2
#define OCSP_NOSIGS			0x4
#define OCSP_NOCHAIN			0x8
#define OCSP_NOVERIFY			0x10
#define OCSP_NOEXPLICIT			0x20
#define OCSP_NOCASIGN			0x40
#define OCSP_NODELEGATED		0x80
#define OCSP_NOCHECKS			0x100
#define OCSP_TRUSTOTHER			0x200
#define OCSP_RESPID_KEY			0x400
#define OCSP_NOTIME			0x800 

typedef struct ocsp_cert_id_st
	{
	X509_ALGOR *hashAlgorithm;
	ASN1_OCTET_STRING *issuerNameHash;
	ASN1_OCTET_STRING *issuerKeyHash;
	ASN1_INTEGER *serialNumber;
	} OCSP_CERTID;

DECLARE_STACK_OF(OCSP_CERTID) 

typedef struct ocsp_one_request_st
	{
	OCSP_CERTID *reqCert;
	STACK_OF(X509_EXTENSION) *singleRequestExtensions;
	} OCSP_ONEREQ;

DECLARE_STACK_OF(OCSP_ONEREQ)
DECLARE_ASN1_SET_OF(OCSP_ONEREQ)  

typedef struct ocsp_req_info_st
	{
	ASN1_INTEGER *version;
	GENERAL_NAME *requestorName;
	STACK_OF(OCSP_ONEREQ) *requestList;
	STACK_OF(X509_EXTENSION) *requestExtensions;
	} OCSP_REQINFO; 

typedef struct ocsp_signature_st
	{
	X509_ALGOR *signatureAlgorithm;
	ASN1_BIT_STRING *signature;
	STACK_OF(X509) *certs;
	} OCSP_SIGNATURE; 

typedef struct ocsp_request_st
	{
	OCSP_REQINFO *tbsRequest;
	OCSP_SIGNATURE *optionalSignature; 
	} OCSP_REQUEST; 

#define OCSP_RESPONSE_STATUS_SUCCESSFUL          0
#define OCSP_RESPONSE_STATUS_MALFORMEDREQUEST     1
#define OCSP_RESPONSE_STATUS_INTERNALERROR        2
#define OCSP_RESPONSE_STATUS_TRYLATER             3
#define OCSP_RESPONSE_STATUS_SIGREQUIRED          5
#define OCSP_RESPONSE_STATUS_UNAUTHORIZED         6 

typedef struct ocsp_resp_bytes_st
	{
	ASN1_OBJECT *responseType;
	ASN1_OCTET_STRING *response;
	} OCSP_RESPBYTES; 

typedef struct ocsp_response_st
	{
	ASN1_ENUMERATED *responseStatus;
	OCSP_RESPBYTES  *responseBytes;
	} OCSP_RESPONSE; 

#define V_OCSP_RESPID_NAME 0
#define V_OCSP_RESPID_KEY  1
typedef struct ocsp_responder_id_st
	{
	int type;
	union   {
		X509_NAME* byName;
        	ASN1_OCTET_STRING *byKey;
		} value;
	} OCSP_RESPID;  

typedef struct ocsp_revoked_info_st
	{
	ASN1_GENERALIZEDTIME *revocationTime;
	ASN1_ENUMERATED *revocationReason;
	} OCSP_REVOKEDINFO; 

#define V_OCSP_CERTSTATUS_GOOD    0
#define V_OCSP_CERTSTATUS_REVOKED 1
#define V_OCSP_CERTSTATUS_UNKNOWN 2
typedef struct ocsp_cert_status_st
	{
	int type;
	union	{
		ASN1_NULL *good;
		OCSP_REVOKEDINFO *revoked;
		ASN1_NULL *unknown;
		} value;
	} OCSP_CERTSTATUS; 

typedef struct ocsp_single_response_st
	{
	OCSP_CERTID *certId;
	OCSP_CERTSTATUS *certStatus;
	ASN1_GENERALIZEDTIME *thisUpdate;
	ASN1_GENERALIZEDTIME *nextUpdate;
	STACK_OF(X509_EXTENSION) *singleExtensions;
	} OCSP_SINGLERESP;

DECLARE_STACK_OF(OCSP_SINGLERESP)
DECLARE_ASN1_SET_OF(OCSP_SINGLERESP) 

typedef struct ocsp_response_data_st
	{
	ASN1_INTEGER *version;
	OCSP_RESPID  *responderId;
	ASN1_GENERALIZEDTIME *producedAt;
	STACK_OF(OCSP_SINGLERESP) *responses;
	STACK_OF(X509_EXTENSION) *responseExtensions;
	} OCSP_RESPDATA; 

  
  
typedef struct ocsp_basic_response_st
	{
	OCSP_RESPDATA *tbsResponseData;
	X509_ALGOR *signatureAlgorithm;
	ASN1_BIT_STRING *signature;
	STACK_OF(X509) *certs;
	} OCSP_BASICRESP; 

#define OCSP_REVOKED_STATUS_NOSTATUS               -1
#define OCSP_REVOKED_STATUS_UNSPECIFIED             0
#define OCSP_REVOKED_STATUS_KEYCOMPROMISE           1
#define OCSP_REVOKED_STATUS_CACOMPROMISE            2
#define OCSP_REVOKED_STATUS_AFFILIATIONCHANGED      3
#define OCSP_REVOKED_STATUS_SUPERSEDED              4
#define OCSP_REVOKED_STATUS_CESSATIONOFOPERATION    5
#define OCSP_REVOKED_STATUS_CERTIFICATEHOLD         6
#define OCSP_REVOKED_STATUS_REMOVEFROMCRL           8 

typedef struct ocsp_crl_id_st
        {
	ASN1_IA5STRING *crlUrl;
	ASN1_INTEGER *crlNum;
	ASN1_GENERALIZEDTIME *crlTime;
        } OCSP_CRLID; 

typedef struct ocsp_service_locator_st
        {
	X509_NAME* issuer;
	STACK_OF(ACCESS_DESCRIPTION) *locator;
        } OCSP_SERVICELOC;
 
#define PEM_STRING_OCSP_REQUEST	"OCSP REQUEST"
#define PEM_STRING_OCSP_RESPONSE "OCSP RESPONSE"

#define d2i_OCSP_REQUEST_bio(bp,p) ASN1_d2i_bio_of(OCSP_REQUEST,OCSP_REQUEST_new,d2i_OCSP_REQUEST,bp,p)

#define d2i_OCSP_RESPONSE_bio(bp,p) ASN1_d2i_bio_of(OCSP_RESPONSE,OCSP_RESPONSE_new,d2i_OCSP_RESPONSE,bp,p)

#define	PEM_read_bio_OCSP_REQUEST(bp,x,cb) (OCSP_REQUEST *)PEM_ASN1_read_bio( \
     (char *(*)())d2i_OCSP_REQUEST,PEM_STRING_OCSP_REQUEST,bp,(char **)x,cb,NULL)

#define	PEM_read_bio_OCSP_RESPONSE(bp,x,cb)(OCSP_RESPONSE *)PEM_ASN1_read_bio(\
     (char *(*)())d2i_OCSP_RESPONSE,PEM_STRING_OCSP_RESPONSE,bp,(char **)x,cb,NULL)

#define PEM_write_bio_OCSP_REQUEST(bp,o) \
    PEM_ASN1_write_bio((int (*)())i2d_OCSP_REQUEST,PEM_STRING_OCSP_REQUEST,\
			bp,(char *)o, NULL,NULL,0,NULL,NULL)

#define PEM_write_bio_OCSP_RESPONSE(bp,o) \
    PEM_ASN1_write_bio((int (*)())i2d_OCSP_RESPONSE,PEM_STRING_OCSP_RESPONSE,\
			bp,(char *)o, NULL,NULL,0,NULL,NULL)

#define i2d_OCSP_RESPONSE_bio(bp,o) ASN1_i2d_bio_of(OCSP_RESPONSE,i2d_OCSP_RESPONSE,bp,o)

#define i2d_OCSP_REQUEST_bio(bp,o) ASN1_i2d_bio_of(OCSP_REQUEST,i2d_OCSP_REQUEST,bp,o)

#define OCSP_REQUEST_sign(o,pkey,md) \
	ASN1_item_sign(ASN1_ITEM_rptr(OCSP_REQINFO),\
		o->optionalSignature->signatureAlgorithm,NULL,\
	        o->optionalSignature->signature,o->tbsRequest,pkey,md)

#define OCSP_BASICRESP_sign(o,pkey,md,d) \
	ASN1_item_sign(ASN1_ITEM_rptr(OCSP_RESPDATA),o->signatureAlgorithm,NULL,\
		o->signature,o->tbsResponseData,pkey,md)

#define OCSP_REQUEST_verify(a,r) ASN1_item_verify(ASN1_ITEM_rptr(OCSP_REQINFO),\
        a->optionalSignature->signatureAlgorithm,\
	a->optionalSignature->signature,a->tbsRequest,r)

#define OCSP_BASICRESP_verify(a,r,d) ASN1_item_verify(ASN1_ITEM_rptr(OCSP_RESPDATA),\
	a->signatureAlgorithm,a->signature,a->tbsResponseData,r)

#define ASN1_BIT_STRING_digest(data,type,md,len) \
	ASN1_item_digest(ASN1_ITEM_rptr(ASN1_BIT_STRING),type,data,md,len)

#define OCSP_CERTID_dup(cid) ASN1_dup_of(OCSP_CERTID,i2d_OCSP_CERTID,d2i_OCSP_CERTID,cid)

#define OCSP_CERTSTATUS_dup(cs)\
                (OCSP_CERTSTATUS*)ASN1_dup((int(*)())i2d_OCSP_CERTSTATUS,\
		(char *(*)())d2i_OCSP_CERTSTATUS,(char *)(cs))

OCSP_RESPONSE *OCSP_sendreq_bio(BIO *b, char *path, OCSP_REQUEST *req);

OCSP_CERTID *OCSP_cert_to_id(const EVP_MD *dgst, X509 *subject, X509 *issuer);

OCSP_CERTID *OCSP_cert_id_new(const EVP_MD *dgst, 
			      X509_NAME *issuerName, 
			      ASN1_BIT_STRING* issuerKey, 
			      ASN1_INTEGER *serialNumber);

OCSP_ONEREQ *OCSP_request_add0_id(OCSP_REQUEST *req, OCSP_CERTID *cid);

int OCSP_request_add1_nonce(OCSP_REQUEST *req, unsigned char *val, int len);
int OCSP_basic_add1_nonce(OCSP_BASICRESP *resp, unsigned char *val, int len);
int OCSP_check_nonce(OCSP_REQUEST *req, OCSP_BASICRESP *bs);
int OCSP_copy_nonce(OCSP_BASICRESP *resp, OCSP_REQUEST *req);

int OCSP_request_set1_name(OCSP_REQUEST *req, X509_NAME *nm);
int OCSP_request_add1_cert(OCSP_REQUEST *req, X509 *cert);

int OCSP_request_sign(OCSP_REQUEST   *req,
		      X509           *signer,
		      EVP_PKEY       *key,
		      const EVP_MD   *dgst,
		      STACK_OF(X509) *certs,
		      unsigned long flags);

int OCSP_response_status(OCSP_RESPONSE *resp);
OCSP_BASICRESP *OCSP_response_get1_basic(OCSP_RESPONSE *resp);

int OCSP_resp_count(OCSP_BASICRESP *bs);
OCSP_SINGLERESP *OCSP_resp_get0(OCSP_BASICRESP *bs, int idx);
int OCSP_resp_find(OCSP_BASICRESP *bs, OCSP_CERTID *id, int last);
int OCSP_single_get0_status(OCSP_SINGLERESP *single, int *reason,
				ASN1_GENERALIZEDTIME **revtime,
				ASN1_GENERALIZEDTIME **thisupd,
				ASN1_GENERALIZEDTIME **nextupd);
int OCSP_resp_find_status(OCSP_BASICRESP *bs, OCSP_CERTID *id, int *status,
				int *reason,
				ASN1_GENERALIZEDTIME **revtime,
				ASN1_GENERALIZEDTIME **thisupd,
				ASN1_GENERALIZEDTIME **nextupd);
int OCSP_check_validity(ASN1_GENERALIZEDTIME *thisupd,
			ASN1_GENERALIZEDTIME *nextupd,
			long sec, long maxsec);

int OCSP_request_verify(OCSP_REQUEST *req, STACK_OF(X509) *certs, X509_STORE *store, unsigned long flags);

int OCSP_parse_url(char *url, char **phost, char **pport, char **ppath, int *pssl);

int OCSP_id_issuer_cmp(OCSP_CERTID *a, OCSP_CERTID *b);
int OCSP_id_cmp(OCSP_CERTID *a, OCSP_CERTID *b);

int OCSP_request_onereq_count(OCSP_REQUEST *req);
OCSP_ONEREQ *OCSP_request_onereq_get0(OCSP_REQUEST *req, int i);
OCSP_CERTID *OCSP_onereq_get0_id(OCSP_ONEREQ *one);
int OCSP_id_get0_info(ASN1_OCTET_STRING **piNameHash, ASN1_OBJECT **pmd,
			ASN1_OCTET_STRING **pikeyHash,
			ASN1_INTEGER **pserial, OCSP_CERTID *cid);
int OCSP_request_is_signed(OCSP_REQUEST *req);
OCSP_RESPONSE *OCSP_response_create(int status, OCSP_BASICRESP *bs);
OCSP_SINGLERESP *OCSP_basic_add1_status(OCSP_BASICRESP *rsp,
						OCSP_CERTID *cid,
						int status, int reason,
						ASN1_TIME *revtime,
					ASN1_TIME *thisupd, ASN1_TIME *nextupd);
int OCSP_basic_add1_cert(OCSP_BASICRESP *resp, X509 *cert);
int OCSP_basic_sign(OCSP_BASICRESP *brsp, 
			X509 *signer, EVP_PKEY *key, const EVP_MD *dgst,
			STACK_OF(X509) *certs, unsigned long flags);

ASN1_STRING *ASN1_STRING_encode(ASN1_STRING *s, i2d_of_void *i2d,
				void *data, STACK_OF(ASN1_OBJECT) *sk);
#define ASN1_STRING_encode_of(type,s,i2d,data,sk) \
	ASN1_STRING_encode(s, CHECKED_I2D_OF(type, i2d), data, sk)

X509_EXTENSION *OCSP_crlID_new(char *url, long *n, char *tim);

X509_EXTENSION *OCSP_accept_responses_new(char **oids);

X509_EXTENSION *OCSP_archive_cutoff_new(char* tim);

X509_EXTENSION *OCSP_url_svcloc_new(X509_NAME* issuer, char **urls);

int OCSP_REQUEST_get_ext_count(OCSP_REQUEST *x);
int OCSP_REQUEST_get_ext_by_NID(OCSP_REQUEST *x, int nid, int lastpos);
int OCSP_REQUEST_get_ext_by_OBJ(OCSP_REQUEST *x, ASN1_OBJECT *obj, int lastpos);
int OCSP_REQUEST_get_ext_by_critical(OCSP_REQUEST *x, int crit, int lastpos);
X509_EXTENSION *OCSP_REQUEST_get_ext(OCSP_REQUEST *x, int loc);
X509_EXTENSION *OCSP_REQUEST_delete_ext(OCSP_REQUEST *x, int loc);
void *OCSP_REQUEST_get1_ext_d2i(OCSP_REQUEST *x, int nid, int *crit, int *idx);
int OCSP_REQUEST_add1_ext_i2d(OCSP_REQUEST *x, int nid, void *value, int crit,
							unsigned long flags);
int OCSP_REQUEST_add_ext(OCSP_REQUEST *x, X509_EXTENSION *ex, int loc);

int OCSP_ONEREQ_get_ext_count(OCSP_ONEREQ *x);
int OCSP_ONEREQ_get_ext_by_NID(OCSP_ONEREQ *x, int nid, int lastpos);
int OCSP_ONEREQ_get_ext_by_OBJ(OCSP_ONEREQ *x, ASN1_OBJECT *obj, int lastpos);
int OCSP_ONEREQ_get_ext_by_critical(OCSP_ONEREQ *x, int crit, int lastpos);
X509_EXTENSION *OCSP_ONEREQ_get_ext(OCSP_ONEREQ *x, int loc);
X509_EXTENSION *OCSP_ONEREQ_delete_ext(OCSP_ONEREQ *x, int loc);
void *OCSP_ONEREQ_get1_ext_d2i(OCSP_ONEREQ *x, int nid, int *crit, int *idx);
int OCSP_ONEREQ_add1_ext_i2d(OCSP_ONEREQ *x, int nid, void *value, int crit,
							unsigned long flags);
int OCSP_ONEREQ_add_ext(OCSP_ONEREQ *x, X509_EXTENSION *ex, int loc);

int OCSP_BASICRESP_get_ext_count(OCSP_BASICRESP *x);
int OCSP_BASICRESP_get_ext_by_NID(OCSP_BASICRESP *x, int nid, int lastpos);
int OCSP_BASICRESP_get_ext_by_OBJ(OCSP_BASICRESP *x, ASN1_OBJECT *obj, int lastpos);
int OCSP_BASICRESP_get_ext_by_critical(OCSP_BASICRESP *x, int crit, int lastpos);
X509_EXTENSION *OCSP_BASICRESP_get_ext(OCSP_BASICRESP *x, int loc);
X509_EXTENSION *OCSP_BASICRESP_delete_ext(OCSP_BASICRESP *x, int loc);
void *OCSP_BASICRESP_get1_ext_d2i(OCSP_BASICRESP *x, int nid, int *crit, int *idx);
int OCSP_BASICRESP_add1_ext_i2d(OCSP_BASICRESP *x, int nid, void *value, int crit,
							unsigned long flags);
int OCSP_BASICRESP_add_ext(OCSP_BASICRESP *x, X509_EXTENSION *ex, int loc);

int OCSP_SINGLERESP_get_ext_count(OCSP_SINGLERESP *x);
int OCSP_SINGLERESP_get_ext_by_NID(OCSP_SINGLERESP *x, int nid, int lastpos);
int OCSP_SINGLERESP_get_ext_by_OBJ(OCSP_SINGLERESP *x, ASN1_OBJECT *obj, int lastpos);
int OCSP_SINGLERESP_get_ext_by_critical(OCSP_SINGLERESP *x, int crit, int lastpos);
X509_EXTENSION *OCSP_SINGLERESP_get_ext(OCSP_SINGLERESP *x, int loc);
X509_EXTENSION *OCSP_SINGLERESP_delete_ext(OCSP_SINGLERESP *x, int loc);
void *OCSP_SINGLERESP_get1_ext_d2i(OCSP_SINGLERESP *x, int nid, int *crit, int *idx);
int OCSP_SINGLERESP_add1_ext_i2d(OCSP_SINGLERESP *x, int nid, void *value, int crit,
							unsigned long flags);
int OCSP_SINGLERESP_add_ext(OCSP_SINGLERESP *x, X509_EXTENSION *ex, int loc);

DECLARE_ASN1_FUNCTIONS(OCSP_SINGLERESP)
DECLARE_ASN1_FUNCTIONS(OCSP_CERTSTATUS)
DECLARE_ASN1_FUNCTIONS(OCSP_REVOKEDINFO)
DECLARE_ASN1_FUNCTIONS(OCSP_BASICRESP)
DECLARE_ASN1_FUNCTIONS(OCSP_RESPDATA)
DECLARE_ASN1_FUNCTIONS(OCSP_RESPID)
DECLARE_ASN1_FUNCTIONS(OCSP_RESPONSE)
DECLARE_ASN1_FUNCTIONS(OCSP_RESPBYTES)
DECLARE_ASN1_FUNCTIONS(OCSP_ONEREQ)
DECLARE_ASN1_FUNCTIONS(OCSP_CERTID)
DECLARE_ASN1_FUNCTIONS(OCSP_REQUEST)
DECLARE_ASN1_FUNCTIONS(OCSP_SIGNATURE)
DECLARE_ASN1_FUNCTIONS(OCSP_REQINFO)
DECLARE_ASN1_FUNCTIONS(OCSP_CRLID)
DECLARE_ASN1_FUNCTIONS(OCSP_SERVICELOC)

char *OCSP_response_status_str(long s);
char *OCSP_cert_status_str(long s);
char *OCSP_crl_reason_str(long s);

int OCSP_REQUEST_print(BIO *bp, OCSP_REQUEST* a, unsigned long flags);
int OCSP_RESPONSE_print(BIO *bp, OCSP_RESPONSE* o, unsigned long flags);

int OCSP_basic_verify(OCSP_BASICRESP *bs, STACK_OF(X509) *certs,
				X509_STORE *st, unsigned long flags);  

void ERR_load_OCSP_strings(void);   

#define OCSP_F_ASN1_STRING_ENCODE			 100
#define OCSP_F_D2I_OCSP_NONCE				 102
#define OCSP_F_OCSP_BASIC_ADD1_STATUS			 103
#define OCSP_F_OCSP_BASIC_SIGN				 104
#define OCSP_F_OCSP_BASIC_VERIFY			 105
#define OCSP_F_OCSP_CERT_ID_NEW				 101
#define OCSP_F_OCSP_CHECK_DELEGATED			 106
#define OCSP_F_OCSP_CHECK_IDS				 107
#define OCSP_F_OCSP_CHECK_ISSUER			 108
#define OCSP_F_OCSP_CHECK_VALIDITY			 115
#define OCSP_F_OCSP_MATCH_ISSUERID			 109
#define OCSP_F_OCSP_PARSE_URL				 114
#define OCSP_F_OCSP_REQUEST_SIGN			 110
#define OCSP_F_OCSP_REQUEST_VERIFY			 116
#define OCSP_F_OCSP_RESPONSE_GET1_BASIC			 111
#define OCSP_F_OCSP_SENDREQ_BIO				 112
#define OCSP_F_REQUEST_VERIFY				 113 

#define OCSP_R_BAD_DATA					 100
#define OCSP_R_CERTIFICATE_VERIFY_ERROR			 101
#define OCSP_R_DIGEST_ERR				 102
#define OCSP_R_ERROR_IN_NEXTUPDATE_FIELD		 122
#define OCSP_R_ERROR_IN_THISUPDATE_FIELD		 123
#define OCSP_R_ERROR_PARSING_URL			 121
#define OCSP_R_MISSING_OCSPSIGNING_USAGE		 103
#define OCSP_R_NEXTUPDATE_BEFORE_THISUPDATE		 124
#define OCSP_R_NOT_BASIC_RESPONSE			 104
#define OCSP_R_NO_CERTIFICATES_IN_CHAIN			 105
#define OCSP_R_NO_CONTENT				 106
#define OCSP_R_NO_PUBLIC_KEY				 107
#define OCSP_R_NO_RESPONSE_DATA				 108
#define OCSP_R_NO_REVOKED_TIME				 109
#define OCSP_R_PRIVATE_KEY_DOES_NOT_MATCH_CERTIFICATE	 110
#define OCSP_R_REQUEST_NOT_SIGNED			 128
#define OCSP_R_RESPONSE_CONTAINS_NO_REVOCATION_DATA	 111
#define OCSP_R_ROOT_CA_NOT_TRUSTED			 112
#define OCSP_R_SERVER_READ_ERROR			 113
#define OCSP_R_SERVER_RESPONSE_ERROR			 114
#define OCSP_R_SERVER_RESPONSE_PARSE_ERROR		 115
#define OCSP_R_SERVER_WRITE_ERROR			 116
#define OCSP_R_SIGNATURE_FAILURE			 117
#define OCSP_R_SIGNER_CERTIFICATE_NOT_FOUND		 118
#define OCSP_R_STATUS_EXPIRED				 125
#define OCSP_R_STATUS_NOT_YET_VALID			 126
#define OCSP_R_STATUS_TOO_OLD				 127
#define OCSP_R_UNKNOWN_MESSAGE_DIGEST			 119
#define OCSP_R_UNKNOWN_NID				 120
#define OCSP_R_UNSUPPORTED_REQUESTORNAME_TYPE		 129

#ifdef  __cplusplus
}
#endif
#endif
