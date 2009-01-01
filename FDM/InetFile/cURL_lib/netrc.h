/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __NETRC_H
#define __NETRC_H

int Curl_parsenetrc(const char *host,
                    char *login,
                    char *password,
                    char *filename);
  
#endif
