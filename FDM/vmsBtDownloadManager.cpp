/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "vmsBtDownloadManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsBtDownloadManager::vmsBtDownloadManager()
{
	m_pDownload = NULL;
	m_pTorrent = NULL;
	m_info.pbFastResumeData = NULL;
	m_info.dwFastResumeDataSize = 0;
	m_pfnEvHandler = NULL;
	m_info.dwFlags = 0;
	m_info.pfProgress = NULL;
	m_info.timeLastDataStatAccess.Now ();
	m_bStoppedByUser = FALSE;
	m_bThreadRunning = m_bThreadDoJob = m_bThreadNeedStop = false;
	m_info.nUploadedBytes = 0;
	m_info.fShareRating = 0;
	m_info.nWastedBytes = 0;
	m_info.fPercentDone = 0;
	m_info.bDone = FALSE;
	m_info.nDownloadedBytes = 0;
	m_nUsingBtDownload = 0;

	m_uTrafficLimit = UINT_MAX;
	m_uLowSpeedMaxTime = UINT_MAX;

	if (_App.Bittorrent_DisableSeedingByDef ())
		enable_Flags (BTDF_DISABLE_SEEDING);
}

vmsBtDownloadManager::~vmsBtDownloadManager()
{
	DeleteBtDownload ();
        SAFE_RELEASE (m_pTorrent);
	SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
	SAFE_DELETE_ARRAY (m_info.pfProgress);
}

BOOL vmsBtDownloadManager::CreateByTorrentFile(LPCSTR pszTorrentFile, LPCSTR pszOutputPath, LPCSTR pszTorrentUrl)
{
	ASSERT (m_pTorrent == NULL);
	if (m_pTorrent)
		return FALSE;

	m_pTorrent = _BT.CreateTorrentFileObject ();

	if (FALSE == m_pTorrent->LoadFromFile (pszTorrentFile))
	{
		m_pTorrent->Release ();
		m_pTorrent = NULL;
		return FALSE;
	}

	m_info.strTorrentUrl = pszTorrentUrl;
	m_info.strOutputPath = pszOutputPath;
	if (m_info.strOutputPath [m_info.strOutputPath.GetLength () - 1] != '\\')
		m_info.strOutputPath += '\\';

	PostCreateTorrentObject ();

	return TRUE;
}

BOOL vmsBtDownloadManager::CreateBtDownload()
{
	if (m_pDownload)
		return TRUE;

	vmsBtSession *pSession = _BT.get_Session ();
	if (pSession == NULL)
		return FALSE;

	CString strOutputPath = m_info.strOutputPath;
	ProcessFilePathMacroses (strOutputPath);

	m_pDownload = pSession->CreateDownload (m_pTorrent, strOutputPath, 
		m_info.pbFastResumeData, m_info.dwFastResumeDataSize, 
		(m_info.dwFlags & BTDF_RESERVE_DISK_SPACE) == 0);

	if (m_pDownload == NULL)
		return FALSE;

	return TRUE;
}

void vmsBtDownloadManager::ProcessFilePathMacroses(CString &str)
{
	ASSERT (m_pTorrent != NULL);

	if (str.Find ('%', 0) == -1)
		return;	

	char szTracker [10000];
	m_pTorrent->get_TrackerUrl (szTracker, 0);
	fsURL url;
	url.Crack (szTracker);

	str.Replace ("%server%", url.GetHostName ());
	str.Replace ("%path_on_server%", url.GetPath ());

	str.Replace ("/", "\\");
	str.Replace ("\\\\", "\\");

	SYSTEMTIME st;
	GetLocalTime (&st);

	str.Replace ("%date%", "%year%-%month%-%day%");

	CString strY, strM, strD;
	strY.Format ("%04d", (int)st.wYear);
	strM.Format ("%02d", (int)st.wMonth);
	strD.Format ("%02d", (int)st.wDay);

	str.Replace ("%year%", strY);
	str.Replace ("%month%", strM);
	str.Replace ("%day%", strD);
}

void vmsBtDownloadManager::DeleteBtDownload()
{
	if (m_pDownload)
	{
		vmsBtDownload *p = m_pDownload;
		while (m_nUsingBtDownload)
			Sleep (0);
		m_pDownload = NULL;
		_BT.get_Session ()->DeleteDownload (p);
	}
}

vmsBtDownloadStateEx vmsBtDownloadManager::get_State()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	vmsBtDownloadStateEx enState = m_pDownload ?
		(vmsBtDownloadStateEx) m_pDownload->GetState () : BTDSE_STOPPED;
	InterlockedDecrement (&m_nUsingBtDownload);
	return enState;
}

fsString vmsBtDownloadManager::get_TorrentName()
{
	ASSERT (m_pTorrent != NULL);
	char sz [10000] = "";
	m_pTorrent->get_TorrentName (sz);
	vmsUtf8ToAscii (sz);
	return sz;
}

UINT vmsBtDownloadManager::GetUploadSpeed()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	BOOL b = m_pDownload ? m_pDownload->GetUploadSpeed () : 0;
	InterlockedDecrement (&m_nUsingBtDownload);
	return b;
}

