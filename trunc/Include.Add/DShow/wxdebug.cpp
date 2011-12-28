/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#define _WINDLL

#include <streams.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef DEBUG
#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif 
#endif 
#endif 

#include <tchar.h>

#ifdef DEBUG

const INT iDEBUGINFO = 1024;                 

TCHAR *pKeyNames[] = {
    TEXT("TIMING"),      
    TEXT("TRACE"),       
    TEXT("MEMORY"),      
    TEXT("LOCKING"),     
    TEXT("ERROR"),       
    TEXT("CUSTOM1"),
    TEXT("CUSTOM2"),
    TEXT("CUSTOM3"),
    TEXT("CUSTOM4"),
    TEXT("CUSTOM5")
    };

const TCHAR CAutoTrace::_szEntering[] = TEXT("Entering: %s");
const TCHAR CAutoTrace::_szLeaving[]  = TEXT("Leaving: %s");

const INT iMAXLEVELS = NUMELMS(pKeyNames);  

HINSTANCE m_hInst;                          
TCHAR m_ModuleName[iDEBUGINFO];             
DWORD m_Levels[iMAXLEVELS];                 
CRITICAL_SECTION m_CSDebug;                 
DWORD m_dwNextCookie;                       
ObjectDesc *pListHead = NULL;               
DWORD m_dwObjectCount;                      
BOOL m_bInit = FALSE;                       
HANDLE m_hOutput = INVALID_HANDLE_VALUE;    
DWORD dwWaitTimeout = INFINITE;             
DWORD dwTimeOffset;			    
bool g_fUseKASSERT = false;                 
bool g_fDbgInDllEntryPoint = false;
bool g_fAutoRefreshLevels = false;

const TCHAR *pBaseKey = TEXT("SOFTWARE\\Debug");
const TCHAR *pGlobalKey = TEXT("GLOBAL");
static CHAR *pUnknownName = "UNKNOWN";

TCHAR *TimeoutName = TEXT("TIMEOUT");

void WINAPI DbgInitialise(HINSTANCE hInst)
{
    InitializeCriticalSection(&m_CSDebug);
    m_bInit = TRUE;

    m_hInst = hInst;
    DbgInitModuleName();
    if (GetProfileInt(m_ModuleName, TEXT("BreakOnLoad"), 0))
       DebugBreak();
    DbgInitModuleSettings(false);
    DbgInitGlobalSettings(true);
    dwTimeOffset = timeGetTime();
}

void WINAPI DbgTerminate()
{
    if (m_hOutput != INVALID_HANDLE_VALUE) {
       EXECUTE_ASSERT(CloseHandle(m_hOutput));
       m_hOutput = INVALID_HANDLE_VALUE;
    }
    DeleteCriticalSection(&m_CSDebug);
    m_bInit = FALSE;
}

void WINAPI DbgInitKeyLevels(HKEY hKey, bool fTakeMax)
{
    LONG lReturn;               
    LONG lKeyPos;               
    DWORD dwKeySize;            
    DWORD dwKeyType;            
    DWORD dwKeyValue;           

    
    for (lKeyPos = 0;lKeyPos < iMAXLEVELS;lKeyPos++) {

        dwKeySize = sizeof(DWORD);
        lReturn = RegQueryValueEx(
            hKey,                       
            pKeyNames[lKeyPos],         
            NULL,                       
            &dwKeyType,                 
            (LPBYTE) &dwKeyValue,       
            &dwKeySize );               

        

        if (lReturn != ERROR_SUCCESS || dwKeyType != REG_DWORD)  {

            dwKeyValue = 0;
            lReturn = RegSetValueEx(
                hKey,                   
                pKeyNames[lKeyPos],     
                (DWORD) 0,              
                REG_DWORD,              
                (PBYTE) &dwKeyValue,    
                sizeof(DWORD));         

            if (lReturn != ERROR_SUCCESS) {
                DbgLog((LOG_ERROR,0,TEXT("Could not create subkey %s"),pKeyNames[lKeyPos]));
                dwKeyValue = 0;
            }
        }
        if(fTakeMax)
        {
            m_Levels[lKeyPos] = max(dwKeyValue,m_Levels[lKeyPos]);
        }
        else
        {
            if((m_Levels[lKeyPos] & LOG_FORCIBLY_SET) == 0) {
                m_Levels[lKeyPos] = dwKeyValue;
            }
        }
    }

    
    dwKeySize = sizeof(DWORD);
    lReturn = RegQueryValueEx(
        hKey,                       
        TimeoutName,                
        NULL,                       
        &dwKeyType,                 
        (LPBYTE) &dwWaitTimeout,    
        &dwKeySize );               

    

    if (lReturn != ERROR_SUCCESS || dwKeyType != REG_DWORD)  {

        dwWaitTimeout = INFINITE;
        lReturn = RegSetValueEx(
            hKey,                   
            TimeoutName,            
            (DWORD) 0,              
            REG_DWORD,              
            (PBYTE) &dwWaitTimeout, 
            sizeof(DWORD));         

        if (lReturn != ERROR_SUCCESS) {
            DbgLog((LOG_ERROR,0,TEXT("Could not create subkey %s"),pKeyNames[lKeyPos]));
            dwWaitTimeout = INFINITE;
        }
    }
}

