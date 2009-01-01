/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_ASN1_H
#define HEADER_ASN1_H

#include <time.h>
#include <openssl/e_os2.h>
#ifndef OPENSSL_NO_BIO
#include <openssl/bio.h>
#endif
#include <openssl/stack.h>
#include <openssl/safestack.h>

#include <openssl/symhacks.h>

#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef OPENSSL_BUILD_SHLIBCRYPTO
# undef OPENSSL_EXTERN
# define OPENSSL_EXTERN OPENSSL_EXPORT
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#define V_ASN1_UNIVERSAL		0x00
#define	V_ASN1_APPLICATION		0x40
#define V_ASN1_CONTEXT_SPECIFIC		0x80
#define V_ASN1_PRIVATE			0xc0

#define V_ASN1_CONSTRUCTED		0x20
#define V_ASN1_PRIMITIVE_TAG		0x1f
#define V_ASN1_PRIMATIVE_TAG		0x1f

#define V_ASN1_APP_CHOOSE		-2	
#define V_ASN1_OTHER			-3	
#define V_ASN1_ANY			-4	

#define V_ASN1_NEG			0x100	

#define V_ASN1_UNDEF			-1
#define V_ASN1_EOC			0
#define V_ASN1_BOOLEAN			1	
#define V_ASN1_INTEGER			2
#define V_ASN1_NEG_INTEGER		(2 | V_ASN1_NEG)
#define V_ASN1_BIT_STRING		3
#define V_ASN1_OCTET_STRING		4
#define V_ASN1_NULL			5
#define V_ASN1_OBJECT			6
#define V_ASN1_OBJECT_DESCRIPTOR	7
#define V_ASN1_EXTERNAL			8
#define V_ASN1_REAL			9
#define V_ASN1_ENUMERATED		10
#define V_ASN1_NEG_ENUMERATED		(10 | V_ASN1_NEG)
#define V_ASN1_UTF8STRING		12
#define V_ASN1_SEQUENCE			16
#define V_ASN1_SET			17
#define V_ASN1_NUMERICSTRING		18	
#define V_ASN1_PRINTABLESTRING		19
#define V_ASN1_T61STRING		20
#define V_ASN1_TELETEXSTRING		20	
#define V_ASN1_VIDEOTEXSTRING		21	
#define V_ASN1_IA5STRING		22
#define V_ASN1_UTCTIME			23
#define V_ASN1_GENERALIZEDTIME		24	
#define V_ASN1_GRAPHICSTRING		25	
#define V_ASN1_ISO64STRING		26	
#define V_ASN1_VISIBLESTRING		26	
#define V_ASN1_GENERALSTRING		27	
#define V_ASN1_UNIVERSALSTRING		28	
#define V_ASN1_BMPSTRING		30 

#define B_ASN1_NUMERICSTRING	0x0001
#define B_ASN1_PRINTABLESTRING	0x0002
#define B_ASN1_T61STRING	0x0004
#define B_ASN1_TELETEXSTRING	0x0004
#define B_ASN1_VIDEOTEXSTRING	0x0008
#define B_ASN1_IA5STRING	0x0010
#define B_ASN1_GRAPHICSTRING	0x0020
#define B_ASN1_ISO64STRING	0x0040
#define B_ASN1_VISIBLESTRING	0x0040
#define B_ASN1_GENERALSTRING	0x0080
#define B_ASN1_UNIVERSALSTRING	0x0100
#define B_ASN1_OCTET_STRING	0x0200
#define B_ASN1_BIT_STRING	0x0400
#define B_ASN1_BMPSTRING	0x0800
#define B_ASN1_UNKNOWN		0x1000
#define B_ASN1_UTF8STRING	0x2000
#define B_ASN1_UTCTIME		0x4000
#define B_ASN1_GENERALIZEDTIME	0x8000
#define B_ASN1_SEQUENCE		0x10000 

#define MBSTRING_FLAG		0x1000
#define MBSTRING_UTF8		(MBSTRING_FLAG)
#define MBSTRING_ASC		(MBSTRING_FLAG|1)
#define MBSTRING_BMP		(MBSTRING_FLAG|2)
#define MBSTRING_UNIV		(MBSTRING_FLAG|4)

struct X509_algor_st;

#define DECLARE_ASN1_SET_OF(type) 
#define IMPLEMENT_ASN1_SET_OF(type)  

typedef struct asn1_ctx_st
	{
	unsigned char *p;
	int eos;	
	int error;	
	int inf;	
	int tag;	
	int xclass;	
	long slen;	
	unsigned char *max; 
	unsigned char *q;
	unsigned char **pp;
	int line;	
	} ASN1_CTX;

typedef struct asn1_const_ctx_st
	{
	const unsigned char *p;
	int eos;	
	int error;	
	int inf;	
	int tag;	
	int xclass;	
	long slen;	
	const unsigned char *max; 
	const unsigned char *q;
	const unsigned char **pp;
	int line;	
	} ASN1_const_CTX; 

#define ASN1_OBJECT_FLAG_DYNAMIC	 0x01	
#define ASN1_OBJECT_FLAG_CRITICAL	 0x02	
#define ASN1_OBJECT_FLAG_DYNAMIC_STRINGS 0x04	
#define ASN1_OBJECT_FLAG_DYNAMIC_DATA 	 0x08	
typedef struct asn1_object_st
	{
	const char *sn,*ln;
	int nid;
	int length;
	unsigned char *data;
	int flags;	
	} ASN1_OBJECT;

#define ASN1_STRING_FLAG_BITS_LEFT 0x08 

#define ASN1_STRING_FLAG_NDEF 0x010 

typedef struct asn1_string_st
	{
	int length;
	int type;
	unsigned char *data;
	
	long flags;
	} ASN1_STRING;  

typedef struct ASN1_ENCODING_st
	{
	unsigned char *enc;	
	long len;		
	int modified;		 
	} ASN1_ENCODING; 

#define ASN1_LONG_UNDEF	0x7fffffffL

#define STABLE_FLAGS_MALLOC	0x01
#define STABLE_NO_MASK		0x02
#define DIRSTRING_TYPE	\
 (B_ASN1_PRINTABLESTRING|B_ASN1_T61STRING|B_ASN1_BMPSTRING|B_ASN1_UTF8STRING)
#define PKCS9STRING_TYPE (DIRSTRING_TYPE|B_ASN1_IA5STRING)

typedef struct asn1_string_table_st {
	int nid;
	long minsize;
	long maxsize;
	unsigned long mask;
	unsigned long flags;
} ASN1_STRING_TABLE;

DECLARE_STACK_OF(ASN1_STRING_TABLE)  

#define ub_name				32768
#define ub_common_name			64
#define ub_locality_name		128
#define ub_state_name			128
#define ub_organization_name		64
#define ub_organization_unit_name	64
#define ub_title			64
#define ub_email_address		128 

typedef struct ASN1_TEMPLATE_st ASN1_TEMPLATE;
typedef struct ASN1_ITEM_st ASN1_ITEM;
typedef struct ASN1_TLC_st ASN1_TLC;

typedef struct ASN1_VALUE_st ASN1_VALUE;  

