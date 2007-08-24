;
;  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
;

; FDM installer 

[Setup]
AppName=Free Download Manager
AppId=Free Download Manager
AppVerName=Free Download Manager 2.5
AppPublisher=FreeDownloadManager.ORG
AppPublisherURL=http://www.freedownloadmanager.org/
AppSupportURL=http://www.freedownloadmanager.org/
AppUpdatesURL=http://www.freedownloadmanager.org/
DefaultDirName={pf}\Free Download Manager
UsePreviousAppDir=yes
UsePreviousGroup=yes
UsePreviousTasks=yes
DefaultGroupName=Free Download Manager
LicenseFile=Free Download Manager\license.txt
Compression=lzma/max
SolidCompression=yes
OutputDir=#Output
OutputBaseFilename=fdminst
PrivilegesRequired=none

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "autorun"; Description: "Launch FDM automatically at Windows startup";

[Files]
; all exe files
Source: "Free Download Manager\fdm.exe"; DestDir: "{app}"; Flags: ignoreversion restartreplace
Source: "Free Download Manager\Updater.exe"; DestDir: "{app}"; Flags: restartreplace
; all htm files
Source: "Free Download Manager\*.htm"; DestDir: "{app}"; Flags: ignoreversion restartreplace
; tlb files
Source: "Free Download Manager\fdm.tlb"; DestDir: "{app}"; Flags: ignoreversion restartreplace
Source: "Free Download Manager\fuminterfaces.tlb"; DestDir: "{commonappdata}\FreeDownloadManager.ORG\files"; Permissions: everyone-full ; Flags: ignoreversion restartreplace regtypelib sharedfile uninsnosharedfileprompt
; help system
Source: "Free Download Manager\Help\*"; DestDir: "{app}\Help"; Flags: ignoreversion restartreplace
; language files
Source: "Free Download Manager\Language\*.lng"; DestDir: "{app}\Language"; Flags: ignoreversion restartreplace
; dbghelp dll
Source: "Free Download Manager\dbghelp.dll"; DestDir: "{app}"; Flags: restartreplace
; IE monitoring dlls
Source: "Free Download Manager\iefdm.dll"; DestDir: "{app}"; Flags: restartreplace
Source: "Free Download Manager\iefdmdm.dll"; DestDir: "{app}"; Flags: restartreplace
Source: "Free Download Manager\iefdm2.dll"; DestDir: "{app}"; Flags: restartreplace
; opera, netscape, mozilla monitoring plugin
Source: "Free Download Manager\npfdm.dll"; DestDir: "{app}"; AfterInstall: UpdateCatchPlugins; Flags: restartreplace
; license file
Source: "Free Download Manager\license.txt"; DestDir: "{app}"; Flags: ignoreversion restartreplace
; customization-free file
Source: "Free Download Manager\fdmcs.dat"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist
; skins
Source: "Free Download Manager\Skins\*"; DestDir: "{app}\Skins"; Flags: recursesubdirs
; msvcp60.dll
Source: "Free Download Manager\msvcp60.dll"; DestDir: "{app}"; Flags: restartreplace
; vs2005 runtime
Source: "Free Download Manager\Microsoft.VC80.CRT.manifest"; DestDir: "{app}"; Flags: restartreplace
Source: "Free Download Manager\msvcp80.dll"; DestDir: "{app}"; Flags: restartreplace
Source: "Free Download Manager\msvcr80.dll"; DestDir: "{app}"; Flags: restartreplace
; FDM banner gif
Source: "Free Download Manager\fdm_01.gif"; DestDir: "{app}"; Flags: ignoreversion restartreplace
; FDM archive support
Source: "Free Download Manager\Archive\*"; DestDir: "{app}\Archive"; Flags: ignoreversion restartreplace recursesubdirs
; FDM integration with Firefox
Source: "Free Download Manager\Firefox\*"; DestDir: "{app}\Firefox"; Flags: restartreplace recursesubdirs
; FDM control web server
Source: "Free Download Manager\Server\*"; DestDir: "{app}\Server"; Flags: restartreplace recursesubdirs
Source: "Free Download Manager\fdmwi.exe"; DestDir: "{app}"; Flags: restartreplace
; bittorrent
Source: "Free Download Manager\fdmbtsupp.dll"; DestDir: "{app}"; Flags: restartreplace
; media converter
Source: "Free Download Manager\MediaConverter.dll"; DestDir: "{app}"; Flags: restartreplace
; swf for playing FLV files using flash player object
Source: "Free Download Manager\player.swf"; DestDir: "{app}"; Flags: restartreplace
; ----------------
;
; helper functions for vista
Source: "Free Download Manager\vistafx.dll"; DestDir: "{app}"; Flags: restartreplace
; helper functions for installer
Source: "Free Download Manager\fdminno.dll"; Flags: dontcopy

