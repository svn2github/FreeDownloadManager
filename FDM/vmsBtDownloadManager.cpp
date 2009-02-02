/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsBtDownloadManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif  

std::vector <vmsBtDownloadManager*>* vmsBtDownloadManager::m_pvpDlds;
LPCRITICAL_SECTION vmsBtDownloadManager::m_pcsvpDlds = NULL;
HANDLE vmsBtDownloadManager::m_htDlds = NULL;
LONG vmsBtDownloadManager::m_cStatDataRefs = 0;        

vmsBtDownloadManager::vmsBtDownloadManager()
{
	if (InterlockedIncrement (&m_cStatDataRefs) == 1)
	{
		m_pcsvpDlds = new CRITICAL_SECTION;
		m_pvpDlds = new std::vector <vmsBtDownloadManager*>;
		InitializeCriticalSection (m_pcsvpDlds);
		DWORD dw;
		m_htDlds = CreateThread (NULL, 0, _threadDlds, NULL, 0, &dw);
	}

	m_pDownload = NULL;
	m_pTorrent = NULL;
	m_info.pbFastResumeData = NULL;
	m_info.dwFastResumeDataSize = 0;
	m_pfnEvHandler = NULL;
	m_info.dwFlags = 0;
	m_info.pfProgress = NULL;
	m_info.timeLastDataStatAccess.Now ();
	m_bStoppedByUser = FALSE;
	m_bDlThreadRunning = m_bDlThreadDoJob = m_bDlThreadNeedStop = false;
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

	m_cache.nDownloadingSectionCount = 0;

	m_bWasFatalErrorInLoadState = false;

	m_cache.nDownloadedBytes_time.m_dwTicks = 0;
	m_cache.nDownloadingSectionCount_time.m_dwTicks = 0;
	m_cache.nPartial_time.m_dwTicks = 0;

	m_fRequiredRatio = 0;
}

vmsBtDownloadManager::~vmsBtDownloadManager()
{
	StopThread ();
	DeleteFromDldsList ();
	DeleteBtDownload ();
    SAFE_RELEASE (m_pTorrent);
	SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
	SAFE_DELETE_ARRAY (m_info.pfProgress);
	if (0 == InterlockedDecrement (&m_cStatDataRefs))
	{
		Shutdown ();
		SAFE_DELETE (m_pvpDlds);
		DeleteCriticalSection (m_pcsvpDlds);
		SAFE_DELETE (m_pcsvpDlds);
	}
}

BOOL vmsBtDownloadManager::CreateByTorrentFile(LPCSTR pszTorrentFile, LPCSTR pszOutputPath, LPCSTR pszTorrentUrl, BOOL bSeedOnly)
{
	if (FALSE == LoadTorrentFile (pszTorrentFile))
		return FALSE;

	if (bSeedOnly)
	{
		if (m_pTorrent->GenerateFastResumeDataForSeed (pszOutputPath, NULL, 0, 
				&m_info.dwFastResumeDataSize))
		{
			m_info.pbFastResumeData = new BYTE [m_info.dwFastResumeDataSize];
			m_pTorrent->GenerateFastResumeDataForSeed (pszOutputPath, m_info.pbFastResumeData, 
				m_info.dwFastResumeDataSize, &m_info.dwFastResumeDataSize);
			m_info.bDone = TRUE;
			m_info.fPercentDone = 100;
			m_info.nDownloadedBytes = GetTotalFilesSize ();
		}

		enable_Flags (BTDF_NEVER_DELETE_FILES_ON_DISK);
	}

	m_info.strTorrentUrl = pszTorrentUrl;
	m_info.strOutputPath = pszOutputPath;
	if (m_info.strOutputPath [m_info.strOutputPath.GetLength () - 1] != '\\')
		m_info.strOutputPath += '\\';

	m_info.pfProgress = new float [get_FileCount () * sizeof (float)];
	for (int i = get_FileCount () - 1; i >= 0; i--)
		m_info.pfProgress [i] = bSeedOnly ? 1.0f : 0;

	PostCreateTorrentObject ();

	m_fRequiredRatio = _App.Bittorrent_RequiredRatio ();

	if (bSeedOnly)
		EnableSeeding (TRUE);

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
		m_info.dwFlags & BTDF_RESERVE_DISK_SPACE ? BTSM_SPARSE : BTSM_COMPACT);

	if (m_pDownload == NULL)
		return FALSE;

	if (m_info.vFilesPriorities.size () && _BT.getBtDllVersion () >= 778)
	{
		int *pi = new int [m_info.vFilesPriorities.size ()];
		for (size_t i = 0; i < m_info.vFilesPriorities.size (); i++)
			pi [i] = m_info.vFilesPriorities [i];
		m_pDownload->PrioritizeFiles (pi);
		delete [] pi;
	}

	return TRUE;
}

