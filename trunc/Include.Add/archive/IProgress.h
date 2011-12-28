/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __IPROGRESS_H
#define __IPROGRESS_H

#include "MyUnknown.h"
#include "Types.h"

DEFINE_GUID(IID_IProgress, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000000050000")
IProgress: public IUnknown
{
  STDMETHOD(SetTotal)(UInt64 total) PURE;
  STDMETHOD(SetCompleted)(const UInt64 *completeValue) PURE;
};

#endif
