/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_NTLM_H
#define __HTTP_NTLM_H 

typedef enum {
  CURLNTLM_NONE, 
  CURLNTLM_BAD,  
  CURLNTLM_FIRST, 
  CURLNTLM_FINE, 

  CURLNTLM_LAST  
} CURLntlm; 

CURLntlm Curl_input_ntlm(struct connectdata *conn, bool proxy, char *header); 

CURLcode Curl_output_ntlm(struct connectdata *conn, bool proxy);

void Curl_ntlm_cleanup(struct connectdata *conn);
#ifndef USE_NTLM
#define Curl_ntlm_cleanup(x)
#endif   

#define NTLMFLAG_NEGOTIATE_UNICODE               (1<<0) 

#define NTLMFLAG_NEGOTIATE_OEM                   (1<<1) 

#define NTLMFLAG_REQUEST_TARGET                  (1<<2)  

#define NTLMFLAG_NEGOTIATE_SIGN                  (1<<4) 

#define NTLMFLAG_NEGOTIATE_SEAL                  (1<<5) 

#define NTLMFLAG_NEGOTIATE_DATAGRAM_STYLE        (1<<6) 

#define NTLMFLAG_NEGOTIATE_LM_KEY                (1<<7) 

#define NTLMFLAG_NEGOTIATE_NETWARE               (1<<8) 

#define NTLMFLAG_NEGOTIATE_NTLM_KEY              (1<<9)   

#define NTLMFLAG_NEGOTIATE_ANONYMOUS             (1<<11) 

#define NTLMFLAG_NEGOTIATE_DOMAIN_SUPPLIED       (1<<12) 

#define NTLMFLAG_NEGOTIATE_WORKSTATION_SUPPLIED  (1<<13) 

#define NTLMFLAG_NEGOTIATE_LOCAL_CALL            (1<<14) 

#define NTLMFLAG_NEGOTIATE_ALWAYS_SIGN           (1<<15) 

#define NTLMFLAG_TARGET_TYPE_DOMAIN              (1<<16) 

#define NTLMFLAG_TARGET_TYPE_SERVER              (1<<17) 

#define NTLMFLAG_TARGET_TYPE_SHARE               (1<<18) 

#define NTLMFLAG_NEGOTIATE_NTLM2_KEY             (1<<19) 

#define NTLMFLAG_REQUEST_INIT_RESPONSE           (1<<20) 

#define NTLMFLAG_REQUEST_ACCEPT_RESPONSE         (1<<21) 

#define NTLMFLAG_REQUEST_NONNT_SESSION_KEY       (1<<22) 

#define NTLMFLAG_NEGOTIATE_TARGET_INFO           (1<<23)       

#define NTLMFLAG_NEGOTIATE_128                   (1<<29) 

#define NTLMFLAG_NEGOTIATE_KEY_EXCHANGE          (1<<30) 

#define NTLMFLAG_NEGOTIATE_56                    (1<<31)

#endif
