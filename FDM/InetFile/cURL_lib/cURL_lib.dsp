# Microsoft Developer Studio Project File - Name="cURL_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cURL_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cURL_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cURL_lib.mak" CFG="cURL_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cURL_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cURL_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "cURL_lib - Win32 Release_nossl" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cURL_lib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Ot /I "../../include.add/" /I "../../include.add/openssl" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /D "USE_SSLEAY" /D "CURL_DISABLE_LDAP" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cURL_lib - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include.add/" /I "../../include.add/openssl" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /D "BUILDING_LIBCURL" /D "USE_SSLEAY" /D "CURL_DISABLE_LDAP" /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cURL_lib - Win32 Release_nossl"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cURL_lib___Win32_Release_nossl"
# PROP BASE Intermediate_Dir "cURL_lib___Win32_Release_nossl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_nossl"
# PROP Intermediate_Dir "Release_nossl"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../include.add/" /I "../../include.add/openssl" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /D "USE_SSLEAY" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /I "../../include.add/" /I "../../include.add/openssl" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "CURL_STATICLIB" /D "CURL_DISABLE_LDAP" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "cURL_lib - Win32 Release"
# Name "cURL_lib - Win32 Debug"
# Name "cURL_lib - Win32 Release_nossl"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\base64.c
# End Source File
# Begin Source File

SOURCE=.\connect.c
# End Source File
# Begin Source File

SOURCE=.\content_encoding.c
# End Source File
# Begin Source File

SOURCE=.\cookie.c
# End Source File
# Begin Source File

SOURCE=.\curl_addrinfo.c
# End Source File
# Begin Source File

SOURCE=.\dict.c
# End Source File
# Begin Source File

SOURCE=.\easy.c
# End Source File
# Begin Source File

SOURCE=.\escape.c
# End Source File
# Begin Source File

SOURCE=.\file.c
# End Source File
# Begin Source File

SOURCE=.\formdata.c
# End Source File
# Begin Source File

SOURCE=.\ftp.c
# End Source File
# Begin Source File

SOURCE=.\getenv.c
# End Source File
# Begin Source File

SOURCE=.\getinfo.c
# End Source File
# Begin Source File

SOURCE=.\gtls.c
# End Source File
# Begin Source File

SOURCE=.\hash.c
# End Source File
# Begin Source File

SOURCE=.\hostares.c
# End Source File
# Begin Source File

SOURCE=.\hostasyn.c
# End Source File
# Begin Source File

SOURCE=.\hostip.c
# End Source File
# Begin Source File

SOURCE=.\hostip4.c
# End Source File
# Begin Source File

SOURCE=.\hostip6.c
# End Source File
# Begin Source File

SOURCE=.\hostsyn.c
# End Source File
# Begin Source File

SOURCE=.\hostthre.c
# End Source File
# Begin Source File

SOURCE=.\http.c
# End Source File
# Begin Source File

SOURCE=.\http_chunks.c
# End Source File
# Begin Source File

SOURCE=.\http_digest.c
# End Source File
# Begin Source File

SOURCE=.\http_negotiate.c
# End Source File
# Begin Source File

SOURCE=.\http_ntlm.c
# End Source File
# Begin Source File

SOURCE=.\if2ip.c
# End Source File
# Begin Source File

SOURCE=.\inet_ntop.c
# End Source File
# Begin Source File

SOURCE=.\inet_pton.c
# End Source File
# Begin Source File

SOURCE=.\krb4.c
# End Source File
# Begin Source File

SOURCE=.\ldap.c
# End Source File
# Begin Source File

SOURCE=.\llist.c
# End Source File
# Begin Source File

SOURCE=.\md5.c
# End Source File
# Begin Source File

SOURCE=.\memdebug.c
# End Source File
# Begin Source File

SOURCE=.\mprintf.c
# End Source File
# Begin Source File

SOURCE=.\multi.c
# End Source File
# Begin Source File

SOURCE=.\netrc.c
# End Source File
# Begin Source File

SOURCE=.\parsedate.c
# End Source File
# Begin Source File

SOURCE=.\progress.c
# End Source File
# Begin Source File

SOURCE=.\rawstr.c
# End Source File
# Begin Source File

SOURCE=.\security.c
# End Source File
# Begin Source File

SOURCE=.\select.c
# End Source File
# Begin Source File

SOURCE=.\sendf.c
# End Source File
# Begin Source File

SOURCE=.\share.c
# End Source File
# Begin Source File

SOURCE=.\socks.c
# End Source File
# Begin Source File

SOURCE=.\speedcheck.c
# End Source File
# Begin Source File

SOURCE=.\splay.c
# End Source File
# Begin Source File

SOURCE=.\sslgen.c
# End Source File
# Begin Source File

SOURCE=.\ssluse.c
# End Source File
# Begin Source File

SOURCE=.\strequal.c
# End Source File
# Begin Source File

SOURCE=.\strerror.c
# End Source File
# Begin Source File

SOURCE=.\strtok.c
# End Source File
# Begin Source File

SOURCE=.\strtoofft.c
# End Source File
# Begin Source File

SOURCE=.\telnet.c
# End Source File
# Begin Source File

SOURCE=.\tftp.c
# End Source File
# Begin Source File

SOURCE=.\timeval.c
# End Source File
# Begin Source File

SOURCE=.\transfer.c
# End Source File
# Begin Source File

SOURCE=.\url.c
# End Source File
# Begin Source File

SOURCE=.\version.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ldap.h
# End Source File
# End Group
# End Target
# End Project
