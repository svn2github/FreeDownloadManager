/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "FdmApp.h"
#include "BtDldSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

IMPLEMENT_DYNAMIC(CBtDldSheet, CPropertySheet)

CBtDldSheet::CBtDldSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CBtDldSheet::CBtDldSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CBtDldSheet::~CBtDldSheet()
{
}  

BEGIN_MESSAGE_MAP(CBtDldSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CBtDldSheet)
		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

void CBtDldSheet::Init(DLDS_LIST *pvDlds, DWORD dwPages)
{
	m_general.m_pvDlds = m_misc.m_pvDlds = pvDlds;

	if (dwPages & BTDS_GENERAL_PAGE)
		AddPage (&m_general);

	if (dwPages & BTDS_MISC_PAGE)
		AddPage (&m_misc);
}

BOOL CBtDldSheet::IsNeedUpdateTasks()
{
	return m_general.m_bNeedUpdateTasks;
}
