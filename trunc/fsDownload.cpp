/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "system.h"
#include "fsDownload.h"
#include "vmsDownloadMgrEx.h"
#include "vmsTpDownloadMgr.h"
#include "vmsBtDownloadManager.h"
#include "fsDownloadMgr.h"
#include "Utils.h"

void fsDownload::getObjectItselfStateBuffer(LPBYTE pb, LPDWORD pdwSize, bool bSaveToStorage)
	{
		DWORD dwNeedSize = 0;

		

		dwNeedSize += sizeof (DWORD); 
		dwNeedSize += sizeof (BYTE); 
		dwNeedSize += sizeof (bAutoStart);
		
		int iReserved = 0;
		dwNeedSize += sizeof (iReserved);
		dwNeedSize += sizeof (nID);
		dwNeedSize += strComment.GetLength () * sizeof(TCHAR) + sizeof (DWORD);
		dwNeedSize += sizeof (UINT);	
		dwNeedSize += sizeof (dwFlags);
		dwNeedSize += sizeof (dwReserved);
		dwNeedSize += sizeof (dateAdded);
		dwNeedSize += sizeof (dateCompleted);

		DWORD cEvents = vEvents.size ();
		if (_App.DontSaveLogs ())
			cEvents = 0;
		dwNeedSize += sizeof (DWORD);  

		DWORD i = 0;
		for (i = 0; i < cEvents; i++)
			dwNeedSize += 2*sizeof (COLORREF) + sizeof (FILETIME) + sizeof (int) + vEvents [i].strEvent.GetLength () * sizeof(TCHAR) + sizeof (DWORD);
		dwNeedSize += sizeof( youTubeDetails.mediaType );
		dwNeedSize += youTubeDetails.youTubeID.length() * sizeof(TCHAR) + sizeof (DWORD);
		dwNeedSize += youTubeDetails.format.length() * sizeof(TCHAR) + sizeof (DWORD);
		dwNeedSize += sizeof( youTubeDetails.fileSize );
		dwNeedSize += sizeof( youTubeDetails.isMerged );
		dwNeedSize += sizeof( youTubeDetails.isHidden );
		dwNeedSize += sizeof( youTubeDetails.is3D );
		dwNeedSize += youTubeDetails.originalURL.length() * sizeof(TCHAR) + sizeof (DWORD);

		if (pb == NULL) {
			*pdwSize = dwNeedSize;

			return;
		}

		if (*pdwSize < dwNeedSize) {
			*pdwSize = dwNeedSize;
			return;
		}

		LPBYTE pB = LPBYTE (pb);

		DWORD dwTmp = 0;
		CopyMemory (pB, &dwTmp, sizeof (dwTmp));
		pB += sizeof (dwTmp);

		BYTE btType = 0;
		if (pMgr->IsBittorrent()) {
			btType = 1;
		} else if (pMgr->IsTp()) {
			btType = 2;
		} else {
			btType = 0;
		}
		CopyMemory (pB, &btType, sizeof (btType));
		pB += sizeof (btType);

		CopyMemory (pB, &bAutoStart, sizeof (bAutoStart));
		pB += sizeof (bAutoStart);

		

		CopyMemory (pB, &iReserved, sizeof (iReserved));
		pB += sizeof (iReserved);

		CopyMemory (pB, &nID, sizeof (nID));
		pB += sizeof (nID);

		DWORD dw = strComment.GetLength ();
		CopyMemory (pB, &dw, sizeof (dw));
		pB += sizeof (dw);
		CopyMemory (pB, (const void*)(LPCTSTR)strComment, dw * sizeof(TCHAR));
		pB += dw * sizeof(TCHAR);

		CopyMemory (pB, &pGroup->nId, sizeof (UINT));
		pB += sizeof (UINT);

		CopyMemory (pB, &dwFlags, sizeof (dwFlags));
		pB += sizeof (dwFlags);

		CopyMemory (pB, &dwReserved, sizeof (dwReserved));
		pB += sizeof (dwReserved);

		CopyMemory (pB, &dateAdded, sizeof (dateAdded));
		pB += sizeof (dateAdded);

		CopyMemory (pB, &dateCompleted, sizeof (dateCompleted));
		pB += sizeof (dateCompleted);

		CopyMemory (pB, &cEvents, sizeof (DWORD));
		pB += sizeof (DWORD);

		

		for (i = 0; i < cEvents; i++)
		{
			fsDownloadEvents *ev = &vEvents [i];
		
			CopyMemory (pB, &ev->clrBg, sizeof (ev->clrBg));
			pB += sizeof (ev->clrBg);

			CopyMemory (pB, &ev->clrText, sizeof (ev->clrText));
			pB += sizeof (ev->clrText);

			CopyMemory (pB, &ev->timeEvent, sizeof (ev->timeEvent));
			pB += sizeof (ev->timeEvent);

			CopyMemory (pB, &ev->iImage, sizeof (ev->iImage));
			pB += sizeof (ev->iImage);

			dw = ev->strEvent.GetLength ();
			CopyMemory (pB, &dw, sizeof (dw));
			pB += sizeof (dw);
			CopyMemory (pB, (const void*)(LPCTSTR)vEvents [i].strEvent, dw * sizeof(TCHAR));
			pB += dw * sizeof(TCHAR);
		}
		CopyMemory (pB, &youTubeDetails.mediaType, sizeof (youTubeDetails.mediaType));
		pB += sizeof (youTubeDetails.mediaType);
				
		dw = youTubeDetails.youTubeID.length();
		CopyMemory (pB, &dw, sizeof (dw));
		pB += sizeof (dw);
		CopyMemory (pB, youTubeDetails.youTubeID.c_str(), dw * sizeof(TCHAR));
		pB += dw * sizeof(TCHAR);
		
		dw = youTubeDetails.format.length();
		CopyMemory (pB, &dw, sizeof (dw));
		pB += sizeof (dw);
		CopyMemory (pB, youTubeDetails.format.c_str(), dw * sizeof(TCHAR));
		pB += dw * sizeof(TCHAR);

		UINT64 fileSize = youTubeDetails.fileSize;
		CopyMemory (pB, &fileSize, sizeof (fileSize));
		pB += sizeof (fileSize);

		bool isMerged = youTubeDetails.isMerged;
		CopyMemory (pB, &isMerged, sizeof (isMerged));
		pB += sizeof (isMerged);

		bool isHidden = youTubeDetails.isHidden;
		CopyMemory (pB, &isHidden, sizeof (isHidden));
		pB += sizeof (isHidden);

		bool is3D = youTubeDetails.is3D;
		CopyMemory (pB, &is3D, sizeof (is3D));
		pB += sizeof (is3D);

		dw = youTubeDetails.originalURL.length();
		CopyMemory (pB, &dw, sizeof (dw));
		pB += sizeof (dw);
		CopyMemory (pB, youTubeDetails.originalURL.c_str(), dw * sizeof(TCHAR));
		pB += dw * sizeof(TCHAR);

		*pdwSize = dwNeedSize;
	}

	bool fsDownload::loadObjectItselfFromStateBuffer(LPBYTE pb, LPDWORD pdwSize, DWORD dwVer)
	{
#define CHECK_BOUNDS(need) if (need < 0 || need > int(*pdwSize) - (pB - LPBYTE (pb))) return false;
	
	LPBYTE pB = LPBYTE (pb);
	DWORD dw = *pdwSize;
	int iReserved;
	USES_CONVERSION;

	
	
	CHECK_BOUNDS (sizeof (DWORD));
	pB += sizeof (DWORD);

	if (pMgr == 0)
		return false;

	CHECK_BOUNDS (sizeof (BYTE));
	BYTE btType = *pB++;
	switch (btType) {
		case 0:
			{
				fsDownloadMgr* pdmMgr = new fsDownloadMgr();
				pMgr->Attach (pdmMgr);
			}
			break;
		case 1:
			{
				vmsBtDownloadManagerPtr spMgr; spMgr.CreateInstance ();
				pMgr->Attach (spMgr);
			}
			break;
		case 2:
			{
				vmsTpDownloadMgr* ptdmMgr = new vmsTpDownloadMgr();
				pMgr->Attach (ptdmMgr);
			}
			break;
		default:
		{
			assert (!"valid");
			return false;
		}
	}

	CHECK_BOUNDS (sizeof (bAutoStart));
	CopyMemory (&bAutoStart, pB, sizeof (bAutoStart));
	pB += sizeof (bAutoStart);

	CHECK_BOUNDS (sizeof (iReserved));
	CopyMemory (&iReserved, pB, sizeof (iReserved));
	pB += sizeof (iReserved);

	CHECK_BOUNDS (sizeof (nID));
	CopyMemory (&nID, pB, sizeof (nID));
	pB += sizeof (nID);

	CHECK_BOUNDS (sizeof (dw));
	CopyMemory (&dw, pB, sizeof (dw));
	if (dwVer > LAST_ANSI_DL_FILE_VERSION)
	{
		CHECK_BOUNDS (int (dw) * sizeof (WCHAR));
		LPWSTR szStr;
		pB += sizeof (dw); fsnew (szStr, WCHAR, dw+1);
		CopyMemory (szStr, pB, dw * sizeof(WCHAR));
		szStr [dw] = 0; strComment = szStr; delete [] szStr;
		pB += dw * sizeof(WCHAR);
	}
	else
	{
		CHECK_BOUNDS (int (dw));
		LPSTR szStr;
		pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
		CopyMemory (szStr, pB, dw);
		szStr [dw] = 0; strComment = CA2CT (szStr); delete [] szStr;
		pB += dw;
	}
	

	UINT nGrpId;
	CHECK_BOUNDS (sizeof (UINT));
	CopyMemory (&nGrpId, pB, sizeof (UINT));
	pB += sizeof (UINT);
	pGroup = _DldsGrps.FindGroup (nGrpId);

	if (pGroup == NULL)
		pGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);

	CHECK_BOUNDS (sizeof (dwFlags));
	CopyMemory (&dwFlags, pB, sizeof (dwFlags));
	pB += sizeof (dwFlags);

	CHECK_BOUNDS (sizeof (dwReserved));
	CopyMemory (&dwReserved, pB, sizeof (dwReserved));
	pB += sizeof (dwReserved);

	CHECK_BOUNDS (sizeof (dateAdded));
	CopyMemory (&dateAdded, pB, sizeof (dateAdded));
	pB += sizeof (dateAdded);

	if (dwVer >= 15)
	{
		CHECK_BOUNDS (sizeof (dateCompleted));
		CopyMemory (&dateCompleted, pB, sizeof (dateCompleted));
		pB += sizeof (dateCompleted);
	}
	
	DWORD cEvents;
	CHECK_BOUNDS (sizeof (cEvents));
	CopyMemory (&cEvents, pB, sizeof (cEvents));
	pB += sizeof (cEvents);

	vEvents.clear ();

	while (cEvents--)
	{
		fsDownloadEvents ev;

		CHECK_BOUNDS (sizeof (ev.clrBg));
		CopyMemory (&ev.clrBg, pB, sizeof (ev.clrBg));
		pB += sizeof (ev.clrBg);

		CHECK_BOUNDS (sizeof (ev.clrText));
		CopyMemory (&ev.clrText, pB, sizeof (ev.clrText));
		pB += sizeof (ev.clrText);

		CHECK_BOUNDS (sizeof (ev.timeEvent));
		CopyMemory (&ev.timeEvent, pB, sizeof (ev.timeEvent));
		pB += sizeof (ev.timeEvent);

		CHECK_BOUNDS (sizeof (ev.iImage));
		CopyMemory (&ev.iImage, pB, sizeof (ev.iImage));
		pB += sizeof (ev.iImage);

		CHECK_BOUNDS (sizeof (dw));
		CopyMemory (&dw, pB, sizeof (dw));
		if (dwVer > LAST_ANSI_DL_FILE_VERSION)
		{
			CHECK_BOUNDS (int (dw) * sizeof (WCHAR));
			LPWSTR szStr;
			pB += sizeof (dw); fsnew (szStr, WCHAR, dw+1);
			CopyMemory (szStr, pB, dw * sizeof(WCHAR));
			szStr [dw] = 0; ev.strEvent = szStr; delete [] szStr;
			pB += dw * sizeof(WCHAR);
		}
		else
		{
			CHECK_BOUNDS (int (dw));
			LPSTR szStr;
			pB += sizeof (dw); fsnew (szStr, CHAR, dw+1);
			CopyMemory (szStr, pB, dw);
			szStr [dw] = 0; ev.strEvent = CA2CT (szStr); delete [] szStr;
			pB += dw;
		}

		vEvents.add (ev);
	}

	
	if (dwVer >= 19) {
		CHECK_BOUNDS (sizeof (youTubeDetails.mediaType));
		CopyMemory (&youTubeDetails.mediaType, pB, sizeof (youTubeDetails.mediaType));
		pB += sizeof (youTubeDetails.mediaType);

		CHECK_BOUNDS (sizeof (dw));
		CopyMemory (&dw, pB, sizeof (dw));		
		pB += sizeof (dw);		
		CHECK_BOUNDS (int (dw));
		youTubeDetails.youTubeID = loadStringFromBuffer( pB, dw);

		CHECK_BOUNDS (sizeof (dw));
		CopyMemory (&dw, pB, sizeof (dw));		
		pB += sizeof (dw);		
		CHECK_BOUNDS (int (dw));
		youTubeDetails.format = loadStringFromBuffer(pB, dw );		
				
		UINT64 fileSize;
		CHECK_BOUNDS (sizeof (fileSize));
		CopyMemory (&fileSize, pB, sizeof (fileSize));
		youTubeDetails.fileSize = fileSize;
		pB += sizeof ( fileSize );

		bool isMerged;
		CHECK_BOUNDS (sizeof (isMerged));
		CopyMemory (&isMerged, pB, sizeof (isMerged));
		youTubeDetails.isMerged = isMerged;
		pB += sizeof ( isMerged );

		bool isHidden;
		CHECK_BOUNDS (sizeof (isHidden));
		CopyMemory (&isHidden, pB, sizeof (isHidden));
		youTubeDetails.isHidden = isHidden;
		pB += sizeof ( isHidden );		
			
		bool is3D;
		CHECK_BOUNDS (sizeof (is3D));
		CopyMemory (&is3D, pB, sizeof (is3D));
		youTubeDetails.is3D = is3D;
		pB += sizeof ( is3D );		

		CHECK_BOUNDS (sizeof (dw));
		CopyMemory (&dw, pB, sizeof (dw));		
		pB += sizeof (dw);		
		CHECK_BOUNDS (int (dw));
		youTubeDetails.originalURL = loadStringFromBuffer( pB, dw );		
	}

	*pdwSize = pB - LPBYTE (pb);
	
	return true;
	}
