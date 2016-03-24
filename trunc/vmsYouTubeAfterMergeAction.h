/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSYOUTUBEAFTERMERGEACTION
#define VMSYOUTUBEAFTERMERGEACTION
#include "vmsAfterMergeAction.h"
#include <boost\filesystem.hpp>
class vmsYouTubeAfterMergeAction : public vmsAfterMergeAction{
public:
	void doAfterMerge( std::shared_ptr<vmsAVMergingTask> mergingTask, int nMergeResult ){
		boost::system::error_code e;
		if ( ( nMergeResult == 0 ) && boost::filesystem::exists( mergingTask->getOutputFileName() ) ){
			boost::filesystem::copy_file( mergingTask->getOutputFileName(), mergingTask->getVideoFileName(),
				boost::filesystem::copy_option::overwrite_if_exists, e);
			boost::filesystem::remove( mergingTask->getOutputFileName(), e );
			_YouTubeDldsMgr.updateMergedYouTubeVideoDownload( mergingTask->getVideoId() );
		}
		auto dld = _DldsMgr.GetDownloadByID (mergingTask->getVideoId ());
		if (dld)
		{
			dld->postDownloadTasks.erase (avmergePostDownloadTaskName);
			_DldsMgr.onPostDownloadTaskFinished (dld, avmergePostDownloadTaskName, nMergeResult == 0);
		}
	}
};
#endif