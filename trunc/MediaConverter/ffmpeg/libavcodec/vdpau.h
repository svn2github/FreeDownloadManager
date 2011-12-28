/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VDPAU_H
#define AVCODEC_VDPAU_H



#include <vdpau/vdpau.h>
#include <vdpau/vdpau_x11.h>


#define FF_VDPAU_STATE_USED_FOR_RENDER 1


#define FF_VDPAU_STATE_USED_FOR_REFERENCE 2


struct vdpau_render_state {
    VdpVideoSurface surface; 

    int state; 

    
    union VdpPictureInfo {
        VdpPictureInfoH264        h264;
        VdpPictureInfoMPEG1Or2    mpeg;
        VdpPictureInfoVC1          vc1;
        VdpPictureInfoMPEG4Part2 mpeg4;
    } info;

    
    int bitstream_buffers_allocated;
    int bitstream_buffers_used;
    
    VdpBitstreamBuffer *bitstream_buffers;
};



#endif 
