/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

      

#include "stdafx.h"

#pragma warning (disable: 4244)

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#define INT64_C __int64

#include <ffmpeg/avformat.h>
#include <ffmpeg/swscale.h>
#include <ffmpeg/avcodec.h>
#include <ffmpeg/fifo.h>
#include <limits.h>
#include <ffmpeg/opt.h>
#include "berrno.h"
#include <math.h>
#include <assert.h>
#include <limits.h>
#include "fifo.c"  

typedef struct AVStreamMap {
    int file_index;
    int stream_index;
    int sync_file_index;
    int sync_stream_index;
} AVStreamMap;  

typedef struct AVMetaDataMap {
    int out_file;
    int in_file;
} AVMetaDataMap;

#define MAX_FILES 20
char* _psz1 = "tex^qComp";
char* _psz2 = "video4linux";
char* _psz3 = "ntsc";
char* _psz4 = "audio_device";

struct vmsMediaConverterJob {

AVFormatContext *input_files[MAX_FILES];
int64_t input_files_ts_offset[MAX_FILES];
int nb_input_files ;

AVFormatContext *output_files[MAX_FILES];
int nb_output_files ;

AVStreamMap stream_maps[MAX_FILES];
int nb_stream_maps;

AVMetaDataMap meta_data_maps[MAX_FILES];
int nb_meta_data_maps;

AVInputFormat *file_iformat;
AVOutputFormat *file_oformat;
int frame_width  ;
int frame_height ;
float frame_aspect_ratio ;
enum PixelFormat frame_pix_fmt ;
int frame_padtop  ;
int frame_padbottom ;
int frame_padleft  ;
int frame_padright ;
int padcolor[3] ; 
int frame_topBand  ;
int frame_bottomBand ;
int frame_leftBand  ;
int frame_rightBand ;
int max_frames[4] ;
int frame_rate ;
int frame_rate_base ;
float video_qscale ;
int video_qdiff ;
uint16_t *intra_matrix ;
uint16_t *inter_matrix ;
#if 0 
float video_rc_qsquish ;
float video_rc_qmod_amp ;
int video_rc_qmod_freq ;
#endif
char *video_rc_override_string ;
char *video_rc_eq ;
int me_method ;
int video_disable ;
int video_discard ;
int video_codec_id ;
int video_codec_tag ;
int same_quality ;
int do_deinterlace ;
int packet_size ;
int strict ;
int top_field_first ;
int me_threshold ;
int intra_dc_precision ;
int loop_input ;
int loop_output ;
int qp_hist ;

int intra_only ;
int audio_sample_rate ;
int audio_bit_rate ;
#define QSCALE_NONE -99999
float audio_qscale ;
int audio_disable ;
int audio_channels ;
int audio_codec_id ;
int audio_codec_tag ;
char *audio_language ;

int subtitle_codec_id ;
char *subtitle_language ;

float mux_preload ;
float mux_max_delay ;

int64_t recording_time ;
int64_t start_time ;
int64_t rec_timestamp ;
int64_t input_ts_offset ;
int file_overwrite ;
char *str_title ;
char *str_author ;
char *str_copyright ;
char *str_comment ;
char *str_album ;
int do_benchmark ;
int do_hex_dump ;
int do_pkt_dump ;
int do_psnr ;
int do_vstats ;
int do_pass ;
char *pass_logfilename ;
int audio_stream_copy ;
int video_stream_copy ;
int subtitle_stream_copy ;
int video_sync_method ;
int audio_sync_method;
int copy_ts;
int opt_shortest ; 
int video_global_header ;

int rate_emu ;

const char *video_grab_format ;

char *video_device ;
char *grab_device ;
int  video_channel ;
char *video_standard ;

const char *audio_grab_format ;
char *audio_device ;
int audio_volume ;

int using_stdin ;
int using_vhook ;

int thread_count ;
int q_pressed ;
int64_t video_size ;
int64_t audio_size ;
int64_t extra_size ;
int nb_frames_dup ;
int nb_frames_drop ;
int input_sync;
int limit_filesize ; 

int pgmyuv_compatibility_hack ;
int dts_delta_threshold ;

int sws_flags ;

const char **opt_names ;
int opt_name_count ;
AVCodecContext *avctx_opts;
AVFormatContext *avformat_opts;

AVBitStreamFilterContext *video_bitstream_filters ;
AVBitStreamFilterContext *audio_bitstream_filters ;
AVBitStreamFilterContext *bitstream_filters[MAX_FILES][MAX_STREAMS];      

#define MAX_AUDIO_PACKET_SIZE (128 * 1024)

int bit_buffer_size;
uint8_t *bit_buffer;

uint8_t *audio_buf ;
uint8_t *audio_out ;

uint8_t *input_tmp ;

FILE *fvstats ;

uint8_t *subtitle_out ;

unsigned int samples_size ;
short *samples ;

vmsMediaConverterJob () 
{
	ZeroMemory (this, sizeof (vmsMediaConverterJob));

	bit_buffer_size = 1024*256;
	bit_buffer = NULL;

	audio_buf = NULL;
	audio_out = NULL;

	input_tmp = NULL;

	fvstats = NULL;

	subtitle_out = NULL;

	samples_size = 0;
	samples = NULL;

	file_iformat = NULL;
	file_oformat = NULL;
	avctx_opts = NULL;
	avformat_opts = NULL;

	nb_input_files = 0;
	nb_output_files = 0;
	frame_width  = 0;
	frame_height = 0;
	frame_aspect_ratio = 0;
	PixelFormat frame_pix_fmt = PIX_FMT_NONE;
	frame_padtop  = 0;
	frame_padbottom = 0;
	frame_padleft  = 0;
	frame_padright = 0;
	padcolor [0] = 16; padcolor [1] = padcolor [2] = 128;
	frame_topBand  = 0;
	frame_bottomBand = 0;
	frame_leftBand  = 0;
	frame_rightBand = 0;
	max_frames [0] = max_frames [1] = max_frames [2] = max_frames [3] = INT_MAX;
	frame_rate = 25;
	frame_rate_base = 1;
	video_qscale = 0;
	video_qdiff = 3;
	intra_matrix = NULL;
	inter_matrix = NULL;
	video_rc_override_string =NULL;
	video_rc_eq =_psz1;
	me_method = ME_EPZS;
	video_disable = 0;
	video_discard = 0;
	video_codec_id = CODEC_ID_NONE;
	video_codec_tag = 0;
	same_quality = 0;
	do_deinterlace = 0;
	packet_size = 0;
	strict = 0;
	top_field_first = -1;
	me_threshold = 0;
	intra_dc_precision = 8;
	loop_input = 0;
	loop_output = AVFMT_NOOUTPUTLOOP;
	qp_hist = 0;

	intra_only = 0;
	audio_sample_rate = 44100;
	audio_bit_rate = 64000;
	audio_qscale = QSCALE_NONE;
	audio_disable = 0;
	audio_channels = 1;
	audio_codec_id = CODEC_ID_NONE;
	audio_codec_tag = 0;
	audio_language = NULL;

	subtitle_codec_id = CODEC_ID_NONE;
	subtitle_language = NULL;

	mux_preload = 0.5f;
	mux_max_delay = 0.7f;

	recording_time = 0;
	start_time = 0;
	rec_timestamp = 0;
	input_ts_offset = 0;
	file_overwrite = 0;
	str_title = NULL;
	str_author = NULL;
	str_copyright = NULL;
	str_comment = NULL;
	str_album = NULL;
	do_benchmark = 0;
	do_hex_dump = 0;
	do_pkt_dump = 0;
	do_psnr = 0;
	do_vstats = 0;
	do_pass = 0;
	pass_logfilename = NULL;
	audio_stream_copy = 0;
	video_stream_copy = 0;
	subtitle_stream_copy = 0;
	video_sync_method = 1;
	audio_sync_method = 0;
	copy_ts = 0;
	opt_shortest = 0; 
	video_global_header = 0;

	rate_emu = 0;

	video_grab_format = _psz2;

	video_device = NULL;
	grab_device = NULL;
	video_channel = 0;
	video_standard = _psz3;

	audio_grab_format = _psz4;
	audio_device = NULL;
	audio_volume = 256;

	using_stdin = 0;
	using_vhook = 0;

	thread_count = 1;
	q_pressed = 0;
	video_size = 0;
	audio_size = 0;
	extra_size = 0;
	nb_frames_dup = 0;
	nb_frames_drop = 0;
	input_sync;
	limit_filesize = 0; 

	pgmyuv_compatibility_hack =0;
	dts_delta_threshold = 10;

	sws_flags = SWS_BICUBIC;

	opt_names =NULL;
	opt_name_count = 0;
	video_bitstream_filters =NULL;
	audio_bitstream_filters =NULL;
}

};

#define DEFAULT_PASS_LOGFILENAME "ffmpeg2pass"

struct AVInputStream;

typedef struct AVOutputStream {
    int file_index;          
    int index;               
    int source_index;        
    AVStream *st;            
    int encoding_needed;     
    int frame_number;
    
    
    struct AVInputStream *sync_ist; 
    int64_t sync_opts;        
    
    int video_resample;
    AVFrame pict_tmp;      
    struct SwsContext *img_resample_ctx; 
    int resample_height;

    int video_crop;
    int topBand;             
    int leftBand;

    int video_pad;
    int padtop;              
    int padbottom;
    int padleft;
    int padright;

    
    int audio_resample;
    ReSampleContext *resample; 
    AVFifoBuffer fifo;     
    FILE *logfile;
} AVOutputStream;

