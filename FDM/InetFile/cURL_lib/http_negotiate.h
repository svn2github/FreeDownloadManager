/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_NEGOTIATE_H
#define __HTTP_NEGOTIATE_H  

#ifdef HAVE_GSSAPI 

int Curl_input_negotiate(struct connectdata *conn, bool proxy, const char *header); 

CURLcode Curl_output_negotiate(struct connectdata *conn, bool proxy);

void Curl_cleanup_negotiate(struct SessionHandle *data);

#endif

#endif
