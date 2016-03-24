/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include "../../win/atl/DialogImplEx.h"
#include "../../win/vmsDialogTemplate.h"
class CDlgUnhandledException :
	public CDialogImplEx < CDlgUnhandledException >
{
public:
	CDlgUnhandledException (const std::wstring& app_name,
		const std::wstring& productPartName,
		bool disableRestartAppOption) :
		m_bRestartApp (false),
		m_hBoldFont (NULL),
		m_app_name (app_name),
		m_productPartName (productPartName),
		m_disableRestartAppOption (disableRestartAppOption)
	{
	}

	~CDlgUnhandledException ()
	{
		if (m_hBoldFont)
			DeleteObject (m_hBoldFont);
	}

	INT_PTR DoModal (HWND hWndParent = ::GetActiveWindow (), LPARAM dwInitParam = NULL)
	{
		std::wstring str;

		str = m_app_name;
		str += L" - fatal error occurred";
		vmsDialogTemplate dlgtempl (str, vmsWinRect (0, 0, 325, 207),
			WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION);

		dlgtempl.add_control (L"Send report", vmsWinRect (188, 186, 188 + 64, 186 + 14), IDOK, vmsDialogTemplate::button,
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP);

		dlgtempl.add_control (L"", vmsWinRect (6, 7, 6 + 21, 7 + 20), IDC__UEICON, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_ICON);

		dlgtempl.add_control (L"Don't send", vmsWinRect (254, 186, 254 + 64, 186 + 14), IDCANCEL, vmsDialogTemplate::button,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP);

		str = m_app_name;
		if (!m_productPartName.empty ())
		{
			str += L" (";
			str += m_productPartName;
			str += L")";
		}
		str += L" - fatal error (bug) occurred. ";
		str += m_app_name; str += L" was terminated.";
		dlgtempl.add_control (str, vmsWinRect (36, 7, 36 + 282, 7 + 20), IDC__MSG1, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_LEFT);

		dlgtempl.add_control (L"This fatal error is the result of unforeseen work of the program. You can help us fix this error by reporting it.",
			vmsWinRect (36, 34, 36 + 282, 34 + 17), IDC__MSG2, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_LEFT);

		dlgtempl.add_control (L"Please report this error to us.", vmsWinRect (36, 57, 36 + 252, 57 + 9), IDC__MSG3, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_LEFT);

		dlgtempl.add_control (L"", vmsWinRect (36, 28, 36 + 281, 28 + 1), -1, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ);

		dlgtempl.add_control (L"Please provide description (if possible) of steps that leads to the bug (optional):",
			vmsWinRect (36, 70, 36 + 283, 70 + 16), IDC__MSG4, vmsDialogTemplate::static_control,
			WS_CHILD | WS_VISIBLE | SS_LEFT);

		dlgtempl.add_control (L"", vmsWinRect (36, 87, 36 + 282, 87 + 75), IDC_DESCRIPTION, vmsDialogTemplate::edit,
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP);

		str = L"Restart "; str += m_app_name; str += L" automatically";
		dlgtempl.add_control (str, vmsWinRect (36, 168, 36 + 283, 168 + 9), IDC_RESTART_APP, vmsDialogTemplate::check_box,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP);

		return DoModalIndirect (dlgtempl, hWndParent, dwInitParam);
	}

	BEGIN_MSG_MAP (CDlgUnhandledException)
		MESSAGE_HANDLER (WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER (WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER (IDOK, OnOKCancel)
		COMMAND_ID_HANDLER (IDCANCEL, OnOKCancel)
		MESSAGE_HANDLER (WM_CTLCOLORSTATIC, OnCtlColor)
	END_MSG_MAP ()

protected:
	enum ids { IDC__UEICON = 1000, IDC__MSG1, IDC__MSG2, IDC__MSG3, IDC__MSG4, IDC_DESCRIPTION, IDC_RESTART_APP };

public:
	LRESULT OnInitDialog (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::SendMessage (GetDlgItem (IDC__UEICON), STM_SETICON,
			(WPARAM)LoadIcon (NULL, IDI_ERROR), 0L);

		LOGFONT lf;
		HFONT hFont = (HFONT)SendMessage (WM_GETFONT);
		::GetObject (hFont, sizeof (LOGFONT), &lf);
		lf.lfWeight = FW_BOLD;
		m_hBoldFont = CreateFontIndirect (&lf);

		if (!m_disableRestartAppOption)
			CheckDlgButton (IDC_RESTART_APP, BST_CHECKED);
		else
			GetDlgItem (IDC_RESTART_APP).EnableWindow (FALSE);

		::SetFocus (GetDlgItem (IDC_DESCRIPTION));

		CenterWindow ();

		return FALSE;
	}

	LRESULT OnCtlColor (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HWND hwndControl = (HWND)lParam;
		if (hwndControl == GetDlgItem (IDC__MSG1) || hwndControl == GetDlgItem (IDC__MSG3))
		{
			SelectObject ((HDC)wParam, m_hBoldFont);
		}

		return NULL;
	}

	LRESULT OnClose (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog (IDCANCEL);
		return 0;
	}

	LRESULT OnOKCancel (WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_bRestartApp = !m_disableRestartAppOption &&
			IsDlgButtonChecked (IDC_RESTART_APP) == BST_CHECKED;
		BSTR bstr = NULL;
		GetDlgItemText (IDC_DESCRIPTION, bstr);
		if (bstr)
		{
			m_wstrDescription = bstr;
			SysFreeString (bstr);
		}
		EndDialog (wID);
		return 0;
	}

public:
	bool m_bRestartApp;
	std::wstring m_wstrDescription;

protected:
	HFONT m_hBoldFont;
	std::wstring m_app_name;
	bool m_disableRestartAppOption;
	std::wstring m_productPartName;
};