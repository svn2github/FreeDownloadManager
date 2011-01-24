/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "fdm.h"
#include "vmsAppTipsMgr.h"
#include "mfchelp.h"
#include "vmsStringParser.h"
#include "vmsAppUtil.h"
#include "vmsSecurity.h"
#include "FdmApp.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsAppSmallTipsMgr::vmsAppSmallTipsMgr()
{
	m_nCurrentTip = -1;
	InitializeCriticalSection (&m_csTipAcc);
}

vmsAppSmallTipsMgr::~vmsAppSmallTipsMgr()
{
	DeleteCriticalSection (&m_csTipAcc);
}

tstring vmsAppSmallTipsMgr::getTip()
{
	if (m_vTips.empty ())
		return _T ("");
	EnterCriticalSection (&m_csTipAcc);
	if (m_nCurrentTip == -1)
		SetupCurrentTipIndex ();
	tstring tstr = m_vTips [m_nCurrentTip].c_str ();
	LeaveCriticalSection (&m_csTipAcc);
	return tstr;
}

vmsAppSmallTipsMgr& vmsAppSmallTipsMgr::o()
{
	static vmsAppSmallTipsMgr o;
	static _bLoaded = false;
	if (!_bLoaded)
	{
		_bLoaded = true;
		o.Load ();
	}
	return o;
}

void vmsAppSmallTipsMgr::Load()
{
	EnterCriticalSection (&m_csTipAcc);

	m_vTips.clear ();

	CString strFile = fsGetDataFilePath ("tips.dat");
	if (GetFileAttributes (strFile) == DWORD (-1))
	{
		strFile = fsGetProgramFilePath ("tips.dat");
		if (GetFileAttributes (strFile) == DWORD (-1))
		{
			LeaveCriticalSection (&m_csTipAcc);
			return;
		}
	}
	
	try
	{
	
	CStdioFile file (strFile, CFile::modeRead | CFile::typeText | CFile::shareDenyNone);

	CString str; 
	bool b1stLine = true;
	bool bForceShow = false;
	int iForceTipIndex = -1;
	while (file.ReadString (str) && str.IsEmpty () == FALSE)
	{
		if (b1stLine)
		{
			if (str.GetLength () >= 3 && BYTE (str [0]) == 0xEF && BYTE (str [1]) == 0xBB && BYTE (str [2]) == 0xBF)
				str.Delete (0, 3); 
			b1stLine = false;
			if (str.IsEmpty ())
				continue;
		}

		if (str [0] == ';')
			continue; 

		vmsUtf8ToAscii (str.GetBuffer (str.GetLength ()));
		str.ReleaseBuffer ();
		
		if (_tcsnicmp (str, _T ("ForceShow"), 9) == 0)
		{
			LPCTSTR ptsz = str;
			ptsz += 9;
			vmsStringParser::SkipWhiteChars (ptsz);
			if (*ptsz == '=')
			{
				ptsz++;
				vmsStringParser::SkipWhiteChars (ptsz);
				tstring tstr;
				vmsStringParser::GetWord (ptsz, tstr);
				if (!tstr.empty ())
				{
					CString strOld = _App.SmallTips_ForceShow ();
					if (strOld.CompareNoCase (tstr.c_str ()))
					{
						_App.SmallTips_Show (TRUE);
						_App.SmallTips_ForceShow (tstr.c_str ());
						bForceShow = true;
					}
				}
				continue;
			}
		}
		else if (_tcsnicmp (str, _T ("ForceTipIndex"), 13) == 0)
		{
			LPCTSTR ptsz = str;
			ptsz += 13;
			vmsStringParser::SkipWhiteChars (ptsz);
			if (*ptsz == '=')
			{
				ptsz++;
				vmsStringParser::SkipWhiteChars (ptsz);
				tstring tstr;
				vmsStringParser::GetWord (ptsz, tstr);
				if (!tstr.empty ())
					iForceTipIndex = _ttoi (tstr.c_str ());
				continue;
			}
		}

		m_vTips.push_back ((LPCTSTR)str);
	}

	if (bForceShow && iForceTipIndex != -1)
	{
		_App.SmallTips_CurrentTip (iForceTipIndex);
		FILETIME ft; ZeroMemory (&ft, sizeof (ft));
		_App.SmallTips_LastTime (ft);
	}  

	}catch (...) {}

	LeaveCriticalSection (&m_csTipAcc);
}

bool vmsAppSmallTipsMgr::UpdateTipsFile()
{
	CString strUrl;
	CString strLng = _LngMgr.GetLngFileNameWoExt (_LngMgr.GetCurLng ());
	strUrl.Format ("%stips/%s.dat", (LPCTSTR)_App.Update_URL (), (LPCTSTR)strLng);
	CString strFile = fsGetDataFilePath ("tips.bak");
	fsInternetResult ir = vmsAppUtil::UrlDownloadToFile (strUrl, strFile);
	if (ir != IR_SUCCESS && strLng.CompareNoCase ("eng") != 0)
	{
		strUrl = _App.Update_URL ();
		strUrl += "tips/eng.dat";
		ir = vmsAppUtil::UrlDownloadToFile (strUrl, strFile);
	}
	if (ir != IR_SUCCESS)
		return false;
	CString strKeyFile = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	strKeyFile += "sigkey.dat";
	if (GetFileAttributes (strKeyFile) == DWORD (-1))
		strKeyFile = fsGetProgramFilePath ("sigkey.dat");
	if (false == vmsSecurity::VerifySign (strFile, strKeyFile))
	{
		DeleteFile (strFile);
		return false;
	}
	if (false == vmsSecurity::ExtractFileFromSignedFile (strFile, fsGetDataFilePath ("tips.dat")))
	{
		ASSERT (false);
		return false; 
	}
	DeleteFile (strFile);
	BOOL b = _App.SmallTips_Show ();
	EnterCriticalSection (&m_csTipAcc);
	Load ();
	SetupCurrentTipIndex ();
	LeaveCriticalSection (&m_csTipAcc);
	if (b == FALSE && _App.SmallTips_Show ())
		((CMainFrame*)AfxGetApp ()->m_pMainWnd)->ApplyShowSmallTipsSetting ();
	return true;
}

void vmsAppSmallTipsMgr::SetupCurrentTipIndex()
{
	int nCurrentTip = _App.SmallTips_CurrentTip ();
	FILETIME ft = _App.SmallTips_LastTime ();
	FILETIME ftCurrent; SYSTEMTIME st;
	GetLocalTime (&st); SystemTimeToFileTime (&st, &ftCurrent);
	if (ft.dwHighDateTime == 0)
	{
		_App.SmallTips_LastTime (ftCurrent);
	}
	else
	{
		int nDelta = fsGetTimeDelta (&ftCurrent, &ft);
		if (nDelta > 24*3600)
		{
			nCurrentTip++;
			_App.SmallTips_CurrentTip (nCurrentTip);
			_App.SmallTips_LastTime (ftCurrent);
		}
	}
	if (nCurrentTip < 0 || nCurrentTip >= (int)m_vTips.size ())
	{
		nCurrentTip = 0;
		_App.SmallTips_CurrentTip (nCurrentTip);
	}
	m_nCurrentTip = nCurrentTip;
}