void vmsBtDownloadManager::ProcessFilePathMacroses(CString &str)
{
	ASSERT (m_pTorrent != NULL);

	if (str.Find ('%', 0) == -1)
		return;	

	char szTracker [10000];
	m_pTorrent->get_TrackerUrl2 (szTracker, 0, sizeof (szTracker));
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
		DeleteFromDldsList ();
		vmsBtDownload *p = m_pDownload;
		while (m_nUsingBtDownload)
			Sleep (0);
		m_pDownload = NULL;
		m_info.nUploadedBytes += p->get_TotalUploadedByteCount ();
		m_info.nWastedBytes += p->get_WastedByteCount ();
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
	m_pTorrent->get_TorrentName2 (sz, sizeof (sz));
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

	if (pfn && m_bWasFatalErrorInLoadState)
	{
		RaiseEvent (BTDME_FATAL_ERROR);
		m_bWasFatalErrorInLoadState = false;
	}
}

fsString vmsBtDownloadManager::get_InfoHash()
{
	ASSERT (m_pTorrent != NULL);
	char sz [1000] = "";
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
	char sz [3000] = "";
	m_pTorrent->get_TorrentComment2 (sz, sizeof (sz));
	vmsUtf8ToAscii (sz);
	return sz;
}

fsString vmsBtDownloadManager::get_FileName(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	char sz [MY_MAX_PATH] = "";
	m_pTorrent->get_FileName2 (nIndex, sz, sizeof (sz));
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
	char sz [10000] = "";
	m_pDownload->get_CurrentTracker2 (sz, sizeof (sz));
	InterlockedDecrement (&m_nUsingBtDownload);
	return sz;
}

UINT64 vmsBtDownloadManager::get_TotalUploadedByteCount()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	UINT64 u = m_info.nUploadedBytes; 
	if (IsDownloadStatCanBeRead ())
		u += m_pDownload->get_TotalUploadedByteCount ();
	InterlockedDecrement (&m_nUsingBtDownload);
	return u;
}

double vmsBtDownloadManager::getRatio()
{
	UINT64 u = (__int64)GetDownloadedBytesCount (true);
	if (!u)
		return 0;
	return (double)(__int64)get_TotalUploadedByteCount () / (__int64)u;
	
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

	if (enState == BTDSE_SEEDING || enState == BTDSE_FINISHED)
		return TRUE;

	if (enState != BTDSE_DOWNLOADING)
		return GetPercentDone () == 100.0f;

	return FALSE;
}

BOOL vmsBtDownloadManager::IsRunning()
{
	return m_bDlThreadDoJob || IsBtDownloadRunning ();
}

int vmsBtDownloadManager::GetNumberOfSections()
{
	
	return m_pTorrent->get_PieceCount ();
}

UINT64 vmsBtDownloadManager::GetTotalFilesSize()
{
	ASSERT (m_pTorrent != NULL);
	return m_pTorrent->get_TotalFilesSize ();
}

UINT64 vmsBtDownloadManager::GetDownloadedBytesCount(bool bFromCache)
{
	if (bFromCache)
		return m_info.nDownloadedBytes;
	InterlockedIncrement (&m_nUsingBtDownload);
	m_info.nDownloadedBytes = IsDownloadStatCanBeRead () ? m_pDownload->get_TotalDownloadedBytesCount () : m_info.nDownloadedBytes;
	InterlockedDecrement (&m_nUsingBtDownload);
	m_cache.nDownloadedBytes_time.Now ();
	return m_info.nDownloadedBytes;
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
	bool bPC = false; 
	if (IsDownloadStatCanBeRead () && get_State () == BTDSE_DOWNLOADING)
		bPC = m_pDownload->is_PieceCompleted (nIndex);
	else if (m_info.vPieces.size ())
		bPC = m_info.vPieces [nIndex];
	sect->uDCurrent = bPC ? sect->uDEnd : sect->uDStart;
	
	InterlockedDecrement (&m_nUsingBtDownload);
}

