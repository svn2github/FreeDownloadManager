/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_TIMEFILTER_H
#define AVFORMAT_TIMEFILTER_H


typedef struct TimeFilter TimeFilter;



TimeFilter * ff_timefilter_new(double clock_period, double feedback2_factor, double feedback3_factor);


double ff_timefilter_update(TimeFilter *self, double system_time, double period);


void ff_timefilter_reset(TimeFilter *);


void ff_timefilter_destroy(TimeFilter *);

#endif 