#define DECLARE_ASN1_FUNCTIONS(type) DECLARE_ASN1_FUNCTIONS_name(type, type)

#define DECLARE_ASN1_ALLOC_FUNCTIONS(type) \
	DECLARE_ASN1_ALLOC_FUNCTIONS_name(type, type)

#define DECLARE_ASN1_FUNCTIONS_name(type, name) \
	DECLARE_ASN1_ALLOC_FUNCTIONS_name(type, name) \
	DECLARE_ASN1_ENCODE_FUNCTIONS(type, name, name)

#define DECLARE_ASN1_FUNCTIONS_fname(type, itname, name) \
	DECLARE_ASN1_ALLOC_FUNCTIONS_name(type, name) \
	DECLARE_ASN1_ENCODE_FUNCTIONS(type, itname, name)

#define	DECLARE_ASN1_ENCODE_FUNCTIONS(type, itname, name) \
	type *d2i_##name(type **a, const unsigned char **in, long len); \
	int i2d_##name(type *a, unsigned char **out); \
	DECLARE_ASN1_ITEM(itname)

#define	DECLARE_ASN1_ENCODE_FUNCTIONS_const(type, name) \
	type *d2i_##name(type **a, const unsigned char **in, long len); \
	int i2d_##name(const type *a, unsigned char **out); \
	DECLARE_ASN1_ITEM(name)

#define	DECLARE_ASN1_NDEF_FUNCTION(name) \
	int i2d_##name##_NDEF(name *a, unsigned char **out);

#define DECLARE_ASN1_FUNCTIONS_const(name) \
	name *name##_new(void); \
	void name##_free(name *a);

#define DECLARE_ASN1_ALLOC_FUNCTIONS_name(type, name) \
	type *name##_new(void); \
	void name##_free(type *a);

#define D2I_OF(type) type *(*)(type **,const unsigned char **,long)
#define I2D_OF(type) int (*)(type *,unsigned char **)
#define I2D_OF_const(type) int (*)(const type *,unsigned char **)

#define CHECKED_D2I_OF(type, d2i) \
    ((d2i_of_void*) (1 ? d2i : ((D2I_OF(type))0)))
#define CHECKED_I2D_OF(type, i2d) \
    ((i2d_of_void*) (1 ? i2d : ((I2D_OF(type))0)))
#define CHECKED_NEW_OF(type, xnew) \
    ((void *(*)(void)) (1 ? xnew : ((type *(*)(void))0)))
#define CHECKED_PTR_OF(type, p) \
    ((void*) (1 ? p : (type*)0))
#define CHECKED_PPTR_OF(type, p) \
    ((void**) (1 ? p : (type**)0))

#define TYPEDEF_D2I_OF(type) typedef type *d2i_of_##type(type **,const unsigned char **,long)
#define TYPEDEF_I2D_OF(type) typedef int i2d_of_##type(type *,unsigned char **)
#define TYPEDEF_D2I2D_OF(type) TYPEDEF_D2I_OF(type); TYPEDEF_I2D_OF(type)

TYPEDEF_D2I2D_OF(void);  

#ifndef OPENSSL_EXPORT_VAR_AS_FUNCTION 

typedef const ASN1_ITEM ASN1_ITEM_EXP; 

#define ASN1_ITEM_ptr(iptr) (iptr) 

#define ASN1_ITEM_ref(iptr) (&(iptr##_it))

#define ASN1_ITEM_rptr(ref) (&(ref##_it))

#define DECLARE_ASN1_ITEM(name) \
	OPENSSL_EXTERN const ASN1_ITEM name##_it;

#else   

typedef const ASN1_ITEM * ASN1_ITEM_EXP(void); 

#define ASN1_ITEM_ptr(iptr) (iptr()) 

#define ASN1_ITEM_ref(iptr) (iptr##_it)

#define ASN1_ITEM_rptr(ref) (ref##_it())

#define DECLARE_ASN1_ITEM(name) \
	const ASN1_ITEM * name##_it(void);

#endif    

#define ASN1_STRFLGS_ESC_2253		1
#define ASN1_STRFLGS_ESC_CTRL		2
#define ASN1_STRFLGS_ESC_MSB		4   

#define ASN1_STRFLGS_ESC_QUOTE		8    

#define CHARTYPE_PRINTABLESTRING	0x10

#define CHARTYPE_FIRST_ESC_2253		0x20

#define CHARTYPE_LAST_ESC_2253		0x40    

#define ASN1_STRFLGS_UTF8_CONVERT	0x10  

#define ASN1_STRFLGS_IGNORE_TYPE	0x20 

#define ASN1_STRFLGS_SHOW_TYPE		0x40  

#define ASN1_STRFLGS_DUMP_ALL		0x80
#define ASN1_STRFLGS_DUMP_UNKNOWN	0x100  

#define ASN1_STRFLGS_DUMP_DER		0x200  

#define ASN1_STRFLGS_RFC2253	(ASN1_STRFLGS_ESC_2253 | \
				ASN1_STRFLGS_ESC_CTRL | \
				ASN1_STRFLGS_ESC_MSB | \
				ASN1_STRFLGS_UTF8_CONVERT | \
				ASN1_STRFLGS_DUMP_UNKNOWN | \
				ASN1_STRFLGS_DUMP_DER)

DECLARE_STACK_OF(ASN1_INTEGER)
DECLARE_ASN1_SET_OF(ASN1_INTEGER)

DECLARE_STACK_OF(ASN1_GENERALSTRING)

typedef struct asn1_type_st
	{
	int type;
	union	{
		char *ptr;
		ASN1_BOOLEAN		boolean;
		ASN1_STRING *		asn1_string;
		ASN1_OBJECT *		object;
		ASN1_INTEGER *		integer;
		ASN1_ENUMERATED *	enumerated;
		ASN1_BIT_STRING *	bit_string;
		ASN1_OCTET_STRING *	octet_string;
		ASN1_PRINTABLESTRING *	printablestring;
		ASN1_T61STRING *	t61string;
		ASN1_IA5STRING *	ia5string;
		ASN1_GENERALSTRING *	generalstring;
		ASN1_BMPSTRING *	bmpstring;
		ASN1_UNIVERSALSTRING *	universalstring;
		ASN1_UTCTIME *		utctime;
		ASN1_GENERALIZEDTIME *	generalizedtime;
		ASN1_VISIBLESTRING *	visiblestring;
		ASN1_UTF8STRING *	utf8string;
		
		ASN1_STRING *		set;
		ASN1_STRING *		sequence;
		} value;
	} ASN1_TYPE;

DECLARE_STACK_OF(ASN1_TYPE)
DECLARE_ASN1_SET_OF(ASN1_TYPE)

typedef struct asn1_method_st
	{
	i2d_of_void *i2d;
	d2i_of_void *d2i;
	void *(*create)(void);
	void (*destroy)(void *);
	} ASN1_METHOD; 

typedef struct asn1_header_st
	{
	ASN1_OCTET_STRING *header;
	void *data;
	ASN1_METHOD *meth;
	} ASN1_HEADER; 

typedef struct BIT_STRING_BITNAME_st {
	int bitnum;
	const char *lname;
	const char *sname;
} BIT_STRING_BITNAME; 