int vmsBtDownloadManager::get_FileCount()
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_FileCount ();
}

fsString vmsBtDownloadManager::get_OutputFilePathName(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	fsString str = get_OutputPath ();
	if (str [str.GetLength () - 1] != '\\')
		str += '\\';
	str += get_FileName (nIndex);
	return str;
}

LPCSTR vmsBtDownloadManager::get_OutputPath()
{
	return m_info.strOutputPath;
}

void vmsBtDownloadManager::SetEventsHandler(fntBtDownloadManagerEventHandler pfn, LPVOID pData)
{
	m_pfnEvHandler = pfn;
	m_lpEvParam = pData;
}

fsString vmsBtDownloadManager::get_InfoHash()
{
	ASSERT (m_pTorrent != NULL);
	char sz [100] = "";
	m_pTorrent->get_InfoHash (sz);
	return sz;
}

LPCSTR vmsBtDownloadManager::get_TorrentUrl()
{
	return m_info.strTorrentUrl.IsEmpty () ? "" : m_info.strTorrentUrl;
}

void vmsBtDownloadManager::get_TrackerLogin(fsString &strUser, fsString &strPassword)
{
	strUser = m_info.strTrackerUser;
	strPassword = m_info.strTrackerPassword;
}

fsString vmsBtDownloadManager::get_URL()
{
	return get_TorrentUrl ();
}

DWORD vmsBtDownloadManager::get_Flags()
{
	return m_info.dwFlags;
}

void vmsBtDownloadManager::enable_Flags(DWORD dw)
{
	m_info.dwFlags |= dw;
}

void vmsBtDownloadManager::disable_Flags(DWORD dw)
{
	m_info.dwFlags &= ~dw;
}

void vmsBtDownloadManager::set_TrackerLogin(LPCSTR pszUser, LPCSTR pszPassword)
{
	m_info.strTrackerUser = pszUser;
	m_info.strTrackerPassword = pszPassword;
	InterlockedIncrement (&m_nUsingBtDownload);
	if (m_pDownload)
		m_pDownload->set_TrackerLogin (pszUser, pszPassword);
	InterlockedDecrement (&m_nUsingBtDownload);
}

fsString vmsBtDownloadManager::get_TorrentComment()
{
	ASSERT (m_pTorrent != NULL);
	char sz [10000] = "";
	m_pTorrent->get_TorrentComment (sz);
	vmsUtf8ToAscii (sz);
	return sz;
}

fsString vmsBtDownloadManager::get_FileName(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	char sz [MY_MAX_PATH] = "";
	m_pTorrent->get_FileName (nIndex, sz);
	vmsUtf8ToAscii (sz);
	LPSTR psz = sz;
	while (*psz)
	{
		if (*psz == '/')
			*psz = '\\';
		psz++;
	}
	return sz;
}

UINT64 vmsBtDownloadManager::get_FileSize(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_FileSize (nIndex);
}

int vmsBtDownloadManager::get_FilePercentDone(int nIndex)
{
	fsTicksMgr time; time.Now ();

	if (time - m_info.timeLastDataStatAccess > 1000 || m_info.pfProgress == NULL)
	{
		if (get_State () != BTDSE_CHECKING_FILES &&
				get_State () != BTDSE_QUEUED)
			SaveBtDownloadState_FileProgress ();
	}

	return m_info.pfProgress ? (int) (m_info.pfProgress [nIndex] * 100) : 0;
}

int vmsBtDownloadManager::get_PieceCount()
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_PieceCount ();
}

int vmsBtDownloadManager::get_PieceSize()
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_PieceSize ();
}

fsString vmsBtDownloadManager::get_CurrentTracker()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	if (m_pDownload == NULL)
	{
		InterlockedDecrement (&m_nUsingBtDownload);
		return m_info.strCurrentTracker;
	}
	char sz [1000] = "";
	m_pDownload->get_CurrentTracker (sz);
	InterlockedDecrement (&m_nUsingBtDownload);
	return sz;
}

UINT64 vmsBtDownloadManager::get_TotalUploadedByteCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT64 u = IsDownloadStatCanBeRead () ? m_pDownload->get_TotalUploadedByteCount () : m_info.nUploadedBytes;
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

double vmsBtDownloadManager::get_ShareRating()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	double d = IsDownloadStatCanBeRead () ? m_pDownload->get_ShareRating () : m_info.fShareRating;
	InterlockedDecrement (&m_nUsingBtDownload);
	return d;
}

UINT64 vmsBtDownloadManager::get_WastedByteCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT64 u = IsDownloadStatCanBeRead () ? m_pDownload->get_WastedByteCount () : m_info.nWastedBytes;
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

void vmsBtDownloadManager::get_PeersStat(int *pnPeersConnected, int *pnSeedsTotal, int *pnLeechersTotal, int *pnSeedsConnected)
{
	InterlockedIncrement (&m_nUsingBtDownload);

	if (m_pDownload) 
	{
		m_pDownload->get_PeersStat(pnPeersConnected, pnSeedsTotal, pnLeechersTotal, pnSeedsConnected);
	}
	else
	{
		if (pnPeersConnected)
			*pnPeersConnected = 0;
		if (pnSeedsTotal)
			*pnSeedsTotal = 0;
		if (pnLeechersTotal)
			*pnLeechersTotal = 0;
		if (pnSeedsConnected)
			*pnSeedsConnected = 0;
	}

	InterlockedDecrement (&m_nUsingBtDownload);
}

