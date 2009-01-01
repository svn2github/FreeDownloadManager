/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __SSH_H
#define __SSH_H  

#ifdef USE_LIBSSH2
extern const struct Curl_handler Curl_handler_scp;
extern const struct Curl_handler Curl_handler_sftp;

ssize_t Curl_scp_send(struct connectdata *conn, int sockindex,
                      const void *mem, size_t len);
ssize_t Curl_scp_recv(struct connectdata *conn, int sockindex,
                      char *mem, size_t len);

ssize_t Curl_sftp_send(struct connectdata *conn, int sockindex,
                       const void *mem, size_t len);
ssize_t Curl_sftp_recv(struct connectdata *conn, int sockindex,
                       char *mem, size_t len);

#define Curl_ssh_enabled(conn,prot) (conn->protocol & prot)

#else 
#define Curl_ssh_enabled(x,y) 0
#define Curl_scp_send(a,b,c,d) 0
#define Curl_sftp_send(a,b,c,d) 0
#define Curl_scp_recv(a,b,c,d) 0
#define Curl_sftp_recv(a,b,c,d) 0

#endif 

#endif 
