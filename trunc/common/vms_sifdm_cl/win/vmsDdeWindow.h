/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsDdeWindow :
	public CWindowImpl <vmsDdeWindow, CWindow, CFrameWinTraits>
{
public:
	using dde_data_process_fn = std::function <LRESULT (HWND, COPYDATASTRUCT*)>;
	using custom_message_process_fn = std::function <void (WPARAM, LPARAM)>;

public:
	vmsDdeWindow (dde_data_process_fn processFn,
		custom_message_process_fn cmpFn = {}) :
		m_processFn (processFn),
		m_customMessageProcessFn (cmpFn)
	{
	}

	virtual ~vmsDdeWindow ()
	{
	}

	BEGIN_MSG_MAP (vmsDdeWindow)
		MESSAGE_HANDLER (WM_COPYDATA, OnCopyData)
		MESSAGE_HANDLER (WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER (CustomMessage, OnCustomMessage)
	END_MSG_MAP ()

public:
	bool Create (const std::wstring &title)
	{
		return nullptr != CWindowImpl::Create (nullptr,
			nullptr, title.c_str ());
	}

	void postCustomMessage (WPARAM wp = 0, LPARAM lp = 0)
	{
		PostMessage (CustomMessage, wp, lp);
	}

protected:
	dde_data_process_fn m_processFn;
	custom_message_process_fn m_customMessageProcessFn;

protected:
	enum {CustomMessage = WM_APP};

protected:
	LRESULT OnCopyData (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return m_processFn ((HWND) wParam, (COPYDATASTRUCT*) lParam);
	}

	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PostQuitMessage (0);
		return 0;
	}

	LRESULT OnCustomMessage (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		assert (m_customMessageProcessFn);
		if (m_customMessageProcessFn)
			m_customMessageProcessFn (wParam, lParam);
		return 0;
	}
};

