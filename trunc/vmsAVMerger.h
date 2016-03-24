/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSAVMERGER
#define VMSAVMERGER
#include <string>
#include <boost\thread\recursive_mutex.hpp>
#include <boost\thread\lock_guard.hpp>

class vmsAVMerger{	
	boost::recursive_mutex m_mtxNeedStop;
public:
	vmsAVMerger(){
		m_bStop = FALSE;
	}
	virtual ~vmsAVMerger(){};
	virtual int merge( std::wstring videoFileName, std::wstring audioFileName,
		std::wstring outputFileName ) = 0;
	virtual void setNeedStop( bool needStop ){
		boost::lock_guard<boost::recursive_mutex> lock(m_mtxNeedStop);
		m_bStop = needStop;
	};
protected:
	BOOL m_bStop;
};
#endif