#define M_ASN1_STRING_length(x)	((x)->length)
#define M_ASN1_STRING_length_set(x, n)	((x)->length = (n))
#define M_ASN1_STRING_type(x)	((x)->type)
#define M_ASN1_STRING_data(x)	((x)->data) 

#define M_ASN1_BIT_STRING_new()	(ASN1_BIT_STRING *)\
		ASN1_STRING_type_new(V_ASN1_BIT_STRING)
#define M_ASN1_BIT_STRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_BIT_STRING_dup(a) (ASN1_BIT_STRING *)\
		ASN1_STRING_dup((ASN1_STRING *)a)
#define M_ASN1_BIT_STRING_cmp(a,b) ASN1_STRING_cmp(\
		(ASN1_STRING *)a,(ASN1_STRING *)b)
#define M_ASN1_BIT_STRING_set(a,b,c) ASN1_STRING_set((ASN1_STRING *)a,b,c)

#define M_ASN1_INTEGER_new()	(ASN1_INTEGER *)\
		ASN1_STRING_type_new(V_ASN1_INTEGER)
#define M_ASN1_INTEGER_free(a)		ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_INTEGER_dup(a) (ASN1_INTEGER *)ASN1_STRING_dup((ASN1_STRING *)a)
#define M_ASN1_INTEGER_cmp(a,b)	ASN1_STRING_cmp(\
		(ASN1_STRING *)a,(ASN1_STRING *)b)

#define M_ASN1_ENUMERATED_new()	(ASN1_ENUMERATED *)\
		ASN1_STRING_type_new(V_ASN1_ENUMERATED)
#define M_ASN1_ENUMERATED_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_ENUMERATED_dup(a) (ASN1_ENUMERATED *)ASN1_STRING_dup((ASN1_STRING *)a)
#define M_ASN1_ENUMERATED_cmp(a,b)	ASN1_STRING_cmp(\
		(ASN1_STRING *)a,(ASN1_STRING *)b)

#define M_ASN1_OCTET_STRING_new()	(ASN1_OCTET_STRING *)\
		ASN1_STRING_type_new(V_ASN1_OCTET_STRING)
#define M_ASN1_OCTET_STRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_OCTET_STRING_dup(a) (ASN1_OCTET_STRING *)\
		ASN1_STRING_dup((ASN1_STRING *)a)
#define M_ASN1_OCTET_STRING_cmp(a,b) ASN1_STRING_cmp(\
		(ASN1_STRING *)a,(ASN1_STRING *)b)
#define M_ASN1_OCTET_STRING_set(a,b,c)	ASN1_STRING_set((ASN1_STRING *)a,b,c)
#define M_ASN1_OCTET_STRING_print(a,b)	ASN1_STRING_print(a,(ASN1_STRING *)b)
#define M_i2d_ASN1_OCTET_STRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_OCTET_STRING,\
		V_ASN1_UNIVERSAL)

#define B_ASN1_TIME \
			B_ASN1_UTCTIME | \
			B_ASN1_GENERALIZEDTIME

#define B_ASN1_PRINTABLE \
			B_ASN1_PRINTABLESTRING| \
			B_ASN1_T61STRING| \
			B_ASN1_IA5STRING| \
			B_ASN1_BIT_STRING| \
			B_ASN1_UNIVERSALSTRING|\
			B_ASN1_BMPSTRING|\
			B_ASN1_UTF8STRING|\
			B_ASN1_SEQUENCE|\
			B_ASN1_UNKNOWN

#define B_ASN1_DIRECTORYSTRING \
			B_ASN1_PRINTABLESTRING| \
			B_ASN1_TELETEXSTRING|\
			B_ASN1_BMPSTRING|\
			B_ASN1_UNIVERSALSTRING|\
			B_ASN1_UTF8STRING

#define B_ASN1_DISPLAYTEXT \
			B_ASN1_IA5STRING| \
			B_ASN1_VISIBLESTRING| \
			B_ASN1_BMPSTRING|\
			B_ASN1_UTF8STRING

#define M_ASN1_PRINTABLE_new()	ASN1_STRING_type_new(V_ASN1_T61STRING)
#define M_ASN1_PRINTABLE_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_PRINTABLE(a,pp) i2d_ASN1_bytes((ASN1_STRING *)a,\
		pp,a->type,V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_PRINTABLE(a,pp,l) \
		d2i_ASN1_type_bytes((ASN1_STRING **)a,pp,l, \
			B_ASN1_PRINTABLE)

#define M_DIRECTORYSTRING_new() ASN1_STRING_type_new(V_ASN1_PRINTABLESTRING)
#define M_DIRECTORYSTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_DIRECTORYSTRING(a,pp) i2d_ASN1_bytes((ASN1_STRING *)a,\
						pp,a->type,V_ASN1_UNIVERSAL)
#define M_d2i_DIRECTORYSTRING(a,pp,l) \
		d2i_ASN1_type_bytes((ASN1_STRING **)a,pp,l, \
			B_ASN1_DIRECTORYSTRING)

#define M_DISPLAYTEXT_new() ASN1_STRING_type_new(V_ASN1_VISIBLESTRING)
#define M_DISPLAYTEXT_free(a) ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_DISPLAYTEXT(a,pp) i2d_ASN1_bytes((ASN1_STRING *)a,\
						pp,a->type,V_ASN1_UNIVERSAL)
#define M_d2i_DISPLAYTEXT(a,pp,l) \
		d2i_ASN1_type_bytes((ASN1_STRING **)a,pp,l, \
			B_ASN1_DISPLAYTEXT)

#define M_ASN1_PRINTABLESTRING_new() (ASN1_PRINTABLESTRING *)\
		ASN1_STRING_type_new(V_ASN1_PRINTABLESTRING)
#define M_ASN1_PRINTABLESTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_PRINTABLESTRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_PRINTABLESTRING,\
		V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_PRINTABLESTRING(a,pp,l) \
		(ASN1_PRINTABLESTRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_PRINTABLESTRING)

#define M_ASN1_T61STRING_new()	(ASN1_T61STRING *)\
		ASN1_STRING_type_new(V_ASN1_T61STRING)
#define M_ASN1_T61STRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_T61STRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_T61STRING,\
		V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_T61STRING(a,pp,l) \
		(ASN1_T61STRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_T61STRING)

#define M_ASN1_IA5STRING_new()	(ASN1_IA5STRING *)\
		ASN1_STRING_type_new(V_ASN1_IA5STRING)
#define M_ASN1_IA5STRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_IA5STRING_dup(a)	\
			(ASN1_IA5STRING *)ASN1_STRING_dup((ASN1_STRING *)a)
#define M_i2d_ASN1_IA5STRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_IA5STRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_IA5STRING(a,pp,l) \
		(ASN1_IA5STRING *)d2i_ASN1_type_bytes((ASN1_STRING **)a,pp,l,\
			B_ASN1_IA5STRING)

#define M_ASN1_UTCTIME_new()	(ASN1_UTCTIME *)\
		ASN1_STRING_type_new(V_ASN1_UTCTIME)