int vmsBtDownloadManager::GetDownloadingSectionCount(bool bFromCache)
{
	if (bFromCache)
		return m_cache.nDownloadingSectionCount;
	
	InterlockedIncrement (&m_nUsingBtDownload);
	m_cache.nDownloadingSectionCount = m_pDownload ? m_pDownload->get_DownloadConnectionCount () : 0;
	InterlockedDecrement (&m_nUsingBtDownload);
	m_cache.nDownloadingSectionCount_time.Now ();
	return m_cache.nDownloadingSectionCount;
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
	BOOL b = enState != BTDSE_SEEDING && enState != BTDSE_FINISHED && enState != BTDSE_QUEUED &&
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
	if (m_pDownload)
		DeleteBtDownload ();

	SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
	m_info.dwFastResumeDataSize = 0;

	SAFE_DELETE_ARRAY (m_info.pfProgress);
		
	m_info.nUploadedBytes = 0;
	m_info.fShareRating = 0;
	m_info.nWastedBytes = 0;
	m_info.fPercentDone = 0;
	m_info.bDone = FALSE;
	m_info.nDownloadedBytes = 0;

	if (FALSE == DeleteFile ())
		return IR_ERROR;

	return IR_SUCCESS;
}

BOOL vmsBtDownloadManager::DeleteFile()
{
	if (m_info.dwFlags & BTDF_NEVER_DELETE_FILES_ON_DISK)
		return TRUE;

	bool bMaySleep = false;

	if (m_pDownload)
	{
		if (IsRunning ())
		{
			StopDownloading ();
			while (m_bDlThreadRunning)
				Sleep (10);
		}
		else
		{
			SaveBtDownloadState ();
			DeleteBtDownload ();
		}

		bMaySleep = true;
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
		{
			if (bMaySleep)
			{
				bMaySleep = false;
				Sleep (300);
				i--;
				continue;
			}
			bAllDeletedOk = false;
		}
	}

	RemoveBtDownloadDirectory ();

	return bAllDeletedOk;
}

fsInternetResult vmsBtDownloadManager::StartDownloading()
{
	if (m_info.bDone || m_bDlThreadRunning)
		return IR_S_FALSE;

	if (m_pDownload == NULL && FALSE == CreateBtDownload ())
		return IR_ERROR;

	

	if (m_bDlThreadRunning == false)
	{
		m_bDlThreadRunning = m_bDlThreadDoJob = true;
		m_bDlThreadNeedStop = false;
		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadBtDownloadManager, this, 0, &dw));
	}

	return IR_SUCCESS;
}