void WINAPI DbgOutString(LPCTSTR psz)
{
    if (m_hOutput != INVALID_HANDLE_VALUE) {
        UINT  cb = lstrlen(psz);
        DWORD dw;
#ifdef UNICODE
        CHAR szDest[2048];
        WideCharToMultiByte(CP_ACP, 0, psz, -1, szDest, NUMELMS(szDest), 0, 0);
        WriteFile (m_hOutput, szDest, cb, &dw, NULL);
#else
        WriteFile (m_hOutput, psz, cb, &dw, NULL);
#endif
    } else {
        OutputDebugString (psz);
    }
}

void WINAPI DbgInitLogTo (
    HKEY hKey)
{
    LONG  lReturn;
    DWORD dwKeyType;
    DWORD dwKeySize;
    TCHAR szFile[MAX_PATH] = {0};
    static const TCHAR cszKey[] = TEXT("LogToFile");

    dwKeySize = MAX_PATH;
    lReturn = RegQueryValueEx(
        hKey,                       
        cszKey,                     
        NULL,                       
        &dwKeyType,                 
        (LPBYTE) szFile,            
        &dwKeySize);                

    
    
    if (lReturn != ERROR_SUCCESS || dwKeyType != REG_SZ)
       {
       dwKeySize = 1;
       lReturn = RegSetValueEx(
            hKey,                   
            cszKey,                 
            (DWORD) 0,              
            REG_SZ,                 
            (PBYTE)szFile,          
            dwKeySize);            
       }

    
    
    if (m_hOutput != INVALID_HANDLE_VALUE) {
       EXECUTE_ASSERT(CloseHandle (m_hOutput));
       m_hOutput = INVALID_HANDLE_VALUE;
    }
    if (szFile[0] != 0)
       {
       if (!lstrcmpi(szFile, TEXT("Console"))) {
          m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
          if (m_hOutput == INVALID_HANDLE_VALUE) {
             AllocConsole ();
             m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
          }
          SetConsoleTitle (TEXT("ActiveX Debug Output"));
       } else if (szFile[0] &&
                lstrcmpi(szFile, TEXT("Debug")) &&
                lstrcmpi(szFile, TEXT("Debugger")) &&
                lstrcmpi(szFile, TEXT("Deb")))
          {
            m_hOutput = CreateFile(szFile, GENERIC_WRITE,
                                 FILE_SHARE_READ,
                                 NULL, OPEN_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);
          if (INVALID_HANDLE_VALUE != m_hOutput)
              {
              static const TCHAR cszBar[] = TEXT("\r\n\r\n=====DbgInitialize()=====\r\n\r\n");
              SetFilePointer (m_hOutput, 0, NULL, FILE_END);
              DbgOutString (cszBar);
              }
          }
       }
}

void WINAPI DbgInitGlobalSettings(bool fTakeMax)
{
    LONG lReturn;               
    TCHAR szInfo[iDEBUGINFO];   
    HKEY hGlobalKey;            

    
    wsprintf(szInfo,TEXT("%s\\%s"),pBaseKey,pGlobalKey);

    
    lReturn = RegCreateKeyEx(HKEY_LOCAL_MACHINE,   
                             szInfo,               
                             (DWORD) 0,            
                             NULL,                 
                             (DWORD) 0,            
                             KEY_ALL_ACCESS,       
                             NULL,                 
                             &hGlobalKey,          
                             NULL);                

    if (lReturn != ERROR_SUCCESS) {
        DbgLog((LOG_ERROR,0,TEXT("Could not access GLOBAL module key")));
        return;
    }

    DbgInitKeyLevels(hGlobalKey, fTakeMax);
    RegCloseKey(hGlobalKey);
}

void WINAPI DbgInitModuleSettings(bool fTakeMax)
{
    LONG lReturn;               
    TCHAR szInfo[iDEBUGINFO];   
    HKEY hModuleKey;            

    
    wsprintf(szInfo,TEXT("%s\\%s"),pBaseKey,m_ModuleName);

    
    lReturn = RegCreateKeyEx(HKEY_LOCAL_MACHINE,   
                             szInfo,               
                             (DWORD) 0,            
                             NULL,                 
                             (DWORD) 0,            
                             KEY_ALL_ACCESS,       
                             NULL,                 
                             &hModuleKey,          
                             NULL);                

    if (lReturn != ERROR_SUCCESS) {
        DbgLog((LOG_ERROR,0,TEXT("Could not access module key")));
        return;
    }

    DbgInitLogTo(hModuleKey);
    DbgInitKeyLevels(hModuleKey, fTakeMax);
    RegCloseKey(hModuleKey);
}