#define M_ASN1_UTCTIME_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_UTCTIME_dup(a) (ASN1_UTCTIME *)ASN1_STRING_dup((ASN1_STRING *)a)

#define M_ASN1_GENERALIZEDTIME_new()	(ASN1_GENERALIZEDTIME *)\
		ASN1_STRING_type_new(V_ASN1_GENERALIZEDTIME)
#define M_ASN1_GENERALIZEDTIME_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_GENERALIZEDTIME_dup(a) (ASN1_GENERALIZEDTIME *)ASN1_STRING_dup(\
	(ASN1_STRING *)a)

#define M_ASN1_TIME_new()	(ASN1_TIME *)\
		ASN1_STRING_type_new(V_ASN1_UTCTIME)
#define M_ASN1_TIME_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_ASN1_TIME_dup(a) (ASN1_TIME *)ASN1_STRING_dup((ASN1_STRING *)a)

#define M_ASN1_GENERALSTRING_new()	(ASN1_GENERALSTRING *)\
		ASN1_STRING_type_new(V_ASN1_GENERALSTRING)
#define M_ASN1_GENERALSTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_GENERALSTRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_GENERALSTRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_GENERALSTRING(a,pp,l) \
		(ASN1_GENERALSTRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_GENERALSTRING)

#define M_ASN1_UNIVERSALSTRING_new()	(ASN1_UNIVERSALSTRING *)\
		ASN1_STRING_type_new(V_ASN1_UNIVERSALSTRING)
#define M_ASN1_UNIVERSALSTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_UNIVERSALSTRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_UNIVERSALSTRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_UNIVERSALSTRING(a,pp,l) \
		(ASN1_UNIVERSALSTRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_UNIVERSALSTRING)

#define M_ASN1_BMPSTRING_new()	(ASN1_BMPSTRING *)\
		ASN1_STRING_type_new(V_ASN1_BMPSTRING)
#define M_ASN1_BMPSTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_BMPSTRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_BMPSTRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_BMPSTRING(a,pp,l) \
		(ASN1_BMPSTRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_BMPSTRING)

#define M_ASN1_VISIBLESTRING_new()	(ASN1_VISIBLESTRING *)\
		ASN1_STRING_type_new(V_ASN1_VISIBLESTRING)
#define M_ASN1_VISIBLESTRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_VISIBLESTRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_VISIBLESTRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_VISIBLESTRING(a,pp,l) \
		(ASN1_VISIBLESTRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_VISIBLESTRING)

#define M_ASN1_UTF8STRING_new()	(ASN1_UTF8STRING *)\
		ASN1_STRING_type_new(V_ASN1_UTF8STRING)
#define M_ASN1_UTF8STRING_free(a)	ASN1_STRING_free((ASN1_STRING *)a)
#define M_i2d_ASN1_UTF8STRING(a,pp) \
		i2d_ASN1_bytes((ASN1_STRING *)a,pp,V_ASN1_UTF8STRING,\
			V_ASN1_UNIVERSAL)
#define M_d2i_ASN1_UTF8STRING(a,pp,l) \
		(ASN1_UTF8STRING *)d2i_ASN1_type_bytes\
		((ASN1_STRING **)a,pp,l,B_ASN1_UTF8STRING)

  
#define IS_SEQUENCE	0
#define IS_SET		1

DECLARE_ASN1_FUNCTIONS_fname(ASN1_TYPE, ASN1_ANY, ASN1_TYPE)

int ASN1_TYPE_get(ASN1_TYPE *a);
void ASN1_TYPE_set(ASN1_TYPE *a, int type, void *value);

ASN1_OBJECT *	ASN1_OBJECT_new(void );
void		ASN1_OBJECT_free(ASN1_OBJECT *a);
int		i2d_ASN1_OBJECT(ASN1_OBJECT *a,unsigned char **pp);
ASN1_OBJECT *	c2i_ASN1_OBJECT(ASN1_OBJECT **a,const unsigned char **pp,
			long length);
ASN1_OBJECT *	d2i_ASN1_OBJECT(ASN1_OBJECT **a,const unsigned char **pp,
			long length);

DECLARE_ASN1_ITEM(ASN1_OBJECT)

DECLARE_STACK_OF(ASN1_OBJECT)
DECLARE_ASN1_SET_OF(ASN1_OBJECT)

ASN1_STRING *	ASN1_STRING_new(void);
void		ASN1_STRING_free(ASN1_STRING *a);
ASN1_STRING *	ASN1_STRING_dup(ASN1_STRING *a);
ASN1_STRING *	ASN1_STRING_type_new(int type );
int 		ASN1_STRING_cmp(ASN1_STRING *a, ASN1_STRING *b);
  
int 		ASN1_STRING_set(ASN1_STRING *str, const void *data, int len);
int ASN1_STRING_length(ASN1_STRING *x);
void ASN1_STRING_length_set(ASN1_STRING *x, int n);
int ASN1_STRING_type(ASN1_STRING *x);
unsigned char * ASN1_STRING_data(ASN1_STRING *x);

DECLARE_ASN1_FUNCTIONS(ASN1_BIT_STRING)
int		i2c_ASN1_BIT_STRING(ASN1_BIT_STRING *a,unsigned char **pp);
ASN1_BIT_STRING *c2i_ASN1_BIT_STRING(ASN1_BIT_STRING **a,const unsigned char **pp,
			long length);
int		ASN1_BIT_STRING_set(ASN1_BIT_STRING *a, unsigned char *d,
			int length );
int		ASN1_BIT_STRING_set_bit(ASN1_BIT_STRING *a, int n, int value);
int		ASN1_BIT_STRING_get_bit(ASN1_BIT_STRING *a, int n);

#ifndef OPENSSL_NO_BIO
int ASN1_BIT_STRING_name_print(BIO *out, ASN1_BIT_STRING *bs,
				BIT_STRING_BITNAME *tbl, int indent);
#endif
int ASN1_BIT_STRING_num_asc(char *name, BIT_STRING_BITNAME *tbl);
int ASN1_BIT_STRING_set_asc(ASN1_BIT_STRING *bs, char *name, int value,
				BIT_STRING_BITNAME *tbl);

int		i2d_ASN1_BOOLEAN(int a,unsigned char **pp);
int 		d2i_ASN1_BOOLEAN(int *a,const unsigned char **pp,long length);

DECLARE_ASN1_FUNCTIONS(ASN1_INTEGER)
int		i2c_ASN1_INTEGER(ASN1_INTEGER *a,unsigned char **pp);
ASN1_INTEGER *c2i_ASN1_INTEGER(ASN1_INTEGER **a,const unsigned char **pp,
			long length);
ASN1_INTEGER *d2i_ASN1_UINTEGER(ASN1_INTEGER **a,const unsigned char **pp,
			long length);
ASN1_INTEGER *	ASN1_INTEGER_dup(ASN1_INTEGER *x);
int ASN1_INTEGER_cmp(ASN1_INTEGER *x, ASN1_INTEGER *y);

DECLARE_ASN1_FUNCTIONS(ASN1_ENUMERATED)

