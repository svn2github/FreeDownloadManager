/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsYouTubeDownloadsMgr.h"
#include "vmsThreadPoolAsio.h"
#include "vmsYouTubeURLReloadingTask.h"

vmsYouTubeDownloadsMgr::vmsYouTubeDownloadsMgr(){	
	m_spThreadPool = std::make_shared<vmsThreadPoolAsio>(1);
}

vmsYouTubeDownloadsMgr::~vmsYouTubeDownloadsMgr(){
};

void vmsYouTubeDownloadsMgr::mergeYouTubeDownloadWithLinkedDownloads( vmsDownloadSmartPtr dld ){
	if ( !dld )
		return;
	if ( ( dld->youTubeDetails.mediaType != YouTubeDownloadDetails::AUDIO ) && 
		( dld->youTubeDetails.mediaType != YouTubeDownloadDetails::VIDEO ))
		return;
	if ( m_spYouTubeAVMergingMgr ){
		YouTubeDownloadDetails::MediaType complementedType;
		if ( dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO )
			complementedType = YouTubeDownloadDetails::VIDEO;
		else 
			complementedType = YouTubeDownloadDetails::AUDIO;
		_DldsMgr.LockList (false);
		size_t nCount = _DldsMgr.GetCount ();
		for (size_t i = 0; i < nCount; ++i){
			vmsDownloadSmartPtr _dld = _DldsMgr.GetDownload( i );
			if (_dld && _dld->pMgr->IsDone() &&(_dld->nID != dld->nID) &&
				( _dld->youTubeDetails.mediaType == complementedType ) &&
				( _dld->youTubeDetails.youTubeID == dld->youTubeDetails.youTubeID ) && 
				( _dld->youTubeDetails.format == dld->youTubeDetails.format ))
			{
				if (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::VIDEO)
				{
					
					if (dld->postDownloadTasks.find (avmergePostDownloadTaskName) == dld->postDownloadTasks.end ())
					{
						std::unique_ptr <vmsPostDownloadTask> pdt (new vmsPostDownloadTask ());
						dld->postDownloadTasks [avmergePostDownloadTaskName] = std::move (pdt);
						_DldsMgr.onPostDownloadTaskCreated (dld, avmergePostDownloadTaskName);
					}
				}

				if (_dld->pMgr->IsDone())
				{
					if ( dld->youTubeDetails.mediaType == YouTubeDownloadDetails::VIDEO )
						m_spYouTubeAVMergingMgr->addMergeTask( dld, _dld );
					else if ( dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO )
						m_spYouTubeAVMergingMgr->addMergeTask( _dld, dld );
				}				
			}

		}
		_DldsMgr.UnlockList (false);
	}
}

void vmsYouTubeDownloadsMgr::processYouTubeDownloadsOnStart(){
	DLDS_LIST vDldsToDelete;
	DLDS_LIST vDldsToMerge;
	_DldsMgr.LockList(true);
	size_t nCount = _DldsMgr.GetCount();
	for (size_t i = 0; i < nCount; ++i){
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownload(i);
		if (dld && dld->pMgr->GetDownloadMgr()){
			if ((dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIOVIDEO) && dld->pMgr->IsDone()){
				updateFileSize(dld);
			}
			else if (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::VIDEO){
				vmsDownloadSmartPtr audioDld = _DldsMgr.findYouTubeDownload(dld->youTubeDetails.youTubeID,
					dld->youTubeDetails.format, YouTubeDownloadDetails::AUDIO);
				if (audioDld)
					addLinkToYouTubeAudioDownload(dld, audioDld);	
				if (dld->pMgr->IsDone())
					vDldsToMerge.push_back( dld );
			}
		}
	}
	for (size_t i = 0; i < nCount; ++i){
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownload(i);
		if (dld && dld->pMgr->GetDownloadMgr() && dld->pMgr->IsDone() &&
			(dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO) &&
			dld->youTubeDetails.isHidden && !checkIfAudioStillNeeded(dld)){
				vDldsToDelete.push_back(dld);
		}
	}
	_DldsMgr.UnlockList(true);
	if (!vDldsToDelete.empty())
		_DldsMgr.DeleteDownloads(vDldsToDelete, true, true, false);
	
	for (auto it = vDldsToMerge.begin(); it != vDldsToMerge.end(); ++it){		
		mergeYouTubeDownloadWithLinkedDownloads(*it);
	}
}

void vmsYouTubeDownloadsMgr::updateMergedYouTubeVideoDownload( UINT videoId ){
	vmsDownloadSmartPtr dld = _DldsMgr.GetDownloadByID( videoId );
	if ( dld ){
		dld->youTubeDetails.mediaType = YouTubeDownloadDetails::AUDIOVIDEO;
		dld->youTubeDetails.isMerged = true;
		updateFileSize(dld);
		dld->setDirty ();
		vmsDownloadSmartPtr audioDld = _DldsMgr.findYouTubeDownload(dld->youTubeDetails.youTubeID,
			dld->youTubeDetails.format, YouTubeDownloadDetails::AUDIO);
		if (audioDld && !checkIfAudioStillNeeded(audioDld)){
			if (audioDld->youTubeDetails.isHidden){
				DLDS_LIST vDlds;
				vDlds.push_back(audioDld);
				_DldsMgr.DeleteDownloads(vDlds, true, true, false);
			}
			else _DldsMgr.OnDldDoneCheckIfNeedDel(audioDld);
		}
		_DldsMgr.OnDldDoneCheckIfNeedDel(dld);
	}
}

