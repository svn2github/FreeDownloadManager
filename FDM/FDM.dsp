# Microsoft Developer Studio Project File - Name="FDM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FDM - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FDM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FDM.mak" CFG="FDM - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FDM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FDM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FDM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /Zi /Ot /I "include.add" /I "include.add\dshow" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "CURL_STATICLIB" /D "USE_SSLEAY" /Yu"stdafx.h" /FD /EHa /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "./uploader/fum/" /D "NDEBUG" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 PartMediaPreview/Release/PartMediaPreview.lib FDMCustomized/FDMCustomized_lib/Release/FDMCustomized_lib.lib inetfile.lib alsfolderbrowser.lib Mswsock.lib Hash\Release\hash.lib inetfile/curl_lib/release/curl_lib.lib dxguid.lib wininet.lib rasapi32.lib Ws2_32.lib htmlhelp.lib winmm.lib strmiids.lib Version.lib Iphlpapi.lib libeay32.lib ssleay32.lib Msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"Lib.add" /OPT:REF /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FDM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "include.add" /I "include.add\dshow" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "CURL_STATICLIB" /D "USE_SSLEAY" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "./uploader/fum/" /D "_DEBUG" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 PartMediaPreview\Debug\PartMediaPreview.lib FDMCustomized/FDMCustomized_lib/Debug/FDMCustomized_lib.lib inetfile_d.lib alsFolderBrowserd.lib Hash\Debug\hash.lib InetFile\curl_lib\debug\curl_lib.lib dxguid.lib wininet.lib rasapi32.lib Ws2_32.lib htmlhelp.lib winmm.lib strmiids.lib Version.lib Iphlpapi.lib libeay32d.lib ssleay32d.lib Msimg32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"./debug\fdm.exe" /pdbtype:sept /libpath:"Lib.add"

!ENDIF 

# Begin Target

# Name "FDM - Win32 Release"
# Name "FDM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AERDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BtDld_Files.cpp
# End Source File
# Begin Source File

SOURCE=.\BtDld_General.cpp
# End Source File
# Begin Source File

SOURCE=.\BtDld_Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\BtDld_Seeding.cpp
# End Source File
# Begin Source File

SOURCE=.\BtDldSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\CBD_SeeResultsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CFDM.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientAreaWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBox_Ex.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboBoxTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateBatchDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadFromTorrentFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DDRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity_Progress.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity_Result.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ChooseGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Convert.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreateFVDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreateNewTorrent.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Download.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Download_OnHide.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_MakePortableVer.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Opinions.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Bittorrent.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Flv.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Mirrors.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Monitoring.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Network.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_NewDld.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_TimeLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_History.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_Misc_Advanced.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Notifications.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Page.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_SiteMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SavePassword.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SetBatchNumbers.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ShutdownType.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2_wb_Events.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2_wb_UIHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp_LinkCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_TUMChanged.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Unpack_AskOverwrite.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Unpack_InstAddPack.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_UnpackProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_UnpackSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_WPD_IgnoreList.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_WPD_IL_Add.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateFlvDownloadsByWebPageUrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCustomizeIEMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeletedProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDL.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDownloadsHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportDownloads.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExportHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExtensionsFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFind.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindAdvanced.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPI2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_IfRemind.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_SureToCancel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_WbCustomization.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgramOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSounds.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgT1ToT2.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVidMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVidSeek.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadAlrExistsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_BtPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_CommunityPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_ListPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MirrorsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MonitorPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MonitorPage_SkipList.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NetworkPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NewDownloadPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NotificationsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderPropertiesSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ConnectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_IntegrityPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_MirrorsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_MiscPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ProtocolPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ProxyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadPropertiesSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_Files.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_General.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_Peers.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Deleted.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Downloads.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Groups.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_History.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_InfoFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Opinions.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Opinions_WBEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Progress.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloads_VideoPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadsGroupsComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadsWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FDCButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Fdm.idl
# End Source File
# Begin Source File

SOURCE=".\fdm.rc"
# End Source File
# Begin Source File