int ASN1_UTCTIME_check(ASN1_UTCTIME *a);
ASN1_UTCTIME *ASN1_UTCTIME_set(ASN1_UTCTIME *s,time_t t);
int ASN1_UTCTIME_set_string(ASN1_UTCTIME *s, const char *str);
int ASN1_UTCTIME_cmp_time_t(const ASN1_UTCTIME *s, time_t t);
#if 0
time_t ASN1_UTCTIME_get(const ASN1_UTCTIME *s);
#endif

int ASN1_GENERALIZEDTIME_check(ASN1_GENERALIZEDTIME *a);
ASN1_GENERALIZEDTIME *ASN1_GENERALIZEDTIME_set(ASN1_GENERALIZEDTIME *s,time_t t);
int ASN1_GENERALIZEDTIME_set_string(ASN1_GENERALIZEDTIME *s, const char *str);

DECLARE_ASN1_FUNCTIONS(ASN1_OCTET_STRING)
ASN1_OCTET_STRING *	ASN1_OCTET_STRING_dup(ASN1_OCTET_STRING *a);
int 	ASN1_OCTET_STRING_cmp(ASN1_OCTET_STRING *a, ASN1_OCTET_STRING *b);
int 	ASN1_OCTET_STRING_set(ASN1_OCTET_STRING *str, const unsigned char *data, int len);

DECLARE_ASN1_FUNCTIONS(ASN1_VISIBLESTRING)
DECLARE_ASN1_FUNCTIONS(ASN1_UNIVERSALSTRING)
DECLARE_ASN1_FUNCTIONS(ASN1_UTF8STRING)
DECLARE_ASN1_FUNCTIONS(ASN1_NULL)
DECLARE_ASN1_FUNCTIONS(ASN1_BMPSTRING)

int UTF8_getc(const unsigned char *str, int len, unsigned long *val);
int UTF8_putc(unsigned char *str, int len, unsigned long value);

DECLARE_ASN1_FUNCTIONS_name(ASN1_STRING, ASN1_PRINTABLE)

DECLARE_ASN1_FUNCTIONS_name(ASN1_STRING, DIRECTORYSTRING)
DECLARE_ASN1_FUNCTIONS_name(ASN1_STRING, DISPLAYTEXT)
DECLARE_ASN1_FUNCTIONS(ASN1_PRINTABLESTRING)
DECLARE_ASN1_FUNCTIONS(ASN1_T61STRING)
DECLARE_ASN1_FUNCTIONS(ASN1_IA5STRING)
DECLARE_ASN1_FUNCTIONS(ASN1_GENERALSTRING)
DECLARE_ASN1_FUNCTIONS(ASN1_UTCTIME)
DECLARE_ASN1_FUNCTIONS(ASN1_GENERALIZEDTIME)
DECLARE_ASN1_FUNCTIONS(ASN1_TIME)

DECLARE_ASN1_ITEM(ASN1_OCTET_STRING_NDEF)

ASN1_TIME *ASN1_TIME_set(ASN1_TIME *s,time_t t);
int ASN1_TIME_check(ASN1_TIME *t);
ASN1_GENERALIZEDTIME *ASN1_TIME_to_generalizedtime(ASN1_TIME *t, ASN1_GENERALIZEDTIME **out);

int i2d_ASN1_SET(STACK *a, unsigned char **pp,
		 i2d_of_void *i2d, int ex_tag, int ex_class, int is_set);
STACK *	d2i_ASN1_SET(STACK **a, const unsigned char **pp, long length,
		     d2i_of_void *d2i, void (*free_func)(void *),
		     int ex_tag, int ex_class);

#ifndef OPENSSL_NO_BIO
int i2a_ASN1_INTEGER(BIO *bp, ASN1_INTEGER *a);
int a2i_ASN1_INTEGER(BIO *bp,ASN1_INTEGER *bs,char *buf,int size);
int i2a_ASN1_ENUMERATED(BIO *bp, ASN1_ENUMERATED *a);
int a2i_ASN1_ENUMERATED(BIO *bp,ASN1_ENUMERATED *bs,char *buf,int size);
int i2a_ASN1_OBJECT(BIO *bp,ASN1_OBJECT *a);
int a2i_ASN1_STRING(BIO *bp,ASN1_STRING *bs,char *buf,int size);
int i2a_ASN1_STRING(BIO *bp, ASN1_STRING *a, int type);
#endif
int i2t_ASN1_OBJECT(char *buf,int buf_len,ASN1_OBJECT *a);

int a2d_ASN1_OBJECT(unsigned char *out,int olen, const char *buf, int num);
ASN1_OBJECT *ASN1_OBJECT_create(int nid, unsigned char *data,int len,
	const char *sn, const char *ln);

int ASN1_INTEGER_set(ASN1_INTEGER *a, long v);
long ASN1_INTEGER_get(ASN1_INTEGER *a);
ASN1_INTEGER *BN_to_ASN1_INTEGER(BIGNUM *bn, ASN1_INTEGER *ai);
BIGNUM *ASN1_INTEGER_to_BN(ASN1_INTEGER *ai,BIGNUM *bn);

int ASN1_ENUMERATED_set(ASN1_ENUMERATED *a, long v);
long ASN1_ENUMERATED_get(ASN1_ENUMERATED *a);
ASN1_ENUMERATED *BN_to_ASN1_ENUMERATED(BIGNUM *bn, ASN1_ENUMERATED *ai);
BIGNUM *ASN1_ENUMERATED_to_BN(ASN1_ENUMERATED *ai,BIGNUM *bn);  

int ASN1_PRINTABLE_type(const unsigned char *s, int max);

int i2d_ASN1_bytes(ASN1_STRING *a, unsigned char **pp, int tag, int xclass);
ASN1_STRING *d2i_ASN1_bytes(ASN1_STRING **a, const unsigned char **pp,
	long length, int Ptag, int Pclass);
unsigned long ASN1_tag2bit(int tag);

ASN1_STRING *d2i_ASN1_type_bytes(ASN1_STRING **a,const unsigned char **pp,
		long length,int type); 

int asn1_Finish(ASN1_CTX *c);
int asn1_const_Finish(ASN1_const_CTX *c); 

int ASN1_get_object(const unsigned char **pp, long *plength, int *ptag,
	int *pclass, long omax);
int ASN1_check_infinite_end(unsigned char **p,long len);
int ASN1_const_check_infinite_end(const unsigned char **p,long len);
void ASN1_put_object(unsigned char **pp, int constructed, int length,
	int tag, int xclass);
int ASN1_put_eoc(unsigned char **pp);
int ASN1_object_size(int constructed, int length, int tag); 

void *ASN1_dup(i2d_of_void *i2d, d2i_of_void *d2i, char *x);

#define ASN1_dup_of(type,i2d,d2i,x) \
    ((type*)ASN1_dup(CHECKED_I2D_OF(type, i2d), \
		     CHECKED_D2I_OF(type, d2i), \
		     CHECKED_PTR_OF(type, x)))

#define ASN1_dup_of_const(type,i2d,d2i,x) \
    ((type*)ASN1_dup(CHECKED_I2D_OF(const type, i2d), \
		     CHECKED_D2I_OF(type, d2i), \
		     CHECKED_PTR_OF(const type, x)))

