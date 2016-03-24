/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskResponse.h"
class vmsFdmBhIpcUiStringsTaskResponse :
	public vmsFdmBhIpcTaskResponse
{
public:
	std::wstring m_dllink, m_dlall, m_dlselected,
		m_dlvideo, m_dlbyfdm, m_showthisbtn,
		m_sniffModuleNotLoaded, m_fdmCallFailed,
		m_noFlashVideoFound, m_flvWebPageNotFound;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		auto node = pStm->SelectOrCreateNode (L"strings");
		assert (node);
		if (!node)
			return false;
		return node->SerializeValue (L"dllink", m_dllink) &&
			node->SerializeValue (L"dlall", m_dlall) &&
			node->SerializeValue (L"dlselected", m_dlselected) &&
			node->SerializeValue (L"dlvideo", m_dlvideo) &&
			node->SerializeValue (L"dlbyfdm", m_dlbyfdm) &&
			node->SerializeValue (L"showthisbtn", m_showthisbtn) &&
			node->SerializeValue (L"sniffModuleNotLoaded", m_sniffModuleNotLoaded) &&
			node->SerializeValue (L"fdmCallFailed", m_fdmCallFailed) &&
			node->SerializeValue (L"noFlashVideoFound", m_noFlashVideoFound) &&
			node->SerializeValue (L"flvWebPageNotFound", m_flvWebPageNotFound) &&
			vmsFdmBhIpcTaskResponse::Serialize (pStm, flags);
	}
};

