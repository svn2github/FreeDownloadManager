/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEDOWNLOADSMGRH
#define VMSYOUTUBEDOWNLOADSMGRH
#include "vmsAVMergingMgr.h"
#include "vmsThreadPool.h"

class vmsYouTubeDownloadsMgr{
	std::shared_ptr<vmsAVMergingMgr> m_spYouTubeAVMergingMgr;
	std::map<UINT, vmsDownloadSmartPtr> m_YouTubeLinkedAudioMap;
	boost::recursive_mutex m_mtxYouTubeLinkedDownload;
	std::shared_ptr<vmsThreadPool> m_spThreadPool;
public:
	vmsYouTubeDownloadsMgr();
	virtual ~vmsYouTubeDownloadsMgr();
	void setYouTubeAVMergingMgr( std::shared_ptr<vmsAVMergingMgr> spYouTubeAVMergingMgr ){
		m_spYouTubeAVMergingMgr = spYouTubeAVMergingMgr;		
	}
	void mergeYouTubeDownloadWithLinkedDownloads( vmsDownloadSmartPtr dld );
	void processYouTubeDownloadsOnStart();
	void updateMergedYouTubeVideoDownload( UINT videoId );
	void addLinkToYouTubeAudioDownload( vmsDownloadSmartPtr dld, vmsDownloadSmartPtr audioDld );
	vmsDownloadSmartPtr getLinkedAudioDownload( vmsDownloadSmartPtr dld );
	void removeLinksToYouTubeAudioDownload( UINT id );
	void handleEvent( fsDownload* dld, fsDLHistoryRecord *rec, fsDownloadsMgrEvent ev );
	bool checkIfAudioStillNeeded(vmsDownloadSmartPtr audioDld);
	void updateFileSize(vmsDownloadSmartPtr dld);
	bool reloadURL(vmsDownloadSmartPtr dld);
	void updateCorrespondingDownloads(fsDownload* dld, std::function<void(fsDownload *dld)> func);
	bool downloadNeedsProcessing(vmsDownloadSmartPtr dld);
	bool existsRunningDownloads();
};
#endif