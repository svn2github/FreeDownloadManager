/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsOperationProgressBase
{
public:
	typedef std::shared_ptr <vmsOperationProgressBase> tSP;

	vmsOperationProgressBase ()
	{

	}

	virtual ~vmsOperationProgressBase ()
	{

	}

	virtual double getProgressCurrentInPercentage () const = NULL;

	virtual bool CancelOperation ()
	{
		return false; 
	}
};

template <class TProgressValue>
class vmsOperationProgressBaseEx : public vmsOperationProgressBase
{
public:
	typedef std::shared_ptr <vmsOperationProgressBase> tSP;
	typedef TProgressValue tProgressValue;

public:
	virtual TProgressValue getProgressMin () const
	{
		return TProgressValue (0);
	}

	virtual TProgressValue getProgressMax () const
	{
		return TProgressValue (100);
	}

	virtual TProgressValue getProgressCurrent () const = NULL;

	
	virtual double getProgressCurrentInPercentage () const
	{
		assert (getProgressMax () != 0 && getProgressMin () != getProgressMax ());
		if (!getProgressMax () || (getProgressMax () == getProgressMin ()))
			return 0;
		return (double)(getProgressCurrent () - getProgressMin ()) / (double)(getProgressMax () - getProgressMin ()) * 100.0;
	}
};