vmsBtDownloadPeerInfoList* vmsBtDownloadManager::get_PeerInfoList()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	vmsBtDownloadPeerInfoList *p = m_pDownload ? m_pDownload->get_PeerInfoList () : NULL;
	InterlockedDecrement (&m_nUsingBtDownload);
	return p;
}

int vmsBtDownloadManager::get_NextAnnounceInterval()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	BOOL b = m_pDownload ? m_pDownload->get_NextAnnounceInterval () : -1;
	InterlockedDecrement (&m_nUsingBtDownload);
	return b;
}

vmsBtDownload* vmsBtDownloadManager::get_BtDownload()
{
	return m_pDownload;
}

fsString vmsBtDownloadManager::get_OutputFilePathName()
{
	ASSERT (m_pTorrent != NULL);
	if (m_pTorrent->get_FileCount () == 1)
		return get_OutputFilePathName (0);
	else
		return get_OutputPath ();
}

float vmsBtDownloadManager::GetPercentDone()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	float f = IsDownloadStatCanBeRead () ? m_pDownload->get_PercentDone () : m_info.fPercentDone;
	InterlockedDecrement (&m_nUsingBtDownload);
	return f;
}

BOOL vmsBtDownloadManager::IsDone()
{
	if (IsDownloadStatCanBeRead () == FALSE)
		return m_info.bDone;
	
	vmsBtDownloadStateEx enState = get_State ();

	if (enState == BTDSE_SEEDING)
		return TRUE;

	if (enState != BTDSE_DOWNLOADING)
		return GetPercentDone () == 100.0f;

	return FALSE;
}

BOOL vmsBtDownloadManager::IsRunning()
{
	return m_bThreadDoJob || IsBtDownloadRunning ();
}

int vmsBtDownloadManager::GetNumberOfSections()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	int i = IsDownloadStatCanBeRead () ? m_pDownload->get_PiecesProgressMap (NULL, NULL) : 
		(int)m_info.vPieces.size ();
	InterlockedDecrement (&m_nUsingBtDownload);
	return i;
}

UINT64 vmsBtDownloadManager::GetTotalFilesSize()
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_TotalFilesSize ();
}

UINT64 vmsBtDownloadManager::GetDownloadedBytesCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT64 u = IsDownloadStatCanBeRead () ? m_pDownload->get_TotalDownloadedBytesCount () : m_info.nDownloadedBytes;
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

BOOL vmsBtDownloadManager::IsDownloading()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	if (m_pDownload == NULL)
	{
		InterlockedDecrement (&m_nUsingBtDownload);
		return FALSE;
	}
	BOOL b = get_State () == BTDSE_DOWNLOADING &&
		m_pDownload->IsPaused () == FALSE;
	InterlockedDecrement (&m_nUsingBtDownload);
	return b;
}

void vmsBtDownloadManager::GetSectionInfo(int nIndex, vmsSectionInfo *sect)
{
	InterlockedIncrement (&m_nUsingBtDownload);

	UINT64 uTotal = GetTotalFilesSize ();
	int ns = GetNumberOfSections ();
	UINT64 uPerPiece = uTotal / ns;
	
	sect->uDStart = nIndex * uPerPiece;
	sect->uDEnd = nIndex == ns-1 ? uTotal : sect->uDStart + uPerPiece - 1;
	bool bPC = IsDownloadStatCanBeRead () ? m_pDownload->is_PieceCompleted (nIndex) : m_info.vPieces [nIndex];
	sect->uDCurrent = bPC ? sect->uDEnd : sect->uDStart;

	InterlockedDecrement (&m_nUsingBtDownload);
}

int vmsBtDownloadManager::GetDownloadingSectionCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	int i = m_pDownload ? m_pDownload->get_DownloadConnectionCount () : 0;
	InterlockedDecrement (&m_nUsingBtDownload);
	return i;
}

UINT vmsBtDownloadManager::GetSpeed()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT u = m_pDownload ? m_pDownload->GetDownloadSpeed () : 0;
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

