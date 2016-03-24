/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSAVMERGINGMGR
#define VMSAVMERGINGMGR
#include "vmsAVMerger.h"
#include "vmsAfterMergeAction.h"
#include <boost\thread\recursive_mutex.hpp>
#include <boost\algorithm\string\predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "Utils.h"
#include "vmsAVMergingMgrCallback.h"

class vmsAVMergingMgr
{	
public:	
	vmsAVMergingMgr(std::shared_ptr<vmsAVMerger> spAVMerger, 
		std::shared_ptr<vmsAfterMergeAction> spAfterMergeAction,
		unsigned int _nThreadCount = 1);
	virtual ~vmsAVMergingMgr();		
	virtual void setAVMerger( std::shared_ptr<vmsAVMerger> _avMerger ){
		m_spAVMerger = _avMerger;
	}
	virtual void setCallback (std::shared_ptr <vmsAVMergingMgrCallback> callback) {
		m_callback = callback;

	}
	virtual void doMerge( std::shared_ptr<vmsAVMergingTask> task );
	virtual void addMergeTask( vmsDownloadSmartPtr videoDld, vmsDownloadSmartPtr audioDld );

protected:
	unsigned int nThreadCount;
	boost::asio::io_service ioService;
	std::shared_ptr<boost::asio::io_service::work> work;
	boost::thread_group threadPool;		
	std::shared_ptr<vmsAVMerger> m_spAVMerger;	
	std::shared_ptr<vmsAfterMergeAction> m_spAfterMergeAction;	
	std::vector<UINT> m_vVideoIds;
	std::shared_ptr <vmsAVMergingMgrCallback> m_callback;
};
#endif