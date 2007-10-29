/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsMediaConvertMgr.h"
#include "WaitDlg.h"
#include "vmsMediaConverter.h"
#include "Dlg_Convert.h"
#include "mfchelp.h"
#include "MyMessageBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsMediaConvertMgr::vmsMediaConvertMgr()
{

}

vmsMediaConvertMgr::~vmsMediaConvertMgr()
{

}

void vmsMediaConvertMgr::AddTask(vmsDownloadSmartPtr dld, const vmsMediaFileConvertSettings &stgs)
{
	vmsConvertMediaFileContext ctx;
	ctx.dld = dld;
	ctx.stgs = stgs;
	m_vTasks.push_back (ctx);
}

BOOL vmsMediaConvertMgr::SaveState()
{
	CString strFile = fsGetDataFilePath ("mctasks.sav");
	HANDLE hFile = CreateFile (strFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_HIDDEN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;

	fsMcMgrFileHdr hdr;
	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		goto _lErr;

	size_t i; i = m_vTasks.size ();
	if (FALSE == WriteFile (hFile, &i, sizeof (i), &dw, NULL))
		goto _lErr;

	for (i = 0; i < m_vTasks.size (); i++)
	{
		vmsConvertMediaFileContext *ctx = &m_vTasks [i];

		if (FALSE == WriteFile (hFile, &ctx->dld->nID, sizeof (UINT), &dw, NULL))
			goto _lErr;

		if (FALSE == fsSaveStrToFile (ctx->stgs.strFormat, hFile))
			goto _lErr;

		if (FALSE == fsSaveStrToFile (ctx->stgs.strExtension, hFile))
			goto _lErr;

		if (FALSE == fsSaveStrToFile (ctx->stgs.strAudioCodec, hFile))
			goto _lErr;

		if (FALSE == fsSaveStrToFile (ctx->stgs.strVideoCodec, hFile))
			goto _lErr;

		if (FALSE == WriteFile (hFile, &ctx->stgs.nAudioBitrate, sizeof (int), &dw, NULL))
			goto _lErr;

		if (FALSE == WriteFile (hFile, &ctx->stgs.nVideoBitrate, sizeof (int), &dw, NULL))
			goto _lErr;

		if (FALSE == WriteFile (hFile, &ctx->stgs.sizeVideo, sizeof (CSize), &dw, NULL))
			goto _lErr;		
	}

	CloseHandle (hFile);

	return TRUE;

_lErr:
	CloseHandle (hFile);
	DeleteFile (strFile);
	return FALSE;
}

BOOL vmsMediaConvertMgr::LoadState()
{
	CString strFile = fsGetDataFilePath ("mctasks.sav");
	HANDLE hFile = CreateFile (strFile, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_HIDDEN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return GetFileAttributes (strFile) == DWORD (-1);

	DWORD dw;

	fsMcMgrFileHdr hdr;
	if (FALSE == ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		goto _lErr;

	if (lstrcmp (hdr.szSig, MCMGRFILE_SIG) || hdr.wVer > MCMGRFILE_CURRENT_VERSION)
		goto _lErr;

	size_t i;
	if (FALSE == ReadFile (hFile, &i, sizeof (i), &dw, NULL))
		goto _lErr;

	m_vTasks.clear ();

	while (i--)
	{
		vmsConvertMediaFileContext ctx;

		UINT nId;
		if (FALSE == ReadFile (hFile, &nId, sizeof (UINT), &dw, NULL))
			goto _lErr;

		if (FALSE == fsReadStrFromFile (&ctx.stgs.strFormat.pszString, hFile))
			goto _lErr;

		if (FALSE == fsReadStrFromFile (&ctx.stgs.strExtension.pszString, hFile))
			goto _lErr;

		if (FALSE == fsReadStrFromFile (&ctx.stgs.strAudioCodec.pszString, hFile))
			goto _lErr;

		if (FALSE == fsReadStrFromFile (&ctx.stgs.strVideoCodec.pszString, hFile))
			goto _lErr;

		if (FALSE == ReadFile (hFile, &ctx.stgs.nAudioBitrate, sizeof (int), &dw, NULL))
			goto _lErr;

		if (FALSE == ReadFile (hFile, &ctx.stgs.nVideoBitrate, sizeof (int), &dw, NULL))
			goto _lErr;

		if (FALSE == ReadFile (hFile, &ctx.stgs.sizeVideo, sizeof (CSize), &dw, NULL))
			goto _lErr;

		ctx.dld = _DldsMgr.GetDownloadByID (nId);
		if (ctx.dld != NULL)
			m_vTasks.push_back (ctx);
	}

	CloseHandle (hFile);

	return TRUE;

_lErr:
	CloseHandle (hFile);
	DeleteFile (strFile);
	return FALSE;
}

DWORD WINAPI vmsMediaConvertMgr::_threadConvertMediaFile(LPVOID lp)
{
	ProcWaitInfo *info = (ProcWaitInfo*) lp;

	vmsConvertMediaFileContext *pcmfc  = (vmsConvertMediaFileContext*) info->lpParam1;
	BOOL bDontDeleteSourceFile = (BOOL)info->lpParam2;

	CString strDst = pcmfc->dld->pMgr->get_OutputFilePathName ();
	strDst = strDst.Left (strDst.GetLength () - 4); 

	for (int i = 0; ; i++)
	{
		CString str;
		if (i)
			str.Format ("%s (%d).%s", strDst, i, pcmfc->stgs.strExtension);
		else
			str.Format ("%s.%s", strDst, pcmfc->stgs.strExtension);

		if (GetFileAttributes (str) == DWORD (-1))
		{
			strDst = str;
			break;
		}
	}
	
	_DldsMgr.AddEvent (pcmfc->dld, LS (L_CONVERTING), EDT_INQUIRY);

	if (vmsMediaConverter::ConvertMedia (pcmfc->dld->pMgr->get_OutputFilePathName (), 
			strDst, pcmfc->stgs.strFormat, pcmfc->stgs.strAudioCodec, -1, pcmfc->stgs.nAudioBitrate, -1, 
			pcmfc->stgs.strVideoCodec, pcmfc->stgs.nVideoBitrate, -1, pcmfc->stgs.sizeVideo.cx, pcmfc->stgs.sizeVideo.cy,
			&info->iProgress, &info->bNeedStop))
	{
		if (bDontDeleteSourceFile == FALSE)
		{
			pcmfc->dld->pMgr->DeleteFile ();

			fsDownload_Properties *dp = pcmfc->dld->pMgr->GetDownloadMgr ()->GetDP ();
			delete [] dp->pszFileName;
			dp->pszFileName = new char [strDst.GetLength () + 1];
			lstrcpy (dp->pszFileName, strDst);
		}
		else
		{
			CString strDst2 = strDst;
			strDst2.SetAt (strDst2.GetLength () - 3, 't');
			strDst2.SetAt (strDst2.GetLength () - 2, 'm');
			strDst2.SetAt (strDst2.GetLength () - 1, 'p');
			if (FALSE == MoveFile (strDst, strDst2))
				strDst2 = strDst;
			CString strFilter; 
			strFilter.Format ("%s files (*.%s)|*.%s||", pcmfc->stgs.strExtension,
				pcmfc->stgs.strExtension, pcmfc->stgs.strExtension);
			CFileDialog dlg (FALSE, pcmfc->stgs.strExtension, strDst, 
				OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter, AfxGetApp ()->m_pMainWnd);
			if (IDCANCEL == _DlgMgr.DoModal (&dlg))
			{
				DeleteFile (strDst2);
			}
			else
			{
				MoveFile (strDst2, strDst);

				info->bWaitDone = TRUE;

				CString strMsg;
				strMsg.Format (LS (L_CONVERTED_OK), strDst);
				UINT nRet = MyMessageBox (NULL, strMsg, LS (L_DONE), NULL, IDI_QUESTION, 
					LS (L_LAUNCH), LS (L_OPENFOLDER), "OK");

				if (nRet == IDC_BTN1)
				{
					ShellExecute (0, "open", strDst, NULL, NULL, SW_SHOW);
				}
				else if (nRet == IDC_BTN2)
				{
					CString strCmd;
					strCmd.Format ("/select,\"%s\"", strDst);
					ShellExecute (NULL, "open", "explorer.exe", strCmd, NULL, SW_SHOW);
				}
			}
		}

		_DldsMgr.AddEvent (pcmfc->dld, LS (L_DONE), EDT_RESPONSE_S);

		if (bDontDeleteSourceFile == FALSE)
			_DldsMgr.DownloadStateChanged (pcmfc->dld);
	}
	else
	{
		DeleteFile (strDst);
		_DldsMgr.AddEvent (pcmfc->dld, LS (L_FAILED), EDT_RESPONSE_E);
		info->bWaitDone = TRUE;
		CString strMsg;
		strMsg.Format (LS (L_CONVERTED_FAILED), strDst);
		MessageBox (NULL, strMsg, LS (L_FAILED), MB_ICONERROR); 
	}

	info->bWaitDone = TRUE;
	delete pcmfc;

	return 0;
}

void vmsMediaConvertMgr::ConvertMediaFile(vmsDownloadSmartPtr dld, vmsMediaConvertMgr_OptionsSource enOs)
{
	vmsConvertMediaFileContext *pcmfc = new vmsConvertMediaFileContext;

	if (enOs == MCM_OS_SEARCH_IN_MGR)
	{
		int nIndex = FindDownload (dld);
		if (nIndex == -1)
			return;

		*pcmfc = m_vTasks [nIndex];
		m_vTasks.erase (m_vTasks.begin () + nIndex);
	}
	else
	{
		pcmfc->dld = dld;
		GetDefaultSettings (pcmfc->stgs);

		if (enOs == MCM_OS_SHOW_OPTIONS_UI)
		{
			CDlg_Convert dlg (&pcmfc->stgs);
			if (IDOK != _DlgMgr.DoModal (&dlg))
			{
				delete pcmfc;
				return;
			}
		}
	}

	CWaitDlg *dlg = new CWaitDlg;
	dlg->StartWaiting (LS (L_CONVERTING), _threadConvertMediaFile, TRUE, pcmfc, 
		(LPVOID)(enOs == MCM_OS_SHOW_OPTIONS_UI), NULL, NULL, NULL, FALSE);
}

int vmsMediaConvertMgr::FindDownload(vmsDownloadSmartPtr dld)
{
	for (size_t i = 0; i < m_vTasks.size (); i++)
	{
		if (m_vTasks [i].dld->nID == dld->nID)
			return i;
	}

	return -1;
}

void vmsMediaConvertMgr::ConvertMediaFile_SetupDefaultOptions()
{
	vmsMediaFileConvertSettings stgs;
	GetDefaultSettings (stgs);
	CDlg_Convert dlg (&stgs);
	dlg.m_bCustomizingDefSettings = true;
	_DlgMgr.DoModal (&dlg);
}

void vmsMediaConvertMgr::GetDefaultSettings(vmsMediaFileConvertSettings &stgs)
{
	stgs.strFormat = _App.Convert_Format ();
	stgs.strExtension = _App.Convert_Extension ();
	stgs.sizeVideo = _App.Convert_VideoSize ();
	stgs.nAudioBitrate = _App.Convert_AudioBitrate ();
	stgs.nVideoBitrate = _App.Convert_VideoBitrate ();
	stgs.strAudioCodec = _App.Convert_AudioCodec ();
	stgs.strVideoCodec = _App.Convert_VideoCodec ();
}

void vmsMediaConvertMgr::SaveSettingsAsDefault(const vmsMediaFileConvertSettings &stgs)
{
	_App.Convert_AudioBitrate (stgs.nAudioBitrate);
	_App.Convert_Format (stgs.strFormat);
	_App.Convert_VideoBitrate (stgs.nVideoBitrate);
	_App.Convert_VideoSize (stgs.sizeVideo);
	_App.Convert_VideoCodec (stgs.strVideoCodec);
	_App.Convert_AudioCodec (stgs.strAudioCodec);
	_App.Convert_Extension (stgs.strExtension);
}

BOOL vmsMediaConvertMgr::ShowSettingsUI(vmsMediaFileConvertSettings &stgs)
{
	CDlg_Convert dlg (&stgs);
	return IDOK == _DlgMgr.DoModal (&dlg);
}