BOOL vmsBtDownloadManager::MoveToFolder(LPCSTR pszPath)
{
	InterlockedIncrement (&m_nUsingBtDownload);

	if (m_pDownload == NULL)
	{
		InterlockedDecrement (&m_nUsingBtDownload);

		USES_CONVERSION;

		
		std::wstring wstrSrcPath = A2W (m_info.strOutputPath);
		if (wstrSrcPath [wstrSrcPath.length () - 1] != '\\')
			wstrSrcPath += '\\';
		
		std::wstring wstrDstPath = A2W (pszPath);
		if (wstrDstPath [wstrDstPath.length () - 1] != '\\')
			wstrDstPath += '\\';

		
		for (int i = 0; i < get_FileCount (); i++)
		{
			std::wstring wstrSrc = wstrSrcPath; wstrSrc += get_FileNameW (i);
			std::wstring wstrDst = wstrDstPath; wstrDst += get_FileNameW (i);
			fsBuildPathToFileW (wstrDst.c_str ());
			if (FALSE == MoveFileW (wstrSrc.c_str (), wstrDst.c_str ()) && 
					GetFileAttributesW (wstrSrc.c_str ()) != DWORD (-1))
				break;
		}

		if (i != get_FileCount ())
		{
			
			while (--i >= 0)
			{
				std::wstring wstrSrc = wstrSrcPath; wstrSrc += get_FileNameW (i);
				std::wstring wstrDst = wstrDstPath; wstrDst += get_FileNameW (i);
				MoveFileW (wstrDst.c_str (), wstrSrc.c_str ());
			}
			
			return FALSE;
		}

		RemoveBtDownloadDirectory ();

		
		m_info.strOutputPath = W2A (wstrDstPath.c_str ());

		return TRUE;
	}

	CString str = pszPath;
	if (str [str.GetLength () - 1] != '\\')
			str += '\\';
	ProcessFilePathMacroses (str);
	if (FALSE == m_pDownload->MoveToFolder (str))
	{
		InterlockedDecrement (&m_nUsingBtDownload);
		return FALSE;
	}

	RemoveBtDownloadDirectory ();

	m_info.strOutputPath = str;

	InterlockedDecrement (&m_nUsingBtDownload);

	return TRUE;
}

BOOL vmsBtDownloadManager::IsCantStart()
{
	if (m_pDownload == NULL)
		return FALSE;

	return IsBtDownloadRunning () && get_State () != BTDSE_DOWNLOADING;
}

BOOL vmsBtDownloadManager::IsBtDownloadRunning()
{
	InterlockedIncrement (&m_nUsingBtDownload);

	if (m_pDownload == NULL)
	{
		InterlockedDecrement (&m_nUsingBtDownload);
		return FALSE;
	}

	vmsBtDownloadState enState = m_pDownload->GetState ();
	BOOL b = enState != BTDSE_SEEDING && enState != BTDSE_QUEUED &&
		m_pDownload->IsPaused () == FALSE;

	InterlockedDecrement (&m_nUsingBtDownload);

	return b;
}

fsInternetResult vmsBtDownloadManager::RestartDownloading()
{
	fsInternetResult ir = SetToRestartState ();

	if (ir != IR_SUCCESS)
		return ir;

	return StartDownloading ();
}

fsInternetResult vmsBtDownloadManager::SetToRestartState()
{
	m_info.bDone = FALSE;

	if (FALSE == DeleteFile ())
		return IR_ERROR;

	return IR_SUCCESS;
}

BOOL vmsBtDownloadManager::DeleteFile()
{
	if (m_pDownload)
	{
		SaveBtDownloadState ();
		DeleteBtDownload ();
	}

	USES_CONVERSION;

	
	std::wstring wstrSrcPath = A2W (m_info.strOutputPath);
	if (wstrSrcPath [wstrSrcPath.length () - 1] != '\\')
		wstrSrcPath += '\\';

	
	bool bAllDeletedOk = true;

	for (int i = 0; i < get_FileCount (); i++)
	{
		std::wstring wstr = wstrSrcPath; wstr += get_FileNameW (i);
		if (FALSE == ::DeleteFileW (wstr.c_str ()) && GetFileAttributesW (wstr.c_str ()) != DWORD (-1))
			bAllDeletedOk = false;
	}

	RemoveBtDownloadDirectory ();

	return bAllDeletedOk;
}

fsInternetResult vmsBtDownloadManager::StartDownloading()
{
	if (m_info.bDone || m_bThreadRunning)
		return IR_S_FALSE;

	if (m_pDownload == NULL && FALSE == CreateBtDownload ())
		return IR_ERROR;

	

	if (m_bThreadRunning == false)
	{
		m_bThreadRunning = m_bThreadDoJob = true;
		m_bThreadNeedStop = false;
		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadBtDownloadManager, this, 0, &dw));
	}

	return IR_SUCCESS;
}

void vmsBtDownloadManager::StopDownloading()
{
	if (m_pDownload)
		m_bThreadNeedStop = true;
}

void vmsBtDownloadManager::LimitTraffic(UINT uLimit)
{
	InterlockedIncrement (&m_nUsingBtDownload);
	if (m_pDownload)
		m_pDownload->SetDownloadLimit ((int)uLimit);
	InterlockedDecrement (&m_nUsingBtDownload);
}

UINT vmsBtDownloadManager::GetTrafficLimit()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT u = m_pDownload ? (UINT)m_pDownload->GetDownloadLimit () : UINT_MAX;
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

void vmsBtDownloadManager::UseDetailedLog(BOOL bUse)
{
	
}

int vmsBtDownloadManager::get_ConnectionCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	int i = m_pDownload ? m_pDownload->get_ConnectionCount () : 0;
	InterlockedDecrement (&m_nUsingBtDownload);
	return i;
}

