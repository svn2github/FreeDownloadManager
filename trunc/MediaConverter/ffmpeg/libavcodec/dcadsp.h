/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DCADSP_H
#define AVCODEC_DCADSP_H

typedef struct DCADSPContext {
    void (*lfe_fir)(float *out, const float *in, const float *coefs,
                    int decifactor, float scale, float bias);
} DCADSPContext;

void ff_dcadsp_init(DCADSPContext *s);
void ff_dcadsp_init_arm(DCADSPContext *s);

#endif 
