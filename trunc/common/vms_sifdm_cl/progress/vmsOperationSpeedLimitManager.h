/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsOperationSpeedLimitManager
{
public:
	
	
	
	virtual void onNeedOperation (uint32_t &amount, bool waitNonZeroAmount) = 0;
	
	
	virtual void onDoneOperation (uint32_t amountAcquired, uint32_t amountPerformed) = 0;
	virtual ~vmsOperationSpeedLimitManager () {}
};