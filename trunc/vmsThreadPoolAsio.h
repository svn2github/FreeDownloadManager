/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSTHREADPOOLASIOH
#define VMSTHREADPOOLASIOH
#include "vmsThreadPool.h"
#include <boost\asio\io_service.hpp>
#include <boost\thread.hpp>

class vmsThreadPoolAsio : public vmsThreadPool{
public:
	vmsThreadPoolAsio(unsigned int _nThreadCount);
	void addTask( std::shared_ptr<vmsThreadPoolTask> task );
	void runTask( std::shared_ptr<vmsThreadPoolTask> task );
	virtual void stop();
	~vmsThreadPoolAsio();
protected:
	unsigned int nThreadCount;
	boost::asio::io_service ioService;
	std::shared_ptr<boost::asio::io_service::work> work;
	boost::thread_group threadPool;		
};
#endif