void WINAPI DbgInitModuleName()
{
    TCHAR FullName[iDEBUGINFO];     
    TCHAR *pName;                   

    GetModuleFileName(m_hInst,FullName,iDEBUGINFO);
    pName = _tcsrchr(FullName,'\\');
    if (pName == NULL) {
        pName = FullName;
    } else {
        pName++;
    }
    lstrcpy(m_ModuleName,pName);
}

struct MsgBoxMsg
{
    HWND hwnd;
    TCHAR *szTitle;
    TCHAR *szMessage;
    DWORD dwFlags;
    INT iResult;
};

DWORD WINAPI MsgBoxThread(
  LPVOID lpParameter   
  )
{
    MsgBoxMsg *pmsg = (MsgBoxMsg *)lpParameter;
    pmsg->iResult = MessageBox(
        pmsg->hwnd,
        pmsg->szTitle,
        pmsg->szMessage,
        pmsg->dwFlags);
    
    return 0;
}

INT MessageBoxOtherThread(
    HWND hwnd,
    TCHAR *szTitle,
    TCHAR *szMessage,
    DWORD dwFlags)
{
    if(g_fDbgInDllEntryPoint)
    {
        
        
        return MessageBox(hwnd, szTitle, szMessage, dwFlags);
    }
    else
    {
        MsgBoxMsg msg = {hwnd, szTitle, szMessage, dwFlags, 0};
        DWORD dwid;
        HANDLE hThread = CreateThread(
            0,                      
            0,                      
            MsgBoxThread,
            (void *)&msg,           
            0,                      
            &dwid);
        if(hThread)
        {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            return msg.iResult;
        }

        
        return IDCANCEL;
    }
}

void WINAPI DbgAssert(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine)
{
    if(g_fUseKASSERT)
    {
        DbgKernelAssert(pCondition, pFileName, iLine);
    }
    else
    {

        TCHAR szInfo[iDEBUGINFO];

        wsprintf(szInfo, TEXT("%s \nAt line %d of %s\nContinue? (Cancel to debug)"),
                 pCondition, iLine, pFileName);

        INT MsgId = MessageBoxOtherThread(NULL,szInfo,TEXT("ASSERT Failed"),
                                          MB_SYSTEMMODAL |
                                          MB_ICONHAND |
                                          MB_YESNOCANCEL |
                                          MB_SETFOREGROUND);
        switch (MsgId)
        {
          case IDNO:              

              FatalAppExit(FALSE, TEXT("Application terminated"));
              break;

          case IDCANCEL:          

              DebugBreak();
              break;

          case IDYES:             
              break;
        }
    }
}

void WINAPI DbgBreakPoint(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine)
{
    if(g_fUseKASSERT)
    {
        DbgKernelAssert(pCondition, pFileName, iLine);
    }
    else
    {
        TCHAR szInfo[iDEBUGINFO];

        wsprintf(szInfo, TEXT("%s \nAt line %d of %s\nContinue? (Cancel to debug)"),
                 pCondition, iLine, pFileName);

        INT MsgId = MessageBoxOtherThread(NULL,szInfo,TEXT("Hard coded break point"),
                                          MB_SYSTEMMODAL |
                                          MB_ICONHAND |
                                          MB_YESNOCANCEL |
                                          MB_SETFOREGROUND);
        switch (MsgId)
        {
          case IDNO:              

              FatalAppExit(FALSE, TEXT("Application terminated"));
              break;

          case IDCANCEL:          

              DebugBreak();
              break;

          case IDYES:             
              break;
        }
    }
}

void WINAPI DbgBreakPoint(const TCHAR *pFileName,INT iLine,const TCHAR* szFormatString,...)
{
    
    
    
    
    
    const DWORD MAX_BREAK_POINT_MESSAGE_SIZE = 2000;

    TCHAR szBreakPointMessage[MAX_BREAK_POINT_MESSAGE_SIZE];
    
    const DWORD MAX_CHARS_IN_BREAK_POINT_MESSAGE = sizeof(szBreakPointMessage) / sizeof(TCHAR);

    va_list va;
    va_start( va, szFormatString );

    int nReturnValue = _vsntprintf( szBreakPointMessage, MAX_CHARS_IN_BREAK_POINT_MESSAGE, szFormatString, va );

    va_end(va);
    
    
    if( -1 == nReturnValue ) {
        DbgBreak( "ERROR in DbgBreakPoint().  The variable length debug message could not be displayed because _vsnprintf() failed." );
        return;
    }

    ::DbgBreakPoint( szBreakPointMessage, pFileName, iLine );
}

