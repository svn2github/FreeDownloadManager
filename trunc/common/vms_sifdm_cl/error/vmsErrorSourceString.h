/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsErrorSource.h"
class vmsErrorSourceString : public vmsErrorSource
{
public:
	std::wstring value () const {
		return m_source;
	}

	vmsErrorSourceString (const std::wstring& val = std::wstring ()) :
		m_source (val) {}

protected:
	std::wstring m_source;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		std::wstring object = L"string";
		return pStm->SerializeValue (L"object", object) && 
			pStm->SerializeValue (L"value", m_source);
	}
};