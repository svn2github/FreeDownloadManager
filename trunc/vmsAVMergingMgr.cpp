/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsAVMergingMgr.h"

vmsAVMergingMgr::vmsAVMergingMgr(std::shared_ptr<vmsAVMerger> spAVMerger,
	std::shared_ptr<vmsAfterMergeAction> spAfterMergeAction,
	unsigned int _nThreadCount )
	:m_spAVMerger(spAVMerger), m_spAfterMergeAction( spAfterMergeAction )
{
	nThreadCount = _nThreadCount;
	work = std::make_shared<boost::asio::io_service::work>( ioService );
	for ( int i = 0; i < nThreadCount; i++ )
		threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
}

vmsAVMergingMgr::~vmsAVMergingMgr(){
	if ( m_spAVMerger )
		m_spAVMerger->setNeedStop( true );
	ioService.stop();
	threadPool.join_all();
}

void vmsAVMergingMgr::doMerge( std::shared_ptr<vmsAVMergingTask> task ){		
	if ( task && m_spAVMerger ){
		
		auto dld = _DldsMgr.GetDownloadByID (task->getVideoId ());
		assert (dld);
		if (dld)
			_DldsMgr.onPostDownloadTaskStarted (dld, avmergePostDownloadTaskName);
		
		if (m_callback)
			m_callback->task_started (task);
		int nMergeResult = m_spAVMerger->merge( task->getVideoFileName(), task->getAudioFileName(), task->getOutputFileName() );
		if ( m_spAfterMergeAction ){
			m_spAfterMergeAction->doAfterMerge( task, nMergeResult );
		}
		if (m_callback)
			m_callback->task_finished (task, nMergeResult);
	}
}

void vmsAVMergingMgr::addMergeTask( vmsDownloadSmartPtr videoDld, vmsDownloadSmartPtr audioDld ){
	if ( !videoDld || !audioDld )
		return;
	auto foundIdPos = std::find(m_vVideoIds.begin(), m_vVideoIds.end(), videoDld->nID);
	if ( foundIdPos != m_vVideoIds.end() )
		return;
	m_vVideoIds.push_back( videoDld->nID );

	std::wstring videoFileName = videoDld->pMgr->get_OutputFilePathName ();		
	std::wstring audioFileName = audioDld->pMgr->get_OutputFilePathName ();
	try{
		if ( !boost::filesystem::exists( videoFileName ) )
			return;
		if ( !boost::filesystem::exists( audioFileName ) )
			return;
	}
	catch( const boost::filesystem::filesystem_error& e ){
		return;
	}
	
	boost::system::error_code err;
	boost::filesystem::path tempPath = boost::filesystem::temp_directory_path(err);	
	if(err)
	{
		return;
	}		
	std::wstring name;
	auto pos = videoFileName.find_last_of(L"\\");
	if ( ( pos != tstring::npos ) && ( pos + 1 < videoFileName.size() ) ){		
		name = videoFileName.substr( pos + 1 );
	}
	tempPath = tempPath / name;	 		
	std::shared_ptr<vmsAVMergingTask> task = std::make_shared<vmsAVMergingTask>(
		videoDld->nID, audioDld->nID, videoFileName, audioFileName, tempPath.string<std::wstring>());

	_DldsMgr.LockList (false);
	if (videoDld->postDownloadTasks.find (avmergePostDownloadTaskName) == videoDld->postDownloadTasks.end ())
	{
		std::unique_ptr <vmsPostDownloadTask> pdt (new vmsPostDownloadTask ());
		videoDld->postDownloadTasks [avmergePostDownloadTaskName] = std::move (pdt);
		_DldsMgr.onPostDownloadTaskCreated (videoDld, avmergePostDownloadTaskName);
	}
	_DldsMgr.UnlockList (false);
	
	if (m_callback)
		m_callback->task_created (task);
	ioService.post( boost::bind( &vmsAVMergingMgr::doMerge, this, task ) );
}

