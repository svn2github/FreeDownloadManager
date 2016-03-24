/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsWinSize : public SIZE
{
public:
	vmsWinSize (int cx = 0, int cy = 0)
	{
		this->cx = cx;
		this->cy = cy;
	}

	vmsWinSize (const SIZE& s)
	{
		operator = (s);
	}

	vmsWinSize& operator = (const SIZE& s)
	{
		cx = s.cx; 
		cy = s.cy;
		return *this;
	}

	bool operator == (const vmsWinSize& s) const
	{
		return cx == s.cx && cy == s.cy;
	}

	bool operator != (const vmsWinSize& s) const
	{
		return !operator == (s);
	}
};