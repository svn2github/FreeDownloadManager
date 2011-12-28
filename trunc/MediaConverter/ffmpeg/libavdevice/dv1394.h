/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVDEVICE_DV1394_H
#define AVDEVICE_DV1394_H

#define DV1394_DEFAULT_CHANNEL 63
#define DV1394_DEFAULT_CARD    0
#define DV1394_RING_FRAMES     20

#define DV1394_WIDTH  720
#define DV1394_NTSC_HEIGHT 480
#define DV1394_PAL_HEIGHT 576



#define DV1394_API_VERSION 0x20011127





#define DV1394_MAX_FRAMES 32


#define DV1394_NTSC_PACKETS_PER_FRAME 250
#define DV1394_PAL_PACKETS_PER_FRAME  300


#define DV1394_NTSC_FRAME_SIZE (480 * DV1394_NTSC_PACKETS_PER_FRAME)
#define DV1394_PAL_FRAME_SIZE  (480 * DV1394_PAL_PACKETS_PER_FRAME)




enum {
        
        DV1394_INVALID = 0,


        
        DV1394_INIT,


        
        DV1394_SHUTDOWN,


        
        DV1394_SUBMIT_FRAMES,


        
        DV1394_WAIT_FRAMES,

        
        DV1394_RECEIVE_FRAMES,


        DV1394_START_RECEIVE,


        
        DV1394_GET_STATUS,
};



enum pal_or_ntsc {
        DV1394_NTSC = 0,
        DV1394_PAL
};





struct dv1394_init {
        
        unsigned int api_version;

        
        unsigned int channel;

        
        unsigned int n_frames;

        
        enum pal_or_ntsc format;

        

        
        unsigned long cip_n;
        unsigned long cip_d;

        
        unsigned int syt_offset;
};









struct dv1394_status {
        
        struct dv1394_init init;

        
        int active_frame;

        
        unsigned int first_clear_frame;

        
        unsigned int n_clear_frames;

        
        unsigned int dropped_frames;

        
};


#endif 
