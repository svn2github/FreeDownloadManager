/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBrowserMenuSettings :
	public vmsSerializable
{
public:
	vmsFdmBrowserMenuSettings ()
	{
	}

	~vmsFdmBrowserMenuSettings ()
	{
	}

public:
	bool m_dllink = true, 
		m_dlall = true, 
		m_dlselected = true, 
		m_dlvideo = true;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValueS (L"dllink", m_dllink) &&
			pStm->SerializeValueS (L"dlall", m_dlall) &&
			pStm->SerializeValueS (L"dlselected", m_dlselected) &&
			pStm->SerializeValueS (L"dlvideo", m_dlvideo);
	}
};

