/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef HEADER_TXT_DB_H
#define HEADER_TXT_DB_H

#include <openssl/opensslconf.h>
#ifndef OPENSSL_NO_BIO
#include <openssl/bio.h>
#endif
#include <openssl/stack.h>
#include <openssl/lhash.h>

#define DB_ERROR_OK			0
#define DB_ERROR_MALLOC			1
#define DB_ERROR_INDEX_CLASH    	2
#define DB_ERROR_INDEX_OUT_OF_RANGE	3
#define DB_ERROR_NO_INDEX		4
#define DB_ERROR_INSERT_INDEX_CLASH    	5

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct txt_db_st
	{
	int num_fields;
	STACK  *data;
	LHASH **index;
	int (**qual)(char **);
	long error;
	long arg1;
	long arg2;
	char **arg_row;
	} TXT_DB;

#ifndef OPENSSL_NO_BIO
TXT_DB *TXT_DB_read(BIO *in, int num);
long TXT_DB_write(BIO *out, TXT_DB *db);
#else
TXT_DB *TXT_DB_read(char *in, int num);
long TXT_DB_write(char *out, TXT_DB *db);
#endif
int TXT_DB_create_index(TXT_DB *db,int field,int (*qual)(char **),
		LHASH_HASH_FN_TYPE hash, LHASH_COMP_FN_TYPE cmp);
void TXT_DB_free(TXT_DB *db);
char **TXT_DB_get_by_index(TXT_DB *db, int idx, char **value);
int TXT_DB_insert(TXT_DB *db,char **value);

#ifdef  __cplusplus
}
#endif

#endif
