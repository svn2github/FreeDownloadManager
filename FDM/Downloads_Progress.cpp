/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "data stretcher.h"
#include "Downloads_Progress.h"
#include "DownloadsWnd.h"
#include "fsDownloadMgr.h"
#include "fsInternetDownloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

CDownloads_Progress::CDownloads_Progress()
{
	m_pActiveDownload = NULL;
}

CDownloads_Progress::~CDownloads_Progress()
{
}  

BEGIN_MESSAGE_MAP(CDownloads_Progress, CWnd)
	//{{AFX_MSG_MAP(CDownloads_Progress)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()        

BOOL CDownloads_Progress::Create(CWnd *pParent)
{
	CRect rc (0, 0, 50, 50);

	if (FALSE == CWnd::Create (AfxRegisterWndClass (0, LoadCursor (NULL, IDC_ARROW), (HBRUSH) (COLOR_WINDOW+1), NULL) ,
			NULL, WS_CHILD, rc, pParent, 0x121))
		return FALSE;

	m_brProgress.CreateSolidBrush (RGB (255, 255, 255));
	m_brDone.CreateSolidBrush (RGB (158, 208, 235));
	m_penQ.CreatePen (PS_SOLID, 1, RGB (0, 115, 170));

	return TRUE;
}

void CDownloads_Progress::OnPaint() 
{
	CPaintDC dc(this); 

	m_vAlreadyDraw.clear ();	
	DrawProgress (&dc);
}

void CDownloads_Progress::DrawProgress(CDC *dc)
{
	if (m_pActiveDownload == NULL) 
		return;

	if (IsWindowVisible () == FALSE)
		return;

	UINT64 uFileSize = m_pActiveDownload->pMgr->GetLDFileSize ();

	if (uFileSize == 0 || uFileSize == _UI64_MAX)
		return;

	bool bDontUseAlreadyDraw = m_pActiveDownload->pMgr->IsBittorrent () != FALSE;

	
	std::vector <vmsSectionInfo> v;
	m_pActiveDownload->pMgr->GetSplittedSectionsList (v);
	for (size_t i = 0; i < v.size (); i++)
		DrawSectionProgress (dc, &v[i], i, uFileSize, bDontUseAlreadyDraw);
}

void CDownloads_Progress::SetActiveDownload(vmsDownloadSmartPtr dld)
{
	m_pActiveDownload = dld;
	Invalidate ();
}

void CDownloads_Progress::OnSize(UINT , int cx, int cy) 
{
	m_size.cx = cx;
	m_size.cy = cy;
	m_vAlreadyDraw.clear ();
	Invalidate (TRUE);
}

void CDownloads_Progress::DrawSectionProgress(CDC *dc, vmsSectionInfo *sect, int iSect, UINT64 uFileSize, bool bDontUseAlreadyDraw)
{
	const int sqDimX = 6;  
	const int sqDimY = 6;  
	const int sqDist = 1; 

	int csqx = (m_size.cx - sqDist) / (sqDimX + sqDist);	
	int csqy = (m_size.cy - sqDist) / (sqDimY + sqDist);	
	int csq = csqx * csqy;					

	double nsqs = double ((INT64)sect->uDStart) / (INT64)uFileSize * csq; 
	double nsqe = double ((INT64)sect->uDEnd) / (INT64)uFileSize * csq;  
	double nsqc = double ((INT64)sect->uDCurrent) / (INT64)uFileSize * csq;  

	int xStart = sqDist;	
	int yStart = sqDist;	

	CBrush* brold = dc->SelectObject (&m_brDone);
	CPen *penold = dc->SelectObject (&m_penQ);

	
	if (m_vAlreadyDraw.size () > (size_t)iSect)
		nsqs = (double) m_vAlreadyDraw [iSect]; 

	for (int i = 0; i < nsqe; i++)
	{
		if (i >= nsqs)	
		{
			if (i >= nsqc)
				dc->SelectObject (&m_brProgress);

			dc->Rectangle (xStart, yStart, xStart + sqDimX, yStart + sqDimY);
		}
		
		
		xStart += sqDimX + sqDist;
		if (xStart + sqDimX + sqDist >= m_size.cx)
		{
			
			xStart = sqDist;
			yStart += sqDimY + sqDist;
		}
	}

	
	if (bDontUseAlreadyDraw == false)
	{
		if (m_vAlreadyDraw.size () > (size_t)iSect)
			m_vAlreadyDraw [iSect] = (UINT) nsqc;	
		else
			m_vAlreadyDraw.push_back ((UINT) nsqc);	
	}

	dc->SelectObject (brold);
	dc->SelectObject (penold);
}

vmsDownloadSmartPtr CDownloads_Progress::GetActiveDownload()
{
	return m_pActiveDownload;
}

void CDownloads_Progress::UpdateDownload()
{
	CDC *dc = GetDC ();
	DrawProgress (dc);
	ReleaseDC (dc);
}

void CDownloads_Progress::set_FullRedraw()
{
	m_vAlreadyDraw.clear ();
}  

