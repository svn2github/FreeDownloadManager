; FDM installer for freedownloadmanager.org site

[Setup]
AppName=Free Download Manager
AppId=Free Download Manager
AppVerName=Free Download Manager 2.6 BETA
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
; FUM bundle
;Source: "Free Upload Manager\fumshext.dll"; DestDir: "{app}\FUM"; Flags: restartreplace
;Source: "Free Upload Manager\fumiebtn.dll"; DestDir: "{app}\FUM"; Flags: restartreplace;
Source: "Free Upload Manager\fumcore.dll"; DestDir: "{app}\FUM"; Flags: restartreplace
;Source: "Free Upload Manager\fumoei.exe"; DestDir: "{app}\FUM"; Flags: restartreplace
;Source: "Free Upload Manager\fumoei.dll"; DestDir: "{app}\FUM"; Flags: restartreplace
Source: "Free Upload Manager\fum.exe"; DestDir: "{app}\FUM"; Flags: restartreplace
Source: "Free Upload Manager\fum.tlb"; DestDir: "{app}\FUM"; Flags: ignoreversion restartreplace
Source: "Free Download Manager\msvcp60.dll"; DestDir: "{app}\FUM"; Flags: restartreplace
Source: "Free Download Manager\Updater.exe"; DestDir: "{app}\FUM"; Flags: restartreplace
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
Name: "{group}\Free Upload Manager"; Filename: "{app}\FUM\fum.exe";
Name: "{group}\FDM remote control server"; Filename: "{app}\fdmwi.exe"
Name: "{group}\Documentation"; Filename: "{app}\Help\Free Download Manager.chm"
Name: "{group}\{cm:ProgramOnTheWeb,Free Download Manager}"; Filename: "{app}\fdm.url"
Name: "{userdesktop}\Free Download Manager"; Filename: "{app}\fdm.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Free Download Manager"; Filename: "{app}\fdm.exe"; Tasks: quicklaunchicon
Name: "{group}\{cm:UninstallProgram,Free Download Manager}"; Filename: "{uninstallexe}"

[Registry]
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager RCS"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Valuetype: string; Valuename: "Path"; Valuedata: "{app}"
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; Valuetype: string; Valuename: "Free Download Manager"; Valuedata: """{app}\fdm.exe"" -autorun"; Flags: uninsdeletevalue; tasks: autorun
Root: HKLM; Subkey: "Software\FreeDownloadManager.ORG\Free Download Manager"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Upload Manager"; Valuetype: string; Valuename: "Path_old"; Valuedata: "{reg:HKCU\Software\FreeDownloadManager.ORG\Free Upload Manager,Path}"; Check: DoesNeedSaveOldFumPath;
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Upload Manager"; Valuetype: string; Valuename: "Path"; Valuedata: "{app}\FUM";
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Upload Manager"; Valuetype: string; Valuename: "lngsroot"; Valuedata: "{app}"; Flags: uninsdeletevalue;
Root: HKCU; Subkey: "Software\FreeDownloadManager.ORG\Free Upload Manager"; Valuetype: string; Valuename: "force_data_folder"; Valuedata: "{userappdata}\Free Download Manager\"; Flags: uninsdeletevalue;
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; Valuetype: string; Valuename: "Free Upload Manager"; Valuedata: """{app}\fum\fum.exe"" -autorun"; Check: FumAutostart; Flags: uninsdeletevalue;

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
lngLabel1: TLabel;
lngLabel2: TLabel;
lngList: TComboBox;
vLngNames, vLngFiles : array [1..100] of String;
vLngIds : array [1..100] of Integer;
nEngIndex : Integer;
nSelectedLanguage : Integer;

function DoesNeedSaveOldFumPath () : Boolean;
var
s1, s2 : String;
begin
  s1 := ExpandConstant ('{app}');
  s1 := AddBackslash (s1);
  s1 := Lowercase (s1);
  
  RegQueryStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'Path', s2);
  s2 := AddBackslash (s2);
  s2 := Lowercase (s2);
  
  Result := s1 <> s2;
end;

function GetSystemDefaultLanguageId () : Cardinal;
external 'GetSystemDefaultLanguageId@files:fdminno.dll stdcall';

procedure InitLngArrays ();
var
  iLng : Integer;