[INI]
Filename: "{app}\fdm.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.freedownloadmanager.org/"

[Icons]
Name: "{group}\Free Download Manager"; Filename: "{app}\fdm.exe"
Name: "{group}\FDM remote control server"; Filename: "{app}\fdmwi.exe"
Name: "{group}\Documentation"; Filename: "{app}\Help\Free Download Manager.chm"
Name: "{group}\{cm:ProgramOnTheWeb,Free Download Manager}"; Filename: "{app}\fdm.url"
Name: "{userdesktop}\Free Download Manager"; Filename: "{app}\fdm.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Free Download Manager"; Filename: "{app}\fdm.exe"; Tasks: quicklaunchicon
Name: "{group}\{cm:UninstallProgram,Free Download Manager}"; Filename: "{uninstallexe}"

[Registry]
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Valuetype: string; Valuename: "Path"; Valuedata: "{app}"
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; Valuetype: string; Valuename: "Free Download Manager"; Valuedata: """{app}\fdm.exe"" -autorun"; Flags: uninsdeletevalue; tasks: autorun
Root: HKLM; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Flags: uninsdeletekey

[CustomMessages]
FDM_CommunityCaption=FDM Community options
FDM_CommunityDescription=You can benefit from community-powered spyware protection and community-edited downloads reviews database.
FDM_BittorrentCaption=Bittorrent protocol support
FDM_BittorrentDescription=
FDM_FUMCaption=Free Upload Manager
FDM_FUMDescription=Easy way to share your files
FDM_LanguageCaption=Language of interface
FDM_LanguageDescription=


[Code]
var
bEnableFDMCommunity : Boolean;
bEnableBT : Boolean;
fdmc_Label1: TLabel;
fdmc_Label2: TLabel;
fdmc_Label3: TLabel;
fdmc_Enable: TCheckBox;
BT_Label1: TLabel;
BT_Label2: TLabel;
BT_Label3: TLabel;
BT_Bevel1: TBevel;
BT_Enable: TCheckBox;
// FUM bundle
fum_Label1: TLabel;
fum_Label2: TLabel;
fum_cbIntegrate: TCheckBox;
fum_cbAutostart: TCheckBox;
bFumIntegration : Boolean;
bFumAutostart : Boolean;
bWasFumNotInstalled : Boolean;
////////////////////
lngLabel1: TLabel;
lngLabel2: TLabel;
lngList: TComboBox;
vLngNames, vLngFiles : array [1..100] of String;
vLngIds : array [1..100] of Integer;
nEngIndex : Integer;
nSelectedLanguage : Integer;

function GetSystemDefaultLanguageId () : Cardinal;
external 'GetSystemDefaultLanguageId@files:fdminno.dll stdcall';

procedure InitLngArrays ();
begin
  vLngNames [1] := 'Albanian'; vLngIds [1] := $1C; vLngFiles [1] := 'alb.lng';
  vLngNames [2] := 'Arabic';  vLngIds [2] := $01; vLngFiles [2] := 'arb.lng';
  vLngNames [3] := 'Portuguese'; vLngIds [3] := $16; vLngFiles [3] := 'ptbr.lng';
  vLngNames [4] := 'Bulgarian'; vLngIds [4] := $02; vLngFiles [4] := 'bul.lng';
  vLngNames [5] := 'Chinese'; vLngIds [5] := $04; vLngFiles [5] := 'cht.lng';
  vLngNames [6] := 'Croatian';  vLngIds [6] := $1A; {only:(041A, 101A)} vLngFiles [6] := 'cro.lng';
  vLngNames [7] := 'Czech';  vLngIds [7] := $05; vLngFiles [7] := 'czk.lng';
  vLngNames [8] := 'Danish'; vLngIds [8] := $06; vLngFiles [8] := 'dan.lng';
  vLngNames [9] := 'Dutch'; vLngIds [9] := $13; vLngFiles [9] := 'dut.lng';
  vLngNames [10] := 'English'; vLngIds [10] := $09; vLngFiles [10] := 'eng.lng'; nEngIndex := 10;
  vLngNames [11] := 'Finnish'; vLngIds [11] := $0B; vLngFiles [11] := 'fin.lng';
  vLngNames [12] := 'French';  vLngIds [12] := $0C; vLngFiles [12] := 'fre.lng';
  vLngNames [13] := 'German'; vLngIds [13] := $07; vLngFiles [13] := 'ger.lng';
  vLngNames [14] := 'Greek'; vLngIds [14] := $08; vLngFiles [14] := 'ell.lng';
  vLngNames [15] := 'Hebrew'; vLngIds [15] := $0D; vLngFiles [15] := 'heb.lng';
  vLngNames [16] := 'Hungarian'; vLngIds [16] := $0E; vLngFiles [16] := 'hun.lng';
  vLngNames [17] := 'Italian'; vLngIds [17] := $10; vLngFiles [17] := 'ita.lng';
  vLngNames [18] := 'Korean';  vLngIds [18] := $12; vLngFiles [18] := 'kor.lng';
  vLngNames [19] := 'Macedonian'; vLngIds [19] := $2F;  vLngFiles [19] := 'mac.lng'; //(http://www.microsoft.com/globaldev/reference/winxp/xp-lcid.mspx)
  vLngNames [20] := 'Polish';  vLngIds [20] := $15; vLngFiles [20] := 'pol.lng';
  vLngNames [21] := 'Romanian'; vLngIds [21] := $18; vLngFiles [21] := 'rom.lng';
  vLngNames [22] := 'Russian'; vLngIds [22] := $19; vLngFiles [22] := 'rus.lng';
  vLngNames [23] := 'Serbian'; vLngIds [23] := $1A; {(except: Croatian)} vLngFiles [23] := 'srb.lng';
  vLngNames [24] := 'Slovak'; vLngIds [24] := $1B; vLngFiles [24] := 'svk.lng';
  vLngNames [25] := 'Slovenian'; vLngIds [25] := $24; vLngFiles [25] := 'slo.lng';
  vLngNames [26] := 'Spanish'; vLngIds [26] := $0A; vLngFiles [26] := 'spn.lng';
  vLngNames [27] := 'Swedish'; vLngIds [27] := $1D; vLngFiles [27] := 'swe.lng';
  vLngNames [28] := 'Turkish'; vLngIds [28] := $1F; vLngFiles [28] := 'tur.lng';
  vLngNames [29] := 'Ukrainian'; vLngIds [29] := $22; vLngFiles [29] := 'ukr.lng';
  vLngNames [30] := 'Uzbek'; vLngIds [30] := $43; vLngFiles [30] := 'uzb.lng';
  vLngNames [31] := 'Vietnamese'; vLngIds [31] := $2A; vLngFiles [31] := 'vie.lng';
  vLngNames [32] := '';
  
  nSelectedLanguage := -1;
end;

function IsFDMCommunitySettingsEnabled () : Cardinal;
var
  dw : Cardinal;
  dwRes : Cardinal;
begin

  dwRes := 2;
  
  if RegQueryDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
     'SwitchToOpinions', dw) then if dw <> 0 then dwRes := 1 else dwRes := 0;
  if dwRes = 1 then begin Result := 1; exit; end;
  
  if RegQueryDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
     'CheckIfMalBeforeDlding', dw) then if dw <> 0 then dwRes := 1 else dwRes := 0;
  if dwRes = 1 then begin Result := 1; exit; end;
  
  if RegQueryDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
     'DisplayOpinionsAtDlding', dw) then if dw <> 0 then dwRes := 1 else dwRes := 0;
  if dwRes = 1 then begin Result := 1; exit; end;
  
  Result := dwRes;

end;

procedure FDM_Community_Activate(Page: TWizardPage);
begin
end;

function FDM_Community_ShouldSkipPage(Page: TWizardPage): Boolean;
var
 dw : Cardinal;
begin
  dw := IsFDMCommunitySettingsEnabled ();
  if dw <> 2 then bEnableFDMCommunity := dw <> 0;
  Result := dw <> 2;
end;

function FDM_Community_BackButtonClick(Page: TWizardPage): Boolean;
begin
  Result := True;
end;

function FDM_Community_NextButtonClick(Page: TWizardPage): Boolean;
begin
  bEnableFDMCommunity := fdmc_Enable.Checked;
  Result := True;
end;

procedure FDM_Community_CancelButtonClick(Page: TWizardPage; var Cancel, Confirm: Boolean);
begin
end;

function FDM_Community_CreatePage(PreviousPageId: Integer): Integer;
var
  Page: TWizardPage;
begin
  Page := CreateCustomPage(
    PreviousPageId,
    ExpandConstant('{cm:FDM_CommunityCaption}'),
    ExpandConstant('{cm:FDM_CommunityDescription}')
  );

  { fdmc_Label1 }
  fdmc_Label1 := TLabel.Create(Page);
  with fdmc_Label1 do
  begin
    Parent := Page.Surface;
    Left := ScaleX(0);
    Top := ScaleY(16);
    Width := ScaleX(415);
    Height := ScaleY(29);
    AutoSize := False;
    WordWrap := True;
    Caption := 'Free Download Manager allows its users to exchange opinions about downloads and warn each other about malicious downloads.';
  end;

  { fdmc_Label2 }
  fdmc_Label2 := TLabel.Create(Page);
  with fdmc_Label2 do
  begin
    Parent := Page.Surface;
    Left := ScaleX(0);
    Top := ScaleY(56);
    Width := ScaleX(415);
    Height := ScaleY(61);
    AutoSize := False;
    WordWrap := True;
    Caption := 'In order to use these features you should enable FDM Community options in your Free Download Manager. In this case Free Download Manager will contact our server before each download to receive information about it, so that you could know other users opinions about the download and be warned against malicious downloads.';
  end;

  { fdmc_Label3 }
  fdmc_Label3 := TLabel.Create(Page);
  with fdmc_Label3 do
  begin
    Parent := Page.Surface;
    Left := ScaleX(0);
    Top := ScaleY(160);
    Width := ScaleX(415);
    Height := ScaleY(53);
    AutoSize := False;
    WordWrap := True;
    Caption := 'You can disable or reenable these features at any time by selecting Options | Download options | FDM Community dialog box. There you can also customize these features and enable/disable some options.';
  end;

  { fdmc_Enable }
  fdmc_Enable := TCheckBox.Create(Page);
  with fdmc_Enable do
  begin
    Parent := Page.Surface;
    Left := ScaleX(0);
    Top := ScaleY(128);
    Width := ScaleX(409);
    Height := ScaleY(17);
    Caption := 'Enable FDM Community options';
    Checked := True;
    State := cbUnchecked;
    TabOrder := 0;
  end;


  with Page do
  begin
    OnActivate := @FDM_Community_Activate;
    OnShouldSkipPage := @FDM_Community_ShouldSkipPage;
    OnBackButtonClick := @FDM_Community_BackButtonClick;
    OnNextButtonClick := @FDM_Community_NextButtonClick;
    OnCancelButtonClick := @FDM_Community_CancelButtonClick;
  end;

  Result := Page.ID;
end;

procedure ApplyFDMCommunitySettingsToForm ();
var
  dw : Cardinal;
begin;
  dw := IsFDMCommunitySettingsEnabled ();
  if dw <> 2 then fdmc_Enable.Checked := dw <> 0;
end;

function IsBtSupportEnabled () : Cardinal;
var
  dw : Cardinal;
  dwRes : Cardinal;
begin
  dwRes := 2;

  if RegQueryDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Network\Bittorrent',
     'Enable', dw) then if dw <> 0 then dwRes := 1 else dwRes := 0;

  Result := dwRes;
end;

procedure ApplyBittorrentSettingsToForm ();
var
  dw : Cardinal;
begin;
  dw := IsBtSupportEnabled ();
  if dw <> 2 then BT_Enable.Checked := dw <> 0;
end;

procedure FDM_Bittorrent_Activate(Page: TWizardPage);
begin
end;

function FDM_Bittorrent_ShouldSkipPage(Page: TWizardPage): Boolean;
var
 dw : Cardinal;
begin
 if UsingWinNT then begin
   dw := IsBtSupportEnabled ();
   if dw <> 2 then bEnableBT := dw <> 0;
   Result := dw <> 2;
 end else begin
   bEnableBT := False;
   Result := True;
 end;
end;

function FDM_Bittorrent_BackButtonClick(Page: TWizardPage): Boolean;
begin
  Result := True;
end;

function FDM_Bittorrent_NextButtonClick(Page: TWizardPage): Boolean;
begin
  bEnableBT := BT_Enable.Checked;
  Result := True;
end;

procedure FDM_Bittorrent_CancelButtonClick(Page: TWizardPage; var Cancel, Confirm: Boolean);
begin
end;

function FDM_Bittorrent_CreatePage(PreviousPageId: Integer): Integer;
var
  Page: TWizardPage;
begin
    Page := CreateCustomPage(
      PreviousPageId,
      ExpandConstant('{cm:FDM_BittorrentCaption}'),
      ExpandConstant('{cm:FDM_BittorrentDescription}')
    );

    { BT_Label1 }
    BT_Label1 := TLabel.Create(Page);
    with BT_Label1 do
    begin
      Parent := Page.Surface;
      Left := ScaleX(0);
      Top := ScaleY(16);
      Width := ScaleX(407);
      Height := ScaleY(21);
      AutoSize := False;
      Caption := 'Free Download Manager can download files using Bittorrent protocol.';
    end;

    { BT_Label2 }
    BT_Label2 := TLabel.Create(Page);
    with BT_Label2 do
    begin
      Parent := Page.Surface;
      Left := ScaleX(0);
      Top := ScaleY(144);
      Width := ScaleX(407);
      Height := ScaleY(69);
      AutoSize := False;
      WordWrap := True;
      Caption := 'Note that when Bittorrent is enabled and there are some Bittorrent downloads in the list of downloads, Free Download Manager will generate upload traffic even if there are no running downloads currently. This is required by Bittorrent protocol specification. However you may limit the upload speed in the options of Bittorrent protocol.';
    end;

    { BT_Label3 }
    BT_Label3 := TLabel.Create(Page);
    with BT_Label3 do
    begin
      Parent := Page.Surface;
      Left := ScaleX(0);
      Top := ScaleY(56);
      Width := ScaleX(407);
      Height := ScaleY(45);
      AutoSize := False;
      WordWrap := True;
      Caption := 'Here you can choose to enable or disable the support of this protocol in Free Download Manager. Also you can enable or disable it later at any time using appropriate options.';
    end;

    { BT_Bevel1 }
    BT_Bevel1 := TBevel.Create(Page);
    with BT_Bevel1 do
    begin
      Parent := Page.Surface;
      Left := ScaleX(-8);
      Top := ScaleY(40);
      Width := ScaleX(421);
      Height := ScaleY(2);
    end;

    { BT_Enable }
    BT_Enable := TCheckBox.Create(Page);
    with BT_Enable do
    begin
      Parent := Page.Surface;
      Left := ScaleX(0);
      Top := ScaleY(112);
      Width := ScaleX(409);
      Height := ScaleY(17);
      Caption := 'Enable Bittorrent protocol support now';
      TabOrder := 0;
    end;


    with Page do
    begin
      OnActivate := @FDM_Bittorrent_Activate;
      OnShouldSkipPage := @FDM_Bittorrent_ShouldSkipPage;
      OnBackButtonClick := @FDM_Bittorrent_BackButtonClick;
      OnNextButtonClick := @FDM_Bittorrent_NextButtonClick;
      OnCancelButtonClick := @FDM_Bittorrent_CancelButtonClick;
    end;

    Result := Page.ID;
end;

procedure FDM_Language_Activate(Page: TWizardPage);
begin
end;

function FDM_Language_ShouldSkipPage(Page: TWizardPage): Boolean;
begin
  Result := RegValueExists (HKEY_CURRENT_USER, 'Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\View', 'Language');
end;

function FDM_Language_BackButtonClick(Page: TWizardPage): Boolean;
begin
  Result := True;
end;

function FDM_Language_NextButtonClick(Page: TWizardPage): Boolean;
begin
  nSelectedLanguage := lngList.ItemIndex + 1;
  Result := True;
end;

procedure FDM_Language_CancelButtonClick(Page: TWizardPage; var Cancel, Confirm: Boolean);
begin
end;

function FDM_Language_CreatePage(PreviousPageId: Integer): Integer;
var
  Page: TWizardPage;
  i : Integer;
label
  _lSearch;
begin
  InitLngArrays ();
  
  Page := CreateCustomPage(
    PreviousPageId,
    ExpandConstant('{cm:FDM_LanguageCaption}'),
    ExpandConstant('{cm:FDM_LanguageDescription}')
  );

  // lngLabel1
  lngLabel1 := TLabel.Create(Page);
  with lngLabel1 do
  begin
    Parent := Page.Surface;
    Left := ScaleX(8);
    Top := ScaleY(56);
    Width := ScaleX(407);
    Height := ScaleY(21);
    AutoSize := False;
    WordWrap := True;
    Caption := 'Choose the language of interface:';
  end;

  // lngLabel2
  lngLabel2 := TLabel.Create(Page);
  with lngLabel2 do
  begin
    Parent := Page.Surface;
    Left := ScaleX(8);
    Top := ScaleY(152);
    Width := ScaleX(399);
    Height := ScaleY(45);
    AutoSize := False;
    WordWrap := True;
    Caption := 'If you do not  see your language here you may translate Free Dow''nload Manager by translating eng.lng file in Language subfolder ''and sending translated file to us.';
  end;

  // lngList
  lngList := TComboBox.Create(Page);
  with lngList do
  begin
    Style := csDropDownList;
    Parent := Page.Surface;
    Left := ScaleX(112);
    Top := ScaleY(96);
    Width := ScaleX(193);
    Height := ScaleY(21);
    TabOrder := 0;
    i := 1;
    while vLngNames [i] <> '' do begin
      Items.Add(vLngNames [i]);
      i := i + 1;
    end;
  end;
  
  i := 1;
_lSearch:
  while (vLngIds [i] <> (GetSystemDefaultLanguageId and $3FF)) and (vLngNames [i] <> '') do
    i := i + 1;
  if vLngIds [i] = $1A then begin
    if (GetSystemDefaultLanguageId <> $041A) and (GetSystemDefaultLanguageId <> $101A) and (vLngNames [i] = 'Croatian') then begin
      i := i + 1;
      goto _lSearch;
    end;
  end;

  if (vLngNames [i] = '') then
    i := nEngIndex;

  lngList.ItemIndex := i - 1;


  with Page do
  begin
    OnActivate := @FDM_Language_Activate;
    OnShouldSkipPage := @FDM_Language_ShouldSkipPage;
    OnBackButtonClick := @FDM_Language_BackButtonClick;
    OnNextButtonClick := @FDM_Language_NextButtonClick;
    OnCancelButtonClick := @FDM_Language_CancelButtonClick;
  end;

  Result := Page.ID;
end;


procedure InitializeWizard();
var
  nPage : Integer;
begin
  nPage := FDM_Community_CreatePage(wpLicense);
  nPage := FDM_Bittorrent_CreatePage (nPage);
  nPage := FDM_Language_CreatePage(nPage);
  ApplyFDMCommunitySettingsToForm ();
  ApplyBittorrentSettingsToForm ();
end;

procedure ShutdownFdmAndFum_old ();
var
 fdm, fum, objNull : Variant;
begin
  if (GetWindowsVersion () shr 24) = 6 then
    exit; // exit because it will not work on Vista
    
  // shutdown fdm
  if CheckForMutexes ('Free Download Manager') then
  try
    fdm := CreateOleObject ('FDM.FDM');
    fdm.Shutdown (False);
    fdm := objNull;
  except
  end;
end;

procedure ShutdownFdmAndFum ();
var
 msg : Longint;
 i : Integer;
begin
  msg := RegisterWindowMessage ('FDM - shutdown');
  PostBroadcastMessage (msg, 0, 0);
  
  i := 0; // 15 second timeout
  while CheckForMutexes ('Free Download Manager') and (i < 20*15) do begin
   Sleep (50);
   i := i + 1;
  end;
  if CheckForMutexes ('Free Download Manager') then
    MsgBox ('Can''t shutdown Free Download Manager. Please do this by itself.', mbError, MB_OK);
end;

function InitializeSetup(): Boolean;
begin

if CheckForMutexes ('Free Download Manager') then
begin
  // ask user
  if IDCANCEL = MsgBox ('Setup has detected that Free Download Manager are currently running.'#13#13'It''s highly recommended to close them.'#13#13'Click OK to close or Cancel to continue without closing (restart of the computer will be required after installation).', mbError, MB_OKCANCEL) then
  begin
      Result := True;
      exit;
  end;
  ShutdownFdmAndFum_old ();
  ShutdownFdmAndFum ();
end;

Result := True;
end;

procedure UpdateCatchPlugin(Plug:String);
var
Path : String;
AppDir : String;
begin
if not RegQueryStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Monitor',
          Plug, Path) then exit;

if Path = '' then exit;

Path := AddBackslash (Path);
AppDir := AddBackslash (WizardDirValue);

while not FileCopy (AppDir + 'npfdm.dll', Path + 'npfdm.dll', false) do
  begin
    if IDCANCEL = MsgBox ('Failed to update click monitoring plugin. Please close all browser windows.', mbError, MB_RETRYCANCEL) then
      exit;
  end;
end;

procedure UpdateCatchPlugins();
begin
  UpdateCatchPlugin ('OPDI');
  UpdateCatchPlugin ('NPDI');
end;

procedure ApplyEnableFDMCommunity ();
var
 dw : Cardinal;
 dwOld : Cardinal;
begin

 if bEnableFDMCommunity then dw := 1 else dw := 0;
 
 // check if fdm already exists with these options modified
 if (bEnableFDMCommunity) then begin
  dwOld := IsFDMCommunitySettingsEnabled ();
  if dwOld = 1 then
    exit;
 end;
 
 RegWriteDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'SwitchToOpinions', dw);
 RegWriteDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'CheckIfMalBeforeDlding', dw);
 RegWriteDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'DisplayOpinionsAtDlding', dw);
    
 RegWriteDWordValue (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'SwitchToOpinions', dw);
 RegWriteDWordValue (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'CheckIfMalBeforeDlding', dw);
 RegWriteDWordValue (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Community',
    'DisplayOpinionsAtDlding', dw);
    
end;

procedure ApplyEnableBittorrent ();
var
 dw : Cardinal;
 dwOld : Cardinal;
begin

 if bEnableBT then dw := 1 else dw := 0;

 // check if fdm already exists with these options modified
 if (bEnableBT) then begin
  dwOld := IsBtSupportEnabled ();
  if dwOld = 1 then
    exit;
 end;

 RegWriteDWordValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Network\Bittorrent',
    'Enable', dw);

 RegWriteDWordValue (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\Network\Bittorrent',
    'Enable', dw);
end;

procedure ApplySelectedLanguage ();
begin
  if nSelectedLanguage <> -1 then begin
    RegWriteStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager\Settings\View', 'Language', vLngFiles [nSelectedLanguage]);
  end;
end;

procedure CurStepChanged(CurStep : TSetupStep);
begin
  if CurStep = ssPostInstall then begin
    ApplyEnableFDMCommunity ();
    ApplyEnableBittorrent ();
    ApplySelectedLanguage ();
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then
    ShutdownFdmAndFum ();
    
  if CurUninstallStep = usPostUninstall then begin

    if IDNO = MsgBox ('Do you want to delete all Free Download Manager''s data files also?', mbConfirmation, MB_YESNO)
      then exit;

    if UsingWinNT then
      DelTree (ExpandConstant ('{userappdata}\Free Download Manager'), True, True, True);

    DelTree (ExpandConstant ('{app}'), True, True, True);

    RegDeleteKeyIncludingSubkeys (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager');
    RegDeleteKeyIncludingSubkeys (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager');
    
  end;
end;

[Run]
Filename: "{app}\fdm.exe"; Parameters: "-regserver"; StatusMsg: "Registering..."
Filename: "{app}\fdm.exe"; Description: "{cm:LaunchProgram,Free Download Manager}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: "{app}\fdm.exe"; Parameters: "-unregserver";

[UninstallDelete]
Type: files; Name: "{app}\fdm.url"

