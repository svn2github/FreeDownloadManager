/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Data Stretcher.h"
#include "FDM.h"
#include "MainFrm.h"        

STDMETHODIMP CFDM::Shutdown(BOOL bAskUser)
{
	if (bAskUser)
		return E_NOTIMPL;

	AfxGetApp ()->m_pMainWnd->PostMessage (WM_COMMAND, ID_APP_EXIT_2);

	return S_OK;
}
