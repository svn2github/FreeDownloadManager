/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVDEVICE_AVDEVICE_H
#define AVDEVICE_AVDEVICE_H

#include "libavutil/avutil.h"

#define LIBAVDEVICE_VERSION_MAJOR 52
#define LIBAVDEVICE_VERSION_MINOR  2
#define LIBAVDEVICE_VERSION_MICRO  0

#define LIBAVDEVICE_VERSION_INT AV_VERSION_INT(LIBAVDEVICE_VERSION_MAJOR, \
                                               LIBAVDEVICE_VERSION_MINOR, \
                                               LIBAVDEVICE_VERSION_MICRO)
#define LIBAVDEVICE_VERSION     AV_VERSION(LIBAVDEVICE_VERSION_MAJOR, \
                                           LIBAVDEVICE_VERSION_MINOR, \
                                           LIBAVDEVICE_VERSION_MICRO)
#define LIBAVDEVICE_BUILD       LIBAVDEVICE_VERSION_INT


unsigned avdevice_version(void);


const char *avdevice_configuration(void);


const char *avdevice_license(void);


void avdevice_register_all(void);

#endif 

