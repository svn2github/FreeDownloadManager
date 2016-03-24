/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsError.h"
class vmsErrorCodeProvider
{
public:
	
	
	virtual vmsError last_error () const = 0;
	virtual ~vmsErrorCodeProvider () {}
};