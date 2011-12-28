/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFILTER_AVFILTER_H
#define AVFILTER_AVFILTER_H

#include "libavutil/avutil.h"

#define LIBAVFILTER_VERSION_MAJOR  1
#define LIBAVFILTER_VERSION_MINOR 19
#define LIBAVFILTER_VERSION_MICRO  0

#define LIBAVFILTER_VERSION_INT AV_VERSION_INT(LIBAVFILTER_VERSION_MAJOR, \
                                               LIBAVFILTER_VERSION_MINOR, \
                                               LIBAVFILTER_VERSION_MICRO)
#define LIBAVFILTER_VERSION     AV_VERSION(LIBAVFILTER_VERSION_MAJOR,   \
                                           LIBAVFILTER_VERSION_MINOR,   \
                                           LIBAVFILTER_VERSION_MICRO)
#define LIBAVFILTER_BUILD       LIBAVFILTER_VERSION_INT

#include <stddef.h>
#include "libavcodec/avcodec.h"


unsigned avfilter_version(void);


const char *avfilter_configuration(void);


const char *avfilter_license(void);


typedef struct AVFilterContext AVFilterContext;
typedef struct AVFilterLink    AVFilterLink;
typedef struct AVFilterPad     AVFilterPad;



typedef struct AVFilterPic
{
    uint8_t *data[4];           
    int linesize[4];            
    enum PixelFormat format;    

    unsigned refcount;          

    
    void *priv;
    
    void (*free)(struct AVFilterPic *pic);

    int w, h;                  
} AVFilterPic;


typedef struct AVFilterPicRef
{
    AVFilterPic *pic;           
    uint8_t *data[4];           
    int linesize[4];            
    int w;                      
    int h;                      

    int64_t pts;                
    int64_t pos;                

    AVRational pixel_aspect;    

    int perms;                  
#define AV_PERM_READ     0x01   
#define AV_PERM_WRITE    0x02   
#define AV_PERM_PRESERVE 0x04   
#define AV_PERM_REUSE    0x08   
#define AV_PERM_REUSE2   0x10   
} AVFilterPicRef;


AVFilterPicRef *avfilter_ref_pic(AVFilterPicRef *ref, int pmask);


void avfilter_unref_pic(AVFilterPicRef *ref);


typedef struct AVFilterFormats AVFilterFormats;
struct AVFilterFormats
{
    unsigned format_count;      
    enum PixelFormat *formats;  

    unsigned refcount;          
    AVFilterFormats ***refs;    
};


AVFilterFormats *avfilter_make_format_list(const enum PixelFormat *pix_fmts);


int avfilter_add_colorspace(AVFilterFormats **avff, enum PixelFormat pix_fmt);


AVFilterFormats *avfilter_all_colorspaces(void);


AVFilterFormats *avfilter_merge_formats(AVFilterFormats *a, AVFilterFormats *b);


void avfilter_formats_ref(AVFilterFormats *formats, AVFilterFormats **ref);


void avfilter_formats_unref(AVFilterFormats **ref);


void avfilter_formats_changeref(AVFilterFormats **oldref,
                                AVFilterFormats **newref);


struct AVFilterPad
{
    
    const char *name;

    
    enum AVMediaType type;

    
    int min_perms;

    
    int rej_perms;

    
    void (*start_frame)(AVFilterLink *link, AVFilterPicRef *picref);

    
    AVFilterPicRef *(*get_video_buffer)(AVFilterLink *link, int perms, int w, int h);

    
    void (*end_frame)(AVFilterLink *link);

    
    void (*draw_slice)(AVFilterLink *link, int y, int height, int slice_dir);

    
    int (*poll_frame)(AVFilterLink *link);

    
    int (*request_frame)(AVFilterLink *link);

    
    int (*config_props)(AVFilterLink *link);
};


void avfilter_default_start_frame(AVFilterLink *link, AVFilterPicRef *picref);

void avfilter_default_draw_slice(AVFilterLink *link, int y, int h, int slice_dir);

void avfilter_default_end_frame(AVFilterLink *link);

int avfilter_default_config_output_link(AVFilterLink *link);

