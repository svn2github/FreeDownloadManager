/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsWinRect.h"
class vmsDialogTemplate
{
public:
	vmsDialogTemplate (const std::wstring& title, const vmsWinRect& rect, DWORD style)
	{
		m_data.resize (16*1024);

		LPDLGTEMPLATEW ptempl = *this;

		ptempl->style = DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU;

		ptempl->cdit = 0;         

		ptempl->x = (short)rect.left;  
		ptempl->y = (short)rect.top;
		ptempl->cx = (short)rect.Width (); 
		ptempl->cy = (short)rect.Height ();

		LPWORD pw = reinterpret_cast <LPWORD> (ptempl + 1);
		assert (pw == ptr_align (pw, 2));
		*pw++ = 0;             
		*pw++ = 0;             

		if (!title.empty ())
		{
			memcpy (pw, &title.front (), title.size () * sizeof (wchar_t));
			pw += title.size ();
		}
		*pw++ = 0;

		ptempl->style |= DS_SETFONT;
		*pw++ = 8; 
		const std::wstring font_title = L"MS Shell Dlg";
		memcpy (pw, &font_title.front (), font_title.size () * sizeof (wchar_t));
		pw += font_title.size ();
		*pw++ = 0;

		m_current_position = ptr_align (pw);
	}

	enum control {button, edit, static_control, list_box, scroll_bar, combo_box, check_box};

	void add_control (const std::wstring& title, const vmsWinRect& rect, WORD id, control ctrl, DWORD style, DWORD ex_style = 0)
	{
		auto ptempl = reinterpret_cast <LPDLGITEMTEMPLATEW> (m_current_position);

		ptempl->style = style | control_class_style_correction (ctrl);
		ptempl->dwExtendedStyle = ex_style;

		ptempl->x = (short)rect.left;
		ptempl->y = (short)rect.top;
		ptempl->cx = (short)rect.Width ();
		ptempl->cy = (short)rect.Height ();

		ptempl->id = id;

		LPWORD pw = reinterpret_cast <LPWORD> (ptempl + 1);
		*pw++ = 0xFFFF;
		*pw++ = control_class_id (ctrl);

		if (!title.empty ())
		{
			memcpy (pw, &title.front (), title.size () * sizeof (wchar_t));
			pw += title.size ();
		}
		*pw++ = 0;
		
		*pw++ = 0;             

		m_current_position = ptr_align (pw);

		static_cast <LPDLGTEMPLATEW> (*this)->cdit++; 
	}

	operator LPDLGTEMPLATEW () {
		return reinterpret_cast <LPDLGTEMPLATEW> (&m_data.front ());
	}

	operator LPCDLGTEMPLATEW () const {
		return *this;
	}

protected:
	std::vector <BYTE> m_data;
	LPWORD m_current_position;

protected:
	template <class T> 
	static T ptr_align (T lpIn, ULONG dw2Power = 4)
	{
		ULONG_PTR ul = (ULONG_PTR)lpIn;
		ul += dw2Power - 1;
		ul &= ~(dw2Power - 1);
		return (T)ul;
	}

	static WORD control_class_id (control ctrl)
	{
		switch (ctrl)
		{
		case button:
			return 0x0080;

		case edit:
			return 0x0081;

		case static_control:
			return 0x0082;

		case list_box:
			return 0x0083;

		case scroll_bar:
			return 0x0084;

		case combo_box:
			return 0x0085;

		case check_box:
			return 0x0080; 

		default:
			assert (!"implemented");
			throw 0;
		}
	}

	static DWORD control_class_style_correction (control ctrl)
	{
		return ctrl == check_box ? BS_CHECKBOX | BS_AUTOCHECKBOX : 0;
	}
};