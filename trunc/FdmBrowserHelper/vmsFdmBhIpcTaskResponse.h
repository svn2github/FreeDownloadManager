/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "fdm_ipc_task_type.h"
class vmsFdmBhIpcTaskResponse :
	public vmsSerializable
{
public:
	virtual ~vmsFdmBhIpcTaskResponse () {}

public:
	std::wstring m_id;
	std::wstring m_type;
	std::wstring m_result; 
	std::wstring m_error; 

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"id", m_id) &&
			pStm->SerializeValue (L"type", m_type) &&
			pStm->SerializeValue (L"error", m_error) &&
			pStm->SerializeValue (L"result", m_result);
	}
};

