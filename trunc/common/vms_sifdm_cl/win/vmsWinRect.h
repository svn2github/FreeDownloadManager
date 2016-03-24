/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsWinSize.h"

class vmsWinRect : public RECT
{
public:
	vmsWinRect (int l = 0, int t = 0, int r = 0, int b = 0)
	{
		top = t;
		left = l;
		right = r;
		bottom = b;
	}

	vmsWinRect (const RECT& rc)
	{
		top = rc.top;
		left = rc.left;
		right = rc.right;
		bottom = rc.bottom;
	}

	int Width () const 
	{
		return right - left;
	}

	int Height () const 
	{
		return bottom - top;
	}

	void SetRect (int l, int t, int r, int b)
	{
		top = t;
		left = l;
		right = r;
		bottom = b;
	}

	vmsWinRect& operator = (const RECT& rc)
	{
		left = rc.left;
		right = rc.right;
		top = rc.top;
		bottom = rc.bottom;
		return *this;
	}

	vmsWinSize Dimensions ()  const
	{
		return vmsWinSize (Width (), Height ());
	}
};