/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#ifndef __WXDEBUG__
#define __WXDEBUG__                                                              

enum {  LOG_TIMING = 0x01,    
        LOG_TRACE = 0x02,     
        LOG_MEMORY =  0x04,   
        LOG_LOCKING = 0x08,   
        LOG_ERROR = 0x10,     
        LOG_CUSTOM1 = 0x20,
        LOG_CUSTOM2 = 0x40,
        LOG_CUSTOM3 = 0x80,
        LOG_CUSTOM4 = 0x100,
        LOG_CUSTOM5 = 0x200,
};

#define LOG_FORCIBLY_SET 0x80000000

enum {  CDISP_HEX = 0x01,
        CDISP_DEC = 0x02};          

typedef struct tag_ObjectDesc {
    const CHAR *m_szName;
    const WCHAR *m_wszName;
    DWORD m_dwCookie;
    tag_ObjectDesc *m_pNext;
} ObjectDesc;

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#ifdef DEBUG

    #define NAME(x) TEXT(x)

    

    void WINAPI DbgInitKeyLevels(HKEY hKey, bool fTakeMax);
    void WINAPI DbgInitGlobalSettings(bool fTakeMax);
    void WINAPI DbgInitModuleSettings(bool fTakeMax);
    void WINAPI DbgInitModuleName();
    DWORD WINAPI DbgRegisterObjectCreation(
        const CHAR *szObjectName, const WCHAR *wszObjectName);

    BOOL WINAPI DbgRegisterObjectDestruction(DWORD dwCookie);

    

    BOOL WINAPI DbgCheckModuleLevel(DWORD Type,DWORD Level);
    void WINAPI DbgSetModuleLevel(DWORD Type,DWORD Level);
    void WINAPI DbgSetAutoRefreshLevels(bool fAuto);

    

    void WINAPI DbgInitialise(HINSTANCE hInst);
    void WINAPI DbgTerminate();

    void WINAPI DbgDumpObjectRegister();

    

    void WINAPI DbgAssert(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine);
    void WINAPI DbgBreakPoint(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine);
    void WINAPI DbgBreakPoint(const TCHAR *pFileName,INT iLine,const TCHAR* szFormatString,...);

    void WINAPI DbgKernelAssert(const TCHAR *pCondition,const TCHAR *pFileName,INT iLine);
    void WINAPI DbgLogInfo(DWORD Type,DWORD Level,const TCHAR *pFormat,...);
#ifdef UNICODE
    void WINAPI DbgLogInfo(DWORD Type,DWORD Level,const CHAR *pFormat,...);
    void WINAPI DbgAssert(const CHAR *pCondition,const CHAR *pFileName,INT iLine);
    void WINAPI DbgBreakPoint(const CHAR *pCondition,const CHAR *pFileName,INT iLine);
    void WINAPI DbgKernelAssert(const CHAR *pCondition,const CHAR *pFileName,INT iLine);
#endif
    void WINAPI DbgOutString(LPCTSTR psz);

    
    DWORD WINAPI DbgWaitForSingleObject(HANDLE h);
    DWORD WINAPI DbgWaitForMultipleObjects(DWORD nCount,
                                    CONST HANDLE *lpHandles,
                                    BOOL bWaitAll);
    void WINAPI DbgSetWaitTimeout(DWORD dwTimeout);

#ifdef __strmif_h__
    
    void WINAPI DisplayType(LPTSTR label, const AM_MEDIA_TYPE *pmtIn);

    
    void WINAPI DumpGraph(IFilterGraph *pGraph, DWORD dwLevel);
