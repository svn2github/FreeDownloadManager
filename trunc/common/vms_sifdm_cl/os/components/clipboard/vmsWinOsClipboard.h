/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOsClipboardBase.h"
class vmsWinOsClipboard : 
	public vmsOsClipboardBase
{
public:
	vmsWinOsClipboard (HWND hwnd) : 
		vmsOsClipboardBase (hwnd) {}

	virtual bool text (const std::wstring& text) override
	{
		if (!OpenClipboard (m_windowHandle))
			return false;

		EmptyClipboard ();

		HANDLE hText = GlobalAlloc (GMEM_MOVEABLE, (text.length () + 1) * 2);
		if (!hText)
		{
			CloseClipboard ();
			return false;
		}

		LPWSTR pszText = (LPWSTR)GlobalLock (hText);
		wcscpy (pszText, text.c_str ());
		GlobalUnlock (hText);

		SetClipboardData (CF_UNICODETEXT, hText);

		CloseClipboard ();

		return true;
	}

	virtual std::wstring text () override
	{
		std::wstring result;

		if (!OpenClipboard (m_windowHandle))
			return result;

		HANDLE hMem = GetClipboardData (CF_UNICODETEXT);

		if (hMem == NULL)
		{
			CloseClipboard ();
			return result;
		}

		LPCWSTR pszText = (LPCWSTR) GlobalLock (hMem);
		result = pszText;
		GlobalUnlock (hMem);

		CloseClipboard ();

		return result;
	}
};