BOOL WINAPI DbgCheckModuleLevel(DWORD Type,DWORD Level)
{
    if(g_fAutoRefreshLevels)
    {
        
        
        static DWORD g_dwLastRefresh = 0;
        DWORD dwTime = timeGetTime();
        if(dwTime - g_dwLastRefresh > 1000) {
            g_dwLastRefresh = dwTime;

            
            
            
            DbgInitModuleSettings(false);
        }
    }

    DWORD Mask = 0x01;

    
    if ((Type & ((1<<iMAXLEVELS)-1))) {

	
	if (0==Level)
	    return(TRUE);
	
        for (LONG lKeyPos = 0;lKeyPos < iMAXLEVELS;lKeyPos++) {
            if (Type & Mask) {
                if (Level <= (m_Levels[lKeyPos] & ~LOG_FORCIBLY_SET)) {
                    return TRUE;
                }
            }
            Mask <<= 1;
        }
    }
    return FALSE;
}

void WINAPI DbgSetModuleLevel(DWORD Type, DWORD Level)
{
    DWORD Mask = 0x01;

    for (LONG lKeyPos = 0;lKeyPos < iMAXLEVELS;lKeyPos++) {
        if (Type & Mask) {
            m_Levels[lKeyPos] = Level | LOG_FORCIBLY_SET;
        }
        Mask <<= 1;
    }
}

void WINAPI DbgSetAutoRefreshLevels(bool fAuto)
{
    g_fAutoRefreshLevels = fAuto;
}

#ifdef UNICODE

void WINAPI DbgLogInfo(DWORD Type,DWORD Level,const CHAR *pFormat,...)
{
    

    BOOL bAccept = DbgCheckModuleLevel(Type,Level);
    if (bAccept == FALSE) {
        return;
    }

    TCHAR szInfo[2000];

    

    va_list va;
    va_start(va, pFormat);

    lstrcpy(szInfo,m_ModuleName);
    wsprintf(szInfo + lstrlen(szInfo),
             TEXT("(tid %x) %8d : "),
             GetCurrentThreadId(), timeGetTime() - dwTimeOffset);

    CHAR szInfoA[2000];
    WideCharToMultiByte(CP_ACP, 0, szInfo, -1, szInfoA, NUMELMS(szInfoA), 0, 0);

    wvsprintfA(szInfoA + lstrlenA(szInfoA), pFormat, va);
    lstrcatA(szInfoA, "\r\n");

    WCHAR wszOutString[2000];
    MultiByteToWideChar(CP_ACP, 0, szInfoA, -1, wszOutString, NUMELMS(wszOutString));
    DbgOutString(wszOutString);

    va_end(va);
}

void DbgAssert(const CHAR *pCondition,const CHAR *pFileName,INT iLine)
{
    if(g_fUseKASSERT)
    {
        DbgKernelAssert(pCondition, pFileName, iLine);
    }
    else
    {

        TCHAR szInfo[iDEBUGINFO];

        wsprintf(szInfo, TEXT("%hs \nAt line %d of %hs\nContinue? (Cancel to debug)"),
                 pCondition, iLine, pFileName);

        INT MsgId = MessageBoxOtherThread(NULL,szInfo,TEXT("ASSERT Failed"),
                                          MB_SYSTEMMODAL |
                                          MB_ICONHAND |
                                          MB_YESNOCANCEL |
                                          MB_SETFOREGROUND);
        switch (MsgId)
        {
          case IDNO:              

              FatalAppExit(FALSE, TEXT("Application terminated"));
              break;

          case IDCANCEL:          

              DebugBreak();
              break;

          case IDYES:             
              break;
        }
    }
}

void WINAPI DbgBreakPoint(const CHAR *pCondition,const CHAR *pFileName,INT iLine)
{
    if(g_fUseKASSERT)
    {
        DbgKernelAssert(pCondition, pFileName, iLine);
    }
    else
    {
        TCHAR szInfo[iDEBUGINFO];

        wsprintf(szInfo, TEXT("%hs \nAt line %d of %hs\nContinue? (Cancel to debug)"),
                 pCondition, iLine, pFileName);

        INT MsgId = MessageBoxOtherThread(NULL,szInfo,TEXT("Hard coded break point"),
                                          MB_SYSTEMMODAL |
                                          MB_ICONHAND |
                                          MB_YESNOCANCEL |
                                          MB_SETFOREGROUND);
        switch (MsgId)
        {
          case IDNO:              

              FatalAppExit(FALSE, TEXT("Application terminated"));
              break;

          case IDCANCEL:          

              DebugBreak();
              break;

          case IDYES:             
              break;
        }
    }
}

void WINAPI DbgKernelAssert(const CHAR *pCondition,const CHAR *pFileName,INT iLine)
{
    DbgLog((LOG_ERROR,0,TEXT("Assertion FAILED (%hs) at line %d in file %hs"),
           pCondition, iLine, pFileName));
    DebugBreak();
}

#endif

