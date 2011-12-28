/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_RATECONTROL_H
#define AVCODEC_RATECONTROL_H



#include <stdio.h>
#include <stdint.h>
#include "eval.h"

typedef struct Predictor{
    double coeff;
    double count;
    double decay;
} Predictor;

typedef struct RateControlEntry{
    int pict_type;
    float qscale;
    int mv_bits;
    int i_tex_bits;
    int p_tex_bits;
    int misc_bits;
    int header_bits;
    uint64_t expected_bits;
    int new_pict_type;
    float new_qscale;
    int mc_mb_var_sum;
    int mb_var_sum;
    int i_count;
    int skip_count;
    int f_code;
    int b_code;
}RateControlEntry;


typedef struct RateControlContext{
    FILE *stats_file;
    int num_entries;              
    RateControlEntry *entry;
    double buffer_index;          
    Predictor pred[5];
    double short_term_qsum;       
    double short_term_qcount;     
    double pass1_rc_eq_output_sum;
    double pass1_wanted_bits;     
    double last_qscale;
    double last_qscale_for[5];    
    int last_mc_mb_var_sum;
    int last_mb_var_sum;
    uint64_t i_cplx_sum[5];
    uint64_t p_cplx_sum[5];
    uint64_t mv_bits_sum[5];
    uint64_t qscale_sum[5];
    int frame_count[5];
    int last_non_b_pict_type;

    void *non_lavc_opaque;        
    float dry_run_qscale;         
    int last_picture_number;      
    AVExpr * rc_eq_eval;
}RateControlContext;

struct MpegEncContext;


int ff_rate_control_init(struct MpegEncContext *s);
float ff_rate_estimate_qscale(struct MpegEncContext *s, int dry_run);
void ff_write_pass1_stats(struct MpegEncContext *s);
void ff_rate_control_uninit(struct MpegEncContext *s);
int ff_vbv_update(struct MpegEncContext *s, int frame_size);
void ff_get_2pass_fcode(struct MpegEncContext *s);

int ff_xvid_rate_control_init(struct MpegEncContext *s);
void ff_xvid_rate_control_uninit(struct MpegEncContext *s);
float ff_xvid_rate_estimate_qscale(struct MpegEncContext *s, int dry_run);

#endif 

