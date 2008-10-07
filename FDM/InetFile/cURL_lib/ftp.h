/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __FTP_H
#define __FTP_H 

#ifndef CURL_DISABLE_FTP
extern const struct Curl_handler Curl_handler_ftp;

#ifdef USE_SSL
extern const struct Curl_handler Curl_handler_ftps;
#endif

#ifndef CURL_DISABLE_HTTP
extern const struct Curl_handler Curl_handler_ftp_proxy;

# ifdef USE_SSL
extern const struct Curl_handler Curl_handler_ftps_proxy;
# endif
#endif

CURLcode Curl_ftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_nbftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_GetFTPResponse(ssize_t *nread, struct connectdata *conn,
                             int *ftpcode);
#endif 
#endif 