begin
  //http://www.microsoft.com/globaldev/reference/winxp/xp-lcid.mspx
  iLng := 1;
  vLngNames [iLng] := 'Albanian'; vLngIds [iLng] := $1C; vLngFiles [iLng] := 'alb.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Arabic';  vLngIds [iLng] := $01; vLngFiles [iLng] := 'arb.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Brazilian Portuguese'; vLngIds [iLng] := $16; vLngFiles [iLng] := 'ptbr.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Bulgarian'; vLngIds [iLng] := $02; vLngFiles [iLng] := 'bul.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Catalan'; vLngIds [iLng] := $03; vLngFiles [iLng] := 'cat.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Chinese'; vLngIds [iLng] := $04; vLngFiles [iLng] := 'cht.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Croatian';  vLngIds [iLng] := $1A; {only:(041A, 101A)} vLngFiles [iLng] := 'cro.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Czech';  vLngIds [iLng] := $05; vLngFiles [iLng] := 'czk.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Danish'; vLngIds [iLng] := $06; vLngFiles [iLng] := 'dan.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Dutch'; vLngIds [iLng] := $13; vLngFiles [iLng] := 'dut.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'English'; vLngIds [iLng] := $09; vLngFiles [iLng] := 'eng.lng'; nEngIndex := iLng; iLng := iLng + 1;
  vLngNames [iLng] := 'Farsi'; vLngIds [iLng] := $29; vLngFiles [iLng] := 'far.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Finnish'; vLngIds [iLng] := $0B; vLngFiles [iLng] := 'fin.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'French';  vLngIds [iLng] := $0C; vLngFiles [iLng] := 'fre.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Galician';  vLngIds [iLng] := $56; vLngFiles [iLng] := 'gal.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'German'; vLngIds [iLng] := $07; vLngFiles [iLng] := 'ger.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Greek'; vLngIds [iLng] := $08; vLngFiles [iLng] := 'ell.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Hebrew'; vLngIds [iLng] := $0D; vLngFiles [iLng] := 'heb.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Hungarian'; vLngIds [iLng] := $0E; vLngFiles [iLng] := 'hun.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Italian'; vLngIds [iLng] := $10; vLngFiles [iLng] := 'ita.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Japanese'; vLngIds [iLng] := $11; vLngFiles [iLng] := 'jpn.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Korean';  vLngIds [iLng] := $12; vLngFiles [iLng] := 'kor.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Lithuanian'; vLngIds [iLng] := $27;  vLngFiles [iLng] := 'lt.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Macedonian'; vLngIds [iLng] := $2F;  vLngFiles [iLng] := 'mac.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Norwegian'; vLngIds [iLng] := $14; vLngFiles [iLng] := 'nor.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Polish';  vLngIds [iLng] := $15; vLngFiles [iLng] := 'pol.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Portuguese'; vLngIds [iLng] := $16; vLngFiles [iLng] := 'pt_PT.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Romanian'; vLngIds [iLng] := $18; vLngFiles [iLng] := 'rom.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Russian'; vLngIds [iLng] := $19; vLngFiles [iLng] := 'rus.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Serbian'; vLngIds [iLng] := $1A; {(except: Croatian)} vLngFiles [iLng] := 'srb.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Slovak'; vLngIds [iLng] := $1B; vLngFiles [iLng] := 'svk.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Slovenian'; vLngIds [iLng] := $24; vLngFiles [iLng] := 'slo.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Spanish'; vLngIds [iLng] := $0A; vLngFiles [iLng] := 'spn.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Swedish'; vLngIds [iLng] := $1D; vLngFiles [iLng] := 'swe.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Turkish'; vLngIds [iLng] := $1F; vLngFiles [iLng] := 'tur.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Ukrainian'; vLngIds [iLng] := $22; vLngFiles [iLng] := 'ukr.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Uzbek'; vLngIds [iLng] := $43; vLngFiles [iLng] := 'uzb.lng'; iLng := iLng + 1;
  vLngNames [iLng] := 'Vietnamese'; vLngIds [iLng] := $2A; vLngFiles [iLng] := 'vie.lng'; iLng := iLng + 1;
  vLngNames [iLng] := '';
  
  nSelectedLanguage := -1;
end;

function FumAutostart () : Boolean;
begin
  Result := bFumAutostart;
end;

function FumIntegration () : Boolean;
begin
  Result := bFumIntegration;
end;

function GetFumExePath () : String;
var
  str : String;
begin
  if RegQueryStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'Path', str) then
  begin
    begin
      str := str + '\fum.exe';
      if FileExists (str) then begin
        Result := str;
        exit;
      end;
    end;
  end;
  Result := '';
end;

function IsFumInstalled (bNotBundledVerOnly : Boolean) : Boolean;
begin
  if bNotBundledVerOnly then
  begin
    if RegValueExists (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'fum_alone') then
      Result := GetFumExePath () <> ''
    else
      Result := False;
    exit;
  end;
  Result := GetFumExePath <> '';
