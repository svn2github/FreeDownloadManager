/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsThreadPoolAsio.h"
#include <boost\bind.hpp>

vmsThreadPoolAsio::vmsThreadPoolAsio( unsigned int _nThreadCount )	
{
	nThreadCount = _nThreadCount;
	work = std::make_shared<boost::asio::io_service::work>( ioService );
	for ( int i = 0; i < nThreadCount; i++ )
		threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
}

void vmsThreadPoolAsio::stop(){
	ioService.stop();
	threadPool.join_all();
}

vmsThreadPoolAsio::~vmsThreadPoolAsio(){	
	stop();
}

void vmsThreadPoolAsio::addTask( std::shared_ptr<vmsThreadPoolTask> task ){
	ioService.post( boost::bind( &vmsThreadPoolAsio::runTask, this, task ) );
}

void vmsThreadPoolAsio::runTask( std::shared_ptr<vmsThreadPoolTask> task ){
	task->run();
}