SOURCE=".\FdmApp.cpp"
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon.cpp
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon_Content.cpp
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon_OnDownloadDone.cpp
# End Source File
# Begin Source File

SOURCE=.\FdmBalloonContent.cpp
# End Source File
# Begin Source File

SOURCE=.\FDMDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\FDMDownloadsStat.cpp
# End Source File
# Begin Source File

SOURCE=.\FDMFlashVideoDownloads.cpp
# End Source File
# Begin Source File

SOURCE=.\FdmFlvDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\FdmTorrentFilesRcvr.cpp
# End Source File
# Begin Source File

SOURCE=.\FDMUploader.cpp
# End Source File
# Begin Source File

SOURCE=.\FDMUploadPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashVideoDownloadsWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatingInfo_ListCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatingInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatingWndDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatingWndsThread.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\fsAppSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\fsArchiveFileStream.cpp
# End Source File
# Begin Source File

SOURCE=.\fsArchiveInternetStream.cpp
# End Source File
# Begin Source File

SOURCE=.\fsArchiveRebuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\fsArchiveRebuilderStream.cpp
# End Source File
# Begin Source File

SOURCE=.\fsAutorunMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsClipboardCatchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsClipboardMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsCmdHistoryMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsCommandLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\fsContextHelpMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDialogsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDirectXVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDldFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDownloadMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDownloadRegSaver.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDownloads_GroupsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDownloadsHistoryMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDownloadsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsEventsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFDMCmdLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFindDownloadMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsHistoriesMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsIECatchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsIEContextMenuMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsIEUserAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetFileListMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsLangMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsMFCLangMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsMirrorURLsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\fsODMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\fsOpNetIntegrationMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsPluginMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsRASMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsScheduleMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsShellBrowsersEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\fsShellWindowsEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\fsSitePingMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsSitesMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsSoundsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsSpeedMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\fsTicksMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsTrayIconMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsUpdateMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsUrlsToDownloadRegKeyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsWebPageDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\fsWebPageDownloadsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\fsZipArchiveFastRebuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\fsZipArchiveRebuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\Uploader\FUM\fuminterfaces_i.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\FVDownloads_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\grutil.cpp
# End Source File
# Begin Source File

SOURCE=.\HFE_Address.cpp
# End Source File
# Begin Source File

SOURCE=.\HFE_FileList.cpp
# End Source File
# Begin Source File

SOURCE=.\HFE_ListLog.cpp
# End Source File
# Begin Source File

SOURCE=.\HFE_Log.cpp
# End Source File
# Begin Source File

SOURCE=.\HFE_ProtocolPage.cpp
# End Source File
# Begin Source File

SOURCE=.\HFESheet.cpp
# End Source File
# Begin Source File

SOURCE=.\HFEWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\iefdm\iefdm_i.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\iefdm\iefdmdm\iefdmdm_i.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\inetutil.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\mfchelp.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBalloon.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBalloon_Content.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NOWDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlugFrame2Tabs.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginFrame2.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_GeneralPage_FWDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_HistoryPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_MiscPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_SkinsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Prg_UpdatePage.cpp
# End Source File
# Begin Source File

SOURCE=.\PrgSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Schedule_AdvancedPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Schedule_HTSPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Schedule_WTSPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Log.cpp
# End Source File
# Begin Source File

SOURCE=.\ScheduleSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SCRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Sheduler_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\ShedulerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.cpp
# End Source File
# Begin Source File

SOURCE=.\SiteManager_GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SiteManager_PasswordsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SiteManagerSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Sites_AdvancedPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Sites_GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SitesSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SitesWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallTipWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpiderAddPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpiderTasks.cpp
# End Source File
# Begin Source File

SOURCE=.\SpiderTaskTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SpiderWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\system.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Torrents_Tasks.cpp
# End Source File
# Begin Source File

