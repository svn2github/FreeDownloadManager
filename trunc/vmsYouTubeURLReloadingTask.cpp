/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsYouTubeURLReloadingTask.h"
#include "vmsYouTubeParserDllMgr.h"
#include "vmsFdmURLDownloadMgr.h"

vmsYouTubeURLReloadingTask::vmsYouTubeURLReloadingTask(int nDownloadId)
{
	m_nDownloadId = nDownloadId;
}

int vmsYouTubeURLReloadingTask::doWork(){
	std::vector<unsigned int> ids;
	DLDS_LIST v;
	ids.push_back(m_nDownloadId);
	vmsDownloadSmartPtr dld;
	_DldsMgr.findDownloadsByIds(ids, v);
	if (!v.empty())
		dld = v.at(0);
	if (dld && dld->isYouTube()){
		auto youTubeParserDllMgr = vmsYouTubeParserDllMgr::get();
		if (youTubeParserDllMgr){
			auto extractor = youTubeParserDllMgr->objectsHolder()->createResourceObject(dld->youTubeDetails.originalURL);
			if (extractor){
				std::shared_ptr <vmsFdmURLDownloadMgr> urlDownloadMgr = std::make_shared <vmsFdmURLDownloadMgr>();
				extractor->object()->setDownloader(urlDownloadMgr->createDownloader());
				tstring oldURL = dld->pMgr->get_URL();
				tstring newURL = extractor->object()->reloadURL(oldURL);
				if (!newURL.empty()){
					std::vector<YouTubeParser::fileNode> nodes = extractor->object()->GetVariantsInfo();
					if (nodes.empty())
						return 0;
					UINT64 nNewFileSize = nodes.at(0).fFile.nFileSize;					
					fsDownload_NetworkProperties *dnp = dld->pMgr->GetDownloadMgr()->GetDNP();
					fsURL url;
					USES_CONVERSION;
					LPCTSTR strUrl = newURL.c_str();
					if ((IR_SUCCESS == url.Crack(strUrl)) && (fsDNP_ApplyUrl(dnp, strUrl) == IR_SUCCESS)
						&& (dld->getState() & DLDS_YOUTUBE_URL_RELOADING))
					{
						std::wstring filePath = dld->pMgr->get_OutputPath();
						boost::system::error_code ec;						
						if ((nNewFileSize != dld->youTubeDetails.fileSize) || !boost::filesystem::exists(filePath, ec)){
							dld->pMgr->SetToRestartState();
						}		
						dld->removeStateFlags(DLDS_YOUTUBE_URL_RELOADING);
						if (dld->pMgr->GetDownloadMgr())
							dld->pMgr->GetDownloadMgr()->SetURLUpdated(true);
						dld->setDirty();																
						
						return 1;
					}
				}
			}
		}
		dld->removeStateFlags(DLDS_YOUTUBE_URL_RELOADING);
		dld->setDirty();
		_DldsMgr.DownloadStateChanged(dld);
	}
	return 0;
}