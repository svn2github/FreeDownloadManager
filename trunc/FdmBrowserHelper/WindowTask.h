/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

enum class WindowTaskType
{
	show,
	hide,
};

ENUM_STREAM_SUPPORT_BEGIN (WindowTaskType)
	ENUM_STREAM_SUPPORT_ENTRY (WindowTaskType::show, L"show")
	ENUM_STREAM_SUPPORT_ENTRY (WindowTaskType::hide, L"hide")
ENUM_STREAM_SUPPORT_END (WindowTaskType)

class WindowTask :
	public vmsSerializable
{
public:
	HWND m_window;
	WindowTaskType m_action;
	bool m_needWait = false; 

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		std::wstring str;
		if (pStm->isInputStream ())
		{
			if (!pStm->SerializeValue (L"windowHandle", str))
				return false;
			m_window = (HWND)wcstoll (str.c_str (), nullptr, 0);
		}
		else
		{
			assert (!"implemented");
			return false; 
		}
		return pStm->SerializeValueS (L"action", m_action) &&
			pStm->SerializeValueS (L"needWait", m_needWait, false);
	}
};