void *ASN1_item_dup(const ASN1_ITEM *it, void *x);

#ifndef OPENSSL_NO_FP_API
void *ASN1_d2i_fp(void *(*xnew)(void), d2i_of_void *d2i, FILE *in, void **x);

#define ASN1_d2i_fp_of(type,xnew,d2i,in,x) \
    ((type*)ASN1_d2i_fp(CHECKED_NEW_OF(type, xnew), \
			CHECKED_D2I_OF(type, d2i), \
			in, \
			CHECKED_PPTR_OF(type, x)))

void *ASN1_item_d2i_fp(const ASN1_ITEM *it, FILE *in, void *x);
int ASN1_i2d_fp(i2d_of_void *i2d,FILE *out,void *x);

#define ASN1_i2d_fp_of(type,i2d,out,x) \
    (ASN1_i2d_fp(CHECKED_I2D_OF(type, i2d), \
		 out, \
		 CHECKED_PTR_OF(type, x)))

#define ASN1_i2d_fp_of_const(type,i2d,out,x) \
    (ASN1_i2d_fp(CHECKED_I2D_OF(const type, i2d), \
		 out, \
		 CHECKED_PTR_OF(const type, x)))

int ASN1_item_i2d_fp(const ASN1_ITEM *it, FILE *out, void *x);
int ASN1_STRING_print_ex_fp(FILE *fp, ASN1_STRING *str, unsigned long flags);
#endif

int ASN1_STRING_to_UTF8(unsigned char **out, ASN1_STRING *in);

#ifndef OPENSSL_NO_BIO
void *ASN1_d2i_bio(void *(*xnew)(void), d2i_of_void *d2i, BIO *in, void **x);

#define ASN1_d2i_bio_of(type,xnew,d2i,in,x) \
    ((type*)ASN1_d2i_bio( CHECKED_NEW_OF(type, xnew), \
			  CHECKED_D2I_OF(type, d2i), \
			  in, \
			  CHECKED_PPTR_OF(type, x)))

void *ASN1_item_d2i_bio(const ASN1_ITEM *it, BIO *in, void *x);
int ASN1_i2d_bio(i2d_of_void *i2d,BIO *out, unsigned char *x);

#define ASN1_i2d_bio_of(type,i2d,out,x) \
    (ASN1_i2d_bio(CHECKED_I2D_OF(type, i2d), \
		  out, \
		  CHECKED_PTR_OF(type, x)))

#define ASN1_i2d_bio_of_const(type,i2d,out,x) \
    (ASN1_i2d_bio(CHECKED_I2D_OF(const type, i2d), \
		  out, \
		  CHECKED_PTR_OF(const type, x)))

int ASN1_item_i2d_bio(const ASN1_ITEM *it, BIO *out, void *x);
int ASN1_UTCTIME_print(BIO *fp,ASN1_UTCTIME *a);
int ASN1_GENERALIZEDTIME_print(BIO *fp,ASN1_GENERALIZEDTIME *a);
int ASN1_TIME_print(BIO *fp,ASN1_TIME *a);
int ASN1_STRING_print(BIO *bp,ASN1_STRING *v);
int ASN1_STRING_print_ex(BIO *out, ASN1_STRING *str, unsigned long flags);
int ASN1_parse(BIO *bp,const unsigned char *pp,long len,int indent);
int ASN1_parse_dump(BIO *bp,const unsigned char *pp,long len,int indent,int dump);
#endif
const char *ASN1_tag2str(int tag); 

int i2d_ASN1_HEADER(ASN1_HEADER *a,unsigned char **pp);
ASN1_HEADER *d2i_ASN1_HEADER(ASN1_HEADER **a,const unsigned char **pp, long length);
ASN1_HEADER *ASN1_HEADER_new(void );
void ASN1_HEADER_free(ASN1_HEADER *a);

int ASN1_UNIVERSALSTRING_to_string(ASN1_UNIVERSALSTRING *s); 

ASN1_METHOD *X509_asn1_meth(void);
ASN1_METHOD *RSAPrivateKey_asn1_meth(void);
ASN1_METHOD *ASN1_IA5STRING_asn1_meth(void);
ASN1_METHOD *ASN1_BIT_STRING_asn1_meth(void);

int ASN1_TYPE_set_octetstring(ASN1_TYPE *a,
	unsigned char *data, int len);
int ASN1_TYPE_get_octetstring(ASN1_TYPE *a,
	unsigned char *data, int max_len);
int ASN1_TYPE_set_int_octetstring(ASN1_TYPE *a, long num,
	unsigned char *data, int len);
int ASN1_TYPE_get_int_octetstring(ASN1_TYPE *a,long *num,
	unsigned char *data, int max_len);

STACK *ASN1_seq_unpack(const unsigned char *buf, int len,
		       d2i_of_void *d2i, void (*free_func)(void *));
unsigned char *ASN1_seq_pack(STACK *safes, i2d_of_void *i2d,
			     unsigned char **buf, int *len );
void *ASN1_unpack_string(ASN1_STRING *oct, d2i_of_void *d2i);
void *ASN1_item_unpack(ASN1_STRING *oct, const ASN1_ITEM *it);
ASN1_STRING *ASN1_pack_string(void *obj, i2d_of_void *i2d,
			      ASN1_OCTET_STRING **oct);

#define ASN1_pack_string_of(type,obj,i2d,oct) \
    (ASN1_pack_string(CHECKED_PTR_OF(type, obj), \
		      CHECKED_I2D_OF(type, i2d), \
		      oct))

ASN1_STRING *ASN1_item_pack(void *obj, const ASN1_ITEM *it, ASN1_OCTET_STRING **oct);

void ASN1_STRING_set_default_mask(unsigned long mask);
int ASN1_STRING_set_default_mask_asc(char *p);
unsigned long ASN1_STRING_get_default_mask(void);
int ASN1_mbstring_copy(ASN1_STRING **out, const unsigned char *in, int len,
					int inform, unsigned long mask);
int ASN1_mbstring_ncopy(ASN1_STRING **out, const unsigned char *in, int len,
					int inform, unsigned long mask, 
					long minsize, long maxsize);

ASN1_STRING *ASN1_STRING_set_by_NID(ASN1_STRING **out, 
		const unsigned char *in, int inlen, int inform, int nid);
ASN1_STRING_TABLE *ASN1_STRING_TABLE_get(int nid);
int ASN1_STRING_TABLE_add(int, long, long, unsigned long, unsigned long);
void ASN1_STRING_TABLE_cleanup(void);   

ASN1_VALUE *ASN1_item_new(const ASN1_ITEM *it);
void ASN1_item_free(ASN1_VALUE *val, const ASN1_ITEM *it);
ASN1_VALUE * ASN1_item_d2i(ASN1_VALUE **val, const unsigned char **in, long len, const ASN1_ITEM *it);
int ASN1_item_i2d(ASN1_VALUE *val, unsigned char **out, const ASN1_ITEM *it);
int ASN1_item_ndef_i2d(ASN1_VALUE *val, unsigned char **out, const ASN1_ITEM *it);

void ASN1_add_oid_module(void);

