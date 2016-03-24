/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSAVMERGERFFMPEGH
#define VMSAVMERGERFFMPEGH
#include "vmsAVMerger.h"
class vmsAVMergerFFMPEG : public vmsAVMerger{
public:
	~vmsAVMergerFFMPEG(){};
	virtual int merge( std::wstring videoFileName, std::wstring audioFileName,
		std::wstring outputFileName ) override;
};
#endif