SOURCE=.\TorrentsWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIconPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UEDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UIThread.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadsWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlWithLoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vms7zipArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\vms7zipArchiveExtractCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\vms7zipFormatDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsAnimatedGIF.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsAppSettingsStore.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsAppTipsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsAppUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsAppVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveRAR.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsBatchList.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsBinaryFileVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsBtDownloadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsBtSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsComDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsCommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDataFile.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDialogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDocHostUIHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadMgrEx.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadsGroupsMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadsListHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsDrawHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiDownloadImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiDownloadsMgrImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFdmAppMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFile.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFileExtensionInOs.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFileRecentList.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFilesToDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFileUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxExtensionInstaller.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxMonitoring.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsFlvSniffInjector.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsImage.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsImage_CtxFree.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsInternetSession.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsMaliciousDownloadChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsMediaConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsMediaConvertMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsMetalinkFile.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsMozillaPrefs.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsNotEverywhereSupportedFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsObject.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsPluginSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsProcessesInjector.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsProcessList.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsRegisteredApp.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSecurity.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSharedData.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSimpleFileDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSingleLineTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSkinMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSourceCodeLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSpeedTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsStringParser.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsStringRecentList.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsTorrentExtension.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsTreeCtrlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUiFonts.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUiLinkWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUiStaticTextWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUiWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUiWindowsHoster.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUnRARDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUploaderDllPersist.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUploadsDllCaller.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsUploadsDllCallerSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsVideoSiteHtmlCodeParser.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsWebBrowserEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsWindowsStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\vmsXmlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaitForConfirmationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\WebBrowser2Customization.cpp
# End Source File
# Begin Source File

SOURCE=.\WgUrlListReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\WGUrlReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnd_Banner.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnd_DownloadProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\Wnd_FDMGetCustVer_Btn.cpp
# End Source File
# Begin Source File

SOURCE=.\WndLog.cpp
# End Source File
# Begin Source File

SOURCE=.\WndSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\WPDS_FilesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WPDS_GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WPDS_ImagesPages.cpp
# End Source File
# Begin Source File

SOURCE=.\WPDS_Sheet.cpp
# End Source File
# Begin Source File

SOURCE=.\WPDS_WebPagesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipPreviewDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AERDlg.h
# End Source File
# Begin Source File

SOURCE=.\AppStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\BtDld_Files.h
# End Source File
# Begin Source File

SOURCE=.\BtDld_General.h
# End Source File
# Begin Source File

SOURCE=.\BtDld_Misc.h
# End Source File
# Begin Source File

SOURCE=.\BtDld_Seeding.h
# End Source File
# Begin Source File

SOURCE=.\BtDldSheet.h
# End Source File
# Begin Source File

SOURCE=.\CBD_SeeResultsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CFDM.h
# End Source File
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=.\ClientAreaWnd.h
# End Source File
# Begin Source File

SOURCE=.\ColumnTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ComboBox_Ex.h
# End Source File
# Begin Source File

SOURCE=.\ComboBoxTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\CreateBatchDownload.h
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadFromTorrentFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadListDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateDownloadsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DDRDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity_Progress.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CheckFileIntegrity_Result.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ChooseGroup.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Convert.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreateFVDownload.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreateNewTorrent.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Download.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Download_OnHide.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_MakePortableVer.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Opinions.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Bittorrent.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Flv.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Mirrors.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Monitoring.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_Network.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_NewDld.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Downloads_TimeLimit.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_History.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_Misc.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_General_Misc_Advanced.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Notifications.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_Page.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Options_SiteMgr.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SavePassword.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SetBatchNumbers.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ShutdownType.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2_wb_Events.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp2_wb_UIHandler.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpreadHelp_LinkCode.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_TUMChanged.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Unpack_AskOverwrite.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Unpack_InstAddPack.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_UnpackProgress.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_UnpackSettings.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_WPD_IgnoreList.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_WPD_IL_Add.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateFlvDownloads.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateFlvDownloadsByWebPageUrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgCustomizeIEMenu.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeletedProperties.h
# End Source File
# Begin Source File

SOURCE=.\DlgDL.h
# End Source File
# Begin Source File

SOURCE=.\DlgDownloadsHistory.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportDownloads.h
# End Source File
# Begin Source File

SOURCE=.\DlgExportHistory.h
# End Source File
# Begin Source File

SOURCE=.\DlgExtensionsFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgFind.h
# End Source File
# Begin Source File

SOURCE=.\DlgFindAdvanced.h
# End Source File
# Begin Source File