int avfilter_default_config_input_link (AVFilterLink *link);

AVFilterPicRef *avfilter_default_get_video_buffer(AVFilterLink *link,
                                                  int perms, int w, int h);

void avfilter_set_common_formats(AVFilterContext *ctx, AVFilterFormats *formats);

int avfilter_default_query_formats(AVFilterContext *ctx);


void avfilter_null_start_frame(AVFilterLink *link, AVFilterPicRef *picref);


void avfilter_null_draw_slice(AVFilterLink *link, int y, int h, int slice_dir);


void avfilter_null_end_frame(AVFilterLink *link);


AVFilterPicRef *avfilter_null_get_video_buffer(AVFilterLink *link,
                                                  int perms, int w, int h);


typedef struct AVFilter
{
    const char *name;         

    int priv_size;      

    
    int (*init)(AVFilterContext *ctx, const char *args, void *opaque);

    
    void (*uninit)(AVFilterContext *ctx);

    
    int (*query_formats)(AVFilterContext *);

    const AVFilterPad *inputs;  
    const AVFilterPad *outputs; 

    
    const char *description;
} AVFilter;


struct AVFilterContext
{
    const AVClass *av_class;              

    AVFilter *filter;               

    char *name;                     

    unsigned input_count;           
    AVFilterPad   *input_pads;      
    AVFilterLink **inputs;          

    unsigned output_count;          
    AVFilterPad   *output_pads;     
    AVFilterLink **outputs;         

    void *priv;                     
};


struct AVFilterLink
{
    AVFilterContext *src;       
    unsigned int srcpad;        

    AVFilterContext *dst;       
    unsigned int dstpad;        

    
    enum {
        AVLINK_UNINIT = 0,      
        AVLINK_STARTINIT,       
        AVLINK_INIT             
    } init_state;

    int w;                      
    int h;                      
    enum PixelFormat format;    

    
    AVFilterFormats *in_formats;
    AVFilterFormats *out_formats;

    
    AVFilterPicRef *srcpic;

    AVFilterPicRef *cur_pic;
    AVFilterPicRef *outpic;
};


int avfilter_link(AVFilterContext *src, unsigned srcpad,
                  AVFilterContext *dst, unsigned dstpad);


int avfilter_config_links(AVFilterContext *filter);


AVFilterPicRef *avfilter_get_video_buffer(AVFilterLink *link, int perms,
                                          int w, int h);


int avfilter_request_frame(AVFilterLink *link);


int avfilter_poll_frame(AVFilterLink *link);


void avfilter_start_frame(AVFilterLink *link, AVFilterPicRef *picref);


void avfilter_end_frame(AVFilterLink *link);


void avfilter_draw_slice(AVFilterLink *link, int y, int h, int slice_dir);


void avfilter_register_all(void);


void avfilter_uninit(void);


int avfilter_register(AVFilter *filter);


AVFilter *avfilter_get_by_name(const char *name);


AVFilter **av_filter_next(AVFilter **filter);


AVFilterContext *avfilter_open(AVFilter *filter, const char *inst_name);


int avfilter_init_filter(AVFilterContext *filter, const char *args, void *opaque);


void avfilter_destroy(AVFilterContext *filter);


int avfilter_insert_filter(AVFilterLink *link, AVFilterContext *filt,
                           unsigned in, unsigned out);


void avfilter_insert_pad(unsigned idx, unsigned *count, size_t padidx_off,
                         AVFilterPad **pads, AVFilterLink ***links,
                         AVFilterPad *newpad);


static inline void avfilter_insert_inpad(AVFilterContext *f, unsigned index,
                                         AVFilterPad *p)
{
    avfilter_insert_pad(index, &f->input_count, offsetof(AVFilterLink, dstpad),
                        &f->input_pads, &f->inputs, p);
}


static inline void avfilter_insert_outpad(AVFilterContext *f, unsigned index,
                                          AVFilterPad *p)
{
    avfilter_insert_pad(index, &f->output_count, offsetof(AVFilterLink, srcpad),
                        &f->output_pads, &f->outputs, p);
}

#endif  
