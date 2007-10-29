/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __KRB4_H
#define __KRB4_H 

struct Curl_sec_client_mech {
  const char *name;
  size_t size;
  int (*init)(void *);
  int (*auth)(void *, struct connectdata *);
  void (*end)(void *);
  int (*check_prot)(void *, int);
  int (*overhead)(void *, int, int);
  int (*encode)(void *, void*, int, int, void**, struct connectdata *);
  int (*decode)(void *, void*, int, int, struct connectdata *);
}; 

#define AUTH_OK         0
#define AUTH_CONTINUE   1
#define AUTH_ERROR      2

#ifdef HAVE_KRB4
extern struct Curl_sec_client_mech Curl_krb4_client_mech;
#endif
#ifdef HAVE_GSSAPI
extern struct Curl_sec_client_mech Curl_krb5_client_mech;
#endif

CURLcode Curl_krb_kauth(struct connectdata *conn);
int Curl_sec_fflush_fd(struct connectdata *conn, int fd);
int Curl_sec_fprintf (struct connectdata *, FILE *, const char *, ...);
int Curl_sec_getc (struct connectdata *conn, FILE *);
int Curl_sec_putc (struct connectdata *conn, int, FILE *);
int Curl_sec_read (struct connectdata *conn, int, void *, int);
int Curl_sec_read_msg (struct connectdata *conn, char *, int);

int Curl_sec_vfprintf(struct connectdata *, FILE *, const char *, va_list);
int Curl_sec_fprintf2(struct connectdata *conn, FILE *f, const char *fmt, ...);
int Curl_sec_vfprintf2(struct connectdata *conn, FILE *, const char *, va_list);
ssize_t Curl_sec_send(struct connectdata *conn, int, char *, int);
int Curl_sec_write(struct connectdata *conn, int, char *, int);

void Curl_sec_end (struct connectdata *);
int Curl_sec_login (struct connectdata *);
void Curl_sec_prot (int, char **);
int Curl_sec_request_prot (struct connectdata *conn, const char *level);
void Curl_sec_set_protection_level(struct connectdata *conn);
void Curl_sec_status (void);

enum protection_level Curl_set_command_prot(struct connectdata *,
                                            enum protection_level); 

#endif
