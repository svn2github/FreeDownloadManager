/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFlvSniffTaskProcessor.h"

vmsFlvSniffTaskProcessor::vmsFlvSniffTaskProcessor ()
{
}

vmsFlvSniffTaskProcessor::~vmsFlvSniffTaskProcessor ()
{
}

bool vmsFlvSniffTaskProcessor::process_task (
	const vmsFlvSniffTask& task,
	vmsFlvSniffTaskResponse &result)
{
	result.m_id = task.m_id;

	std::wstringstream wssResult, wssError, wssError2;

	HMODULE sniffDll = GetModuleHandle (L"flvsniff.dll");
	
	if (!sniffDll)
	{
		wssError << L"sniffer_not_loaded";
		if (task.m_callProc.m_name == L"IsVideoFlash")
			wssResult << L"0";
	}

	else if (task.m_callProc.m_name == L"IsVideoFlash")
	{
		typedef BOOL (WINAPI *FNIVF)(LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR);
		FNIVF pfn = (FNIVF)GetProcAddress (sniffDll, "isVideoFlash");
		if (pfn != NULL)
		{
			wssResult << pfn (utf8FromWide (task.m_callProc.m_webPageUrl).c_str (),
				utf8FromWide (task.m_callProc.m_frameUrl).c_str (),
				utf8FromWide (task.m_callProc.m_swfUrl).c_str (),
				utf8FromWide (task.m_callProc.m_flashVars).c_str (),
				utf8FromWide (task.m_callProc.m_otherSwfUrls).c_str (),
				utf8FromWide (task.m_callProc.m_otherFlashVars).c_str ()) ? L"1" : L"0";
		}
	}

	else if (task.m_callProc.m_name == L"CreateVideoDownloadFromUrl")
	{
		typedef HRESULT (WINAPI *FNBTNCLCKD)(LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR);
		FNBTNCLCKD pfn = (FNBTNCLCKD)GetProcAddress (sniffDll, "onGetItBtnClicked3");
		if (pfn != NULL)
		{
			HRESULT hr = pfn (utf8FromWide (task.m_callProc.m_webPageUrl).c_str (),
				utf8FromWide (task.m_callProc.m_frameUrl).c_str (),
				utf8FromWide (task.m_callProc.m_swfUrl).c_str (),
				utf8FromWide (task.m_callProc.m_flashVars).c_str (),
				utf8FromWide (task.m_callProc.m_otherSwfUrls).c_str (),
				utf8FromWide (task.m_callProc.m_otherFlashVars).c_str ());

			if (FAILED (hr))
			{
				if (hr == E_INVALIDARG)
				{
					wssError << L"url_not_found";
				}
				else
				{
					wssError << L"fdm_call_failed";
					wssError2 << std::hex << hr;
				}
			}
			else if (hr == S_FALSE)
			{
				wssResult << L"flv_not_found";
			}
		}
	}

	else if (task.m_callProc.m_name == L"NewRequestNotification")
	{
		typedef void (WINAPI *FNONHR)(LPCSTR, LPCSTR);
		FNONHR pfn = (FNONHR)GetProcAddress (sniffDll, "onNewHttpRequest");
		if (pfn)
		{
			pfn (utf8FromWide (task.m_callProc.m_url).c_str (),
				utf8FromWide (task.m_callProc.m_srcTabUrl).c_str ());
		}
	}

	else if (task.m_callProc.m_name == L"RequestResponseNotification")
	{
		typedef UINT (WINAPI *FNNHD)(LPCSTR, LPCSTR, LPCSTR);
		FNNHD pfn = (FNNHD)GetProcAddress (sniffDll, "onNewHttpDialog");
		if (pfn)
		{
			UINT id = pfn (utf8FromWide (task.m_callProc.m_url).c_str (),
				utf8FromWide (task.m_callProc.m_requestHeaders).c_str (),
				utf8FromWide (task.m_callProc.m_responseHeaders).c_str ());
			assert (!task.m_callProc.m_requestId.empty ());
			if (id != UINT_MAX)
				m_requestResponseIdMap [task.m_callProc.m_requestId] = id;
		}
	}

	else if (task.m_callProc.m_name == L"RequestResponseCompleteNotification")
	{
		typedef UINT (WINAPI *FNHDC)(UINT);
		FNHDC pfn = (FNHDC)GetProcAddress (sniffDll, "onHttpDialogClosed");
		if (pfn)
		{
			assert (!task.m_callProc.m_requestId.empty ());
			auto it = m_requestResponseIdMap.find (task.m_callProc.m_requestId);
			assert (it != m_requestResponseIdMap.end ());
			if (it != m_requestResponseIdMap.end ())
			{
				pfn (it->second);
				m_requestResponseIdMap.erase (it);
			}
		}
	}

	else
	{
		return false;
	}

	result.m_callProc.m_result = wssResult.str ();
	result.m_callProc.m_error = wssError.str ();
	result.m_callProc.m_error2 = wssError2.str ();

	return true;
}
