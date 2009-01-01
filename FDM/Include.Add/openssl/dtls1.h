/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

   

#ifndef HEADER_DTLS1_H 
#define HEADER_DTLS1_H 

#include <openssl/buffer.h>
#include <openssl/pqueue.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define DTLS1_VERSION			0xFEFF
#define DTLS1_BAD_VER			0x0100

#define DTLS1_AD_MISSING_HANDSHAKE_MESSAGE    110 

#define DTLS1_COOKIE_LENGTH                     32

#define DTLS1_RT_HEADER_LENGTH                  13

#define DTLS1_HM_HEADER_LENGTH                  12

#define DTLS1_HM_BAD_FRAGMENT                   -2
#define DTLS1_HM_FRAGMENT_RETRY                 -3

#define DTLS1_CCS_HEADER_LENGTH                  1

#define DTLS1_AL_HEADER_LENGTH                   7 

typedef struct dtls1_bitmap_st
	{
	PQ_64BIT map;
	unsigned long length;     
	PQ_64BIT max_seq_num;  
	} DTLS1_BITMAP;

struct hm_header_st
	{
	unsigned char type;
	unsigned long msg_len;
	unsigned short seq;
	unsigned long frag_off;
	unsigned long frag_len;
	unsigned int is_ccs;
	};

struct ccs_header_st
	{
	unsigned char type;
	unsigned short seq;
	};

struct dtls1_timeout_st
	{
	
	unsigned int read_timeouts;
	
	
	unsigned int write_timeouts;
	
	
	unsigned int num_alerts;
	};

typedef struct record_pqueue_st
	{
	unsigned short epoch;
	pqueue q;
	} record_pqueue;

typedef struct hm_fragment_st
	{
	struct hm_header_st msg_header;
	unsigned char *fragment;
	} hm_fragment;

typedef struct dtls1_state_st
	{
	unsigned int send_cookie;
	unsigned char cookie[DTLS1_COOKIE_LENGTH];
	unsigned char rcvd_cookie[DTLS1_COOKIE_LENGTH];
	unsigned int cookie_len;

	
	unsigned short r_epoch;
	unsigned short w_epoch;

	
	DTLS1_BITMAP bitmap;

	
	DTLS1_BITMAP next_bitmap;

	
	unsigned short handshake_write_seq;
	unsigned short next_handshake_write_seq;

	unsigned short handshake_read_seq;

	
	record_pqueue unprocessed_rcds;
	record_pqueue processed_rcds;

	
	pqueue buffered_messages;

	
	pqueue sent_messages;

	unsigned int mtu; 

	struct hm_header_st w_msg_hdr;
	struct hm_header_st r_msg_hdr;

	struct dtls1_timeout_st timeout;
	
	
	unsigned char alert_fragment[DTLS1_AL_HEADER_LENGTH];
	unsigned int alert_fragment_len;
	unsigned char handshake_fragment[DTLS1_HM_HEADER_LENGTH];
	unsigned int handshake_fragment_len;

	unsigned int retransmitting;

	} DTLS1_STATE;

typedef struct dtls1_record_data_st
	{
	unsigned char *packet;
	unsigned int   packet_length;
	SSL3_BUFFER    rbuf;
	SSL3_RECORD    rrec;
	} DTLS1_RECORD_DATA;  

#define DTLS1_TMO_READ_COUNT                      2
#define DTLS1_TMO_WRITE_COUNT                     2

#define DTLS1_TMO_ALERT_COUNT                     12

#ifdef  __cplusplus
}
#endif
#endif

