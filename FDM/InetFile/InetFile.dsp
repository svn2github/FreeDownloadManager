# Microsoft Developer Studio Project File - Name="InetFile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=InetFile - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "InetFile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "InetFile.mak" CFG="InetFile - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "InetFile - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "InetFile - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "InetFile - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /WX /GX /ZI /Od /I "../include.add/" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /FD /EHa /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib.add\InetFile.lib"

!ELSEIF  "$(CFG)" == "InetFile - Win32 Debug"

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
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "../include.add/" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /D "BUILDING_LIBCURL" /YX /FD /GZ /EHa /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib.add\InetFile_d.lib"

!ENDIF 

# Begin Target

# Name "InetFile - Win32 Release"
# Name "InetFile - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fsFtpConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFtpFile.cpp
# End Source File
# Begin Source File

SOURCE=.\fsFtpFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\fsHTMLParser.cpp
# End Source File
# Begin Source File

SOURCE=.\fsHttpConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\fsHttpFile.cpp
# End Source File
# Begin Source File

SOURCE=.\fsHttpFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\fsinet.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetFile.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetFile2.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetSession.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFile.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFileDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLServer.cpp
# End Source File
# Begin Source File

SOURCE=.\fsLocalFile.cpp
# End Source File
# Begin Source File

SOURCE=.\fsServerConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\fsSpeaking.cpp
# End Source File
# Begin Source File

SOURCE=.\fsURL.cpp
# End Source File
# Begin Source File

SOURCE=.\strparsing.cpp
# End Source File
# Begin Source File

SOURCE=.\system.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\fsFtpConnection.h
# End Source File
# Begin Source File

SOURCE=.\fsFtpFile.h
# End Source File
# Begin Source File

SOURCE=.\fsFtpFiles.h
# End Source File
# Begin Source File

SOURCE=.\fsHTMLParser.h
# End Source File
# Begin Source File

SOURCE=.\fsHttpConnection.h
# End Source File
# Begin Source File

SOURCE=.\fsHttpFile.h
# End Source File
# Begin Source File

SOURCE=.\fsHttpFiles.h
# End Source File
# Begin Source File

SOURCE=.\fsinet.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetFile.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetFile2.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetFiles.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetSession.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFile.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFileDownloader.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLFiles.h
# End Source File
# Begin Source File

SOURCE=.\fsInternetURLServer.h
# End Source File
# Begin Source File

SOURCE=.\fsLocalFile.h
# End Source File
# Begin Source File

SOURCE=.\fsServerConnection.h
# End Source File
# Begin Source File

SOURCE=.\fsSpeaking.h
# End Source File
# Begin Source File

SOURCE=.\fsURL.h
# End Source File
# Begin Source File

SOURCE=.\strparsing.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# End Group
# End Target
# End Project