void WINAPI DbgLogInfo(DWORD Type,DWORD Level,const TCHAR *pFormat,...)
{
    
    

    BOOL bAccept = DbgCheckModuleLevel(Type,Level);
    if (bAccept == FALSE) {
        return;
    }

    TCHAR szInfo[2000];

    

    va_list va;
    va_start(va, pFormat);

    lstrcpy(szInfo,m_ModuleName);
    wsprintf(szInfo + lstrlen(szInfo),
             TEXT("(tid %x) %8d : "),
             GetCurrentThreadId(), timeGetTime() - dwTimeOffset);

    _vstprintf(szInfo + lstrlen(szInfo), pFormat, va);
    lstrcat(szInfo, TEXT("\r\n"));
    DbgOutString(szInfo);

    va_end(va);
}

void WINAPI DbgKernelAssert(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine)
{
    DbgLog((LOG_ERROR,0,TEXT("Assertion FAILED (%s) at line %d in file %s"),
           pCondition, iLine, pFileName));
    DebugBreak();
}

DWORD WINAPI DbgRegisterObjectCreation(const CHAR *szObjectName,
                                       const WCHAR *wszObjectName)
{
    

    ASSERT(!!szObjectName ^ !!wszObjectName);

    

    ObjectDesc *pObject = new ObjectDesc;
    ASSERT(pObject);

    
    if (pObject == NULL) {
        return FALSE;
    }

    

    if (m_bInit == FALSE) {
        DbgInitialise(GetModuleHandle(NULL));
    }

    
    EnterCriticalSection(&m_CSDebug);

    
    if (!szObjectName && !wszObjectName) {
        szObjectName = pUnknownName;
    }

    

    pObject->m_szName = szObjectName;
    pObject->m_wszName = wszObjectName;
    pObject->m_dwCookie = ++m_dwNextCookie;
    pObject->m_pNext = pListHead;

    pListHead = pObject;
    m_dwObjectCount++;

    DWORD ObjectCookie = pObject->m_dwCookie;
    ASSERT(ObjectCookie);

    if(wszObjectName) {
        DbgLog((LOG_MEMORY,2,TEXT("Object created   %d (%ls) %d Active"),
                pObject->m_dwCookie, wszObjectName, m_dwObjectCount));
    } else {
        DbgLog((LOG_MEMORY,2,TEXT("Object created   %d (%hs) %d Active"),
                pObject->m_dwCookie, szObjectName, m_dwObjectCount));
    }

    LeaveCriticalSection(&m_CSDebug);
    return ObjectCookie;
}

BOOL WINAPI DbgRegisterObjectDestruction(DWORD dwCookie)
{
    
    EnterCriticalSection(&m_CSDebug);

    ObjectDesc *pObject = pListHead;
    ObjectDesc *pPrevious = NULL;

    

    while (pObject) {
        if (pObject->m_dwCookie == dwCookie) {
            break;
        }
        pPrevious = pObject;
        pObject = pObject->m_pNext;
    }

    if (pObject == NULL) {
        DbgBreak("Apparently destroying a bogus object");
        LeaveCriticalSection(&m_CSDebug);
        return FALSE;
    }

    

    if (pPrevious == NULL) {
        pListHead = pObject->m_pNext;
    } else {
        pPrevious->m_pNext = pObject->m_pNext;
    }

    

    m_dwObjectCount--;

    if(pObject->m_wszName) {
        DbgLog((LOG_MEMORY,2,TEXT("Object destroyed %d (%ls) %d Active"),
                pObject->m_dwCookie, pObject->m_wszName, m_dwObjectCount));
    } else {
        DbgLog((LOG_MEMORY,2,TEXT("Object destroyed %d (%hs) %d Active"),
                pObject->m_dwCookie, pObject->m_szName, m_dwObjectCount));
    }

    delete pObject;
    LeaveCriticalSection(&m_CSDebug);
    return TRUE;
}

void WINAPI DbgDumpObjectRegister()
{
    TCHAR szInfo[iDEBUGINFO];

    

    EnterCriticalSection(&m_CSDebug);
    ObjectDesc *pObject = pListHead;

    

    DbgLog((LOG_MEMORY,2,TEXT("")));
    DbgLog((LOG_MEMORY,2,TEXT("   ID             Object Description")));
    DbgLog((LOG_MEMORY,2,TEXT("")));

    while (pObject) {
        if(pObject->m_wszName) {
            wsprintf(szInfo,TEXT("%5d (%8x) %30ls"),pObject->m_dwCookie, &pObject, pObject->m_wszName);
        } else {
            wsprintf(szInfo,TEXT("%5d (%8x) %30hs"),pObject->m_dwCookie, &pObject, pObject->m_szName);
        }
        DbgLog((LOG_MEMORY,2,szInfo));
        pObject = pObject->m_pNext;
    }

    wsprintf(szInfo,TEXT("Total object count %5d"),m_dwObjectCount);
    DbgLog((LOG_MEMORY,2,TEXT("")));
    DbgLog((LOG_MEMORY,1,szInfo));
    LeaveCriticalSection(&m_CSDebug);
}