SOURCE=.\DlgPI2.h
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_IfRemind.h
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_SureToCancel.h
# End Source File
# Begin Source File

SOURCE=.\DlgPI2_WbCustomization.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgramOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgSounds.h
# End Source File
# Begin Source File

SOURCE=.\DlgT1ToT2.h
# End Source File
# Begin Source File

SOURCE=.\DlgVidMan.h
# End Source File
# Begin Source File

SOURCE=.\DlgVidSeek.h
# End Source File
# Begin Source File

SOURCE=.\DownloadAlrExistsDlg.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_BtPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_CommunityPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_ListPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MirrorsPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MonitorPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_MonitorPage_SkipList.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NetworkPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NewDownloadPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderProperties_NotificationsPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderPropertiesSheet.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ConnectionPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_IntegrityPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_MirrorsPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_MiscPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ProtocolPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadProperties_ProxyPage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadPropertiesSheet.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_Files.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_General.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Bittorrent_Peers.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Deleted.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Downloads.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Groups.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_History.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Info.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_InfoFrame.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Log.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Opinions.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Opinions_WBEvents.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Progress.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\Downloads_VideoPreview.h
# End Source File
# Begin Source File

SOURCE=.\DownloadsGroupsComboBox.h
# End Source File
# Begin Source File

SOURCE=.\DownloadsWnd.h
# End Source File
# Begin Source File

SOURCE=.\EnterKeyDlg.h
# End Source File
# Begin Source File

SOURCE=.\FDCButton.h
# End Source File
# Begin Source File

SOURCE=".\FdmApp.h"
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon.h
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon_Content.h
# End Source File
# Begin Source File

SOURCE=.\FdmBalloon_OnDownloadDone.h
# End Source File
# Begin Source File

SOURCE=.\FdmBalloonContent.h
# End Source File
# Begin Source File

SOURCE=.\FDMDownload.h
# End Source File
# Begin Source File

SOURCE=.\FDMDownloadsStat.h
# End Source File
# Begin Source File

SOURCE=.\FDMFlashVideoDownloads.h
# End Source File
# Begin Source File

SOURCE=.\FdmFlvDownload.h
# End Source File
# Begin Source File

SOURCE=.\FdmTorrentFilesRcvr.h
# End Source File
# Begin Source File

SOURCE=.\FDMUploader.h
# End Source File
# Begin Source File

SOURCE=.\FDMUploadPackage.h
# End Source File
# Begin Source File

SOURCE=.\FlashVideoDownloadsWnd.h
# End Source File
# Begin Source File

SOURCE=.\FloatingInfo_ListCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\FloatingInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\FloatingWnd.h
# End Source File
# Begin Source File

SOURCE=.\FloatingWndDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\FloatingWndsThread.h
# End Source File
# Begin Source File

SOURCE=.\FolderBrowser.h
# End Source File
# Begin Source File

SOURCE=.\fsAppSettings.h
# End Source File
# Begin Source File

SOURCE=.\fsArchiveFileStream.h
# End Source File
# Begin Source File

SOURCE=.\fsArchiveInternetStream.h
# End Source File
# Begin Source File

SOURCE=.\fsArchiveRebuilder.h
# End Source File
# Begin Source File

SOURCE=.\fsArchiveRebuilderStream.h
# End Source File
# Begin Source File

SOURCE=.\fsAutorunMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsClipboardCatchMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsClipboardMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsCmdHistoryMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsCommandLineParser.h
# End Source File
# Begin Source File

SOURCE=.\fsContextHelpMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsDialogsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsDirectXVersion.h
# End Source File
# Begin Source File

SOURCE=.\fsDldFilter.h
# End Source File
# Begin Source File

SOURCE=.\fsDownload.h
# End Source File
# Begin Source File

SOURCE=.\fsDownloadMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsDownloadRegSaver.h
# End Source File
# Begin Source File

SOURCE=.\fsDownloads_GroupsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsDownloadsHistoryMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsDownloadsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsEventsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsFDMCmdLineParser.h
# End Source File
# Begin Source File