void vmsYouTubeDownloadsMgr::addLinkToYouTubeAudioDownload( vmsDownloadSmartPtr dld, vmsDownloadSmartPtr audioDld ){
	if ( !dld || !audioDld )
		return;

	boost::lock_guard<boost::recursive_mutex> lock(m_mtxYouTubeLinkedDownload);	
	m_YouTubeLinkedAudioMap[dld->nID] = audioDld;	
}

vmsDownloadSmartPtr vmsYouTubeDownloadsMgr::getLinkedAudioDownload( vmsDownloadSmartPtr dld ){
	if ( !dld )
		return 0;
	boost::lock_guard<boost::recursive_mutex> lock(m_mtxYouTubeLinkedDownload);	
	vmsDownloadSmartPtr audioDld = m_YouTubeLinkedAudioMap[dld->nID];
	return audioDld;
}

void vmsYouTubeDownloadsMgr::removeLinksToYouTubeAudioDownload( UINT id )
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mtxYouTubeLinkedDownload);	
	std::vector<UINT> idsToDelete;
	for (auto it = m_YouTubeLinkedAudioMap.begin(); it!=m_YouTubeLinkedAudioMap.end(); ++it){
		if ( it->second && ( it->second->nID == id ) )
			idsToDelete.push_back( it->first );
	}
	for ( auto it = idsToDelete.begin(); it != idsToDelete.end(); ++it ){
		m_YouTubeLinkedAudioMap.erase( *it );
	}
}

void vmsYouTubeDownloadsMgr::handleEvent( fsDownload* dld, fsDLHistoryRecord *rec, fsDownloadsMgrEvent ev ){
	if ( dld && dld->isYouTube() ){
		switch (ev)
		{	
			case DME_DOWNLOADSTOPPEDORDONE:
				if ( ( ( dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO )
					|| (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::VIDEO) ) && dld->pMgr->IsDone()){
						mergeYouTubeDownloadWithLinkedDownloads( dld );				
				}
				break;
			case DME_DOWNLOADWASDELETEDFROMLIST:
				if ( dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO )
					removeLinksToYouTubeAudioDownload( dld->nID );
				break;	
		}
	}
}

bool vmsYouTubeDownloadsMgr::checkIfAudioStillNeeded(vmsDownloadSmartPtr audioDld){
	if (audioDld == NULL)
		return false;
	vmsDownloadSmartPtr videoDld = _DldsMgr.findYouTubeDownload(audioDld->youTubeDetails.youTubeID,
		audioDld->youTubeDetails.format, YouTubeDownloadDetails::VIDEO);
	return (videoDld != NULL);
}

void vmsYouTubeDownloadsMgr::updateFileSize(vmsDownloadSmartPtr dld){
	if (dld && (dld->youTubeDetails.mediaType != YouTubeDownloadDetails::EMPTY) &&
		dld->youTubeDetails.isMerged && dld->pMgr->GetDownloadMgr() &&
		!(dld->dwFlags & DLD_YOUTUBE_FILESIZE_MERGED)){		
			try{
				std::wstring filePath = dld->pMgr->get_OutputPath();
				UINT64 uSize = boost::filesystem::file_size(filePath);
				dld->pMgr->GetDownloadMgr()->SetFileSize(uSize);
				dld->dwFlags |= DLD_YOUTUBE_FILESIZE_MERGED;
			}
			catch (const boost::filesystem::filesystem_error& ex){				
			}
	}
}

bool vmsYouTubeDownloadsMgr::reloadURL(vmsDownloadSmartPtr dld){
	if (!dld || !dld->isYouTube() )
		return false;
	if (m_spThreadPool){
		auto task = std::make_shared<vmsYouTubeURLReloadingTask>(dld->nID);
		if (task){
			m_spThreadPool->addTask(task);
			return true; 
		}
	}
	return false;
}

void vmsYouTubeDownloadsMgr::updateCorrespondingDownloads(fsDownload* dld, std::function<void(fsDownload *dld)> func){
	if (dld && (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO)){
		DLDS_LIST vDlds;
		_DldsMgr.findYouTubeDownloads(dld->youTubeDetails.youTubeID, dld->youTubeDetails.format,
			YouTubeDownloadDetails::VIDEO, vDlds);
		for (size_t i = 0; i < vDlds.size(); i++)
		{
			vmsDownloadSmartPtr videoDld = vDlds[i];
			if (videoDld && func)
				func(videoDld);
		}
	}
}

bool vmsYouTubeDownloadsMgr::downloadNeedsProcessing(vmsDownloadSmartPtr dld){
	if (dld && dld->isYouTube()){
		if (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIOVIDEO)
			return false;
		else if (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::VIDEO)
			return true;
		else if (dld->youTubeDetails.mediaType == YouTubeDownloadDetails::AUDIO){
			return checkIfAudioStillNeeded(dld);
		}
	}
	return false;
}

bool vmsYouTubeDownloadsMgr::existsRunningDownloads(){
	bool bResult = false;
	_DldsMgr.LockList(true);
	size_t nCount = _DldsMgr.GetCount();
	for (size_t i = 0; i < nCount; ++i){
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownload(i);
		if (dld && dld->pMgr->GetDownloadMgr() && dld->isYouTube() && dld->pMgr->IsRunning()){
			bResult = true;
			break;
		}
	}	
	_DldsMgr.UnlockList(true);

	return bResult;
}