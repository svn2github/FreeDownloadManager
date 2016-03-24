/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSAVMERGINGTASKH
#define VMSAVMERGINGTASKH
#include "vmsAVMerger.h"

class vmsAVMergingTask : public std::enable_shared_from_this<vmsAVMergingTask>{
	unsigned int nVideoID;
	unsigned int nAudioID;
	std::wstring videoFileName;
	std::wstring audioFileName;
	std::wstring outputFileName;
public:
	vmsAVMergingTask( unsigned int _nVideoID, unsigned int _nAudioID, std::wstring _videoFileName,
		std::wstring _audioFileName, std::wstring _outputFileName ){
		nVideoID = _nVideoID;
		nAudioID = _nAudioID;
		videoFileName = _videoFileName;
		audioFileName = _audioFileName;
		outputFileName = _outputFileName;
	}
	unsigned int getVideoId(){ return nVideoID; }
	unsigned int getAudioId(){ return nAudioID; }
	std::wstring getVideoFileName(){ return videoFileName; }
	std::wstring getAudioFileName(){ return audioFileName; }
	std::wstring getOutputFileName(){ return outputFileName; }

	virtual bool merge( std::shared_ptr<vmsAVMerger> avMerger ){
		if ( avMerger ){
			return avMerger->merge( videoFileName, audioFileName, outputFileName );
		}
	}
};
#endif