typedef struct AVInputStream {
    int file_index;
    int index;
    AVStream *st;
    int discard;             
    int decoding_needed;     
    int64_t sample_index;      

    int64_t       start;     
    unsigned long frame;     
    int64_t       next_pts;  
    int64_t       pts;       
    int is_start;            
} AVInputStream;

typedef struct AVInputFile {
    int eof_reached;      
    int ist_index;        
    int buffer_size;      
    int buffer_size_max;  
    int nb_streams;       
} AVInputFile;  

bool opt_input_file(vmsMediaConverterJob *job, const char *filename)
{
	AVFormatContext *ic;
    AVFormatParameters params, *ap = &params;
    int err, i, ret, rfps, rfps_base;
    int64_t timestamp;

    
    ic = av_alloc_format_context();

    memset(ap, 0, sizeof(*ap));
    ap->prealloced_context = 1;
    ap->sample_rate = job->audio_sample_rate;
    ap->channels = job->audio_channels;
    ap->time_base.den = job->frame_rate;
    ap->time_base.num = job->frame_rate_base;
    ap->width = job->frame_width + job->frame_padleft + job->frame_padright;
    ap->height = job->frame_height + job->frame_padtop + job->frame_padbottom;
    ap->pix_fmt = job->frame_pix_fmt;
    ap->device  = job->grab_device;
    ap->channel = job->video_channel;
    ap->standard = job->video_standard;
    ap->video_codec_id = (CodecID)job->video_codec_id;
    ap->audio_codec_id = (CodecID)job->audio_codec_id;
    if(job->pgmyuv_compatibility_hack)
        ap->video_codec_id= CODEC_ID_PGMYUV;

    

    
    err = av_open_input_file(&ic, filename, job->file_iformat, 0, ap);
    if (err < 0)
        throw 0;
 
    ic->loop_input = job->loop_input;

    
    ret = av_find_stream_info(ic);
    if (ret < 0) 
		throw 0;

    timestamp = job->start_time;
    
    if (ic->start_time != AV_NOPTS_VALUE)
        timestamp += ic->start_time;

    
    if (job->start_time != 0) {
        ret = av_seek_frame(ic, -1, timestamp, AVSEEK_FLAG_BACKWARD);
        if (ret < 0) {
            
        }
        
        job->start_time = 0;
    }

    
    for(i=0;i<(int)ic->nb_streams;i++) {
        AVCodecContext *enc = ic->streams[i]->codec;
#if defined(HAVE_THREADS)
        if(job->thread_count>1)
            avcodec_thread_init(enc, thread_count);
#endif
        enc->thread_count= job->thread_count;
        switch(enc->codec_type) {
        case CODEC_TYPE_AUDIO:
            
            
            job->audio_channels = enc->channels;
            job->audio_sample_rate = enc->sample_rate;
            if(job->audio_disable)
                ic->streams[i]->discard= AVDISCARD_ALL;
            break;
        case CODEC_TYPE_VIDEO:
			enc->bit_rate = job->avctx_opts->bit_rate;
            
            job->frame_height = enc->height;
            job->frame_width = enc->width;
            job->frame_aspect_ratio = (float)av_q2d(enc->sample_aspect_ratio) * enc->width / enc->height;
            job->frame_pix_fmt = enc->pix_fmt;
            rfps      = ic->streams[i]->r_frame_rate.num;
            rfps_base = ic->streams[i]->r_frame_rate.den;
            if(enc->lowres) enc->flags |= CODEC_FLAG_EMU_EDGE;
            if(job->me_threshold)
                enc->debug |= FF_DEBUG_MV;

            if (enc->time_base.den != rfps || enc->time_base.num != rfps_base) {

                
                    
            }
            
            job->frame_rate      = rfps;
            job->frame_rate_base = rfps_base;

            enc->rate_emu = job->rate_emu;
            if(job->video_disable)
                ic->streams[i]->discard= AVDISCARD_ALL;
            else if(job->video_discard)
                ic->streams[i]->discard= (AVDiscard)job->video_discard;
            break;
        case CODEC_TYPE_DATA:
            break;
        case CODEC_TYPE_SUBTITLE:
            break;
        case CODEC_TYPE_UNKNOWN:
            break;
        default:
            throw 0;
        }
    }

    job->input_files[job->nb_input_files] = ic;
    job->input_files_ts_offset[job->nb_input_files] = job->input_ts_offset - (job->copy_ts ? 0 : timestamp);
    

    job->nb_input_files++;
    job->file_iformat = NULL;
    job->file_oformat = NULL;

    job->grab_device = NULL;
    job->video_channel = 0;

    job->rate_emu = 0;

	return true;
}

static bool check_audio_video_inputs(vmsMediaConverterJob *job, int *has_video_ptr, int *has_audio_ptr)
{
    int has_video, has_audio, i, j;
    AVFormatContext *ic;

    has_video = 0;
    has_audio = 0;
    for(j=0;j<job->nb_input_files;j++) {
        ic = job->input_files[j];
        for(i=0;i<(int)ic->nb_streams;i++) {
            AVCodecContext *enc = ic->streams[i]->codec;
            switch(enc->codec_type) {
            case CODEC_TYPE_AUDIO:
                has_audio = 1;
                break;
            case CODEC_TYPE_VIDEO:
                has_video = 1;
                break;
            case CODEC_TYPE_DATA:
            case CODEC_TYPE_UNKNOWN:
            case CODEC_TYPE_SUBTITLE:
                break;
            default:
                throw 0; 
            }
        }
    }
    *has_video_ptr = has_video;
    *has_audio_ptr = has_audio;
	return true;
}  

static bool new_video_stream(vmsMediaConverterJob* job, AVFormatContext *oc)
{
    AVStream *st;
    AVCodecContext *video_enc;
    int codec_id;

    st = av_new_stream(oc, oc->nb_streams);
    if (!st)
        throw 0;

    job->bitstream_filters[job->nb_output_files][oc->nb_streams - 1]= job->video_bitstream_filters;
    job->video_bitstream_filters= NULL;

#if defined(HAVE_THREADS)
    if(thread_count>1)
        avcodec_thread_init(st->codec, thread_count);
#endif

    video_enc = st->codec;

    if(job->video_codec_tag)
        video_enc->codec_tag= job->video_codec_tag;

    if(   (job->video_global_header&1)
       || (job->video_global_header==0 && (oc->oformat->flags & AVFMT_GLOBALHEADER))){
        video_enc->flags |= CODEC_FLAG_GLOBAL_HEADER;
        job->avctx_opts->flags|= CODEC_FLAG_GLOBAL_HEADER;
    }
    if(job->video_global_header&2){
        video_enc->flags2 |= CODEC_FLAG2_LOCAL_HEADER;
        job->avctx_opts->flags2|= CODEC_FLAG2_LOCAL_HEADER;
    }

    if (job->video_stream_copy) {
        st->stream_copy = 1;
        video_enc->codec_type = CODEC_TYPE_VIDEO;
    } else {
        char *p;
        int i = 0;
        AVCodec *codec;

        codec_id = av_guess_codec(oc->oformat, NULL, oc->filename, NULL, CODEC_TYPE_VIDEO);
        if (job->video_codec_id != CODEC_ID_NONE)
            codec_id = job->video_codec_id;

        video_enc->codec_id = (CodecID)codec_id;
        codec = avcodec_find_encoder((CodecID)codec_id);

		video_enc->bit_rate = job->avctx_opts->bit_rate;

        

        video_enc->time_base.den = job->frame_rate;
        video_enc->time_base.num = job->frame_rate_base;
        if(codec && codec->supported_framerates){
            const AVRational *p= codec->supported_framerates;
            AVRational req= {job->frame_rate, job->frame_rate_base};
            const AVRational *best=NULL;
            AVRational best_error= {INT_MAX, 1};
            for(; p->den!=0; p++){
                AVRational error= av_sub_q(req, *p);
                if(error.num <0) error.num *= -1;
                if(av_cmp_q(error, best_error) < 0){
                    best_error= error;
                    best= p;
                }
            }
            video_enc->time_base.den= best->num;
            video_enc->time_base.num= best->den;
        }

        video_enc->width = job->frame_width + job->frame_padright + job->frame_padleft;
        video_enc->height = job->frame_height + job->frame_padtop + job->frame_padbottom;
        video_enc->sample_aspect_ratio = av_d2q(job->frame_aspect_ratio*video_enc->height/video_enc->width, 255);
        video_enc->pix_fmt = job->frame_pix_fmt;

        if(codec && codec->pix_fmts){
            const enum PixelFormat *p= codec->pix_fmts;
            for(; *p!=-1; p++){
                if(*p == video_enc->pix_fmt)
                    break;
            }
            if(*p == -1)
                video_enc->pix_fmt = codec->pix_fmts[0];
        }

        if (job->intra_only)
            video_enc->gop_size = 0;
        if (job->video_qscale || job->same_quality) {
            video_enc->flags |= CODEC_FLAG_QSCALE;
            video_enc->global_quality=
                st->quality = FF_QP2LAMBDA * job->video_qscale;
        }

        if(job->intra_matrix)
            video_enc->intra_matrix = job->intra_matrix;
        if(job->inter_matrix)
            video_enc->inter_matrix = job->inter_matrix;

        video_enc->max_qdiff = job->video_qdiff;
        video_enc->rc_eq = job->video_rc_eq;
        video_enc->thread_count = job->thread_count;
        p= job->video_rc_override_string;
        
        video_enc->rc_override_count=i;
        if (!video_enc->rc_initial_buffer_occupancy)
            video_enc->rc_initial_buffer_occupancy = video_enc->rc_buffer_size*3/4;
        video_enc->me_threshold= job->me_threshold;
        video_enc->intra_dc_precision= job->intra_dc_precision - 8;
        video_enc->strict_std_compliance = job->strict;

        if(job->packet_size){
            video_enc->rtp_mode= 1;
            video_enc->rtp_payload_size= job->packet_size;
        }

        if (job->do_psnr)
            video_enc->flags|= CODEC_FLAG_PSNR;

        video_enc->me_method = job->me_method;

        
        if (job->do_pass) {
            if (job->do_pass == 1) {
                video_enc->flags |= CODEC_FLAG_PASS1;
            } else {
                video_enc->flags |= CODEC_FLAG_PASS2;
            }
        }
    }

    
    job->video_disable = 0;
    job->video_codec_id = CODEC_ID_NONE;
    job->video_stream_copy = 0;

	return true;
}