SOURCE=.\fsFindDownloadMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsHistoriesMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsIECatchMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsIEContextMenuMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsIEUserAgent.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetDownloader.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetFileListMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsLangMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsMFCLangMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsMirrorURLsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsMutex.h
# End Source File
# Begin Source File

SOURCE=.\fsODMenu.h
# End Source File
# Begin Source File

SOURCE=.\fsOpNetIntegrationMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsPluginMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsRASMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsScheduleMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsSitePingMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsSitesMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsSoundsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsSpeedMeter.h
# End Source File
# Begin Source File

SOURCE=.\fsTicksMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsTrayIconMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsUpdateMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsUrlsToDownloadRegKeyMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsWebPageDownloader.h
# End Source File
# Begin Source File

SOURCE=.\fsWebPageDownloadsMgr.h
# End Source File
# Begin Source File

SOURCE=.\fsZipArchiveFastRebuilder.h
# End Source File
# Begin Source File

SOURCE=.\fsZipArchiveRebuilder.h
# End Source File
# Begin Source File

SOURCE=.\FVDownloads_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\GroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\grutil.h
# End Source File
# Begin Source File

SOURCE=.\HFE_Address.h
# End Source File
# Begin Source File

SOURCE=.\HFE_FileList.h
# End Source File
# Begin Source File

SOURCE=.\HFE_ListLog.h
# End Source File
# Begin Source File

SOURCE=.\HFE_Log.h
# End Source File
# Begin Source File

SOURCE=.\HFE_ProtocolPage.h
# End Source File
# Begin Source File

SOURCE=.\HFESheet.h
# End Source File
# Begin Source File

SOURCE=.\HFEWnd.h
# End Source File
# Begin Source File

SOURCE=.\inetutil.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\mfchelp.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\MyBalloon.h
# End Source File
# Begin Source File

SOURCE=.\MyBalloon_Content.h
# End Source File
# Begin Source File

SOURCE=.\MyMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\NOWDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlugFrame2Tabs.h
# End Source File
# Begin Source File

SOURCE=.\plugincmds.h
# End Source File
# Begin Source File

SOURCE=.\PluginFrame2.h
# End Source File
# Begin Source File

SOURCE=.\plugins.h
# End Source File
# Begin Source File

SOURCE=.\Prg_Data.h
# End Source File
# Begin Source File

SOURCE=.\Prg_GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\Prg_GeneralPage_FWDlg.h
# End Source File
# Begin Source File

SOURCE=.\Prg_HistoryPage.h
# End Source File
# Begin Source File

SOURCE=.\Prg_MiscPage.h
# End Source File
# Begin Source File

SOURCE=.\Prg_SkinsPage.h
# End Source File
# Begin Source File

SOURCE=.\Prg_UpdatePage.h
# End Source File
# Begin Source File

SOURCE=.\PrgSheet.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Schedule_AdvancedPage.h
# End Source File
# Begin Source File

SOURCE=.\Schedule_HTSPage.h
# End Source File
# Begin Source File

SOURCE=.\Schedule_WTSPage.h
# End Source File
# Begin Source File

SOURCE=.\Scheduler_Log.h
# End Source File
# Begin Source File

SOURCE=.\ScheduleSheet.h
# End Source File
# Begin Source File

SOURCE=.\SCRDlg.h
# End Source File
# Begin Source File

SOURCE=.\Sheduler_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\ShedulerWnd.h
# End Source File
# Begin Source File

SOURCE=.\shockwaveflash.h
# End Source File
# Begin Source File

SOURCE=.\SiteManager_GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\SiteManager_PasswordsPage.h
# End Source File
# Begin Source File

SOURCE=.\SiteManagerSheet.h
# End Source File
# Begin Source File

SOURCE=.\Sites_AdvancedPage.h
# End Source File
# Begin Source File

SOURCE=.\Sites_GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\SitesSheet.h
# End Source File
# Begin Source File

SOURCE=.\SitesWnd.h
# End Source File
# Begin Source File

SOURCE=.\SmallTipWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpiderAddPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpiderTasks.h
# End Source File
# Begin Source File

SOURCE=.\SpiderTaskTree.h
# End Source File
# Begin Source File