ASN1_TYPE *ASN1_generate_nconf(char *str, CONF *nconf);
ASN1_TYPE *ASN1_generate_v3(char *str, X509V3_CTX *cnf);
	 

void ERR_load_ASN1_strings(void);   

#define ASN1_F_A2D_ASN1_OBJECT				 100
#define ASN1_F_A2I_ASN1_ENUMERATED			 101
#define ASN1_F_A2I_ASN1_INTEGER				 102
#define ASN1_F_A2I_ASN1_STRING				 103
#define ASN1_F_APPEND_EXP				 176
#define ASN1_F_ASN1_BIT_STRING_SET_BIT			 183
#define ASN1_F_ASN1_CB					 177
#define ASN1_F_ASN1_CHECK_TLEN				 104
#define ASN1_F_ASN1_COLLATE_PRIMITIVE			 105
#define ASN1_F_ASN1_COLLECT				 106
#define ASN1_F_ASN1_D2I_EX_PRIMITIVE			 108
#define ASN1_F_ASN1_D2I_FP				 109
#define ASN1_F_ASN1_D2I_READ_BIO			 107
#define ASN1_F_ASN1_DIGEST				 184
#define ASN1_F_ASN1_DO_ADB				 110
#define ASN1_F_ASN1_DUP					 111
#define ASN1_F_ASN1_ENUMERATED_SET			 112
#define ASN1_F_ASN1_ENUMERATED_TO_BN			 113
#define ASN1_F_ASN1_EX_C2I				 204
#define ASN1_F_ASN1_FIND_END				 190
#define ASN1_F_ASN1_GENERALIZEDTIME_SET			 185
#define ASN1_F_ASN1_GENERATE_V3				 178
#define ASN1_F_ASN1_GET_OBJECT				 114
#define ASN1_F_ASN1_HEADER_NEW				 115
#define ASN1_F_ASN1_I2D_BIO				 116
#define ASN1_F_ASN1_I2D_FP				 117
#define ASN1_F_ASN1_INTEGER_SET				 118
#define ASN1_F_ASN1_INTEGER_TO_BN			 119
#define ASN1_F_ASN1_ITEM_D2I_FP				 206
#define ASN1_F_ASN1_ITEM_DUP				 191
#define ASN1_F_ASN1_ITEM_EX_COMBINE_NEW			 121
#define ASN1_F_ASN1_ITEM_EX_D2I				 120
#define ASN1_F_ASN1_ITEM_I2D_BIO			 192
#define ASN1_F_ASN1_ITEM_I2D_FP				 193
#define ASN1_F_ASN1_ITEM_PACK				 198
#define ASN1_F_ASN1_ITEM_SIGN				 195
#define ASN1_F_ASN1_ITEM_UNPACK				 199
#define ASN1_F_ASN1_ITEM_VERIFY				 197
#define ASN1_F_ASN1_MBSTRING_NCOPY			 122
#define ASN1_F_ASN1_OBJECT_NEW				 123
#define ASN1_F_ASN1_PACK_STRING				 124
#define ASN1_F_ASN1_PCTX_NEW				 205
#define ASN1_F_ASN1_PKCS5_PBE_SET			 125
#define ASN1_F_ASN1_SEQ_PACK				 126
#define ASN1_F_ASN1_SEQ_UNPACK				 127
#define ASN1_F_ASN1_SIGN				 128
#define ASN1_F_ASN1_STR2TYPE				 179
#define ASN1_F_ASN1_STRING_SET				 186
#define ASN1_F_ASN1_STRING_TABLE_ADD			 129
#define ASN1_F_ASN1_STRING_TYPE_NEW			 130
#define ASN1_F_ASN1_TEMPLATE_EX_D2I			 132
#define ASN1_F_ASN1_TEMPLATE_NEW			 133
#define ASN1_F_ASN1_TEMPLATE_NOEXP_D2I			 131
#define ASN1_F_ASN1_TIME_SET				 175
#define ASN1_F_ASN1_TYPE_GET_INT_OCTETSTRING		 134
#define ASN1_F_ASN1_TYPE_GET_OCTETSTRING		 135
#define ASN1_F_ASN1_UNPACK_STRING			 136
#define ASN1_F_ASN1_UTCTIME_SET				 187
#define ASN1_F_ASN1_VERIFY				 137
#define ASN1_F_BITSTR_CB				 180
#define ASN1_F_BN_TO_ASN1_ENUMERATED			 138
#define ASN1_F_BN_TO_ASN1_INTEGER			 139
#define ASN1_F_C2I_ASN1_BIT_STRING			 189
#define ASN1_F_C2I_ASN1_INTEGER				 194
#define ASN1_F_C2I_ASN1_OBJECT				 196
#define ASN1_F_COLLECT_DATA				 140
#define ASN1_F_D2I_ASN1_BIT_STRING			 141
#define ASN1_F_D2I_ASN1_BOOLEAN				 142
#define ASN1_F_D2I_ASN1_BYTES				 143
#define ASN1_F_D2I_ASN1_GENERALIZEDTIME			 144
#define ASN1_F_D2I_ASN1_HEADER				 145
#define ASN1_F_D2I_ASN1_INTEGER				 146
#define ASN1_F_D2I_ASN1_OBJECT				 147
#define ASN1_F_D2I_ASN1_SET				 148
#define ASN1_F_D2I_ASN1_TYPE_BYTES			 149
#define ASN1_F_D2I_ASN1_UINTEGER			 150
#define ASN1_F_D2I_ASN1_UTCTIME				 151
#define ASN1_F_D2I_NETSCAPE_RSA				 152
#define ASN1_F_D2I_NETSCAPE_RSA_2			 153
#define ASN1_F_D2I_PRIVATEKEY				 154
#define ASN1_F_D2I_PUBLICKEY				 155
#define ASN1_F_D2I_RSA_NET				 200
#define ASN1_F_D2I_RSA_NET_2				 201
#define ASN1_F_D2I_X509					 156
#define ASN1_F_D2I_X509_CINF				 157
#define ASN1_F_D2I_X509_PKEY				 159
#define ASN1_F_I2D_ASN1_SET				 188
#define ASN1_F_I2D_ASN1_TIME				 160
#define ASN1_F_I2D_DSA_PUBKEY				 161
#define ASN1_F_I2D_EC_PUBKEY				 181
#define ASN1_F_I2D_PRIVATEKEY				 163
#define ASN1_F_I2D_PUBLICKEY				 164
#define ASN1_F_I2D_RSA_NET				 162
#define ASN1_F_I2D_RSA_PUBKEY				 165
#define ASN1_F_LONG_C2I					 166
#define ASN1_F_OID_MODULE_INIT				 174
#define ASN1_F_PARSE_TAGGING				 182
#define ASN1_F_PKCS5_PBE2_SET				 167
#define ASN1_F_PKCS5_PBE_SET				 202
#define ASN1_F_X509_CINF_NEW				 168
#define ASN1_F_X509_CRL_ADD0_REVOKED			 169
#define ASN1_F_X509_INFO_NEW				 170
#define ASN1_F_X509_NAME_ENCODE				 203
#define ASN1_F_X509_NAME_EX_D2I				 158
#define ASN1_F_X509_NAME_EX_NEW				 171
#define ASN1_F_X509_NEW					 172
#define ASN1_F_X509_PKEY_NEW				 173 