static bool new_audio_stream(vmsMediaConverterJob* job, AVFormatContext *oc)
{
    AVStream *st;
    AVCodecContext *audio_enc;
    int codec_id;

    st = av_new_stream(oc, oc->nb_streams);
    if (!st)
        throw 0;

    job->bitstream_filters[job->nb_output_files][oc->nb_streams - 1]= job->audio_bitstream_filters;
    job->audio_bitstream_filters= NULL;

#if defined(HAVE_THREADS)
    if(thread_count>1)
        avcodec_thread_init(st->codec, thread_count);
#endif

    audio_enc = st->codec;
    audio_enc->codec_type = CODEC_TYPE_AUDIO;
    audio_enc->strict_std_compliance = job->strict;

    if(job->audio_codec_tag)
        audio_enc->codec_tag= job->audio_codec_tag;

    if (oc->oformat->flags & AVFMT_GLOBALHEADER) {
        audio_enc->flags |= CODEC_FLAG_GLOBAL_HEADER;
        job->avctx_opts->flags|= CODEC_FLAG_GLOBAL_HEADER;
    }
    if (job->audio_stream_copy) {
        st->stream_copy = 1;
        audio_enc->channels = job->audio_channels;
    } else {
        codec_id = av_guess_codec(oc->oformat, NULL, oc->filename, NULL, CODEC_TYPE_AUDIO);

        

        if (job->audio_codec_id != CODEC_ID_NONE)
            codec_id = job->audio_codec_id;
        audio_enc->codec_id = (CodecID)codec_id;

        audio_enc->bit_rate = job->audio_bit_rate;
        if (job->audio_qscale > QSCALE_NONE) {
            audio_enc->flags |= CODEC_FLAG_QSCALE;
            audio_enc->global_quality = st->quality = FF_QP2LAMBDA * job->audio_qscale;
        }
        audio_enc->thread_count = job->thread_count;
        audio_enc->channels = job->audio_channels;
    }
    audio_enc->sample_rate = job->audio_sample_rate;
	AVRational tmp = {1, job->audio_sample_rate};
    audio_enc->time_base= tmp;
    if (job->audio_language) {
        strcpy(st->language, job->audio_language);
        av_free(job->audio_language);
        job->audio_language = NULL;
    }

    
    job->audio_disable = 0;
    job->audio_codec_id = CODEC_ID_NONE;
    job->audio_stream_copy = 0;
	return true;
}

static bool opt_output_file(vmsMediaConverterJob *job, const char *filename)
{
    AVFormatContext *oc;
    int use_video, use_audio, input_has_video, input_has_audio;
    AVFormatParameters params, *ap = &params;

    oc = av_alloc_format_context();

    if (!job->file_oformat) {
        job->file_oformat = guess_format(NULL, filename, NULL);
        if (!job->file_oformat)
            throw 0;
    }

    oc->oformat = job->file_oformat;
    strcpy(oc->filename, filename);

    if (false) {
        
    } else {
        use_video = job->file_oformat->video_codec != CODEC_ID_NONE || job->video_stream_copy || job->video_codec_id != CODEC_ID_NONE;
        use_audio = job->file_oformat->audio_codec != CODEC_ID_NONE || job->audio_stream_copy || job->audio_codec_id != CODEC_ID_NONE;

        
        if (job->nb_input_files > 0) {
            check_audio_video_inputs(job, &input_has_video, &input_has_audio);
            if (!input_has_video)
                use_video = 0;
            if (!input_has_audio)
                use_audio = 0;
        }

        
        if (job->audio_disable) {
            use_audio = 0;
        }
        if (job->video_disable) {
            use_video = 0;
        }

        if (use_video) {
            new_video_stream(job, oc);
        }

        if (use_audio) {
            new_audio_stream(job, oc);
        }

        oc->timestamp = job->rec_timestamp;

        if (job->str_title)
            strcpy(oc->title, job->str_title);
        if (job->str_author)
            strcpy(oc->author, job->str_author);
        if (job->str_copyright)
            strcpy(oc->copyright, job->str_copyright);
        if (job->str_comment)
            strcpy(oc->comment, job->str_comment);
        if (job->str_album)
            strcpy(oc->album, job->str_album);
    }

    job->output_files[job->nb_output_files++] = oc;

    
    if (oc->oformat->flags & AVFMT_NEEDNUMBER) {
        if (!av_filename_number_test(oc->filename))
            throw 0;
    }

    if (!(oc->oformat->flags & AVFMT_NOFILE)) {
               
        
        if (url_fopen(&oc->pb, filename, URL_WRONLY) < 0) {
            throw 0;
        }
    }

    memset(ap, 0, sizeof(*ap));
    if (av_set_parameters(oc, ap) < 0)
        throw 0;

    oc->preload= (int)(job->mux_preload*AV_TIME_BASE);
    oc->max_delay= (int)(job->mux_max_delay*AV_TIME_BASE);
    oc->loop_output = job->loop_output;

    

    
    job->file_oformat = NULL;
    job->file_iformat = NULL;
	return true;
}

static void pre_process_video_frame(vmsMediaConverterJob* job, AVInputStream *ist, AVPicture *picture, void **bufp)
{
    AVCodecContext *dec;
    AVPicture *picture2;
    AVPicture picture_tmp;
    uint8_t *buf = 0;

    dec = ist->st->codec;

    
    if (job->do_deinterlace || job->using_vhook) {
        int size;

        
        size = avpicture_get_size(dec->pix_fmt, dec->width, dec->height);
        buf = (unsigned char*)av_malloc(size);
        if (!buf)
            return;

        picture2 = &picture_tmp;
        avpicture_fill(picture2, buf, dec->pix_fmt, dec->width, dec->height);

        if (job->do_deinterlace){
            if(avpicture_deinterlace(picture2, picture,
                                     dec->pix_fmt, dec->width, dec->height) < 0) {
                
                av_free(buf);
                buf = NULL;
                picture2 = picture;
            }
        } else {
            img_copy(picture2, picture, dec->pix_fmt, dec->width, dec->height);
        }
    } else {
        picture2 = picture;
    }

    

    if (picture != picture2)
        *picture = *picture2;
    *bufp = buf;
}

static double get_sync_ipts(vmsMediaConverterJob* job, const AVOutputStream *ost)
{
    const AVInputStream *ist = ost->sync_ist;
    return (double)(ist->pts + job->input_files_ts_offset[ist->file_index] - job->start_time)/AV_TIME_BASE;
}

static void write_frame(AVFormatContext *s, AVPacket *pkt, AVCodecContext *avctx, AVBitStreamFilterContext *bsfc){
    while(bsfc){
        AVPacket new_pkt= *pkt;
        int a= av_bitstream_filter_filter(bsfc, avctx, NULL,
                                          &new_pkt.data, &new_pkt.size,
                                          pkt->data, pkt->size,
                                          pkt->flags & PKT_FLAG_KEY);
        if(a){
            av_free_packet(pkt);
            new_pkt.destruct= av_destruct_packet;
        }
        *pkt= new_pkt;

        bsfc= bsfc->next;
    }

    av_interleaved_write_frame(s, pkt);
}  