#endif

    #define KASSERT(_x_) if (!(_x_))         \
        DbgKernelAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    
    

    #define KDbgBreak(_x_)                   \
        DbgKernelAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    
    
    
    
    #ifndef ASSERT
        #define ASSERT(_x_) if (!(_x_))         \
            DbgAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)
    #endif

    #define DbgAssertAligned( _ptr_, _alignment_ ) ASSERT( ((DWORD_PTR) (_ptr_)) % (_alignment_) == 0)

    
    

    #define DbgBreak(_x_)                   \
        DbgBreakPoint(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    #define EXECUTE_ASSERT(_x_) ASSERT(_x_)
    #define DbgLog(_x_) DbgLogInfo _x_
    

    #define NOTE(_x_)             DbgLog((LOG_TRACE,5,TEXT(_x_)))
    #define NOTE1(_x_,a)          DbgLog((LOG_TRACE,5,TEXT(_x_),a))
    #define NOTE2(_x_,a,b)        DbgLog((LOG_TRACE,5,TEXT(_x_),a,b))
    #define NOTE3(_x_,a,b,c)      DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c))
    #define NOTE4(_x_,a,b,c,d)    DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c,d))
    #define NOTE5(_x_,a,b,c,d,e)  DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c,d,e))

#else

    
    
    
    

    #define NAME(_x_) ((TCHAR *) NULL)

    #define DbgInitialise(hInst)
    #define DbgTerminate()
    #define DbgLog(_x_) 0
    #define DbgOutString(psz)
    #define DbgAssertAligned( _ptr_, _alignment_ ) 0

    #define DbgRegisterObjectCreation(pObjectName)
    #define DbgRegisterObjectDestruction(dwCookie)
    #define DbgDumpObjectRegister()

    #define DbgCheckModuleLevel(Type,Level)
    #define DbgSetModuleLevel(Type,Level)
    #define DbgSetAutoRefreshLevels(fAuto)

    #define DbgWaitForSingleObject(h)  WaitForSingleObject(h, INFINITE)
    #define DbgWaitForMultipleObjects(nCount, lpHandles, bWaitAll)     \
               WaitForMultipleObjects(nCount, lpHandles, bWaitAll, INFINITE)
    #define DbgSetWaitTimeout(dwTimeout)

    #define KDbgBreak(_x_)
    #define DbgBreak(_x_)

    #define KASSERT(_x_) ((void)0)
    #ifndef ASSERT
	#define ASSERT(_x_) ((void)0)
    #endif
    #define EXECUTE_ASSERT(_x_) ((void)(_x_))

    

    #define NOTE(_x_) ((void)0)
    #define NOTE1(_x_,a) ((void)0)
    #define NOTE2(_x_,a,b) ((void)0)
    #define NOTE3(_x_,a,b,c) ((void)0)
    #define NOTE4(_x_,a,b,c,d) ((void)0)
    #define NOTE5(_x_,a,b,c,d,e) ((void)0)

    #define DisplayType(label, pmtIn) ((void)0)
    #define DumpGraph(pGraph, label) ((void)0)
#endif      

#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}                              

#ifdef DEBUG
    #define VFWROBUST
#endif

#ifdef VFWROBUST

    #define ValidateReadPtr(p,cb) \
        {if(IsBadReadPtr((PVOID)p,cb) == TRUE) \
            DbgBreak("Invalid read pointer");}

    #define ValidateWritePtr(p,cb) \
        {if(IsBadWritePtr((PVOID)p,cb) == TRUE) \
            DbgBreak("Invalid write pointer");}

    #define ValidateReadWritePtr(p,cb) \
        {ValidateReadPtr(p,cb) ValidateWritePtr(p,cb)}

    #define ValidateStringPtr(p) \
        {if(IsBadStringPtr((LPCTSTR)p,INFINITE) == TRUE) \
            DbgBreak("Invalid string pointer");}

    #define ValidateStringPtrA(p) \
        {if(IsBadStringPtrA((LPCSTR)p,INFINITE) == TRUE) \
            DbgBreak("Invalid ANSI string pointer");}

    #define ValidateStringPtrW(p) \
        {if(IsBadStringPtrW((LPCWSTR)p,INFINITE) == TRUE) \
            DbgBreak("Invalid UNICODE string pointer");}

#else
    #define ValidateReadPtr(p,cb) 0
    #define ValidateWritePtr(p,cb) 0
    #define ValidateReadWritePtr(p,cb) 0
    #define ValidateStringPtr(p) 0
    #define ValidateStringPtrA(p) 0
    #define ValidateStringPtrW(p) 0
#endif  

#ifdef _OBJBASE_H_

    
    
    
    
    
    

    typedef struct {
        CHAR   *szName;
        GUID    guid;
    } GUID_STRING_ENTRY;

    class CGuidNameList {
    public:
        CHAR *operator [] (const GUID& guid);
    };

    extern CGuidNameList GuidNames;

#endif

#ifndef REMIND
    
    
    
      

    #define QUOTE(x) #x
    #define QQUOTE(y) QUOTE(y)
    #define REMIND(str) __FILE__ "(" QQUOTE(__LINE__) ") :  " str
#endif              

class CDispBasic
{
public:
    CDispBasic() { m_pString = m_String; };
    ~CDispBasic();
protected:
    PTCHAR m_pString;  
    TCHAR m_String[50];
};
class CDisp : public CDispBasic
{
public:
    CDisp(LONGLONG ll, int Format = CDISP_HEX); 
    CDisp(REFCLSID clsid);      
    CDisp(double d);            
#ifdef __strmif_h__
#ifdef __STREAMS__
    CDisp(CRefTime t);          
#endif
    CDisp(IPin *pPin);          
    CDisp(IUnknown *pUnk);      
#endif 
    ~CDisp();

    
    operator LPCTSTR()
    {
        return (LPCTSTR)m_pString;
    };
};

 
#if defined(DEBUG)
class CAutoTrace
{
private:
    const TCHAR* _szBlkName;
    const int _level;
    static const TCHAR _szEntering[];
    static const TCHAR _szLeaving[];
public:
    CAutoTrace(const TCHAR* szBlkName, const int level = 2)
        : _szBlkName(szBlkName), _level(level)
    {DbgLog((LOG_TRACE, _level, _szEntering, _szBlkName));}
 
    ~CAutoTrace()
    {DbgLog((LOG_TRACE, _level, _szLeaving, _szBlkName));}
};
 
#define AMTRACE(_x_) CAutoTrace __trace _x_
#else
#define AMTRACE(_x_)
#endif

#endif   

