/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_LDAP_H
#define __CURL_LDAP_H 

#ifndef CURL_DISABLE_LDAP
extern const struct Curl_handler Curl_handler_ldap;

#ifdef HAVE_LDAP_SSL
extern const struct Curl_handler Curl_handler_ldaps;
#endif

#endif
#endif 