static void do_audio_out(vmsMediaConverterJob *job,
						 AVFormatContext *s,
                         AVOutputStream *ost,
                         AVInputStream *ist,
                         unsigned char *buf, int size)
{
    uint8_t *buftmp;
    
    const int audio_out_size= 4*MAX_AUDIO_PACKET_SIZE;

    int size_out, frame_bytes, ret;
    AVCodecContext *enc= ost->st->codec;

    
    if (!job->audio_buf)
        job->audio_buf = (unsigned char*)av_malloc(2*MAX_AUDIO_PACKET_SIZE);
    if (!job->audio_out)
        job->audio_out = (unsigned char*)av_malloc(audio_out_size);
    if (!job->audio_buf || !job->audio_out)
        return;               

    if(job->audio_sync_method){
        double delta = get_sync_ipts(job, ost) * enc->sample_rate - ost->sync_opts
                - av_fifo_size(&ost->fifo)/(ost->st->codec->channels * 2);
        double idelta= delta*ist->st->codec->sample_rate / enc->sample_rate;
        int byte_delta= ((int)idelta)*2*ist->st->codec->channels;

        
        if(fabs(delta) > 50){
            if(ist->is_start){
                if(byte_delta < 0){
                    byte_delta= FFMAX(byte_delta, -size);
                    size += byte_delta;
                    buf  -= byte_delta;
                    
                    if(!size)
                        return;
                    ist->is_start=0;
                }else{
                    job->input_tmp= (unsigned char*)av_realloc(job->input_tmp, byte_delta + size);

                    if(byte_delta + size <= MAX_AUDIO_PACKET_SIZE)
                        ist->is_start=0;
                    else
                        byte_delta= MAX_AUDIO_PACKET_SIZE - size;

                    memset(job->input_tmp, 0, byte_delta);
                    memcpy(job->input_tmp + byte_delta, buf, size);
                    buf= job->input_tmp;
                    size += byte_delta;
                    
                }
            }else if(job->audio_sync_method>1){
                int comp= clip(delta, -job->audio_sync_method, job->audio_sync_method);
                assert((int)ost->audio_resample);
                
				
                av_resample_compensate(*(struct AVResampleContext**)ost->resample, comp, enc->sample_rate);
            }
        }
    }else
        ost->sync_opts= (int)(get_sync_ipts(job, ost) * enc->sample_rate + 0.5)
                        - av_fifo_size(&ost->fifo)/(ost->st->codec->channels * 2); 

    if (ost->audio_resample) {
        buftmp = job->audio_buf;
        size_out = audio_resample(ost->resample,
                                  (short *)buftmp, (short *)buf,
                                  size / (ist->st->codec->channels * 2));
        size_out = size_out * enc->channels * 2;
    } else {
        buftmp = buf;
        size_out = size;
    }

    
    if (enc->frame_size > 1) {
        
        av_fifo_write(&ost->fifo, buftmp, size_out);

        frame_bytes = enc->frame_size * 2 * enc->channels;

        while (av_fifo_read(&ost->fifo, job->audio_buf, frame_bytes) == 0) {
            AVPacket pkt;
            av_init_packet(&pkt);

            ret = avcodec_encode_audio(enc, job->audio_out, audio_out_size,
                                       (short *)job->audio_buf);
            job->audio_size += ret;
            pkt.stream_index= ost->index;
            pkt.data= job->audio_out;
            pkt.size= ret;
            if(enc->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
                pkt.pts= av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);
            pkt.flags |= PKT_FLAG_KEY;
            write_frame(s, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);

            ost->sync_opts += enc->frame_size;
        }
    } else {
        AVPacket pkt;
        av_init_packet(&pkt);

        ost->sync_opts += size_out / (2 * enc->channels);

        
        
        switch(enc->codec->id) {
        case CODEC_ID_PCM_S32LE:
        case CODEC_ID_PCM_S32BE:
        case CODEC_ID_PCM_U32LE:
        case CODEC_ID_PCM_U32BE:
            size_out = size_out << 1;
            break;
        case CODEC_ID_PCM_S24LE:
        case CODEC_ID_PCM_S24BE:
        case CODEC_ID_PCM_U24LE:
        case CODEC_ID_PCM_U24BE:
        case CODEC_ID_PCM_S24DAUD:
            size_out = size_out / 2 * 3;
            break;
        case CODEC_ID_PCM_S16LE:
        case CODEC_ID_PCM_S16BE:
        case CODEC_ID_PCM_U16LE:
        case CODEC_ID_PCM_U16BE:
            break;
        default:
            size_out = size_out >> 1;
            break;
        }
        ret = avcodec_encode_audio(enc, job->audio_out, size_out,
                                   (short *)buftmp);
        job->audio_size += ret;
        pkt.stream_index= ost->index;
        pkt.data= job->audio_out;
        pkt.size= ret;
        if(enc->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
            pkt.pts= av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);
        pkt.flags |= PKT_FLAG_KEY;
        write_frame(s, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);
    }
}

static bool do_video_out(vmsMediaConverterJob *job,
						 AVFormatContext *s,
                         AVOutputStream *ost,
                         AVInputStream *ist,
                         AVFrame *in_picture,
                         int *frame_size)
{
    int nb_frames, i, ret;
    AVFrame *final_picture, *formatted_picture, *resampling_dst, *padding_src;
    AVFrame picture_crop_temp, picture_pad_temp;
    uint8_t *buf = NULL, *buf1 = NULL;
    AVCodecContext *enc, *dec;

    avcodec_get_frame_defaults(&picture_crop_temp);
    avcodec_get_frame_defaults(&picture_pad_temp);

    enc = ost->st->codec;
    dec = ist->st->codec;

    
    nb_frames = 1;

    *frame_size = 0;

    if(job->video_sync_method){
        double vdelta;
        vdelta = get_sync_ipts(job, ost) / av_q2d(enc->time_base) - ost->sync_opts;
        
        if (vdelta < -1.1)
            nb_frames = 0;
        else if (vdelta > 1.1)
            nb_frames = (int)(vdelta + 0.5);

        if (nb_frames == 0){
            ++job->nb_frames_drop;
            
        }else if (nb_frames > 1) {
            job->nb_frames_dup += nb_frames;
            
        }
    }else
        ost->sync_opts= (int)(get_sync_ipts(job, ost) / av_q2d(enc->time_base) + 0.5);

    nb_frames= FFMIN(nb_frames, job->max_frames[CODEC_TYPE_VIDEO] - ost->frame_number);
    if (nb_frames <= 0)
        return true;

    if (ost->video_crop) {
        if (img_crop((AVPicture *)&picture_crop_temp, (AVPicture *)in_picture, dec->pix_fmt, ost->topBand, ost->leftBand) < 0) {
            
            goto the_end;
        }
        formatted_picture = &picture_crop_temp;
    } else {
        formatted_picture = in_picture;
    }

    final_picture = formatted_picture;
    padding_src = formatted_picture;
    resampling_dst = &ost->pict_tmp;
    if (ost->video_pad) {
        final_picture = &ost->pict_tmp;
        if (ost->video_resample) {
            if (img_crop((AVPicture *)&picture_pad_temp, (AVPicture *)final_picture, enc->pix_fmt, ost->padtop, ost->padleft) < 0) {
                
                goto the_end;
            }
            resampling_dst = &picture_pad_temp;
        }
    }

    if (ost->video_resample) {
        padding_src = NULL;
        final_picture = &ost->pict_tmp;
        sws_scale(ost->img_resample_ctx, formatted_picture->data, formatted_picture->linesize,
              0, ost->resample_height, resampling_dst->data, resampling_dst->linesize);
    }

    if (ost->video_pad) {
        img_pad((AVPicture*)final_picture, (AVPicture *)padding_src,
                enc->height, enc->width, enc->pix_fmt,
                ost->padtop, ost->padbottom, ost->padleft, ost->padright, job->padcolor);
    }

    
    for(i=0;i<nb_frames;i++) {
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.stream_index= ost->index;

        if (s->oformat->flags & AVFMT_RAWPICTURE) {
            
            AVFrame* old_frame = enc->coded_frame;
            enc->coded_frame = dec->coded_frame; 
            pkt.data= (uint8_t *)final_picture;
            pkt.size=  sizeof(AVPicture);
            if(dec->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
                pkt.pts= av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);
            if(dec->coded_frame && dec->coded_frame->key_frame)
                pkt.flags |= PKT_FLAG_KEY;

            write_frame(s, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);
            enc->coded_frame = old_frame;
        } else {
            AVFrame big_picture;

            big_picture= *final_picture;
            
            big_picture.interlaced_frame = in_picture->interlaced_frame;
            if(job->avctx_opts->flags & (CODEC_FLAG_INTERLACED_DCT|CODEC_FLAG_INTERLACED_ME)){
                if(job->top_field_first == -1)
                    big_picture.top_field_first = in_picture->top_field_first;
                else
                    big_picture.top_field_first = job->top_field_first;
            }

            
            if (job->same_quality) {
                big_picture.quality = (int)ist->st->quality;
            }else
                big_picture.quality = (int)ost->st->quality;
            if(!job->me_threshold)
                big_picture.pict_type = 0;

            big_picture.pts= ost->sync_opts;

            ret = avcodec_encode_video(enc,
                                       job->bit_buffer, job->bit_buffer_size,
                                       &big_picture);
            if (ret == -1)
                throw 0;
     
            
            if(ret>0){
                pkt.data= job->bit_buffer;
                pkt.size= ret;
                if(enc->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
                    pkt.pts= av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);  

                if(enc->coded_frame && enc->coded_frame->key_frame)
                    pkt.flags |= PKT_FLAG_KEY;
                write_frame(s, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);
                *frame_size = ret;
                
                
            }
        }
        ost->sync_opts++;
        ost->frame_number++;
    }
 the_end:
    av_free(buf);
    av_free(buf1);
	return true;
}

static bool do_video_stats(vmsMediaConverterJob *job, AVFormatContext *os, 
						   AVOutputStream *ost, int frame_size)
{
    char filename[40];
    time_t today2;
    struct tm *today;
    AVCodecContext *enc;
    int frame_number;
    int64_t ti;
    double ti1, bitrate, avg_bitrate;

    if (!job->fvstats) {
        today2 = time(NULL);
        today = localtime(&today2);
        sprintf(filename, "vstats_%02d%02d%02d.log", today->tm_hour,
                                               today->tm_min,
                                               today->tm_sec);
        job->fvstats = fopen(filename,"w");
        if (!job->fvstats)
            throw 0;
    }

    ti = _I64_MAX;
    enc = ost->st->codec;
    if (enc->codec_type == CODEC_TYPE_VIDEO) {
        frame_number = ost->frame_number;
        
        if (enc->flags&CODEC_FLAG_PSNR)	{
			
		}

        
        
        ti1 = ost->sync_opts * av_q2d(enc->time_base);
        if (ti1 < 0.01)
            ti1 = 0.01;

        bitrate = (frame_size * 8) / av_q2d(enc->time_base) / 1000.0;
        avg_bitrate = (double)(job->video_size * 8) / ti1 / 1000.0;
        
        
    }

	return true;
}

