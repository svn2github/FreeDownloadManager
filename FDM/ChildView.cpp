/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "FdmApp.h"
#include "ChildView.h"
#include "plugincmds.h"
#include "DownloadsWnd.h"

extern CDownloadsWnd* _pwndDownloads;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif    

extern fsPluginMgr _PluginMgr;

IMPLEMENT_DYNAMIC(CChildView, CWnd)

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}  

BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE (WGP_MENU_CMDFIRST, WGP_MENU_CMDLAST, OnPluginCommand)
	ON_UPDATE_COMMAND_UI_RANGE (WGP_MENU_CMDFIRST, WGP_MENU_VIEWSMPLITEM_CMDSTART-1, OnUpdatePluginCommand)
END_MESSAGE_MAP()        

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle = 0; 
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_BTNFACE+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); 
	
	CRect rc;
	GetClientRect (&rc);

	
	
}

void CChildView::OnSize(UINT, int cx, int cy) 
{
	m_wndClient.MoveWindow (0, 3, cx, cy-3, TRUE);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (FALSE == m_wndClient.Create (this))
		return -1;
	
	return 0;
}

void CChildView::OnPluginCommand(UINT uCmd) 
{
	m_wndClient.m_plugs2.SendPluginCommand (uCmd);
}

void CChildView::OnUpdatePluginCommand(CCmdUI *pCmdUI)
{
	
	int state = _PluginMgr.OnUpdateToolBar (pCmdUI->m_nID);

	pCmdUI->Enable ((state & WGP_CMDITEMSTATE_DISABLED) == 0);
	pCmdUI->SetCheck (state & WGP_CMDITEMSTATE_CHECKED ? 1 : 0);
}

void CChildView::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd ::OnSetFocus(pOldWnd);
	m_wndClient.SetFocus ();	
}    

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	
	
	
	
	if (_pwndDownloads->m_wndDownloads.m_info.m_video.m_wndVideo.m_player.Get_State () != VFPS_CLOSED)
	{
    		CRect rc;

		
		_pwndDownloads->m_wndDownloads.m_info.m_video.GetWindowRect(&rc);
		ScreenToClient(&rc);

		
		pDC->ExcludeClipRect(&rc);
	}
	
	return CWnd ::OnEraseBkgnd(pDC);
}
