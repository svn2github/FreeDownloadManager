/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsOperationProgressCallback
{
public:
	
	virtual bool onOperationProgressPercentage (double) = 0;
	virtual ~vmsOperationProgressCallback () {}
};