#define ASN1_R_ADDING_OBJECT				 171
#define ASN1_R_AUX_ERROR				 100
#define ASN1_R_BAD_CLASS				 101
#define ASN1_R_BAD_OBJECT_HEADER			 102
#define ASN1_R_BAD_PASSWORD_READ			 103
#define ASN1_R_BAD_TAG					 104
#define ASN1_R_BN_LIB					 105
#define ASN1_R_BOOLEAN_IS_WRONG_LENGTH			 106
#define ASN1_R_BUFFER_TOO_SMALL				 107
#define ASN1_R_CIPHER_HAS_NO_OBJECT_IDENTIFIER		 108
#define ASN1_R_DATA_IS_WRONG				 109
#define ASN1_R_DECODE_ERROR				 110
#define ASN1_R_DECODING_ERROR				 111
#define ASN1_R_DEPTH_EXCEEDED				 174
#define ASN1_R_ENCODE_ERROR				 112
#define ASN1_R_ERROR_GETTING_TIME			 173
#define ASN1_R_ERROR_LOADING_SECTION			 172
#define ASN1_R_ERROR_PARSING_SET_ELEMENT		 113
#define ASN1_R_ERROR_SETTING_CIPHER_PARAMS		 114
#define ASN1_R_EXPECTING_AN_INTEGER			 115
#define ASN1_R_EXPECTING_AN_OBJECT			 116
#define ASN1_R_EXPECTING_A_BOOLEAN			 117
#define ASN1_R_EXPECTING_A_TIME				 118
#define ASN1_R_EXPLICIT_LENGTH_MISMATCH			 119
#define ASN1_R_EXPLICIT_TAG_NOT_CONSTRUCTED		 120
#define ASN1_R_FIELD_MISSING				 121
#define ASN1_R_FIRST_NUM_TOO_LARGE			 122
#define ASN1_R_HEADER_TOO_LONG				 123
#define ASN1_R_ILLEGAL_BITSTRING_FORMAT			 175
#define ASN1_R_ILLEGAL_BOOLEAN				 176
#define ASN1_R_ILLEGAL_CHARACTERS			 124
#define ASN1_R_ILLEGAL_FORMAT				 177
#define ASN1_R_ILLEGAL_HEX				 178
#define ASN1_R_ILLEGAL_IMPLICIT_TAG			 179
#define ASN1_R_ILLEGAL_INTEGER				 180
#define ASN1_R_ILLEGAL_NESTED_TAGGING			 181
#define ASN1_R_ILLEGAL_NULL				 125
#define ASN1_R_ILLEGAL_NULL_VALUE			 182
#define ASN1_R_ILLEGAL_OBJECT				 183
#define ASN1_R_ILLEGAL_OPTIONAL_ANY			 126
#define ASN1_R_ILLEGAL_OPTIONS_ON_ITEM_TEMPLATE		 170
#define ASN1_R_ILLEGAL_TAGGED_ANY			 127
#define ASN1_R_ILLEGAL_TIME_VALUE			 184
#define ASN1_R_INTEGER_NOT_ASCII_FORMAT			 185
#define ASN1_R_INTEGER_TOO_LARGE_FOR_LONG		 128
#define ASN1_R_INVALID_BMPSTRING_LENGTH			 129
#define ASN1_R_INVALID_DIGIT				 130
#define ASN1_R_INVALID_MODIFIER				 186
#define ASN1_R_INVALID_NUMBER				 187
#define ASN1_R_INVALID_SEPARATOR			 131
#define ASN1_R_INVALID_TIME_FORMAT			 132
#define ASN1_R_INVALID_UNIVERSALSTRING_LENGTH		 133
#define ASN1_R_INVALID_UTF8STRING			 134
#define ASN1_R_IV_TOO_LARGE				 135
#define ASN1_R_LENGTH_ERROR				 136
#define ASN1_R_LIST_ERROR				 188
#define ASN1_R_MISSING_EOC				 137
#define ASN1_R_MISSING_SECOND_NUMBER			 138
#define ASN1_R_MISSING_VALUE				 189
#define ASN1_R_MSTRING_NOT_UNIVERSAL			 139
#define ASN1_R_MSTRING_WRONG_TAG			 140
#define ASN1_R_NESTED_ASN1_STRING			 197
#define ASN1_R_NON_HEX_CHARACTERS			 141
#define ASN1_R_NOT_ASCII_FORMAT				 190
#define ASN1_R_NOT_ENOUGH_DATA				 142
#define ASN1_R_NO_MATCHING_CHOICE_TYPE			 143
#define ASN1_R_NULL_IS_WRONG_LENGTH			 144
#define ASN1_R_OBJECT_NOT_ASCII_FORMAT			 191
#define ASN1_R_ODD_NUMBER_OF_CHARS			 145
#define ASN1_R_PRIVATE_KEY_HEADER_MISSING		 146
#define ASN1_R_SECOND_NUMBER_TOO_LARGE			 147
#define ASN1_R_SEQUENCE_LENGTH_MISMATCH			 148
#define ASN1_R_SEQUENCE_NOT_CONSTRUCTED			 149
#define ASN1_R_SEQUENCE_OR_SET_NEEDS_CONFIG		 192
#define ASN1_R_SHORT_LINE				 150
#define ASN1_R_STRING_TOO_LONG				 151
#define ASN1_R_STRING_TOO_SHORT				 152
#define ASN1_R_TAG_VALUE_TOO_HIGH			 153
#define ASN1_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD 154
#define ASN1_R_TIME_NOT_ASCII_FORMAT			 193
#define ASN1_R_TOO_LONG					 155
#define ASN1_R_TYPE_NOT_CONSTRUCTED			 156
#define ASN1_R_UNABLE_TO_DECODE_RSA_KEY			 157
#define ASN1_R_UNABLE_TO_DECODE_RSA_PRIVATE_KEY		 158
#define ASN1_R_UNEXPECTED_EOC				 159
#define ASN1_R_UNKNOWN_FORMAT				 160
#define ASN1_R_UNKNOWN_MESSAGE_DIGEST_ALGORITHM		 161
#define ASN1_R_UNKNOWN_OBJECT_TYPE			 162
#define ASN1_R_UNKNOWN_PUBLIC_KEY_TYPE			 163
#define ASN1_R_UNKNOWN_TAG				 194
#define ASN1_R_UNKOWN_FORMAT				 195
#define ASN1_R_UNSUPPORTED_ANY_DEFINED_BY_TYPE		 164
#define ASN1_R_UNSUPPORTED_CIPHER			 165
#define ASN1_R_UNSUPPORTED_ENCRYPTION_ALGORITHM		 166
#define ASN1_R_UNSUPPORTED_PUBLIC_KEY_TYPE		 167
#define ASN1_R_UNSUPPORTED_TYPE				 196
#define ASN1_R_WRONG_TAG				 168
#define ASN1_R_WRONG_TYPE				 169

#ifdef  __cplusplus
}
#endif
#endif
