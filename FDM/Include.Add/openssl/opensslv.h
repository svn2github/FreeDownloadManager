/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef HEADER_OPENSSLV_H
#define HEADER_OPENSSLV_H 

#define OPENSSL_VERSION_NUMBER	0x0090807fL
#ifdef OPENSSL_FIPS
#define OPENSSL_VERSION_TEXT	"OpenSSL 0.9.8g-fips 19 Oct 2007"
#else
#define OPENSSL_VERSION_TEXT	"OpenSSL 0.9.8g 19 Oct 2007"
#endif
#define OPENSSL_VERSION_PTEXT	" part of " OPENSSL_VERSION_TEXT  

#define SHLIB_VERSION_HISTORY ""
#define SHLIB_VERSION_NUMBER "0.9.8" 

#endif 