BOOL vmsBtDownloadManager::SaveState(LPBYTE pb, LPDWORD pdwSize)
{
	if (m_pDownload && pb == NULL)
		SaveBtDownloadState ();

	#define CHECK_SIZE(need) {if (pb != NULL && *pdwSize < (UINT)(pb + need - pbOld)) goto _lSizeErr;}

	LPBYTE pbOld = pb;

	DWORD dwNeedSize = sizeof (DWORD); 

	dwNeedSize += sizeof (int) + m_info.strOutputPath.GetLength ();
	dwNeedSize += sizeof (int) + m_info.strTorrentUrl.GetLength ();
	dwNeedSize += sizeof (int) + m_info.strTrackerUser.GetLength ();
	dwNeedSize += sizeof (int) + m_info.strTrackerPassword.GetLength ();

	dwNeedSize += sizeof (float) * get_FileCount (); 
	dwNeedSize += sizeof (int) + m_info.strCurrentTracker.GetLength ();
	dwNeedSize += sizeof (m_info.nUploadedBytes);
	dwNeedSize += sizeof (m_info.fShareRating);
	dwNeedSize += sizeof (m_info.nWastedBytes);
	dwNeedSize += sizeof (m_info.fPercentDone);
	dwNeedSize += sizeof (m_info.bDone);
	dwNeedSize += sizeof (int) + sizeof (BYTE) * m_info.vPieces.size ();
	dwNeedSize += sizeof (m_info.nDownloadedBytes);

	dwNeedSize += sizeof (m_uLowSpeedMaxTime) + sizeof (m_uTrafficLimit);

	if (pb == NULL)
	{
		DWORD dw;
		if (FALSE == m_pTorrent->get_TorrentBuffer (NULL, 0, &dw))
			return FALSE;
		*pdwSize = dw + sizeof (DWORD);
		*pdwSize += m_info.dwFastResumeDataSize + sizeof (DWORD);
		*pdwSize += dwNeedSize;
		return TRUE;
	}

	int i;

	
	i = m_info.strTorrentUrl.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strTorrentUrl, i);
	pb += i;

	
	i = m_info.strOutputPath.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strOutputPath, i);
	pb += i;

	
	i = m_info.strTrackerUser.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strTrackerUser, i);
	pb += i;

	i = m_info.strTrackerPassword.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strTrackerPassword, i);
	pb += i;

	
	CHECK_SIZE (sizeof (DWORD));
	DWORD dw; dw = *pdwSize - (pb - pbOld - sizeof (DWORD));
	if (FALSE == m_pTorrent->get_TorrentBuffer (pb + sizeof (DWORD), dw, &dw))
		goto _lSizeErr;
	*((LPDWORD)pb) = dw;
	pb += sizeof (DWORD) + dw;

	
	CHECK_SIZE (sizeof (DWORD));
	*((LPDWORD)pb) = m_info.dwFastResumeDataSize;
	pb += sizeof (DWORD);
	if (m_info.dwFastResumeDataSize != 0)
	{
		CHECK_SIZE (m_info.dwFastResumeDataSize);
		CopyMemory (pb, m_info.pbFastResumeData, m_info.dwFastResumeDataSize);
		pb += m_info.dwFastResumeDataSize;
	}

	
	CHECK_SIZE (get_FileCount () * sizeof (float));
	CopyMemory (pb, m_info.pfProgress, get_FileCount () * sizeof (float));
	pb += get_FileCount () * sizeof (float);

	i = m_info.strCurrentTracker.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strCurrentTracker, i);
	pb += i;

	CHECK_SIZE (sizeof (m_info.nUploadedBytes));
	CopyMemory (pb, &m_info.nUploadedBytes, sizeof (m_info.nUploadedBytes));
	pb += sizeof (m_info.nUploadedBytes);

	CHECK_SIZE (sizeof (m_info.fShareRating));
	CopyMemory (pb, &m_info.fShareRating, sizeof (m_info.fShareRating));
	pb += sizeof (m_info.fShareRating);

	CHECK_SIZE (sizeof (m_info.nWastedBytes));
	CopyMemory (pb, &m_info.nWastedBytes, sizeof (m_info.nWastedBytes));
	pb += sizeof (m_info.nWastedBytes);

	CHECK_SIZE (sizeof (m_info.fPercentDone));
	CopyMemory (pb, &m_info.fPercentDone, sizeof (m_info.fPercentDone));
	pb += sizeof (m_info.fPercentDone);

	CHECK_SIZE (sizeof (m_info.bDone));
	CopyMemory (pb, &m_info.bDone, sizeof (m_info.bDone));
	pb += sizeof (m_info.bDone);

	i = m_info.vPieces.size ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	int j;
	for (j = 0; j < i; j++)
		*pb++ = m_info.vPieces [j];

	CHECK_SIZE (sizeof (m_info.nDownloadedBytes));
	CopyMemory (pb, &m_info.nDownloadedBytes, sizeof (m_info.nDownloadedBytes));
	pb += sizeof (m_info.nDownloadedBytes);

	

	CHECK_SIZE (sizeof (m_info.dwFlags));
	*((LPDWORD)pb) = m_info.dwFlags;
	pb += sizeof (m_info.dwFlags);

	CHECK_SIZE (sizeof (m_uLowSpeedMaxTime));
	*((LPDWORD)pb) = m_uLowSpeedMaxTime;
	pb += sizeof (m_uLowSpeedMaxTime);

	CHECK_SIZE (sizeof (m_uTrafficLimit));
	*((LPDWORD)pb) = m_uTrafficLimit;
	pb += sizeof (m_uTrafficLimit);

	*pdwSize = pb - pbOld;

	return TRUE;