SOURCE=.\SpiderWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# Begin Source File

SOURCE=.\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\Torrents_Tasks.h
# End Source File
# Begin Source File

SOURCE=.\TorrentsWnd.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\UEDlg.h
# End Source File
# Begin Source File

SOURCE=.\UIThread.h
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\UploadsWnd.h
# End Source File
# Begin Source File

SOURCE=.\UrlWithLoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\UrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\VideoWindow.h
# End Source File
# Begin Source File

SOURCE=.\vms7zipArchive.h
# End Source File
# Begin Source File

SOURCE=.\vms7zipArchiveExtractCallback.h
# End Source File
# Begin Source File

SOURCE=.\vms7zipFormatDLL.h
# End Source File
# Begin Source File

SOURCE=.\vmsAnimatedGIF.h
# End Source File
# Begin Source File

SOURCE=.\vmsAppSettingsStore.h
# End Source File
# Begin Source File

SOURCE=.\vmsAppTipsMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsAppUtil.h
# End Source File
# Begin Source File

SOURCE=.\vmsAppVersion.h
# End Source File
# Begin Source File

SOURCE=.\vmsArchive.h
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveCallback.h
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsArchiveRAR.h
# End Source File
# Begin Source File

SOURCE=.\vmsBatchList.h
# End Source File
# Begin Source File

SOURCE=.\vmsBinaryFileVersionInfo.h
# End Source File
# Begin Source File

SOURCE=.\vmsBtDownloadManager.h
# End Source File
# Begin Source File

SOURCE=.\vmsBtSupport.h
# End Source File
# Begin Source File

SOURCE=.\vmsComDLL.h
# End Source File
# Begin Source File

SOURCE=.\vmsCriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\vmsDataFile.h
# End Source File
# Begin Source File

SOURCE=.\vmsDialogHelper.h
# End Source File
# Begin Source File

SOURCE=.\vmsDLL.h
# End Source File
# Begin Source File

SOURCE=.\vmsDocHostUIHandler.h
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadMgrEx.h
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadsGroupsMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsDownloadsListHelper.h
# End Source File
# Begin Source File

SOURCE=.\vmsDrawHelper.h
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiDownloadImpl.h
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiDownloadsMgrImpl.h
# End Source File
# Begin Source File

SOURCE=.\vmsFdmApiImpl.h
# End Source File
# Begin Source File

SOURCE=.\vmsFdmAppMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsFile.h
# End Source File
# Begin Source File

SOURCE=.\vmsFileExtensionInOs.h
# End Source File
# Begin Source File

SOURCE=.\vmsFileRecentList.h
# End Source File
# Begin Source File

SOURCE=.\vmsFilesToDelete.h
# End Source File
# Begin Source File

SOURCE=.\vmsFileUtil.h
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxExtensionInstaller.h
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxMonitoring.h
# End Source File
# Begin Source File

SOURCE=.\vmsFirefoxUtil.h
# End Source File
# Begin Source File

SOURCE=.\vmsFlvSniffInjector.h
# End Source File
# Begin Source File

SOURCE=.\vmsImage.h
# End Source File
# Begin Source File

SOURCE=.\vmsImage_CtxFree.h
# End Source File
# Begin Source File

SOURCE=.\vmsInternetSession.h
# End Source File
# Begin Source File

SOURCE=.\vmsMaliciousDownloadChecker.h
# End Source File
# Begin Source File

SOURCE=.\vmsMediaConverter.h
# End Source File
# Begin Source File

SOURCE=.\vmsMediaConvertMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsMetalinkFile.h
# End Source File
# Begin Source File

SOURCE=.\vmsMozillaPrefs.h
# End Source File
# Begin Source File

SOURCE=.\vmsNotEverywhereSupportedFunctions.h
# End Source File
# Begin Source File

SOURCE=.\vmsObject.h
# End Source File
# Begin Source File

SOURCE=.\vmsObjectSmartPtr.h
# End Source File
# Begin Source File

SOURCE=.\vmsPluginSupport.h
# End Source File
# Begin Source File

SOURCE=.\vmsProcessesInjector.h
# End Source File
# Begin Source File

