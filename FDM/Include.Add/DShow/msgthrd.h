/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                        

class CMsg {
public:
    UINT uMsg;
    DWORD dwFlags;
    LPVOID lpParam;
    CAMEvent *pEvent;

    CMsg(UINT u, DWORD dw, LPVOID lp, CAMEvent *pEvnt)
        : uMsg(u), dwFlags(dw), lpParam(lp), pEvent(pEvnt) {}

    CMsg()
        : uMsg(0), dwFlags(0L), lpParam(NULL), pEvent(NULL) {}
};          

class AM_NOVTABLE CMsgThread {
private:
    static DWORD WINAPI DefaultThreadProc(LPVOID lpParam);
    DWORD               m_ThreadId;
    HANDLE              m_hThread;

protected:

    
    
    CGenericList<CMsg>        m_ThreadQueue;
    CCritSec                  m_Lock;
    HANDLE                    m_hSem;
    LONG                      m_lWaiting;

public:
    CMsgThread()
        : m_ThreadId(0),
        m_hThread(NULL),
        m_lWaiting(0),
        m_hSem(NULL),
        
        m_ThreadQueue(NAME("MsgThread list"), 5)
        {
        }

    ~CMsgThread();
    
    
    void virtual GetThreadMsg(CMsg *msg);

    
    virtual void OnThreadInit() {
    };

    BOOL CreateThread();

    BOOL WaitForThreadExit(LPDWORD lpdwExitCode) {
        if (m_hThread != NULL) {
            WaitForSingleObject(m_hThread, INFINITE);
            return GetExitCodeThread(m_hThread, lpdwExitCode);
        }
        return FALSE;
    }

    DWORD ResumeThread() {
        return ::ResumeThread(m_hThread);
    }

    DWORD SuspendThread() {
        return ::SuspendThread(m_hThread);
    }

    int GetThreadPriority() {
        return ::GetThreadPriority(m_hThread);
    }

    BOOL SetThreadPriority(int nPriority) {
        return ::SetThreadPriority(m_hThread, nPriority);
    }

    HANDLE GetThreadHandle() {
        return m_hThread;
    }

    DWORD GetThreadId() {
        return m_ThreadId;
    }  

    void PutThreadMsg(UINT uMsg, DWORD dwMsgFlags,
                      LPVOID lpMsgParam, CAMEvent *pEvent = NULL) {
        CAutoLock lck(&m_Lock);
        CMsg* pMsg = new CMsg(uMsg, dwMsgFlags, lpMsgParam, pEvent);
        m_ThreadQueue.AddTail(pMsg);
        if (m_lWaiting != 0) {
            ReleaseSemaphore(m_hSem, m_lWaiting, 0);
            m_lWaiting = 0;
        }
    }

    
    
    
    
    virtual LRESULT ThreadMessageProc(
        UINT uMsg, DWORD dwFlags, LPVOID lpParam, CAMEvent *pEvent) = 0;
};

