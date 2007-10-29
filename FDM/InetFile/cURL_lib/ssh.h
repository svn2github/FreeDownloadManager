/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SSH_H
#define __SSH_H  

#ifdef USE_LIBSSH2

CURLcode Curl_ssh_connect(struct connectdata *conn, bool *done);
CURLcode Curl_ssh_multi_statemach(struct connectdata *conn, bool *done);

CURLcode Curl_scp_do(struct connectdata *conn, bool *done);
CURLcode Curl_scp_done(struct connectdata *conn, CURLcode, bool premature);

ssize_t Curl_scp_send(struct connectdata *conn, int sockindex,
                      void *mem, size_t len);
ssize_t Curl_scp_recv(struct connectdata *conn, int sockindex,
                      char *mem, size_t len);

CURLcode Curl_sftp_do(struct connectdata *conn, bool *done);
CURLcode Curl_sftp_done(struct connectdata *conn, CURLcode, bool premature);

ssize_t Curl_sftp_send(struct connectdata *conn, int sockindex,
                       void *mem, size_t len);
ssize_t Curl_sftp_recv(struct connectdata *conn, int sockindex,
                       char *mem, size_t len);

#endif 

#endif 