DWORD WINAPI DbgWaitForSingleObject(HANDLE h)
{
    DWORD dwWaitResult;
    do {
        dwWaitResult = WaitForSingleObject(h, dwWaitTimeout);
        ASSERT(dwWaitResult == WAIT_OBJECT_0);
    } while (dwWaitResult == WAIT_TIMEOUT);
    return dwWaitResult;
}
DWORD WINAPI DbgWaitForMultipleObjects(DWORD nCount,
                                CONST HANDLE *lpHandles,
                                BOOL bWaitAll)
{
    DWORD dwWaitResult;
    do {
        dwWaitResult = WaitForMultipleObjects(nCount,
                                              lpHandles,
                                              bWaitAll,
                                              dwWaitTimeout);
        ASSERT((DWORD)(dwWaitResult - WAIT_OBJECT_0) < MAXIMUM_WAIT_OBJECTS);
    } while (dwWaitResult == WAIT_TIMEOUT);
    return dwWaitResult;
}

void WINAPI DbgSetWaitTimeout(DWORD dwTimeout)
{
    dwWaitTimeout = dwTimeout;
}

#endif 

#ifdef _OBJBASE_H_

    

    GUID_STRING_ENTRY g_GuidNames[] = {
    #define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    { #name, { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } },
        #include <uuids.h>
    };

    CGuidNameList GuidNames;
    int g_cGuidNames = sizeof(g_GuidNames) / sizeof(g_GuidNames[0]);

    char *CGuidNameList::operator [] (const GUID &guid)
    {
        for (int i = 0; i < g_cGuidNames; i++) {
            if (g_GuidNames[i].guid == guid) {
                return g_GuidNames[i].szName;
            }
        }
        if (guid == GUID_NULL) {
            return "GUID_NULL";
        }

	
	
	
        return "Unknown GUID Name";
    }

#endif 

CDisp::CDisp(LONGLONG ll, int Format)
{
    
    
    LARGE_INTEGER li;
    li.QuadPart = ll;
    switch (Format) {
	case CDISP_DEC:
	{
	    TCHAR  temp[20];
	    int pos=20;
	    temp[--pos] = 0;
	    int digit;
	    
	    do {
		
	        digit = li.LowPart % 10;
		li.QuadPart /= 10;
		temp[--pos] = (TCHAR) digit+L'0';
	    } while (li.QuadPart);
	    wsprintf(m_String, TEXT("%s"), temp+pos);
	    break;
	}
	case CDISP_HEX:
	default:
	    wsprintf(m_String, TEXT("0x%X%8.8X"), li.HighPart, li.LowPart);
    }
};

CDisp::CDisp(REFCLSID clsid)
{
    WCHAR strClass[CHARS_IN_GUID+1];
    StringFromGUID2(clsid, strClass, sizeof(strClass) / sizeof(strClass[0]));
    ASSERT(sizeof(m_String)/sizeof(m_String[0]) >= CHARS_IN_GUID+1);
    wsprintf(m_String, TEXT("%ls"), strClass);
};

#ifdef __STREAMS__

CDisp::CDisp(CRefTime llTime)
{
    LPTSTR lpsz = m_String;
    LONGLONG llDiv;
    if (llTime < 0) {
        llTime = -llTime;
        lpsz += wsprintf(lpsz, TEXT("-"));
    }
    llDiv = (LONGLONG)24 * 3600 * 10000000;
    if (llTime >= llDiv) {
        lpsz += wsprintf(lpsz, TEXT("%d days "), (LONG)(llTime / llDiv));
        llTime = llTime % llDiv;
    }
    llDiv = (LONGLONG)3600 * 10000000;
    if (llTime >= llDiv) {
        lpsz += wsprintf(lpsz, TEXT("%d hrs "), (LONG)(llTime / llDiv));
        llTime = llTime % llDiv;
    }
    llDiv = (LONGLONG)60 * 10000000;
    if (llTime >= llDiv) {
        lpsz += wsprintf(lpsz, TEXT("%d mins "), (LONG)(llTime / llDiv));
        llTime = llTime % llDiv;
    }
    wsprintf(lpsz, TEXT("%d.%3.3d sec"),
             (LONG)llTime / 10000000,
             (LONG)((llTime % 10000000) / 10000));
};

#endif 

CDisp::CDisp(IPin *pPin)
{
    PIN_INFO pi;
    TCHAR str[MAX_PIN_NAME];
    CLSID clsid;

    if (pPin) {
       pPin->QueryPinInfo(&pi);
       pi.pFilter->GetClassID(&clsid);
       QueryPinInfoReleaseFilter(pi);
      #ifndef UNICODE
       WideCharToMultiByte(GetACP(), 0, pi.achName, lstrlenW(pi.achName) + 1,
                           str, MAX_PIN_NAME, NULL, NULL);
      #else
       lstrcpy(str, pi.achName);
      #endif
    } else {
       lstrcpy(str, TEXT("NULL IPin"));
    }

    m_pString = (PTCHAR) new TCHAR[lstrlen(str)+64];
    if (!m_pString) {
	return;
    }

    wsprintf(m_pString, TEXT("%hs(%s)"), GuidNames[clsid], str);
}

