/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "Dlg_Convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlg_Convert::CDlg_Convert(vmsMediaFileConvertSettings *stgs, CWnd* pParent )
	: CDialog(CDlg_Convert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Convert)
		
	//}}AFX_DATA_INIT
	m_bCustomizingDefSettings = false;
	m_stgs = stgs;
}

void CDlg_Convert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Convert)
	DDX_Control(pDX, IDC_VIDEOSIZE, m_wndVideoSize);
	DDX_Control(pDX, IDC_QUALITY, m_wndQuality);
	DDX_Control(pDX, IDC_FORMAT, m_wndFormat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlg_Convert, CDialog)
	//{{AFX_MSG_MAP(CDlg_Convert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SIZE _aQualityRates [] = {
	{384, 64},	
	{512, 80}, 
	{640, 96},
	{800, 96},
	{1000, 128},
	{1200, 128},
};
UINT _anQualityNames [] = {
	L_VERYLOW_2,
	L_LOW_2,
	L_NORMAL_2,
	L_AVERAGE_2,
	L_HIGH_2,
	L_VERYHIGH_2,
};

LPCTSTR _apszFormats [] = {
	
	NULL,						_T("avi"), _T("avi"),			_T("msmpeg4v2"), _T("mp3"),
	NULL,						_T("avi"), _T("avi"),			_T("mpeg4"), _T("mp3"),
	_T("FLV (*.flv)"),			_T("flv"), _T("flv"),			_T(""), _T(""),
	_T("WMV (*.wmv)") ,			_T("wmv"), _T("asf"),			_T("wmv2"), _T("wmav2"),
	_T("MPEG1 (*.mpg)"),		_T("mpg"), _T("mpeg"),			_T("mpeg1video"), _T("mp3"),
	_T("MPEG2 (*.mpg)"),		_T("mpg"), _T("mpegts"),		_T("mpeg2video"), _T("aac"),
	NULL,						_T("mp4"), _T("mp4"),			_T("mpeg4"), _T("aac"),
	_T("MP3 (*.mp3)"),			_T("mp3"), _T("mp3"),			_T("none"), _T("mp3"),
};

SIZE _aVideoSizes [] = {
	
	{-1, -1},		
	{160, 120},
	{240, 180},
	{320, 240},
	{640, 480},
	{720, 480},
	{720, 756},
};

BOOL CDlg_Convert::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_apszFormats [0*5] = LS (L_AVI_USING_MPEG);
	_apszFormats [1*5] = LS (L_AVI_USING_XVID);
	_apszFormats [6*5] = LS (L_MP4_FORMAT);

	int i = 0;
	for (i = 0; i < sizeof (_apszFormats) / sizeof (LPCSTR) / 5; i++)
	{
		m_wndFormat.AddString (_apszFormats [i * 5]);
		if (m_stgs->strFormat == _apszFormats [i * 5 + 2] && 
				m_stgs->strVideoCodec == _apszFormats [i * 5 + 3])
			m_wndFormat.SetCurSel (i);
	}
	if (m_wndFormat.GetCurSel () == CB_ERR)
		m_wndFormat.SetCurSel (0);
	
	for (i = 0; i < sizeof (_aVideoSizes) / sizeof (SIZE); i++)
	{
		if (i == 0)
		{
			m_wndVideoSize.AddString (LS (L_DONT_CHANGE));
		}
		else
		{
			CString str;
			str.Format (_T("%d x %d"), _aVideoSizes [i].cx, _aVideoSizes [i].cy);
			m_wndVideoSize.AddString (str);
		}
		if (m_stgs->sizeVideo == _aVideoSizes [i])
			m_wndVideoSize.SetCurSel (i);
	}
	if (m_wndVideoSize.GetCurSel () == CB_ERR)
		m_wndVideoSize.SetCurSel (0);

	int m = 0;
	for (i = 0; i < sizeof (_anQualityNames) / sizeof (UINT); i++)
		m = max (m, lstrlen (LS (_anQualityNames [i])));
	for (i = 0; i < sizeof (_anQualityNames) / sizeof (UINT); i++)
	{
		CString str, str2;
		int k = m - lstrlen (LS (_anQualityNames [i]));
		k = (int)(k * 1.6);
		while (k--)
			str2 += _T(' '); 
		str.Format (_T("%s %s (%s: %dkbps; %s: %dkbps)"), LS (_anQualityNames [i]), str2,
			LS (L_VIDEO_BITRATE), _aQualityRates [i].cx, 
			LS (L_AUDIO_BITRATE), _aQualityRates [i].cy);
		m_wndQuality.AddString (str);

		if (m_stgs->nVideoBitrate == _aQualityRates [i].cx && 
				m_stgs->nAudioBitrate == _aQualityRates [i].cy)
			m_wndQuality.SetCurSel (i);
	}		
	if (m_wndQuality.GetCurSel () == CB_ERR)
		m_wndQuality.SetCurSel (0);

	if (m_bCustomizingDefSettings)
	{
		CheckDlgButton (IDC_SET_AS_DEF, BST_CHECKED);
		GetDlgItem (IDC_SET_AS_DEF)->EnableWindow (FALSE);
	}

	ApplyLanguage ();
	
	return TRUE;  
	              
}

void CDlg_Convert::OnOK() 
{
	int fmt = m_wndFormat.GetCurSel ();
	m_stgs->strExtension = _apszFormats [fmt * 5 + 1];
	m_stgs->strFormat = _apszFormats [fmt * 5 + 2];
	m_stgs->strVideoCodec = _apszFormats [fmt * 5 + 3];
	m_stgs->strAudioCodec = _apszFormats [fmt * 5 + 4];

	int vs = m_wndVideoSize.GetCurSel ();
	m_stgs->sizeVideo = _aVideoSizes [vs];

	int q = m_wndQuality.GetCurSel ();
	m_stgs->nVideoBitrate = _aQualityRates [q].cx;
	m_stgs->nAudioBitrate = _aQualityRates [q].cy;
	
	if (IsDlgButtonChecked (IDC_SET_AS_DEF) == BST_CHECKED)
		vmsMediaConvertMgr::SaveSettingsAsDefault (*m_stgs);
	
	CDialog::OnOK();
}

void CDlg_Convert::ApplyLanguage()
{
	fsDlgLngInfo lnginfo [] =  {
		fsDlgLngInfo (IDC__FORMAT, L_FILE_FORMAT, TRUE),
		fsDlgLngInfo (IDC__VIDEOSIZE, L_VIDEO_SIZE, TRUE),
		fsDlgLngInfo (IDC__QUALITY, L_QUALITY, TRUE),
		fsDlgLngInfo (IDC_SET_AS_DEF, L_REMEMBER_AS_DEFAULT),
		fsDlgLngInfo (IDCANCEL, L_CANCEL),
	};

	_LngMgr.ApplyLanguage (this, lnginfo, sizeof (lnginfo) / sizeof (fsDlgLngInfo), L_CONVERT_OPTS);
}
