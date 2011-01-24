# Microsoft Developer Studio Project File - Name="PartMediaPreview" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PartMediaPreview - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PartMediaPreview.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PartMediaPreview.mak" CFG="PartMediaPreview - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PartMediaPreview - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PartMediaPreview - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PartMediaPreview - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /I "..\include.add" /I "..\include.add\dshow" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PartMediaPreview - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include.add" /I "..\include.add\dshow" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "PartMediaPreview - Win32 Release"
# Name "PartMediaPreview - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Source Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fsAVIIdx1Builder.cpp
# End Source File
# Begin Source File

SOURCE=.\fsDShowFilterDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFileStream.cpp
# End Source File
# End Group
# Begin Group "DShow Base Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Include.Add\DShow\amextra.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\amfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\amvideo.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\asyncio.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\asyncrdr.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\combase.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\cprop.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\ctlutil.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\ddmm.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\dllentry.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\dllsetup.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\mtype.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\outputq.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\pstream.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\pullpin.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\refclock.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\renbase.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\schedule.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\seekpt.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\source.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\strmctl.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\sysclock.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\transfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\transip.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\videoctl.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\vtrans.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\winctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\winutil.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\wxdebug.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\wxlist.cpp
# End Source File
# Begin Source File

SOURCE=..\Include.Add\DShow\wxutil.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\fsPartMediaPlayer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Header Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fsAVIIdx1Builder.h
# End Source File
# Begin Source File

SOURCE=.\fsDShowFilterDetector.h
# End Source File
# Begin Source File

SOURCE=.\fsFileReader.h
# End Source File
# Begin Source File

SOURCE=.\fsFileStream.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\fsPartMediaPlayer.h
# End Source File
# End Group
# End Target
# End Project