static void do_subtitle_out(vmsMediaConverterJob *job,
							AVFormatContext *s,
                            AVOutputStream *ost,
                            AVInputStream *ist,
                            AVSubtitle *sub,
                            int64_t pts)
{
    int subtitle_out_max_size = 65536;
    int subtitle_out_size, nb, i;
    AVCodecContext *enc;
    AVPacket pkt;

    if (pts == AV_NOPTS_VALUE) {
        
        return;
    }

    enc = ost->st->codec;

    if (!job->subtitle_out) {
        job->subtitle_out = (unsigned char*)av_malloc(subtitle_out_max_size);
    }

    
    
    if (enc->codec_id == CODEC_ID_DVB_SUBTITLE)
        nb = 2;
    else
        nb = 1;

    for(i = 0; i < nb; i++) {
        subtitle_out_size = avcodec_encode_subtitle(enc, job->subtitle_out,
                                                    subtitle_out_max_size, sub);

        av_init_packet(&pkt);
        pkt.stream_index = ost->index;
        pkt.data = job->subtitle_out;
        pkt.size = subtitle_out_size;
		AVRational tmp = {1, AV_TIME_BASE};
        pkt.pts = av_rescale_q(av_rescale_q(pts, ist->st->time_base, tmp) + job->input_files_ts_offset[ist->file_index], tmp,  ost->st->time_base);
        if (enc->codec_id == CODEC_ID_DVB_SUBTITLE) {
            
            if (i == 0)
                pkt.pts += 90 * sub->start_display_time;
            else
                pkt.pts += 90 * sub->end_display_time;
        }
        write_frame(s, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);
    }
}  

static int output_packet(vmsMediaConverterJob *job,
						 AVInputStream *ist, int ist_index,
                         AVOutputStream **ost_table, int nb_ostreams,
                         const AVPacket *pkt)
{
    AVFormatContext *os;
    AVOutputStream *ost;
    uint8_t *ptr;
    int len, ret, i;
    uint8_t *data_buf;
    int data_size, got_picture;
    AVFrame picture;
    void *buffer_to_free;
    AVSubtitle subtitle, *subtitle_to_free;
    int got_subtitle;

    if(!pkt){
        ist->pts= ist->next_pts; 
    } else if (pkt->dts != AV_NOPTS_VALUE) { 
		AVRational tmp = {1, AV_TIME_BASE};
        ist->next_pts = ist->pts = av_rescale_q(pkt->dts, ist->st->time_base, tmp);
    } else {

    }

    if (pkt == NULL) {
        
        ptr = NULL;
        len = 0;
        goto handle_eof;
    }

    len = pkt->size;
    ptr = pkt->data;
    while (len > 0) {
    handle_eof:
        
        data_buf = NULL; 
        data_size = 0;
        subtitle_to_free = NULL;
        if (ist->decoding_needed) {
            switch(ist->st->codec->codec_type) {
            case CODEC_TYPE_AUDIO:{
                if(pkt)
                    job->samples= (short*)av_fast_realloc(job->samples, &job->samples_size, FFMAX(pkt->size, AVCODEC_MAX_AUDIO_FRAME_SIZE));
                    
                ret = avcodec_decode_audio(ist->st->codec, (int*)job->samples, &data_size,
                                           ptr, len);
                if (ret < 0)
                    goto fail_decode;
                ptr += ret;
                len -= ret;
                
                
                if (data_size <= 0) {
                    
                    continue;
                }
                data_buf = (uint8_t *)job->samples;
                ist->next_pts += ((int64_t)AV_TIME_BASE/2 * data_size) /
                    (ist->st->codec->sample_rate * ist->st->codec->channels);
                break;}
            case CODEC_TYPE_VIDEO:
                    data_size = (ist->st->codec->width * ist->st->codec->height * 3) / 2;
                    
                    avcodec_get_frame_defaults(&picture);

                    ret = avcodec_decode_video(ist->st->codec,
                                               &picture, &got_picture, ptr, len);
                    ist->st->quality= (float)picture.quality;
                    if (ret < 0)
                        goto fail_decode;
                    if (!got_picture) {
                        
                        goto discard_packet;
                    }
                    if (ist->st->codec->time_base.num != 0) {
                        ist->next_pts += ((int64_t)AV_TIME_BASE *
                                          ist->st->codec->time_base.num) /
                            ist->st->codec->time_base.den;
                    }
                    len = 0;
                    break;
            case CODEC_TYPE_SUBTITLE:
                ret = avcodec_decode_subtitle(ist->st->codec,
                                              &subtitle, &got_subtitle, ptr, len);
                if (ret < 0)
                    goto fail_decode;
                if (!got_subtitle) {
                    goto discard_packet;
                }
                subtitle_to_free = &subtitle;
                len = 0;
                break;
            default:
                goto fail_decode;
            }
        } else {
                switch(ist->st->codec->codec_type) {
                case CODEC_TYPE_AUDIO:
                    ist->next_pts += ((int64_t)AV_TIME_BASE * ist->st->codec->frame_size) /
                        (ist->st->codec->sample_rate * ist->st->codec->channels);
                    break;
                case CODEC_TYPE_VIDEO:
                    if (ist->st->codec->time_base.num != 0) {
                        ist->next_pts += ((int64_t)AV_TIME_BASE *
                                          ist->st->codec->time_base.num) /
                            ist->st->codec->time_base.den;
                    }
                    break;
                }
                data_buf = ptr;
                data_size = len;
                ret = len;
                len = 0;
            }

            buffer_to_free = NULL;
            if (ist->st->codec->codec_type == CODEC_TYPE_VIDEO) {
                pre_process_video_frame(job, ist, (AVPicture *)&picture,
                                        &buffer_to_free);
            }

            
            if (ist->st->codec->codec_type == CODEC_TYPE_AUDIO) {
                if (job->audio_volume != 256) {
                    short *volp;
                    volp = job->samples;
                    for(i=0;i<(int)(data_size / sizeof(short));i++) {
                        int v = ((*volp) * job->audio_volume + 128) >> 8;
                        if (v < -32768) v = -32768;
                        if (v >  32767) v = 32767;
                        *volp++ = v;
                    }
                }
            }

            
            if (ist->st->codec->rate_emu) {
                int64_t pts = av_rescale((int64_t) ist->frame * ist->st->codec->time_base.num, 1000000, ist->st->codec->time_base.den);
                int64_t now = av_gettime() - ist->start;
                if (pts > now)
                    Sleep((DWORD)((pts - now) / 1000));

                ist->frame++;
            }

#if 0
            
            
            if (ist->st->codec->codec_id == CODEC_ID_MPEG1VIDEO) {
                if (ist->st->codec->pict_type != B_TYPE) {
                    int64_t tmp;
                    tmp = ist->last_ip_pts;
                    ist->last_ip_pts  = ist->frac_pts.val;
                    ist->frac_pts.val = tmp;
                }
            }
#endif
            
            if (job->start_time == 0 || ist->pts >= job->start_time)
                for(i=0;i<nb_ostreams;i++) {
                    int frame_size;

                    ost = ost_table[i];
                    if (ost->source_index == ist_index) {
                        os = job->output_files[ost->file_index];

                        
                        

                        if (ost->encoding_needed) {
                            switch(ost->st->codec->codec_type) {
                            case CODEC_TYPE_AUDIO:
                                do_audio_out(job, os, ost, ist, data_buf, data_size);
                                break;
                            case CODEC_TYPE_VIDEO:
                                    do_video_out(job, os, ost, ist, &picture, &frame_size);
                                    job->video_size += frame_size;
                                    if (job->do_vstats && frame_size)
                                        do_video_stats(job, os, ost, frame_size);
                                break;
                            case CODEC_TYPE_SUBTITLE:
                                do_subtitle_out(job, os, ost, ist, &subtitle, pkt->pts);
                                break;
                            default:
                                throw 0; 
                            }
                        } else {
                            AVFrame avframe; 
                            AVPacket opkt;
                            av_init_packet(&opkt);

                            
                            

                            avcodec_get_frame_defaults(&avframe);
                            ost->st->codec->coded_frame= &avframe;
                            avframe.key_frame = pkt->flags & PKT_FLAG_KEY;

                            if(ost->st->codec->codec_type == CODEC_TYPE_AUDIO)
                                job->audio_size += data_size;
                            else if (ost->st->codec->codec_type == CODEC_TYPE_VIDEO) {
                                job->video_size += data_size;
                                ost->sync_opts++;
                            }

							AVRational tmp = {1, AV_TIME_BASE};
                            opkt.stream_index= ost->index;
                            if(pkt->pts != AV_NOPTS_VALUE)
                                opkt.pts= av_rescale_q(av_rescale_q(pkt->pts, ist->st->time_base, tmp) + job->input_files_ts_offset[ist->file_index], tmp,  ost->st->time_base);
                            else
                                opkt.pts= AV_NOPTS_VALUE;

                            {
                                int64_t dts;
                                if (pkt->dts == AV_NOPTS_VALUE)
                                    dts = ist->next_pts;
                                else
                                    dts= av_rescale_q(pkt->dts, ist->st->time_base, tmp);
                                opkt.dts= av_rescale_q(dts + job->input_files_ts_offset[ist->file_index], tmp,  ost->st->time_base);
                            }
                            opkt.flags= pkt->flags;

                            
                            if(av_parser_change(ist->st->parser, ost->st->codec, &opkt.data, &opkt.size, data_buf, data_size, pkt->flags & PKT_FLAG_KEY))
                                opkt.destruct= av_destruct_packet;

                            write_frame(os, &opkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt->stream_index]);
                            ost->st->codec->frame_number++;
                            ost->frame_number++;
                            av_free_packet(&opkt);
                        }
                    }
                }
            av_free(buffer_to_free);
            
            if (subtitle_to_free) {
                if (subtitle_to_free->rects != NULL) {
                    for (i = 0; i < (int)subtitle_to_free->num_rects; i++) {
                        av_free(subtitle_to_free->rects[i].bitmap);
                        av_free(subtitle_to_free->rects[i].rgba_palette);
                    }
                    av_freep(&subtitle_to_free->rects);
                }
                subtitle_to_free->num_rects = 0;
                subtitle_to_free = NULL;
            }
        }
 discard_packet:
    if (pkt == NULL) {
        

        for(i=0;i<nb_ostreams;i++) {
            ost = ost_table[i];
            if (ost->source_index == ist_index) {
                AVCodecContext *enc= ost->st->codec;
                os = job->output_files[ost->file_index];

                if(ost->st->codec->codec_type == CODEC_TYPE_AUDIO && enc->frame_size <=1)
                    continue;
                if(ost->st->codec->codec_type == CODEC_TYPE_VIDEO && (os->oformat->flags & AVFMT_RAWPICTURE))
                    continue;

                if (ost->encoding_needed) {
                    for(;;) {
                        AVPacket pkt;
                        int fifo_bytes;
                        av_init_packet(&pkt);
                        pkt.stream_index= ost->index;

                        switch(ost->st->codec->codec_type) {
                        case CODEC_TYPE_AUDIO:
                            fifo_bytes = av_fifo_size(&ost->fifo);
                            ret = 0;
                            
                            if(fifo_bytes > 0 && enc->codec->capabilities & CODEC_CAP_SMALL_LAST_FRAME) {
                                int fs_tmp = enc->frame_size;
                                enc->frame_size = fifo_bytes / (2 * enc->channels);
                                if(av_fifo_read(&ost->fifo, (uint8_t*)job->samples, fifo_bytes) == 0) {
                                    ret = avcodec_encode_audio(enc, job->bit_buffer, job->bit_buffer_size, job->samples);
                                }
                                enc->frame_size = fs_tmp;
                            }
                            if(ret <= 0) {
                                ret = avcodec_encode_audio(enc, job->bit_buffer, job->bit_buffer_size, NULL);
                            }
                            job->audio_size += ret;
                            pkt.flags |= PKT_FLAG_KEY;
                            break;
                        case CODEC_TYPE_VIDEO:
                            ret = avcodec_encode_video(enc, job->bit_buffer, job->bit_buffer_size, NULL);
                            job->video_size += ret;
                            if(enc->coded_frame && enc->coded_frame->key_frame)
                                pkt.flags |= PKT_FLAG_KEY;
                            if (ost->logfile && enc->stats_out) {
                                
                            }
                            break;
                        default:
                            ret=-1;
                        }

                        if(ret<=0)
                            break;
                        pkt.data= job->bit_buffer;
                        pkt.size= ret;
                        if(enc->coded_frame && enc->coded_frame->pts != AV_NOPTS_VALUE)
                            pkt.pts= av_rescale_q(enc->coded_frame->pts, enc->time_base, ost->st->time_base);
                        write_frame(os, &pkt, ost->st->codec, job->bitstream_filters[ost->file_index][pkt.stream_index]);
                    }
                }
            }
        }
    }

    return 0;
 fail_decode:
    return -1;
}  