end;

function WasFumNotInstalled () : Boolean;
begin
  Result := bWasFumNotInstalled;
end;

procedure FDM_FUM_Activate(Page: TWizardPage);
begin
end;

function FDM_FUM_ShouldSkipPage(Page: TWizardPage): Boolean;
begin
  Result := IsFumInstalled (False);
end;

function FDM_FUM_BackButtonClick(Page: TWizardPage): Boolean;
begin
  Result := True;
end;

function FDM_FUM_NextButtonClick(Page: TWizardPage): Boolean;
begin
  bFumIntegration := False; //fum_cbIntegrate.Checked;
  bFumAutostart := False; //fum_cbAutostart.Checked;
  Result := True;
end;

procedure FDM_FUM_CancelButtonClick(Page: TWizardPage; var Cancel, Confirm: Boolean);
begin
end;

function FDM_FUM_CreatePage(PreviousPageId: Integer): Integer;
var
  Page: TWizardPage;
begin
  Page := CreateCustomPage(
    PreviousPageId,
    ExpandConstant('{cm:FDM_FUMCaption}'),
    ExpandConstant('{cm:FDM_FUMDescription}')
  );
   // fum_Label1
fum_Label1 := TLabel.Create(Page);
with fum_Label1 do
begin
  Parent := Page.Surface;
  Left := ScaleX(0);
  Top := ScaleY(16);
  Width := ScaleX(415);
  Height := ScaleY(40);
  AutoSize := False;
  WordWrap := True;
  Caption := 'Free Download Manager now includes Free Upload Manager. It allows to upload and share any of your files with the other users in an easy way.';
end;

// fum_Label2
fum_Label2 := TLabel.Create(Page);
with fum_Label2 do
begin
  Parent := Page.Surface;
  Left := ScaleX(0);
  Top := ScaleY(72);
  Width := ScaleX(415);
  Height := ScaleY(29);
  AutoSize := False;
  WordWrap := True;
  Caption := 'Free Upload Manager can be also easily integrated with your system (Windows Explorer,  Internet Explorer, Outlook Express).';
end;

// fum_cbIntegrate
fum_cbIntegrate := TCheckBox.Create(Page);
with fum_cbIntegrate do
begin
  Parent := Page.Surface;
  Left := ScaleX(0);
  Top := ScaleY(128);
  Width := ScaleX(409);
  Height := ScaleY(25);
  Caption := 'Integrate FUM with Windows Explorer, Internet Explorer and Outlook Express';
  Checked := True;
  State := cbChecked;
  TabOrder := 0;
end;

// fum_cbAutostart
fum_cbAutostart := TCheckBox.Create(Page);
with fum_cbAutostart do
begin
  Parent := Page.Surface;
  Left := ScaleX(0);
  Top := ScaleY(152);
  Width := ScaleX(409);
  Height := ScaleY(25);
  Caption := 'Launch FUM automatically at Windows startup';
  Checked := True;
  State := cbChecked;
  TabOrder := 1;
end;


   with Page do
   begin
     OnActivate := @FDM_FUM_Activate;
     OnShouldSkipPage := @FDM_FUM_ShouldSkipPage;
     OnBackButtonClick := @FDM_FUM_BackButtonClick;
     OnNextButtonClick := @FDM_FUM_NextButtonClick;
     OnCancelButtonClick := @FDM_FUM_CancelButtonClick;
   end;
     Result := Page.ID;
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
    Caption := 'Choose the interface language:';
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
    Caption := 'If you do not  see your language here you may translate Free Download Manager by translating eng.lng file in Language subfolder and sending translated file to us.';
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
  if vLngIds [i] = $16 then begin
    if (GetSystemDefaultLanguageId <> $0416) and (vLngNames [i] = 'Brazilian Portuguese') then begin
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
  //nPage := FDM_FUM_CreatePage(nPage);
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

  // shutdown fum
  if CheckForMutexes ('Free Upload Manager') then
  try
    fum := CreateOleObject ('FUM.FUMApplication');
    fum.Shutdown (False);
    fum := objNull;
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
  
  msg := RegisterWindowMessage ('FUM - shutdown');
  PostBroadcastMessage (msg, 0, 0);
    
  i := 0; // 15 second timeout
  while CheckForMutexes ('Free Download Manager,Free Upload Manager') and (i < 20*15) do begin
   Sleep (50);
   i := i + 1;
  end;
  if CheckForMutexes ('Free Download Manager,Free Upload Manager') then
    MsgBox ('Can''t shutdown Free Download Manager or Free Upload Manager. Please do this by itself.', mbError, MB_OK);
