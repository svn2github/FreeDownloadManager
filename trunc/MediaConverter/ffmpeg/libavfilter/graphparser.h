/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFILTER_GRAPHPARSER_H
#define AVFILTER_GRAPHPARSER_H

#include "avfilter.h"
#include "avfiltergraph.h"


typedef struct AVFilterInOut {
    char *name;
    AVFilterContext *filter;
    int pad_idx;

    struct AVFilterInOut *next;
} AVFilterInOut;


int avfilter_graph_parse(AVFilterGraph *graph, const char *filters,
                         AVFilterInOut *inputs, AVFilterInOut *outputs,
                         AVClass *log_ctx);

#endif  