static void av_encode(vmsMediaConverterJob *job,
					 AVFormatContext **output_files,
                     int nb_output_files,
                     AVFormatContext **input_files,
                     int nb_input_files,
                     AVStreamMap *stream_maps, int nb_stream_maps,
					 int *pnProgress, BOOL *pbCancel)
{
    int ret, i, j, k, n, nb_istreams = 0, nb_ostreams = 0;
    AVFormatContext *is, *os;
    AVCodecContext *codec, *icodec;
    AVOutputStream *ost, **ost_table = NULL;
    AVInputStream *ist, **ist_table = NULL;
    AVInputFile *file_table;
    AVFormatContext *stream_no_data;
    int key;

    file_table= (AVInputFile*) av_mallocz(nb_input_files * sizeof(AVInputFile));
    if (!file_table)
        goto fail;

    
    j = 0;
    for(i=0;i<nb_input_files;i++) {
        is = input_files[i];
        file_table[i].ist_index = j;
        file_table[i].nb_streams = is->nb_streams;
        j += is->nb_streams;
    }
    nb_istreams = j;

    ist_table = (AVInputStream**)av_mallocz(nb_istreams * sizeof(AVInputStream *));
    if (!ist_table)
        goto fail;

    for(i=0;i<nb_istreams;i++) {
        ist = (AVInputStream*)av_mallocz(sizeof(AVInputStream));
        if (!ist)
            goto fail;
        ist_table[i] = ist;
    }
    j = 0;
    for(i=0;i<nb_input_files;i++) {
        is = input_files[i];
        for(k=0;k<(int)is->nb_streams;k++) {
            ist = ist_table[j++];
            ist->st = is->streams[k];
            ist->file_index = i;
            ist->index = k;
            ist->discard = 1; 

            if (ist->st->codec->rate_emu) {
                ist->start = av_gettime();
                ist->frame = 0;
            }
        }
    }

    
    nb_ostreams = 0;
    for(i=0;i<nb_output_files;i++) {
        os = output_files[i];
        if (!os->nb_streams)
            throw 0;
        nb_ostreams += os->nb_streams;
    }
    if (nb_stream_maps > 0 && nb_stream_maps != nb_ostreams)
        throw 0;

    
    for(i=0;i<nb_stream_maps;i++) {
        int fi = stream_maps[i].file_index;
        int si = stream_maps[i].stream_index;

        if (fi < 0 || fi > nb_input_files - 1 ||
				si < 0 || si > file_table[fi].nb_streams - 1)
            throw 0;

        fi = stream_maps[i].sync_file_index;
        si = stream_maps[i].sync_stream_index;
        if (fi < 0 || fi > nb_input_files - 1 ||
				si < 0 || si > file_table[fi].nb_streams - 1)
            throw 0;

    }

    ost_table = (AVOutputStream**)av_mallocz(sizeof(AVOutputStream *) * nb_ostreams);
    if (!ost_table)
        goto fail;
    for(i=0;i<nb_ostreams;i++) {
        ost = (AVOutputStream*)av_mallocz(sizeof(AVOutputStream));
        if (!ost)
            goto fail;
        ost_table[i] = ost;
    }

    n = 0;
    for(k=0;k<nb_output_files;k++) {
        os = output_files[k];
        for(i=0;i<(int)os->nb_streams;i++) {
            int found;
            ost = ost_table[n++];
            ost->file_index = k;
            ost->index = i;
            ost->st = os->streams[i];
            if (nb_stream_maps > 0) {
                ost->source_index = file_table[stream_maps[n-1].file_index].ist_index +
                    stream_maps[n-1].stream_index;

                
                if (ist_table[ost->source_index]->st->codec->codec_type != ost->st->codec->codec_type) {
                    throw 0;
                }

            } else {
                
                found = 0;
                for(j=0;j<nb_istreams;j++) {
                    ist = ist_table[j];
                    if (ist->discard &&
                        ist->st->codec->codec_type == ost->st->codec->codec_type) {
                        ost->source_index = j;
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    
                    for(j=0;j<nb_istreams;j++) {
                        ist = ist_table[j];
                        if (ist->st->codec->codec_type == ost->st->codec->codec_type) {
                            ost->source_index = j;
                            found = 1;
                        }
                    }
                    if (!found) {
                        throw 0;
                    }
                }
            }
            ist = ist_table[ost->source_index];
            ist->discard = 0;
            ost->sync_ist = (nb_stream_maps > 0) ?
                ist_table[file_table[stream_maps[n-1].sync_file_index].ist_index +
                         stream_maps[n-1].sync_stream_index] : ist;
        }
    }

    
    for(i=0;i<nb_ostreams;i++) {
        ost = ost_table[i];
        ist = ist_table[ost->source_index];

        codec = ost->st->codec;
        icodec = ist->st->codec;

        if (ost->st->stream_copy) {
            
            codec->codec_id = icodec->codec_id;
            codec->codec_type = icodec->codec_type;
            if(!codec->codec_tag) codec->codec_tag = icodec->codec_tag;
            codec->bit_rate = 128000;
            codec->extradata= icodec->extradata;
            codec->extradata_size= icodec->extradata_size;
            if(av_q2d(icodec->time_base) > av_q2d(ist->st->time_base) && av_q2d(ist->st->time_base) < 1.0/1000)
                codec->time_base = icodec->time_base;
            else
                codec->time_base = ist->st->time_base;
            switch(codec->codec_type) {
            case CODEC_TYPE_AUDIO:
                codec->sample_rate = icodec->sample_rate;
                codec->channels = icodec->channels;
                codec->frame_size = icodec->frame_size;
                codec->block_align= icodec->block_align;
                break;
            case CODEC_TYPE_VIDEO:
                codec->pix_fmt = icodec->pix_fmt;
                codec->width = icodec->width;
                codec->height = icodec->height;
                codec->has_b_frames = icodec->has_b_frames;
                break;
            case CODEC_TYPE_SUBTITLE:
                break;
            default:
                throw 0; 
            }
        } else {
            switch(codec->codec_type) {
            case CODEC_TYPE_AUDIO:
                if (av_fifo_init(&ost->fifo, 2 * MAX_AUDIO_PACKET_SIZE))
                    goto fail;

                if (codec->channels == icodec->channels &&
                    codec->sample_rate == icodec->sample_rate) {
                    ost->audio_resample = 0;
                } else {
                    if (codec->channels != icodec->channels &&
                        (icodec->codec_id == CODEC_ID_AC3 ||
                         icodec->codec_id == CODEC_ID_DTS)) {
                        
                        
                        
                        icodec->channels = codec->channels;
                        if (codec->sample_rate == icodec->sample_rate)
                            ost->audio_resample = 0;
                        else {
                            ost->audio_resample = 1;
                        }
                    } else {
                        ost->audio_resample = 1;
                    }
                }
                if(job->audio_sync_method>1)
                    ost->audio_resample = 1;

                if(ost->audio_resample){
                    ost->resample = audio_resample_init(codec->channels, icodec->channels,
                                                    codec->sample_rate, icodec->sample_rate);
                    if(!ost->resample){
                        throw 0; 
                    }
                }
                ist->decoding_needed = 1;
                ost->encoding_needed = 1;
                break;
            case CODEC_TYPE_VIDEO:
                ost->video_crop = ((job->frame_leftBand + job->frame_rightBand + job->frame_topBand + job->frame_bottomBand) != 0);
                ost->video_pad = ((job->frame_padleft + job->frame_padright + job->frame_padtop + job->frame_padbottom) != 0);
                ost->video_resample = ((codec->width != icodec->width -
                                (job->frame_leftBand + job->frame_rightBand) +
                                (job->frame_padleft + job->frame_padright)) ||
                        (codec->height != icodec->height -
                                (job->frame_topBand  + job->frame_bottomBand) +
                                (job->frame_padtop + job->frame_padbottom)) ||
                        (codec->pix_fmt != icodec->pix_fmt));
                if (ost->video_crop) {
                    ost->topBand = job->frame_topBand;
                    ost->leftBand = job->frame_leftBand;
                }
                if (ost->video_pad) {
                    ost->padtop = job->frame_padtop;
                    ost->padleft = job->frame_padleft;
                    ost->padbottom = job->frame_padbottom;
                    ost->padright = job->frame_padright;
                    if (!ost->video_resample) {
                        avcodec_get_frame_defaults(&ost->pict_tmp);
                        if( avpicture_alloc( (AVPicture*)&ost->pict_tmp, codec->pix_fmt,
                                         codec->width, codec->height ) )
                            goto fail;
                    }
                }
                if (ost->video_resample) {
                    avcodec_get_frame_defaults(&ost->pict_tmp);
                    if( avpicture_alloc( (AVPicture*)&ost->pict_tmp, codec->pix_fmt,
                                         codec->width, codec->height ) )
                        goto fail;

                    ost->img_resample_ctx = sws_getContext(
                            icodec->width - (job->frame_leftBand + job->frame_rightBand),
                            icodec->height - (job->frame_topBand + job->frame_bottomBand),
                            icodec->pix_fmt,
                            codec->width - (job->frame_padleft + job->frame_padright),
                            codec->height - (job->frame_padtop + job->frame_padbottom),
                            codec->pix_fmt,
                            job->sws_flags, NULL, NULL, NULL);
                    if (ost->img_resample_ctx == NULL) {
                        throw 0;
                    }
                    ost->resample_height = icodec->height - (job->frame_topBand + job->frame_bottomBand);
                }
                ost->encoding_needed = 1;
                ist->decoding_needed = 1;
                break;
            case CODEC_TYPE_SUBTITLE:
                ost->encoding_needed = 1;
                ist->decoding_needed = 1;
                break;
            default:
                
                break;
            }
            
            if (ost->encoding_needed &&
                (codec->flags & (CODEC_FLAG_PASS1 | CODEC_FLAG_PASS2))) {
                char logfilename[1024];
                FILE *f;
                int size;
                char *logbuffer;

                sprintf(logfilename, "%s-%d.log",
                         job->pass_logfilename ?
                         job->pass_logfilename : DEFAULT_PASS_LOGFILENAME, i);
                if (codec->flags & CODEC_FLAG_PASS1) {
                    f = fopen(logfilename, "w");
                    if (!f) {
                        throw 0;
                    }
                    ost->logfile = f;
                } else {
                    
                    f = fopen(logfilename, "r");
                    if (!f) {
                        throw 0;
                    }
                    fseek(f, 0, SEEK_END);
                    size = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    logbuffer = (char*)av_malloc(size + 1);
                    if (!logbuffer) {
                        throw 0;
                    }
                    size = fread(logbuffer, 1, size, f);
                    fclose(f);
                    logbuffer[size] = '\0';
                    codec->stats_in = logbuffer;
                }
            }
        }
        if(codec->codec_type == CODEC_TYPE_VIDEO){
            int size= codec->width * codec->height;
            job->bit_buffer_size= FFMAX(job->bit_buffer_size, 4*size);
        }
    }

    if (!job->bit_buffer)
        job->bit_buffer = (unsigned char*)av_malloc(job->bit_buffer_size);
    if (!job->bit_buffer)
        goto fail;

    

    
    for(i=0;i<nb_ostreams;i++) {
        ost = ost_table[i];
        if (ost->encoding_needed) {
            AVCodec *codec;
            codec = avcodec_find_encoder(ost->st->codec->codec_id);
            if (!codec) {
                throw 0;
            }
            if (avcodec_open(ost->st->codec, codec) < 0) {
                throw 0;
            }
            job->extra_size += ost->st->codec->extradata_size;
        }
    }

    
    for(i=0;i<nb_istreams;i++) {
        ist = ist_table[i];
        if (ist->decoding_needed) {
            AVCodec *codec;
            codec = avcodec_find_decoder(ist->st->codec->codec_id);
            if (!codec) {
                throw 0;
            }
            if (avcodec_open(ist->st->codec, codec) < 0) {
                throw 0;
            }
            
            
        }
    }

    
    for(i=0;i<nb_istreams;i++) {
        ist = ist_table[i];
        is = input_files[ist->file_index];
        ist->pts = 0;
		AVRational tmp = {1, AV_TIME_BASE};
        ist->next_pts = av_rescale_q(ist->st->start_time, ist->st->time_base, tmp);
        if(ist->st->start_time == AV_NOPTS_VALUE)
            ist->next_pts=0;
        if(job->input_files_ts_offset[ist->file_index])
            ist->next_pts= AV_NOPTS_VALUE;
        ist->is_start = 1;
    }

    
    for(i=0;i<nb_input_files;i++) {
        file_table[i].buffer_size_max = 2048;
    }

    
    for (i=0;i<job->nb_meta_data_maps;i++) {
        AVFormatContext *out_file;
        AVFormatContext *in_file;

        int out_file_index = job->meta_data_maps[i].out_file;
        int in_file_index = job->meta_data_maps[i].in_file;
        if ( out_file_index < 0 || out_file_index >= nb_output_files ) {
            ret = -EINVAL;
            goto fail;
        }
        if ( in_file_index < 0 || in_file_index >= nb_input_files ) {
            ret = -EINVAL;
            goto fail;
        }

        out_file = output_files[out_file_index];
        in_file = input_files[in_file_index];

        strcpy(out_file->title, in_file->title);
        strcpy(out_file->author, in_file->author);
        strcpy(out_file->copyright, in_file->copyright);
        strcpy(out_file->comment, in_file->comment);
        strcpy(out_file->album, in_file->album);
        out_file->year = in_file->year;
        out_file->track = in_file->track;
        strcpy(out_file->genre, in_file->genre);
    }

    
    for(i=0;i<nb_output_files;i++) {
        os = output_files[i];
        if (av_write_header(os) < 0) {
            ret = -EINVAL;
            goto fail;
        }
    }

    
  
    stream_no_data = 0;
    key = -1;

	int totalsize; totalsize = 0;
	bool bWritten; bWritten = false;

    for(;;) {
        int file_index, ist_index;
        AVPacket pkt;
        double ipts_min;
        double opts_min;

    redo:
        ipts_min= 1e100;
        opts_min= 1e100;  

        
        file_index = -1;
        for(i=0;i<nb_ostreams;i++) {
            double ipts, opts;
            ost = ost_table[i];
            os = output_files[ost->file_index];
            ist = ist_table[ost->source_index];
            if(ost->st->codec->codec_type == CODEC_TYPE_VIDEO)
                opts = ost->sync_opts * av_q2d(ost->st->codec->time_base);
            else
                opts = ost->st->pts.val * av_q2d(ost->st->time_base);
            ipts = (double)ist->pts;
            if (!file_table[ist->file_index].eof_reached){
                if(ipts < ipts_min) {
                    ipts_min = ipts;
                    if(job->input_sync ) file_index = ist->file_index;
                }
                if(opts < opts_min) {
                    opts_min = opts;
                    if(!job->input_sync) file_index = ist->file_index;
                }
            }
            if(ost->frame_number >= job->max_frames[ost->st->codec->codec_type]){
                file_index= -1;
                break;
            }
        }
        
        if (file_index < 0) {
            break;
        }

        
        if (job->recording_time > 0 && opts_min >= (job->recording_time / 1000000.0))
            break;

        
        if (job->limit_filesize != 0 && (job->limit_filesize * 1024) < url_ftell(&output_files[0]->pb))
            break;

        
        is = input_files[file_index];
        if (av_read_frame(is, &pkt) < 0) {
            file_table[file_index].eof_reached = 1;
            if (job->opt_shortest) break; else continue; 
        }

        if (!pkt.size) {
            stream_no_data = is;
        } else {
            stream_no_data = 0;
        }
       
        
        if (pkt.stream_index >= file_table[file_index].nb_streams)
            goto discard_packet;
        ist_index = file_table[file_index].ist_index + pkt.stream_index;
        ist = ist_table[ist_index];
        if (ist->discard)
            goto discard_packet;

        if (pkt.dts != AV_NOPTS_VALUE && ist->next_pts != AV_NOPTS_VALUE) {
			AVRational tmp = {1, AV_TIME_BASE};
            int64_t delta= av_rescale_q(pkt.dts, ist->st->time_base, tmp) - ist->next_pts;
            if(FFABS(delta) > 1*job->dts_delta_threshold*AV_TIME_BASE && !job->copy_ts){
                job->input_files_ts_offset[ist->file_index]-= delta;
                
                for(i=0; i<file_table[file_index].nb_streams; i++){
                    int index= file_table[file_index].ist_index + i;
                    ist_table[index]->next_pts += delta;
                    ist_table[index]->is_start=1;
                }
            }
        }

		totalsize += pkt.size;

		if (pbCancel && *pbCancel)
			break;
		
        
        if (output_packet(job, ist, ist_index, ost_table, nb_ostreams, &pkt) < 0) {                
            av_free_packet(&pkt);
            goto redo;
        }

		bWritten = true;

		if (pnProgress)
			*pnProgress = MulDiv (totalsize, 100, input_files [0]->file_size); 

    discard_packet:
        av_free_packet(&pkt);

        
        
    }

    
    for(i=0;i<nb_istreams;i++) {
        ist = ist_table[i];
        if (ist->decoding_needed) {
            output_packet(job, ist, i, ost_table, nb_ostreams, NULL);
        }
    }

    
    for(i=0;i<nb_output_files;i++) {
        os = output_files[i];
        av_write_trailer(os);
    }

    
    

    
    for(i=0;i<nb_ostreams;i++) {
        ost = ost_table[i];
        if (ost->encoding_needed) {
            av_freep(&ost->st->codec->stats_in);
            avcodec_close(ost->st->codec);
        }
    }

    
    for(i=0;i<nb_istreams;i++) {
        ist = ist_table[i];
        if (ist->decoding_needed) {
            avcodec_close(ist->st->codec);
        }
    }

    

 fail1:
    av_freep(&job->bit_buffer);
    av_free(file_table);

    if (ist_table) {
        for(i=0;i<nb_istreams;i++) {
            ist = ist_table[i];
            av_free(ist);
        }
        av_free(ist_table);
    }
    if (ost_table) {
        for(i=0;i<nb_ostreams;i++) {
            ost = ost_table[i];
            if (ost) {
                if (ost->logfile) {
                    fclose(ost->logfile);
                    ost->logfile = NULL;
                }
                av_fifo_free(&ost->fifo); 
                av_free(ost->pict_tmp.data[0]);
                if (ost->video_resample)
                    sws_freeContext(ost->img_resample_ctx);
                if (ost->audio_resample)
                    audio_resample_close(ost->resample);
                av_free(ost);
            }
        }
        av_free(ost_table);
    }

	if (bWritten == false)
		throw 0;

    return;
 fail:
    ret = -ENOMEM;
    goto fail1;
}

static bool opt_format(vmsMediaConverterJob *job, const char *arg)
{
    
    if (!strcmp(arg, "pgmyuv")) {
        job->pgmyuv_compatibility_hack=1;

        arg = "image2";
    }

    job->file_iformat = av_find_input_format(arg);
    job->file_oformat = guess_format(arg, NULL, NULL);
    if (!job->file_iformat && !job->file_oformat) {
        throw 0;
    }

	return true;
}

static bool opt_codec(int *pstream_copy, int *pcodec_id,
                      int codec_type, const char *arg)
{
    AVCodec *p;

    if (!strcmp(arg, "copy")) {
        *pstream_copy = 1;
    } else {
        p = first_avcodec;
        while (p) {
            if (!strcmp(p->name, arg) && p->type == codec_type)
                break;
            p = p->next;
        }
        if (p == NULL) {
            throw 0;
        } else {
            *pcodec_id = p->id;
        }
    }

	return true;
}

static void opt_video_codec(vmsMediaConverterJob *job, const char *arg)
{
    opt_codec(&job->video_stream_copy, &job->video_codec_id, CODEC_TYPE_VIDEO, arg);
}

static void opt_audio_codec(vmsMediaConverterJob *job, const char *arg)
{
    opt_codec(&job->audio_stream_copy, &job->audio_codec_id, CODEC_TYPE_AUDIO, arg);
}

static void opt_audio_channels(vmsMediaConverterJob *job, int n)
{
    job->audio_channels = n;
}

static void opt_audio_bitrate(vmsMediaConverterJob *job, int n)
{
    job->audio_bit_rate = n * 1000;
}

static bool opt_frame_size(vmsMediaConverterJob *job, int w, int h)
{
	job->frame_width = w;
	job->frame_height = h;

    if ((job->frame_width % 2) != 0 || (job->frame_height % 2) != 0)
        throw 0;
    
	return true;
}

static void opt_audio_rate(vmsMediaConverterJob *job, int n)
{
    job->audio_sample_rate = n;
}

static bool opt_frame_rate(vmsMediaConverterJob *job, double rate)
{
	AVRational time_base = av_d2q(rate, 1001000);
	job->frame_rate = time_base.num;
	job->frame_rate_base = time_base.den;

	if (!job->frame_rate || !job->frame_rate_base)
        throw 0;

	return true;
}

BOOL ConvertMediaFile (LPCSTR pszSrcFile, LPCSTR pszDstFile, LPCSTR pszDstFormat,
						  LPCSTR pszAudioCodec, int nAudioChannels, int nAudioBitrate, 
						  int nAudioRate,
						  LPCSTR pszVideoCodec, int nVideoBitrate, int nVideoFrameRate,
						  int nVideoFrameWidth, int nVideoFrameHeight,
						  int* pnProgress, BOOL *pbCancel)
{
	
	vmsMediaConverterJob job;
	bool bRes = true;

	try{

	job.avctx_opts = avcodec_alloc_context();
    job.avformat_opts = av_alloc_format_context();

	opt_input_file (&job, pszSrcFile);

	
	opt_format (&job, pszDstFormat ? pszDstFormat : "avi");

	

	if (pszVideoCodec && *pszVideoCodec)
	{
		if (lstrcmpi (pszVideoCodec, "none") == 0)
			job.video_disable = true;
		else
			opt_video_codec (&job, pszVideoCodec);
	}

	if (nVideoBitrate != -1)
		job.avctx_opts->bit_rate = nVideoBitrate * 1000  ;

	if (nVideoFrameRate != -1)
		opt_frame_rate (&job, nVideoFrameRate );

	if (nVideoFrameWidth != -1 && nVideoFrameHeight != -1)
		opt_frame_size (&job, nVideoFrameWidth, nVideoFrameHeight);

	

	if (pszAudioCodec && *pszAudioCodec)
	{
		if (lstrcmpi (pszAudioCodec, "none") == 0)
			job.audio_disable = true;
		else
			opt_audio_codec (&job, pszAudioCodec);
	}

	if (nAudioChannels != -1)
		opt_audio_channels (&job, nAudioChannels );

	if (nAudioBitrate != -1)
		opt_audio_bitrate (&job, nAudioBitrate );

	if (nAudioRate != -1)
		opt_audio_rate (&job, nAudioRate );

	

	opt_output_file (&job, pszDstFile);

	av_encode(&job, job.output_files, job.nb_output_files, job.input_files, job.nb_input_files,
              job.stream_maps, job.nb_stream_maps, pnProgress, pbCancel);

	if (pnProgress)
		*pnProgress = 100;

	}
	catch (int) {
		bRes = false;
	}

	
    for(int i=0;i<job.nb_output_files;i++) {
        
        AVFormatContext *s = job.output_files[i];
        int j;
        if (!(s->oformat->flags & AVFMT_NOFILE))
            url_fclose(&s->pb);
        for(j=0;j<(int)s->nb_streams;j++) {
            av_free(s->streams[j]->codec);
            av_free(s->streams[j]);
        }
        av_free(s);
    }
    for(i=0;i<job.nb_input_files;i++)
        av_close_input_file(job.input_files[i]);

    if(job.intra_matrix)
        av_free(job.intra_matrix);
    if(job.inter_matrix)
        av_free(job.inter_matrix);

	if (pbCancel && *pbCancel)
	{
		DeleteFile (pszDstFile);
		bRes = FALSE;
	}

	return bRes;
}

void Initialize ()
{
	av_register_all ();
}

void Shutdown ()
{
	av_free_static ();
}

