/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "vmsGetItButton.h"
#include <commctrl.h>
#include "resource.h"        

HIMAGELIST vmsGetItButton::m_hImgs = NULL;

vmsGetItButton::vmsGetItButton()
{
	m_hWnd = NULL;
	m_enBR = NONE;
	m_hrLast = S_OK;

	Initialize ();
}

vmsGetItButton::~vmsGetItButton()
{
	if (m_hWnd)
		DestroyWindow (m_hWnd);
}

void vmsGetItButton::Initialize()
{
	static bool _bInitialized = false;

	if (_bInitialized)
		return;

	_bInitialized = true;

	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof (wc);
	wc.lpfnWndProc = _TBParentWndProc;
	wc.hInstance = _Module.GetModuleInstance ();
	wc.lpszClassName = _T ("{E93F4E35-5841-41e7-AFF6-F456BBAE488B}");

	RegisterClassEx (&wc);

	m_hImgs = ImageList_Create (16, 16, ILC_COLOR24 | ILC_MASK, 1, 1);
	HICON hIco = LoadIcon (_Module.GetModuleInstance (), MAKEINTRESOURCE (IDI_FDM));
	ImageList_AddIcon (m_hImgs, hIco);
}

LRESULT CALLBACK vmsGetItButton::_TBParentWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_TIMER)
	{
		
		HWND hwndParent = GetParent (hwnd);
		RECT rc;
		GetWindowRect (hwndParent, &rc);
		POINT pt; GetCursorPos (&pt);
		if (PtInRect (&rc, pt) == FALSE)
			PostMessage (hwnd, WM_CLOSE, 0, 0);
	}

	else if (msg == WM_COMMAND)
	{
		vmsGetItButton *pthis = (vmsGetItButton*)GetWindowLong (hwnd, GWL_USERDATA);
		PostMessage (hwnd, WM_CLOSE, 0, 0);
		pthis->onClick ();
	}

	return DefWindowProc (hwnd, msg, wp, lp);	
}

void vmsGetItButton::Create(HWND hwndParent)
{
	if (m_hWnd)
		return;

	POINT pt; GetCursorPos (&pt);
	ScreenToClient (hwndParent, &pt);

	m_hWnd = CreateWindowEx (0, _T ("{E93F4E35-5841-41e7-AFF6-F456BBAE488B}"), NULL, 
		WS_CHILD | WS_VISIBLE, pt.x, pt.y, 16+7, 16+7, hwndParent, NULL, 
		_Module.GetModuleInstance (), NULL);

	HWND hwndTB = CreateWindow (TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT |
		TBSTYLE_TRANSPARENT | TBSTYLE_LIST | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN, 0, 0, 16+70, 16+7, 
		m_hWnd, NULL, _Module.GetModuleInstance (), NULL);

	SetWindowLongPtr (hwndTB, GWL_USERDATA, (LONG_PTR)this);
	SetWindowLongPtr (m_hWnd, GWL_USERDATA, (LONG_PTR)this);

	SendMessage (hwndTB, TB_BUTTONSTRUCTSIZE, sizeof (TBBUTTON), 0);

	SendMessage (hwndTB, TB_ADDSTRING, 0, (LPARAM)_T ("Get It"));

	SendMessage (hwndTB, TB_SETBITMAPSIZE, 0, MAKELONG (16, 16));
	SendMessage (hwndTB, TB_SETIMAGELIST, 0, (LPARAM)m_hImgs);
	
	TBBUTTON btn = {0};
	btn.iBitmap = 0;
	btn.idCommand = 100;
	btn.fsState = TBSTATE_ENABLED;
	btn.fsStyle = TBSTYLE_BUTTON;
	btn.dwData = 0;
	btn.iString = 0;
	SendMessage (hwndTB, TB_ADDBUTTONS, 1, (LPARAM)&btn);

	RECT rc;
	SendMessage (hwndTB, TB_GETRECT, btn.idCommand, (LPARAM)&rc);

	SetWindowPos (m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
	SetWindowPos (hwndTB, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	HRGN hRgn = CreateRoundRectRgn (0, 0, rc.right - rc.left + 1, rc.bottom - rc.top + 1, 3, 3);
	SetWindowRgn (m_hWnd, hRgn, TRUE);

	SetTimer (m_hWnd, 1, 100, NULL);
}  

void vmsGetItButton::onClick()
{
	HMODULE hDll = GetModuleHandle ("flvsniff.dll");
	if (!hDll)
	{
		m_enBR = E_SNIFF_MODULE_NOT_FOUND;
		return;
	}
	typedef HRESULT (WINAPI *FNBTNCLCKD)(LPCSTR);
	FNBTNCLCKD pfn = (FNBTNCLCKD) GetProcAddress (hDll, "onGetItBtnClicked");
	if (pfn != NULL)
	{
		HRESULT hr = pfn (m_strHtmlPageUrl.c_str ());
		if (FAILED (hr))
		{
			if (hr == E_INVALIDARG)
				m_enBR = E_URL_NOT_FOUND;
			else
				m_enBR = E_FAILED;
			m_hrLast = hr;
		}
		else if (hr == S_FALSE)
		{
			m_enBR = E_NO_FLV_FOUND;
		}
	}
}

vmsGetItButton::ButtonResult vmsGetItButton::getResult() const
{
	return m_enBR;
}

HRESULT vmsGetItButton::getHResult() const
{
	return m_hrLast;
}

HWND vmsGetItButton::getHWND() const
{
	return m_hWnd;
}