void vmsBtDownloadManager::StopDownloading()
{
	m_bDlThreadNeedStop = true;
	if (isSeeding ())
		StopSeeding ();
	else if (m_pDownload && IsDone ())
		DeleteBtDownload ();
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
	dwNeedSize += sizeof (int) + sizeof (BYTE) * m_info.vFilesPriorities.size ();
	dwNeedSize += sizeof (m_fRequiredRatio);

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
	ASSERT (m_info.pfProgress != NULL);
	CopyMemory (pb, m_info.pfProgress, get_FileCount () * sizeof (float));
	pb += get_FileCount () * sizeof (float);

	i = m_info.strCurrentTracker.GetLength ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	CopyMemory (pb, m_info.strCurrentTracker, i);
	pb += i;

	UINT64 nUploadedBytes; nUploadedBytes = get_TotalUploadedByteCount ();
	CHECK_SIZE (sizeof (nUploadedBytes));
	CopyMemory (pb, &nUploadedBytes, sizeof (nUploadedBytes));
	pb += sizeof (nUploadedBytes);

	CHECK_SIZE (sizeof (m_info.fShareRating));
	CopyMemory (pb, &m_info.fShareRating, sizeof (m_info.fShareRating));
	pb += sizeof (m_info.fShareRating);

	UINT64 nWastedBytes; nWastedBytes = get_WastedByteCount ();
	CHECK_SIZE (sizeof (nWastedBytes));
	CopyMemory (pb, &nWastedBytes, sizeof (nWastedBytes));
	pb += sizeof (nWastedBytes);

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

	i = m_info.vFilesPriorities.size ();
	CHECK_SIZE (sizeof (int));
	CopyMemory (pb, &i, sizeof (int));
	pb += sizeof (int);
	CHECK_SIZE (i);
	for (j = 0; j < i; j++)
		*pb++ = m_info.vFilesPriorities [j];

	

	CHECK_SIZE (sizeof (m_fRequiredRatio));
	*((float*)pb) = m_fRequiredRatio;
	pb += sizeof (m_fRequiredRatio);
	
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

		if (wVer > 12)
		{
			CopyMemory (&i, lpBuffer, sizeof (int));
			lpBuffer += sizeof (int);
			m_info.vFilesPriorities.clear ();
			while (i--)
				m_info.vFilesPriorities.push_back (*lpBuffer++);

			if (wVer > 13)
			{
				m_fRequiredRatio = *((float*)lpBuffer);
				lpBuffer += sizeof (float);	
			}
		}
	}
	else
	{
		m_info.pfProgress = new float [get_FileCount () * sizeof (float)];
		for (int i = get_FileCount () - 1; i >= 0; i--)
			m_info.pfProgress [i] = 0;

		PostCreateTorrentObject ();
	}

	m_info.dwFlags = *((LPDWORD)lpBuffer);
	lpBuffer += sizeof (DWORD);

	m_uLowSpeedMaxTime = *((LPUINT)lpBuffer);
	lpBuffer += sizeof (UINT);

	m_uTrafficLimit = *((LPUINT)lpBuffer);
	lpBuffer += sizeof (UINT);

	*pdwSize = lpBuffer - pbStart;

	if (m_info.bDone)
	{
		for (int i = 0; i < get_FileCount (); i++)
		{
			if (GetFileAttributes (get_OutputFilePathName (i)) == DWORD (-1))
			{
				if (m_info.vFilesPriorities.size () == 0 || m_info.vFilesPriorities [i] != 0)
					break;
			}
		}

		if (i != get_FileCount ())
		{
			SAFE_DELETE_ARRAY (m_info.pbFastResumeData);
			m_info.dwFastResumeDataSize = 0;
			for (int i = get_FileCount () - 1; i >= 0; i--)
				m_info.pfProgress [i] = 0;
			m_info.nUploadedBytes = 0;
			m_info.fShareRating = 0;
			m_info.nWastedBytes = 0;
			m_info.fPercentDone = 0;
			m_info.bDone = FALSE;
			m_info.nDownloadedBytes = 0;
			m_nUsingBtDownload = 0;	
			m_cache.nDownloadingSectionCount = 0;
			if (m_pfnEvHandler)
				RaiseEvent (BTDME_FATAL_ERROR);
			else
				m_bWasFatalErrorInLoadState = true;			
		}
	}

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
	vmsBtDownloadManager *pthis = (vmsBtDownloadManager*)lp;
	vmsBtDownloadStateEx enPrevState = BTDSE_QUEUED;

	pthis->m_cache.nDownloadingSectionCount = 0;
	pthis->m_cache.nDownloadingSectionCount_time.Now ();

	pthis->RaiseEvent (BTDME_DOWNLOAD_STARTED);

	while (pthis->get_State () == BTDSE_QUEUED && pthis->m_bDlThreadNeedStop == false)
		Sleep (100);

	bool bDownloading = false;
	bool bMayFilesEvent = true;
	fsTicksMgr time0Speed;

	while (pthis->IsBtDownloadRunning () && pthis->m_bDlThreadNeedStop == false)
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
		else
		{
			fsTicksMgr time;
			if (time - pthis->m_cache.nDownloadingSectionCount_time > 3000)
			{
				pthis->GetDownloadingSectionCount (false);
				pthis->GetDownloadedBytesCount (false);
			}

			if (pthis->GetSpeed () == 0)
			{
				fsTicksMgr now;
				
				if (now - time0Speed > 120 * 1000)
				{
					
					pthis->m_pDownload->Pause ();
					pthis->SaveBtDownloadState ();
					pthis->DeleteBtDownload ();
					pthis->CreateBtDownload ();
					
					time0Speed.Now ();
					bDownloading = false;
					enPrevState = BTDSE_QUEUED;
					while (pthis->get_State () == BTDSE_QUEUED && pthis->m_bDlThreadNeedStop == false)
						Sleep (100);
				}
			}
			else
			{
				time0Speed.Now (); 
			}
		}
	}

	
	pthis->m_cache.nDownloadingSectionCount = 0;
	pthis->GetDownloadedBytesCount (false);

	if (pthis->m_pDownload != NULL &&
			(pthis->m_bDlThreadNeedStop || 
				(pthis->get_State () != BTDSE_SEEDING && pthis->get_State () != BTDSE_FINISHED) ||
				(pthis->m_info.dwFlags & BTDF_DISABLE_SEEDING) != 0 ||
				_DldsMgr.AllowStartNewDownloads () == FALSE))
	{
		pthis->m_pDownload->Pause ();
		pthis->SaveBtDownloadState ();
		pthis->DeleteBtDownload ();
	}
	else
	{
		pthis->SaveBtDownloadState ();
	}

	pthis->m_bDlThreadDoJob = false;

	try{pthis->RaiseEvent (BTDME_DOWNLOAD_STOPPED_OR_DONE);}catch(...){}

	if (pthis->get_State () == BTDSE_SEEDING || pthis->get_State () == BTDSE_FINISHED)
	{
		pthis->AddToDldsList ();
		pthis->RaiseEvent (BTDME_SEEDING);
	}

	pthis->m_bDlThreadRunning = false;

	return 0;
}

