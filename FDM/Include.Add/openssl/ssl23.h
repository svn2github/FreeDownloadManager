/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_SSL23_H 
#define HEADER_SSL23_H 

#ifdef  __cplusplus
extern "C" {
#endif  

#define SSL23_ST_CW_CLNT_HELLO_A	(0x210|SSL_ST_CONNECT)
#define SSL23_ST_CW_CLNT_HELLO_B	(0x211|SSL_ST_CONNECT)

#define SSL23_ST_CR_SRVR_HELLO_A	(0x220|SSL_ST_CONNECT)
#define SSL23_ST_CR_SRVR_HELLO_B	(0x221|SSL_ST_CONNECT)  

#define SSL23_ST_SR_CLNT_HELLO_A	(0x210|SSL_ST_ACCEPT)
#define SSL23_ST_SR_CLNT_HELLO_B	(0x211|SSL_ST_ACCEPT)

#ifdef  __cplusplus
}
#endif
#endif

