/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsErrorSource.h"
class vmsErrorSourceBinaryData : 
	public vmsErrorSource
{
public:
	vmsErrorSourceBinaryData () {}

public:
	std::vector <unsigned char> m_source;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		assert (pStm->isOutputStream ());
		if (!pStm->isOutputStream ())
			return false; 
		std::string data;
		if (!m_source.empty ())
			data = GentlePercentEncode (&m_source.front (), m_source.size ());
		std::wstring wdata (data.begin (), data.end ());
		std::wstring object = L"binary_data";
		return pStm->SerializeValue (L"object", object) && 
			pStm->SerializeValue (L"value", wdata, false);
	}
};