_lSizeErr:
	SaveState (NULL, pdwSize);
	return FALSE;
}

BOOL vmsBtDownloadManager::LoadState(LPBYTE lpBuffer, LPDWORD pdwSize, WORD wVer)
{
	ASSERT (wVer >= 10);

	LPBYTE pbStart = lpBuffer;

	int i;
	char sz [10000];

	if (wVer > 10)
	{
		CopyMemory (&i, lpBuffer, sizeof (int));
		lpBuffer += sizeof (int);
		CopyMemory (sz, lpBuffer, i);
		sz [i] = 0;
		lpBuffer += i;
		m_info.strTorrentUrl = sz;
	}
	else
	{
		m_info.strTorrentUrl = "";
	}

	CopyMemory (&i, lpBuffer, sizeof (int));
	lpBuffer += sizeof (int);
	CopyMemory (sz, lpBuffer, i);
	sz [i] = 0;
	m_info.strOutputPath = sz;
	lpBuffer += i;

	CopyMemory (&i, lpBuffer, sizeof (int));
	lpBuffer += sizeof (int);
	CopyMemory (sz, lpBuffer, i);
	sz [i] = 0;
	lpBuffer += i;
	m_info.strTrackerUser = sz;

	CopyMemory (&i, lpBuffer, sizeof (int));
	lpBuffer += sizeof (int);
	CopyMemory (sz, lpBuffer, i);
	sz [i] = 0;
	lpBuffer += i;
	m_info.strTrackerPassword = sz;

	DWORD dwTorrentSize = *((LPDWORD)lpBuffer);
	LPBYTE pbTorrent = lpBuffer + sizeof (DWORD);
	lpBuffer += sizeof (DWORD) + dwTorrentSize;

	m_pTorrent = _BT.CreateTorrentFileObject ();
	if (m_pTorrent == NULL)
		return FALSE;
	if (FALSE == m_pTorrent->LoadFromBuffer (pbTorrent, dwTorrentSize))
		return FALSE;

	m_info.dwFastResumeDataSize = *((LPDWORD)lpBuffer);
	lpBuffer += sizeof (DWORD);
	SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
	if (m_info.dwFastResumeDataSize)
	{
		m_info.pbFastResumeData = new BYTE [m_info.dwFastResumeDataSize];
		CopyMemory (m_info.pbFastResumeData, lpBuffer, m_info.dwFastResumeDataSize);
		lpBuffer += m_info.dwFastResumeDataSize;
	}

	if (wVer > 11)
	{
		m_info.pfProgress = new float [get_FileCount () * sizeof (float)];
		CopyMemory (m_info.pfProgress, lpBuffer, get_FileCount () * sizeof (float));
		lpBuffer += get_FileCount () * sizeof (float);

		CopyMemory (&i, lpBuffer, sizeof (int));
		lpBuffer += sizeof (int);
		CopyMemory (sz, lpBuffer, i);
		sz [i] = 0;
		lpBuffer += i;
		m_info.strCurrentTracker = sz;

		CopyMemory (&m_info.nUploadedBytes, lpBuffer, sizeof (m_info.nUploadedBytes));
		lpBuffer += sizeof (m_info.nUploadedBytes);

		CopyMemory (&m_info.fShareRating, lpBuffer, sizeof (m_info.fShareRating));
		lpBuffer += sizeof (m_info.fShareRating);

		CopyMemory (&m_info.nWastedBytes, lpBuffer, sizeof (m_info.nWastedBytes));
		lpBuffer += sizeof (m_info.nWastedBytes);

		CopyMemory (&m_info.fPercentDone, lpBuffer, sizeof (m_info.fPercentDone));
		lpBuffer += sizeof (m_info.fPercentDone);

		CopyMemory (&m_info.bDone, lpBuffer, sizeof (m_info.bDone));
		lpBuffer += sizeof (m_info.bDone);

		CopyMemory (&i, lpBuffer, sizeof (int));
		lpBuffer += sizeof (int);
		m_info.vPieces.clear ();
		while (i--)
			m_info.vPieces.push_back ((*lpBuffer++) != 0);
		
		CopyMemory (&m_info.nDownloadedBytes, lpBuffer, sizeof (m_info.nDownloadedBytes));
		lpBuffer += sizeof (m_info.nDownloadedBytes);
	}
	else
	{
		PostCreateTorrentObject ();
	}

	m_info.dwFlags = *((LPDWORD)lpBuffer);
	lpBuffer += sizeof (DWORD);

	m_uLowSpeedMaxTime = *((LPUINT)lpBuffer);
	lpBuffer += sizeof (UINT);

	m_uTrafficLimit = *((LPUINT)lpBuffer);
	lpBuffer += sizeof (UINT);

	*pdwSize = lpBuffer - pbStart;

	if ((m_info.dwFlags & BTDF_DISABLE_SEEDING) == 0 && m_info.bDone)
		EnableSeeding (TRUE);

	return TRUE;
}

