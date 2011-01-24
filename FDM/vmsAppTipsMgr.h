/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSAPPTIPSMGR_H__76294552_6C8D_47F7_9B6A_5AC9E8F1C6B7__INCLUDED_)
#define AFX_VMSAPPTIPSMGR_H__76294552_6C8D_47F7_9B6A_5AC9E8F1C6B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsAppSmallTipsMgr  
{
public:
	void SetupCurrentTipIndex();
	bool UpdateTipsFile();
	static vmsAppSmallTipsMgr& o ();
	tstring getTip ();
	vmsAppSmallTipsMgr();
	virtual ~vmsAppSmallTipsMgr();

protected:
	CRITICAL_SECTION m_csTipAcc;
	int m_nCurrentTip;
	void Load ();
	std::vector <tstring> m_vTips;

};

#endif 