CDisp::CDisp(IUnknown *pUnk)
{
    IBaseFilter *pf;
    HRESULT hr = pUnk->QueryInterface(IID_IBaseFilter, (void **)&pf);
    if(SUCCEEDED(hr))
    {
        FILTER_INFO fi;
        hr = pf->QueryFilterInfo(&fi);
        if(SUCCEEDED(hr))
        {
            QueryFilterInfoReleaseGraph(fi);

            m_pString = new TCHAR[lstrlenW(fi.achName)  + 1];
            if(m_pString)
            {
                wsprintf(m_pString, TEXT("%ls"), fi.achName);
            }
        }

        pf->Release();

        return;
    }

    IPin *pp;
    hr = pUnk->QueryInterface(IID_IPin, (void **)&pp);
    if(SUCCEEDED(hr))
    {
        CDisp::CDisp(pp);
        pp->Release();
        return;
    }
}

CDisp::~CDisp()
{
}

CDispBasic::~CDispBasic()
{
    if (m_pString != m_String) {
	delete [] m_pString;
    }
}

CDisp::CDisp(double d)
{
#ifdef DEBUG
    _stprintf(m_String, TEXT("%.16g"), d);
#else
    wsprintf(m_String, TEXT("%d.%03d"), (int) d, (int) ((d - (int) d) * 1000));
#endif
}

#ifdef DEBUG
void WINAPI DisplayType(LPTSTR label, const AM_MEDIA_TYPE *pmtIn)
{

    

    DbgLog((LOG_TRACE,5,TEXT("")));
    DbgLog((LOG_TRACE,2,TEXT("%s  M type %s  S type %s"), label,
	    GuidNames[pmtIn->majortype],
	    GuidNames[pmtIn->subtype]));
    DbgLog((LOG_TRACE,5,TEXT("Subtype description %s"),GetSubtypeName(&pmtIn->subtype)));

    

    if (pmtIn->bTemporalCompression) {
	DbgLog((LOG_TRACE,5,TEXT("Temporally compressed")));
    } else {
	DbgLog((LOG_TRACE,5,TEXT("Not temporally compressed")));
    }

    if (pmtIn->bFixedSizeSamples) {
	DbgLog((LOG_TRACE,5,TEXT("Sample size %d"),pmtIn->lSampleSize));
    } else {
	DbgLog((LOG_TRACE,5,TEXT("Variable size samples")));
    }

    if (pmtIn->formattype == FORMAT_VideoInfo) {
	
	BITMAPINFOHEADER *pbmi = HEADER(pmtIn->pbFormat);
	VIDEOINFOHEADER *pVideoInfo = (VIDEOINFOHEADER *)pmtIn->pbFormat;

	DbgLog((LOG_TRACE,5,TEXT("Source rectangle (Left %d Top %d Right %d Bottom %d)"),
	       pVideoInfo->rcSource.left,
	       pVideoInfo->rcSource.top,
	       pVideoInfo->rcSource.right,
	       pVideoInfo->rcSource.bottom));

	DbgLog((LOG_TRACE,5,TEXT("Target rectangle (Left %d Top %d Right %d Bottom %d)"),
	       pVideoInfo->rcTarget.left,
	       pVideoInfo->rcTarget.top,
	       pVideoInfo->rcTarget.right,
	       pVideoInfo->rcTarget.bottom));

	DbgLog((LOG_TRACE,5,TEXT("Size of BITMAPINFO structure %d"),pbmi->biSize));
	if (pbmi->biCompression < 256) {
	    DbgLog((LOG_TRACE,2,TEXT("%dx%dx%d bit  (%d)"),
		    pbmi->biWidth, pbmi->biHeight,
		    pbmi->biBitCount, pbmi->biCompression));
	} else {
	    DbgLog((LOG_TRACE,2,TEXT("%dx%dx%d bit '%4.4hs'"),
		    pbmi->biWidth, pbmi->biHeight,
		    pbmi->biBitCount, &pbmi->biCompression));
	}

	DbgLog((LOG_TRACE,2,TEXT("Image size %d"),pbmi->biSizeImage));
	DbgLog((LOG_TRACE,5,TEXT("Planes %d"),pbmi->biPlanes));
	DbgLog((LOG_TRACE,5,TEXT("X Pels per metre %d"),pbmi->biXPelsPerMeter));
	DbgLog((LOG_TRACE,5,TEXT("Y Pels per metre %d"),pbmi->biYPelsPerMeter));
	DbgLog((LOG_TRACE,5,TEXT("Colours used %d"),pbmi->biClrUsed));

    } else if (pmtIn->majortype == MEDIATYPE_Audio) {
	DbgLog((LOG_TRACE,2,TEXT("     Format type %hs"),
	    GuidNames[pmtIn->formattype]));
	DbgLog((LOG_TRACE,2,TEXT("     Subtype %hs"),
	    GuidNames[pmtIn->subtype]));

	if ((pmtIn->subtype != MEDIASUBTYPE_MPEG1Packet)
	  && (pmtIn->cbFormat >= sizeof(PCMWAVEFORMAT)))
	{
	    

	    WAVEFORMATEX *pwfx = (WAVEFORMATEX *) pmtIn->pbFormat;
            DbgLog((LOG_TRACE,2,TEXT("wFormatTag %u"), pwfx->wFormatTag));
            DbgLog((LOG_TRACE,2,TEXT("nChannels %u"), pwfx->nChannels));
            DbgLog((LOG_TRACE,2,TEXT("nSamplesPerSec %lu"), pwfx->nSamplesPerSec));
            DbgLog((LOG_TRACE,2,TEXT("nAvgBytesPerSec %lu"), pwfx->nAvgBytesPerSec));
            DbgLog((LOG_TRACE,2,TEXT("nBlockAlign %u"), pwfx->nBlockAlign));
            DbgLog((LOG_TRACE,2,TEXT("wBitsPerSample %u"), pwfx->wBitsPerSample));

            

            if (pmtIn->cbFormat >= sizeof(WAVEFORMATEX)) {
                DbgLog((LOG_TRACE,2,TEXT("cbSize %u"), pwfx->cbSize));
            }
	} else {
	}

    } else {
	DbgLog((LOG_TRACE,2,TEXT("     Format type %hs"),
	    GuidNames[pmtIn->formattype]));
	
    }
}