BOOL vmsBtDownloadManager::IsStoppedByUser()
{
	return m_bStoppedByUser;
}

DWORD WINAPI vmsBtDownloadManager::_threadBtDownloadManager(LPVOID lp)
{
#define LOG_local(x) LOG("vmsBTDM::_tBTDM(" << (DWORD)lp << "): " << x << nl)

	vmsBtDownloadManager *pthis = (vmsBtDownloadManager*)lp;
	vmsBtDownloadStateEx enPrevState = BTDSE_QUEUED;

	pthis->RaiseEvent (BTDME_DOWNLOAD_STARTED);

	LOG_local ("start");

	while (pthis->get_State () == BTDSE_QUEUED && pthis->m_bThreadNeedStop == false)
		Sleep (100);

	LOG_local ("1");
	
	bool bDownloading = false;
	bool bMayFilesEvent = true;

	while (pthis->IsBtDownloadRunning () && pthis->m_bThreadNeedStop == false)
	{
		
		{
			vmsBtDownloadStateEx enCurrState = pthis->get_State ();
			if (enCurrState != enPrevState)
			{
				switch (enCurrState)
				{
				case BTDSE_CHECKING_FILES:
					if (bMayFilesEvent)
						pthis->RaiseEvent (BTDME_CHECKING_FILES);
					bMayFilesEvent = false;
					break;

				case BTDSE_ALLOCATING:
					if (bMayFilesEvent)
						pthis->RaiseEvent (BTDME_ALLOCATING);
					bMayFilesEvent = false;
					break;

				case BTDSE_DOWNLOADING:
					_DldsMgr.ProcessDownloads ();
					break;
				}

				enPrevState = enCurrState;
			}
		}

		Sleep (100);
		if (bDownloading == false)
		{
			if (pthis->GetSpeed ())
			{
				bDownloading = true;
				pthis->RaiseEvent (BTDME_DOWNLOADING);
			}
		}
	}

	LOG_local ("2");

	if (pthis->m_pDownload != NULL &&
			(pthis->get_State () != BTDSE_SEEDING || (pthis->m_info.dwFlags & BTDF_DISABLE_SEEDING) != 0 ||
				_DldsMgr.AllowStartNewDownloads () == FALSE))
	{
		LOG_local ("need delete bt download");
		pthis->m_pDownload->Pause ();
		LOG_local ("saving state");
		pthis->SaveBtDownloadState ();
		LOG_local ("deleting");
		pthis->DeleteBtDownload ();
	}
	else
	{
		#ifdef _USELOGGING
		if (pthis->m_pDownload != NULL)
			LOG_local ("download was not deleted for some reason");
		#endif
	}

	LOG_local ("3");

	pthis->m_bThreadDoJob = false;

	try{pthis->RaiseEvent (BTDME_DOWNLOAD_STOPPED_OR_DONE);}catch(...){}

	LOG_local ("4");

	if (pthis->get_State () == BTDSE_SEEDING)
		pthis->RaiseEvent (BTDME_SEEDING);

	LOG_local ("done");

	pthis->m_bThreadRunning = false;

	return 0;
}

void vmsBtDownloadManager::StopThread()
{
	if (m_bThreadRunning)
	{
		StopDownloading ();
		while (m_bThreadRunning)
			Sleep (10);
	}
}

void vmsBtDownloadManager::SaveBtDownloadState()
{
	InterlockedIncrement (&m_nUsingBtDownload);

	if (m_pDownload == NULL || m_pDownload->is_HandleValid () == FALSE || 
			get_State () == BTDSE_QUEUED)
	{
		InterlockedDecrement (&m_nUsingBtDownload);
		return;
	}

	SaveBtDownloadState_FileProgress ();
	m_info.strCurrentTracker = get_CurrentTracker ();
	m_info.nUploadedBytes = get_TotalUploadedByteCount ();
	m_info.fShareRating = get_ShareRating ();
	m_info.nWastedBytes = get_WastedByteCount ();
	m_info.fPercentDone = GetPercentDone ();
	m_info.bDone = IsDone ();

	m_info.vPieces.clear ();
	int ns = GetNumberOfSections ();
	for (int i = 0; i < ns; i++)
		m_info.vPieces.push_back (m_pDownload->is_PieceCompleted (i));

	m_info.nDownloadedBytes = GetDownloadedBytesCount ();

	SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
	m_info.dwFastResumeDataSize = 0;
	if (m_pDownload->get_FastResumeData (NULL, 0, &m_info.dwFastResumeDataSize) &&
			m_info.dwFastResumeDataSize != 0)
	{
		m_info.pbFastResumeData = new BYTE [m_info.dwFastResumeDataSize];
		if (FALSE == m_pDownload->get_FastResumeData (m_info.pbFastResumeData, m_info.dwFastResumeDataSize, &m_info.dwFastResumeDataSize))
		{
			SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
			m_info.dwFastResumeDataSize = 0;
		}
	}

	InterlockedDecrement (&m_nUsingBtDownload);
}

