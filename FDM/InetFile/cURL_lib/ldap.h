/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __LDAP_H
#define __LDAP_H 

#ifndef CURL_DISABLE_LDAP
CURLcode Curl_ldap(struct connectdata *conn, bool *done);
#endif
#endif 