void WINAPI DumpGraph(IFilterGraph *pGraph, DWORD dwLevel)
{
    if( !pGraph )
    {
        return;
    }

    IEnumFilters *pFilters;

    DbgLog((LOG_TRACE,dwLevel,TEXT("DumpGraph [%x]"), pGraph));

    if (FAILED(pGraph->EnumFilters(&pFilters))) {
	DbgLog((LOG_TRACE,dwLevel,TEXT("EnumFilters failed!")));
    }

    IBaseFilter *pFilter;
    ULONG	n;
    while (pFilters->Next(1, &pFilter, &n) == S_OK) {
	FILTER_INFO	info;

	if (FAILED(pFilter->QueryFilterInfo(&info))) {
	    DbgLog((LOG_TRACE,dwLevel,TEXT("    Filter [%x]  -- failed QueryFilterInfo"), pFilter));
	} else {
	    QueryFilterInfoReleaseGraph(info);

	    
	
	    DbgLog((LOG_TRACE,dwLevel,TEXT("    Filter [%x]  '%ls'"), pFilter, info.achName));

	    IEnumPins *pins;

	    if (FAILED(pFilter->EnumPins(&pins))) {
		DbgLog((LOG_TRACE,dwLevel,TEXT("EnumPins failed!")));
	    } else {

		IPin *pPin;
		while (pins->Next(1, &pPin, &n) == S_OK) {
		    PIN_INFO	info;

		    if (FAILED(pPin->QueryPinInfo(&info))) {
			DbgLog((LOG_TRACE,dwLevel,TEXT("          Pin [%x]  -- failed QueryPinInfo"), pPin));
		    } else {
			QueryPinInfoReleaseFilter(info);

			IPin *pPinConnected = NULL;

			HRESULT hr = pPin->ConnectedTo(&pPinConnected);

			if (pPinConnected) {
			    DbgLog((LOG_TRACE,dwLevel,TEXT("          Pin [%x]  '%ls' [%sput]")
							   TEXT("  Connected to pin [%x]"),
				    pPin, info.achName,
				    info.dir == PINDIR_INPUT ? TEXT("In") : TEXT("Out"),
				    pPinConnected));

			    pPinConnected->Release();

			    
			    
			    if (info.dir == PINDIR_OUTPUT) {
				AM_MEDIA_TYPE mt;

				hr = pPin->ConnectionMediaType(&mt);

				if (SUCCEEDED(hr)) {
				    DisplayType(TEXT("Connection type"), &mt);

				    FreeMediaType(mt);
				}
			    }
			} else {
			    DbgLog((LOG_TRACE,dwLevel,
				    TEXT("          Pin [%x]  '%ls' [%sput]"),
				    pPin, info.achName,
				    info.dir == PINDIR_INPUT ? TEXT("In") : TEXT("Out")));

			}
		    }

		    pPin->Release();

		}

		pins->Release();
	    }

	}
	
	pFilter->Release();
    }

    pFilters->Release();

}

#endif

