/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __DICT_H
#define __DICT_H 

#ifndef CURL_DISABLE_DICT
CURLcode Curl_dict(struct connectdata *conn, bool *done);
CURLcode Curl_dict_done(struct connectdata *conn);
#endif
#endif