end;

procedure ShutdownFumIntegration ();
var
  str : String;
  rc : Integer;
begin
  str := GetFumExePath ();
  if str = '' then
    exit;
  Exec (str, '-shui', '', SW_HIDE, ewWaitUntilTerminated, rc);
end;

function InitializeSetup(): Boolean;
begin
bWasFumNotInstalled := IsFumInstalled (False) = False;
if CheckForMutexes ('Free Download Manager,Free Upload Manager') then
begin
  // ask user
  if IDCANCEL = MsgBox ('Setup has detected that Free Download Manager or Free Upload Manager or both are currently running.'#13#13'It''s highly recommended to close them.'#13#13'Click OK to close or Cancel to continue without closing (restart of the computer will be required after installation).', mbError, MB_OKCANCEL) then
  begin
      Result := True;
      exit;
  end;
  ShutdownFdmAndFum_old ();
  ShutdownFdmAndFum ();
end;
ShutdownFumIntegration ();

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
    RegWriteStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager\Settings\View', 'Language', vLngFiles [nSelectedLanguage]);
  end;
end;

procedure CurStepChanged(CurStep : TSetupStep);
begin
  if CurStep = ssInstall then begin
    ApplyEnableFDMCommunity ();
    ApplyEnableBittorrent ();
    ApplySelectedLanguage ();
  end;
end;

procedure RegisterOldFumInstallation ();
var
  s : String;
  nError : Integer;
begin
  if DoesNeedSaveOldFumPath () then
    exit;
    
  if False = RegQueryStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'Path_old', s) then
    exit;

  RegWriteStringValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'Path', s);

  RegDeleteValue (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager', 'Path_old');

  s := AddBackslash (s);

  ShellExec ('open', s + 'fum.exe', '-regserver', s, SW_SHOW, ewNoWait, nError);
end;

procedure UnregisterUploaderIntegration ();
begin
  UnregisterServer (False, ExpandConstant ('{app}\fum\fumshext.dll'), True);
  UnregisterServer (False, ExpandConstant ('{app}\fum\fumiebtn.dll'), True);
  RestartReplace (ExpandConstant ('{app}\fum\fumshext.dll'), '');
  RestartReplace (ExpandConstant ('{app}\fum\fumiebtn.dll'), '');
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then
    ShutdownFdmAndFum ();
    
  if CurUninstallStep = usPostUninstall then begin

    UnregisterUploaderIntegration ();

    RegisterOldFumInstallation ();

    if IDNO = MsgBox ('Do you want to delete all Free Download Manager''s data files also?', mbConfirmation, MB_YESNO)
      then exit;

    if UsingWinNT then
      DelTree (ExpandConstant ('{userappdata}\Free Download Manager'), True, True, True);

    DelTree (ExpandConstant ('{app}'), True, True, True);

    RegDeleteKeyIncludingSubkeys (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Download Manager');
    RegDeleteKeyIncludingSubkeys (HKEY_LOCAL_MACHINE, 'Software\FreeDownloadManager.ORG\Free Download Manager');
    
    if GetFumExePath () = '' then
      RegDeleteKeyIncludingSubkeys (HKEY_CURRENT_USER, 'Software\FreeDownloadManager.ORG\Free Upload Manager');

  end;
end;

function FUMIntegrationCmdLine (str : String) : String;
begin
  if bFumIntegration then
    Result := '-euis'
  else
    Result := '-duis';
end;

function IsFumIntegrationEnabled () : Boolean;
begin
  Result := RegValueExists (HKEY_CURRENT_USER, 'Software\Microsoft\Windows\CurrentVersion\Run',
    'Free Uploader Oe Integration');
end;

[Run]
Filename: "{app}\fdm.exe"; Parameters: "-regserver"; StatusMsg: "Registering..."
Filename: "{app}\FUM\fum.exe"; Parameters: "-regserver"; StatusMsg: "Registering...";
;Filename: "{app}\FUM\fum.exe"; Parameters: "{code:FUMIntegrationCmdLine}"; StatusMsg: "Registering...";
;Filename: "{app}\FUM\fum.exe"; Parameters: "-stui"; Check: IsFumIntegrationEnabled;
Filename: "{app}\fdm.exe"; Description: "{cm:LaunchProgram,Free Download Manager}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: "{app}\fum\fum.exe"; Parameters: "-duis";
Filename: "{app}\fdm.exe"; Parameters: "-unregserver";

[UninstallDelete]
Type: files; Name: "{app}\fdm.url"