void vmsBtDownloadManager::SaveBtDownloadState_FileProgress()
{
	if (m_info.pfProgress == NULL)
	{
		if (m_pDownload == NULL)
			return;
		m_info.pfProgress = new float [get_FileCount ()];
	}
		
	if (m_pDownload)
	{
		m_pDownload->get_FileProgress (m_info.pfProgress);
		m_info.timeLastDataStatAccess.Now ();
	}
}

DWORD vmsBtDownloadManager::RaiseEvent(vmsBtDownloadManagerEvent ev, DWORD dw)
{
	if (m_pfnEvHandler == NULL)
		return 0;

	return m_pfnEvHandler (this, ev, dw, m_lpEvParam);
}  

void vmsBtDownloadManager::PostCreateTorrentObject()
{
	for (int i = 0; i < get_PieceCount (); i++)
		m_info.vPieces.push_back (false);
}

BOOL vmsBtDownloadManager::IsDownloadStatCanBeRead()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	BOOL b = m_pDownload && get_State () != BTDSE_QUEUED &&
		get_State () != BTDSE_CHECKING_FILES;
	InterlockedDecrement (&m_nUsingBtDownload);
	return b;
}

void vmsBtDownloadManager::EnableSeeding(BOOL bEnable)
{
	if (bEnable)
	{
		disable_Flags (BTDF_DISABLE_SEEDING);
		if (m_pDownload == NULL && m_info.bDone)
		{
			CreateBtDownload ();

			m_bThreadRunning = true;
			DWORD dw;
			CloseHandle (
				CreateThread (NULL, 0, _threadCheckStartSeeding, this, 0, &dw));
		}
	}
	else
	{
		enable_Flags (BTDF_DISABLE_SEEDING);
		StopSeeding ();
	}
}

void vmsBtDownloadManager::RecursiveRemoveDirectory(LPCWSTR pwszPath)
{
	std::wstring wstrPath = pwszPath;
	if (wstrPath [wstrPath.length () - 1] != '\\')
		wstrPath += '\\';
	std::wstring wstrMask = wstrPath; wstrMask += L"*.*";

	fs::list <std::wstring> vFolders;

	WIN32_FIND_DATAW wfd;
	HANDLE hFind = FindFirstFileW (wstrMask.c_str (), &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring wstr = wstrPath; wstr += wfd.cFileName;
			DWORD dw = GetFileAttributesW (wstr.c_str ());
			if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY) != 0 && 
					wcscmp (wfd.cFileName, L".") && wcscmp (wfd.cFileName, L".."))
			{
				vFolders.add (wstr);
			}
		}
		while (FindNextFileW (hFind, &wfd));

		FindClose (hFind);

		for (int i = 0; i < vFolders.size (); i++)
			RecursiveRemoveDirectory (vFolders [i].c_str ());

		RemoveDirectoryW (pwszPath);
	}
}

void vmsBtDownloadManager::RemoveBtDownloadDirectory()
{
	USES_CONVERSION;
	std::wstring wstr = get_FileNameW (0);
	if (wcschr (wstr.c_str (), '\\'))
	{
		std::wstring wstr2 = A2W (m_info.strOutputPath);
		for (int i = 0; wstr [i] != '\\'; i++)
			wstr2 += wstr [i];	
		RecursiveRemoveDirectory (wstr2.c_str ());
	}
}

DWORD WINAPI vmsBtDownloadManager::_threadCheckStartSeeding(LPVOID lp)
{
	vmsBtDownloadManager *pthis = (vmsBtDownloadManager*)lp;

	while (pthis->m_bThreadNeedStop == false && pthis->m_pDownload != NULL)
	{
		vmsBtDownloadStateEx s = pthis->get_State ();
		if (s != BTDSE_QUEUED && s != BTDSE_CHECKING_FILES && s != BTDSE_CONNECTING_TRACKER)
			break;
		if (s == BTDSE_CONNECTING_TRACKER && pthis->GetPercentDone () != 100.0f)
			break;
		Sleep (5);
	}

	if (pthis->get_State () != BTDSE_SEEDING || 
			(pthis->m_info.dwFlags & BTDF_DISABLE_SEEDING) != 0)
		pthis->DeleteBtDownload ();
	else
		pthis->RaiseEvent (BTDME_SEEDING);

	pthis->m_bThreadRunning = false;

	return 0;
}

void vmsBtDownloadManager::StopSeeding()
{
	if (get_State () == BTDSE_SEEDING)
	{
		SaveBtDownloadState ();
		DeleteBtDownload ();
	}
}

std::wstring vmsBtDownloadManager::get_FileNameW(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	char sz [MY_MAX_PATH] = "";
	m_pTorrent->get_FileName (nIndex, sz);
	std::wstring wstr = vmsUtf8Unicode (sz);
	LPWSTR pwsz = (LPWSTR)wstr.c_str ();
	while (*pwsz)
	{
		if (*pwsz == '/')
			*pwsz = '\\';
		pwsz++;
	}
	return wstr;
}
