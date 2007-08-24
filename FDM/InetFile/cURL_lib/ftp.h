/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __FTP_H
#define __FTP_H 

#ifndef CURL_DISABLE_FTP
CURLcode Curl_ftp(struct connectdata *conn, bool *done);
CURLcode Curl_ftp_done(struct connectdata *conn, CURLcode);
CURLcode Curl_ftp_connect(struct connectdata *conn, bool *done);
CURLcode Curl_ftp_disconnect(struct connectdata *conn);
CURLcode Curl_ftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_nbftpsendf(struct connectdata *, const char *fmt, ...);
CURLcode Curl_GetFTPResponse(ssize_t *nread, struct connectdata *conn,
                             int *ftpcode);
CURLcode Curl_ftp_nextconnect(struct connectdata *conn);
CURLcode Curl_ftp_multi_statemach(struct connectdata *conn, bool *done);
CURLcode Curl_ftp_fdset(struct connectdata *conn,
                        fd_set *read_fd_set,
                        fd_set *write_fd_set,
                        int *max_fdp);
CURLcode Curl_ftp_doing(struct connectdata *conn,
                        bool *dophase_done);
#endif 
#endif 