SOURCE=.\vmsProcessList.h
# End Source File
# Begin Source File

SOURCE=.\vmsRegisteredApp.h
# End Source File
# Begin Source File

SOURCE=.\vmsRenderer.h
# End Source File
# Begin Source File

SOURCE=.\vmsSecurity.h
# End Source File
# Begin Source File

SOURCE=.\vmsSharedData.h
# End Source File
# Begin Source File

SOURCE=.\vmsSimpleFileDownloader.h
# End Source File
# Begin Source File

SOURCE=.\vmsSingleLineTooltip.h
# End Source File
# Begin Source File

SOURCE=.\vmsSkinMgr.h
# End Source File
# Begin Source File

SOURCE=.\vmsSourceCodeLogger.h
# End Source File
# Begin Source File

SOURCE=.\vmsSpeedTracker.h
# End Source File
# Begin Source File

SOURCE=.\vmsStringList.h
# End Source File
# Begin Source File

SOURCE=.\vmsStringParser.h
# End Source File
# Begin Source File

SOURCE=.\vmsStringRecentList.h
# End Source File
# Begin Source File

SOURCE=.\vmsSystem.h
# End Source File
# Begin Source File

SOURCE=.\vmsTheme.h
# End Source File
# Begin Source File

SOURCE=.\vmsTorrentExtension.h
# End Source File
# Begin Source File

SOURCE=.\vmsTreeCtrlHelper.h
# End Source File
# Begin Source File

SOURCE=.\vmsUiFonts.h
# End Source File
# Begin Source File

SOURCE=.\vmsUiLinkWindow.h
# End Source File
# Begin Source File

SOURCE=.\vmsUiStaticTextWindow.h
# End Source File
# Begin Source File

SOURCE=.\vmsUiWindow.h
# End Source File
# Begin Source File

SOURCE=.\vmsUiWindowsHoster.h
# End Source File
# Begin Source File

SOURCE=.\vmsUnRARDLL.h
# End Source File
# Begin Source File

SOURCE=.\vmsUploaderDllPersist.h
# End Source File
# Begin Source File

SOURCE=.\vmsUploadsDllCaller.h
# End Source File
# Begin Source File

SOURCE=.\vmsUploadsDllCallerSettings.h
# End Source File
# Begin Source File

SOURCE=.\vmsVideoSiteHtmlCodeParser.h
# End Source File
# Begin Source File

SOURCE=.\vmsWebBrowserEvents.h
# End Source File
# Begin Source File

SOURCE=.\vmsXmlHelper.h
# End Source File
# Begin Source File

SOURCE=.\WaitDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaitForConfirmationDlg.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\WebBrowser2Customization.h
# End Source File
# Begin Source File

SOURCE=.\WgUrlListReceiver.h
# End Source File
# Begin Source File

SOURCE=.\WGUrlReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Wnd_Banner.h
# End Source File
# Begin Source File

SOURCE=.\Wnd_DownloadProgress.h
# End Source File
# Begin Source File

SOURCE=.\Wnd_FDMGetCustVer_Btn.h
# End Source File
# Begin Source File

SOURCE=.\WndDlDoneNotification.h
# End Source File
# Begin Source File

SOURCE=.\WndLog.h
# End Source File
# Begin Source File

SOURCE=.\WndSplitter.h
# End Source File
# Begin Source File

SOURCE=.\WPDS_FilesPage.h
# End Source File
# Begin Source File

SOURCE=.\WPDS_GeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\WPDS_ImagesPages.h
# End Source File
# Begin Source File

SOURCE=.\WPDS_Sheet.h
# End Source File
# Begin Source File

SOURCE=.\WPDS_WebPagesPage.h
# End Source File
# Begin Source File

SOURCE=.\XInfoTip.h
# End Source File
# Begin Source File

SOURCE=.\ZipPreviewDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\back_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\back_go.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checks.bmp
# End Source File
# Begin Source File

SOURCE=.\res\choosefolder.ico
# End Source File
# Begin Source File

SOURCE=.\res\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\closebtn.ico
# End Source File
# Begin Source File

