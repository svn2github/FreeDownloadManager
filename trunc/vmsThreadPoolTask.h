/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSTHREADPOOLTASKH
#define VMSTHREADPOOLTASKH

class vmsThreadPoolTask{
public:
	vmsThreadPoolTask(){};
	virtual int doWork() = 0;
	virtual std::wstring getMessage(){ return L""; }
	virtual ~vmsThreadPoolTask(){};
	virtual void run(){
		int result = doWork();
		std::wstring message = getMessage();
	}
};
#endif