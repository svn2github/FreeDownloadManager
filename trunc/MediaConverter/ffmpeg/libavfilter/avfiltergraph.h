/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFILTER_AVFILTERGRAPH_H
#define AVFILTER_AVFILTERGRAPH_H

#include "avfilter.h"

typedef struct AVFilterGraph {
    unsigned filter_count;
    AVFilterContext **filters;

    char *scale_sws_opts; 
} AVFilterGraph;


AVFilterContext *avfilter_graph_get_filter(AVFilterGraph *graph, char *name);


int avfilter_graph_add_filter(AVFilterGraph *graphctx, AVFilterContext *filter);


int avfilter_graph_check_validity(AVFilterGraph *graphctx, AVClass *log_ctx);


int avfilter_graph_config_links(AVFilterGraph *graphctx, AVClass *log_ctx);


int avfilter_graph_config_formats(AVFilterGraph *graphctx, AVClass *log_ctx);


void avfilter_graph_destroy(AVFilterGraph *graph);

#endif  