SOURCE=.\res\creategr.ico
# End Source File
# Begin Source File

SOURCE=.\res\dldsinfo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dldsinfo_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dldtasks_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\download.bmp
# End Source File
# Begin Source File

SOURCE=.\res\downloadstasks.bmp
# End Source File
# Begin Source File

SOURCE=".\res\dropbox.ico"
# End Source File
# Begin Source File

SOURCE=.\res\eng_lngs.bin
# End Source File
# Begin Source File

SOURCE=.\res\Fdm.ico
# End Source File
# Begin Source File

SOURCE=.\res\fdm.ico
# End Source File
# Begin Source File

SOURCE=".\res\fdm.rc2"
# End Source File
# Begin Source File

SOURCE=.\FDM.rgs
# End Source File
# Begin Source File

SOURCE=.\res\fdm_getcver_btn.bmp
# End Source File
# Begin Source File

SOURCE=".\FdmApp.rgs"
# End Source File
# Begin Source File

SOURCE=.\FDMDownload.rgs
# End Source File
# Begin Source File

SOURCE=.\FDMDownloadsStat.rgs
# End Source File
# Begin Source File

SOURCE=.\FDMFlashVideoDownloads.rgs
# End Source File
# Begin Source File

SOURCE=.\FdmFlvDownload.rgs
# End Source File
# Begin Source File

SOURCE=.\FdmTorrentFilesRcvr.rgs
# End Source File
# Begin Source File

SOURCE=.\FDMUploader.rgs
# End Source File
# Begin Source File

SOURCE=.\FDMUploadPackage.rgs
# End Source File
# Begin Source File

SOURCE=.\res\filelist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filelist_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\go.bmp
# End Source File
# Begin Source File

SOURCE=.\res\go.ico
# End Source File
# Begin Source File

SOURCE=.\res\groups.bmp
# End Source File
# Begin Source File

SOURCE=.\res\groupsmenu_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\grpmenu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\logbitma.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo2.ico
# End Source File
# Begin Source File

SOURCE=.\res\logstat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\menu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mute.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mute_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\plugins.bmp
# End Source File
# Begin Source File

SOURCE=.\res\scheduler_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\setfolderdef.ico
# End Source File
# Begin Source File

SOURCE=.\res\settime.ico
# End Source File
# Begin Source File

SOURCE=.\res\sheduler.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sitelist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sitelist_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sortimages.bmp
# End Source File
# Begin Source File

SOURCE=.\res\test_ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\tip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool0_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool0_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool0_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_bt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_bt_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_bt_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_bt_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_dld.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_dld_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_dld_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_hfe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_hfe_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_hfe_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sch.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sch_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sch_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sites.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sites_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sites_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_sites_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_spider.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_spider_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_spider_16_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tool_spider_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tooldlds_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolhfe_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolschedule_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tosel.ico
# End Source File
# Begin Source File

SOURCE=.\res\tounsel.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray_dow.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray_err.ico
# End Source File
# Begin Source File

SOURCE=.\res\tray_unk.ico
# End Source File
# Begin Source File

SOURCE=.\res\vidman.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vidman_d.bmp
# End Source File
# Begin Source File

SOURCE=.\WgUrlListReceiver.rgs
# End Source File
# Begin Source File

SOURCE=.\WGUrlReceiver.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\Language\eng.lng
# End Source File
# Begin Source File

SOURCE=.\res\phrases.txt
# End Source File
# End Target
# End Project
# Section Fdm : {D27CDB6C-AE6D-11CF-96B8-444553540000}
# 	2:5:Class:CShockwaveFlash
# 	2:10:HeaderFile:shockwaveflash.h
# 	2:8:ImplFile:shockwaveflash.cpp
# End Section
# Section Fdm : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section Fdm : {00B19758-C708-00B0-106C-B400F0835601}
# 	1:10:IDR_FDMAPP:102
# End Section
# Section Fdm : {D27CDB6E-AE6D-11CF-96B8-444553540000}
# 	2:21:DefaultSinkHeaderFile:shockwaveflash.h
# 	2:16:DefaultSinkClass:CShockwaveFlash
# End Section
# Section Fdm : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
