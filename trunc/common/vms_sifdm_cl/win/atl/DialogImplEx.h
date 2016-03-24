/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
template <class T, class TBase = CWindow >
class ATL_NO_VTABLE CDialogImplEx : public ATL::CDialogImpl < T, TBase >
{
public:
	INT_PTR DoModalIndirect (LPCDLGTEMPLATE pDialogTemplate, HWND hWndParent = ::GetActiveWindow (), LPARAM dwInitParam = NULL)
	{
		ATLASSERT (m_hWnd == NULL);
		_AtlWinModule.AddCreateWndData (&m_thunk.cd, (CDialogImplBaseT< TBase >*)this);
#ifdef _DEBUG
		m_bModal = true;
#endif 
		return ::DialogBoxIndirectParam (_AtlBaseModule.GetResourceInstance (), pDialogTemplate,
			hWndParent, T::StartDialogProc, dwInitParam);
	}
};