void vmsBtDownloadManager::StopThread()
{
	if (m_bDlThreadRunning)
	{
		StopDownloading ();
		while (m_bDlThreadRunning)
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
	m_info.fShareRating = getRatio ();
	m_info.fPercentDone = GetPercentDone ();
	m_info.bDone = IsDone ();

	SaveBtDownloadState_Pieces ();

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
		get_State () != BTDSE_CHECKING_FILES && get_State () != BTDSE_ALLOCATING;
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
			if (getRequiredRatio () && getRatio () >= getRequiredRatio ())
				return; 

			CreateBtDownload ();

			m_bDlThreadRunning = true;
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

	bool bNeedSaveFRD = false;
	
	while (pthis->m_bDlThreadNeedStop == false && pthis->m_pDownload != NULL)
	{
		vmsBtDownloadStateEx s = pthis->get_State ();

		if (s == BTDSE_CHECKING_FILES)
		{
			if (!bNeedSaveFRD)
			{
				bNeedSaveFRD = true;
				pthis->RaiseEvent (BTDME_CHECKING_FILES);
			}
		}

		if (s != BTDSE_QUEUED && s != BTDSE_CHECKING_FILES && s != BTDSE_CONNECTING_TRACKER)
			break;

		if (s == BTDSE_CONNECTING_TRACKER && 
				(pthis->m_info.bDone == FALSE && pthis->GetPercentDone () != 100.0f))
			break;

		Sleep (5);
	}

	if (bNeedSaveFRD)
		pthis->SaveBtDownloadState ();

	if (pthis->m_bDlThreadNeedStop == false && pthis->m_pDownload != NULL)
	{
		if ((pthis->get_State () != BTDSE_SEEDING && pthis->get_State () != BTDSE_FINISHED) || 
				(pthis->m_info.dwFlags & BTDF_DISABLE_SEEDING) != 0)
			pthis->DeleteBtDownload ();
		else
		{
			pthis->AddToDldsList ();
			pthis->RaiseEvent (BTDME_SEEDING);
		}
	}

	pthis->m_bDlThreadRunning = false;

	return 0;
}

void vmsBtDownloadManager::StopSeeding()
{
	if (get_State () == BTDSE_SEEDING || get_State () == BTDSE_FINISHED)
	{
		DeleteFromDldsList ();
		SaveBtDownloadState ();
		DeleteBtDownload ();
		RaiseEvent (BTDME_STOP_SEEDING);
	}
}

std::wstring vmsBtDownloadManager::get_FileNameW(int nIndex)
{
	ASSERT (m_pTorrent != NULL);
	char sz [MY_MAX_PATH] = "";
	m_pTorrent->get_FileName2 (nIndex, sz, MY_MAX_PATH);
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

int vmsBtDownloadManager::get_CurrentTaskProgress()
{
	if (m_pDownload == NULL)
		return -1;
	return m_pDownload->get_CurrentTaskProgress ();
}

void vmsBtDownloadManager::SaveBtDownloadState_Pieces()
{
	m_info.vPieces.clear ();
	int ns = GetNumberOfSections ();
	bool *pbPieces = (bool*)_alloca (get_PieceCount () * sizeof (bool));
	m_pDownload->get_PiecesProgressMap (pbPieces, NULL);
	for (int i = 0; i < ns; i++)
		m_info.vPieces.push_back (pbPieces [i]);
}

void vmsBtDownloadManager::GetSectionsInfo(std::vector <vmsSectionInfo> &v)
{
	InterlockedIncrement (&m_nUsingBtDownload);

	v.clear ();

	vmsSectionInfo sect;
	
	UINT64 uTotal = GetTotalFilesSize ();
	int ns = GetNumberOfSections ();
	UINT64 uPerPiece = uTotal / ns;
	BOOL bDSCBR = IsDownloadStatCanBeRead () && get_State () == BTDSE_DOWNLOADING;

	for (int i = 0; i < ns; i++)
	{
		sect.uDStart = i * uPerPiece;
		sect.uDEnd = i == ns-1 ? uTotal : sect.uDStart + uPerPiece - 1;
		bool bPC = false;
		if (bDSCBR)
			bPC = m_pDownload->is_PieceCompleted (i);
		else if (m_info.vPieces.size ())
			bPC = m_info.vPieces [i];
		sect.uDCurrent = bPC ? sect.uDEnd : sect.uDStart;
		v.push_back (sect);
	}
	
	InterlockedDecrement (&m_nUsingBtDownload);
}

UINT64 vmsBtDownloadManager::get_SplittedByteCountAtBeginningOfFile()
{
	InterlockedIncrement (&m_nUsingBtDownload);
	
	UINT64 uTotal = GetTotalFilesSize ();
	int ns = GetNumberOfSections ();
	UINT64 uPerPiece = uTotal / ns;
	
	UINT64 uRes = 0;
	bool bIDSCBR = IsDownloadStatCanBeRead () != 0;

	for (int i = 0; i < ns; i++)
	{
		if (false == (bIDSCBR ? m_pDownload->is_PieceCompleted (i) : m_info.vPieces [i]))
			break;

		uRes += uPerPiece;
	}
	
	InterlockedDecrement (&m_nUsingBtDownload);

	return uRes;
}

fsString vmsBtDownloadManager::get_RootFolderName()
{
	if (get_FileCount () == 1)
		return "";

	int nOffset = 0;
	
	for (int i = 0; i < get_FileCount (); i++)
	{
		fsString str = get_FileName (i);
		if (nOffset == 0)
		{
			LPCSTR psz = strchr (str, '\\');
			if (psz)
				nOffset = psz - str + 1;
			else
				break;
		}
		else
		{
			LPCSTR psz = strchr (str, '\\');
			int nOffset2 = 0;
			if (psz)
				nOffset2 = psz - str + 1;
			if (nOffset2 != nOffset || strnicmp (str, get_FileName (i-1), nOffset))
			{
				nOffset = 0;
				break;
			}
		}
	}

	if (nOffset == 0)
		return "";

	char sz [MY_MAX_PATH];
	lstrcpyn (sz, get_FileName (0), nOffset);
	return sz;	
}

BOOL vmsBtDownloadManager::isSeeding()
{
	return get_State () == BTDSE_SEEDING || get_State () == BTDSE_FINISHED;
}

void vmsBtDownloadManager::GetFilesTree(fs::ListTree <vmsFile> &tree)
{
	int cFiles = get_FileCount ();

	for (int i = 0; i < cFiles; i++)
	{
		fsString strFile = get_FileName (i);
		addFileToTree (&tree, strFile, i);
	}

	tree.GetData ().nIndex = -1;
	calculateFoldersSizesInTree (&tree);
}

void vmsBtDownloadManager::addFileToTree(fs::ListTree <vmsFile> *pTree, LPCSTR pszFile, int nFileIndex)
{
	fsString strFilePart;
	while (*pszFile && *pszFile != '\\' && *pszFile != '/')
		strFilePart += *pszFile++;
	if (*pszFile)
		pszFile++;

	fs::ListTree <vmsFile> *ptLeaf = pTree;

	for (int i = 0; i < pTree->GetLeafCount (); i++)
	{
		if (lstrcmpi (pTree->GetLeaf (i)->GetData ().strName, strFilePart) == 0)
		{
			ptLeaf = pTree->GetLeaf (i);
			break;
		}
	}

	if (*pszFile)
	{
		if (ptLeaf == pTree)
		{
			vmsFile file; 
			file.strName = strFilePart;
			file.nIndex = -1;
			file.nFileSize = _UI64_MAX;
			ptLeaf = pTree->AddLeaf (file);
		}

		addFileToTree (ptLeaf, pszFile, nFileIndex);
	}
	else
	{
		vmsFile file; 
		file.strName = strFilePart;
		file.nIndex = nFileIndex;
		file.nFileSize = get_FileSize (nFileIndex);
		file.iPriority = m_info.vFilesPriorities.size () ? m_info.vFilesPriorities [nFileIndex] : 1;
		ptLeaf->AddLeaf (file);
	}
}

void vmsBtDownloadManager::calculateFoldersSizesInTree(fs::ListTree <vmsFile> *pTree)
{
	ASSERT (pTree->GetData ().nIndex == -1);
	if (pTree->GetData ().nIndex != -1)
		return;

	pTree->GetData ().nFileSize = 0;

	for (int i = 0; i < pTree->GetLeafCount (); i++)
	{
		if (pTree->GetLeaf (i)->GetData ().nFileSize == _UI64_MAX)
			calculateFoldersSizesInTree (pTree->GetLeaf (i));
		pTree->GetData ().nFileSize += pTree->GetLeaf (i)->GetData ().nFileSize;
	}
}

BOOL vmsBtDownloadManager::LoadTorrentFile(LPCSTR pszFile)
{
	if (m_pTorrent)
		return TRUE;
	
	m_pTorrent = _BT.CreateTorrentFileObject ();
	if (m_pTorrent == NULL)
		return FALSE;
	
	if (FALSE == m_pTorrent->LoadFromFile (pszFile))
	{
		m_pTorrent->Release ();
		m_pTorrent = NULL;
		return FALSE;
	}

	return TRUE;
}

void vmsBtDownloadManager::PrioritizeFiles(int *piPriorities)
{
	int cFiles = get_FileCount ();
	bool bWasEmpty = m_info.vFilesPriorities.size () == 0;

	for (int i = 0; i < cFiles; i++)
	{
		if (bWasEmpty)
			m_info.vFilesPriorities.push_back ((BYTE)piPriorities [i]);
		else
			m_info.vFilesPriorities [i] = (BYTE)piPriorities [i];
	}

	ApplyNewFilesPriorities ();
}

void vmsBtDownloadManager::CalculateFilesPieces()
{
	if (m_vFilesPieces.size ())
		return;
	int cFiles = get_FileCount ();
	UINT64 pos = 0;
	UINT64 piece_size = get_PieceSize ();
	for (int i = 0; i < cFiles; i++)
	{
		UINT64 file_size = get_FileSize (i);
		m_vFilesPieces.push_back (_inc_FilePieces (
			(int)(pos / piece_size), (int)((pos+file_size-1) / piece_size)));
		pos += file_size;
	}
}

void vmsBtDownloadManager::getPartialDownloadProgress(UINT64 *pnBytesToDownload, UINT64 *pnBytesDownloaded)
{
	if (m_info.vFilesPriorities.size () == 0)
	{
		*pnBytesDownloaded = GetDownloadedBytesCount ();
		*pnBytesToDownload = GetTotalFilesSize ();
		return;
	}

	fsTicksMgr now;
	if (now - m_cache.nPartial_time < 1000)
	{
		*pnBytesDownloaded = m_cache.nPartial_BytesDownloaded;
		*pnBytesToDownload = m_cache.nPartial_BytesToDownload;
		return;
	}

	*pnBytesDownloaded = *pnBytesToDownload = 0;

	CalculateFilesPieces ();

	bool *pbPieces = (bool*)_alloca (get_PieceCount () * sizeof (bool));
	int piece_size = get_PieceSize ();

	InterlockedIncrement (&m_nUsingBtDownload);
	int cPieces = 0;
	if (IsDownloadStatCanBeRead ())
		cPieces = m_pDownload->get_PiecesProgressMap (pbPieces, NULL);
	InterlockedDecrement (&m_nUsingBtDownload);
	if (cPieces != get_PieceCount ())
	{
		cPieces = get_PieceCount ();
		for (size_t i = 0; i < m_info.vPieces.size (); i++)
			pbPieces [i] = m_info.vPieces [i];
	}

	int last_piece = -1;
	for (size_t i = 0; i < m_info.vFilesPriorities.size (); i++)
	{
		if (m_info.vFilesPriorities [i] == 0)
			continue;

		_inc_FilePieces &fp = m_vFilesPieces [i];
		
		for (int j = fp.nStartPiece; j <= fp.nEndPiece; j++)
		{
			if (j == last_piece)
				continue;
			if (j == cPieces - 1)
				piece_size = (int) (GetTotalFilesSize () % piece_size);
			*pnBytesToDownload += piece_size;
			if (pbPieces [j])
				*pnBytesDownloaded += piece_size;
			last_piece = j;
		}
	}

	m_cache.nPartial_BytesDownloaded = *pnBytesDownloaded;
	m_cache.nPartial_BytesToDownload = *pnBytesToDownload;
	m_cache.nPartial_time.Now ();
}

int vmsBtDownloadManager::getFilePriority(int nFileIndex) const
{
	return m_info.vFilesPriorities.empty () ? 1 : m_info.vFilesPriorities [nFileIndex];
}

void vmsBtDownloadManager::setFilePriority(int nFileIndex, int iPriority, bool bApply)
{
	if (m_info.vFilesPriorities.empty ())
	{
		if (iPriority == 1)
			return;
		for (int i = get_FileCount () - 1; i >= 0; i--)
			m_info.vFilesPriorities.push_back (1);
	}

	ASSERT (iPriority >= 0 && iPriority <= 7);

	m_info.vFilesPriorities [nFileIndex] = (BYTE)iPriority;

	if (bApply)
		ApplyNewFilesPriorities ();
}

void vmsBtDownloadManager::ApplyNewFilesPriorities()
{
	if (isSeeding ())
	{
		StopSeeding ();
		m_info.bDone = FALSE;
		StartDownloading ();
	}
	else if (m_pDownload)
	{
		InterlockedIncrement (&m_nUsingBtDownload);
		int *pi = new int [m_info.vFilesPriorities.size ()];
		for (size_t i = 0; i < m_info.vFilesPriorities.size (); i++)
			pi [i] = m_info.vFilesPriorities [i];
		m_pDownload->PrioritizeFiles (pi);
		InterlockedDecrement (&m_nUsingBtDownload);		
		delete [] pi;
	}
	
	
	if (!m_info.strOutputPath.IsEmpty ())
	{
		USES_CONVERSION;
		std::wstring wstrSrcPath = A2W (m_info.strOutputPath);
		if (wstrSrcPath [wstrSrcPath.length () - 1] != '\\')
			wstrSrcPath += '\\';
		
		for (int i = 0; i < (int)m_info.vFilesPriorities.size (); i++)
		{
			std::wstring wstr = wstrSrcPath; wstr += get_FileNameW (i);
			DWORD dw = GetFileAttributesW (wstr.c_str ());
			if (dw != DWORD (-1))
			{
				if (m_info.vFilesPriorities [i] == 0 && (dw & FILE_ATTRIBUTE_HIDDEN) == 0)
					SetFileAttributesW (wstr.c_str (), dw | FILE_ATTRIBUTE_HIDDEN);
				else if (m_info.vFilesPriorities [i] != 0 && (dw & FILE_ATTRIBUTE_HIDDEN) != 0)
					SetFileAttributesW (wstr.c_str (), dw & (~FILE_ATTRIBUTE_HIDDEN));
			}
		}
	}
}

bool vmsBtDownloadManager::isSeedingEnabled() const
{
	return (m_info.dwFlags & BTDF_DISABLE_SEEDING) == 0;
}

void vmsBtDownloadManager::setRequiredRatio(float f)
{
	m_fRequiredRatio = f;
}

float vmsBtDownloadManager::getRequiredRatio()
{
	return m_fRequiredRatio;
}

void vmsBtDownloadManager::Shutdown()
{
	if (m_htDlds)
	{
		EnterCriticalSection (m_pcsvpDlds);
		TerminateThread (m_htDlds, 0);
		m_htDlds = NULL;
		LeaveCriticalSection (m_pcsvpDlds);
	}
}

DWORD WINAPI vmsBtDownloadManager::_threadDlds(LPVOID)
{
	for (;;)
	{
		Sleep (30*1000);
		EnterCriticalSection (m_pcsvpDlds);

		for (size_t i = 0; i < m_pvpDlds->size (); i++)
		{
			if (m_pvpDlds->at (i)->getRequiredRatio () && 
					m_pvpDlds->at (i)->getRatio () >= m_pvpDlds->at (i)->getRequiredRatio () &&
					m_pvpDlds->at (i)->isSeeding ())
			{
				vmsBtDownloadManager *dld = m_pvpDlds->at (i);
				dld->StopSeeding ();
			}
		}

		LeaveCriticalSection (m_pcsvpDlds);		
	}
}

void vmsBtDownloadManager::DeleteFromDldsList()
{
	EnterCriticalSection (m_pcsvpDlds);
	for (size_t i = 0; i < m_pvpDlds->size (); i++)
	{
		if (m_pvpDlds->at (i) == this)
		{
			m_pvpDlds->erase (m_pvpDlds->begin () + i);
			break;
		}
	}
	LeaveCriticalSection (m_pcsvpDlds);
}

void vmsBtDownloadManager::AddToDldsList()
{
	EnterCriticalSection (m_pcsvpDlds);
	for (size_t i = 0; i < m_pvpDlds->size (); i++)
	{
		if (m_pvpDlds->at (i) == this)
			return;
	}
	m_pvpDlds->push_back (this);
	LeaveCriticalSection (m_pcsvpDlds);
}
