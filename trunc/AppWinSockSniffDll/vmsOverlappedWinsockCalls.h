/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#pragma once
class vmsOverlappedWinsockCalls
{
public:
	struct Call
	{
		SOCKET s;
		LPWSABUF lpBuffers;
		DWORD dwBufferCount;
		LPWSAOVERLAPPED lpOverlapped;
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine;
		bool bSend;
	};
public:
	vmsOverlappedWinsockCalls(void);
	virtual ~vmsOverlappedWinsockCalls(void);

protected:
	vector <Call> m_vCalls;
	vmsCriticalSection m_csCalls;
public:
	void AddCall(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, bool bSend);
	int FindCallIndex(LPWSAOVERLAPPED lpOverlapped);
	Call* getCall(int nIndex);
	void LockList(bool bLock);
	void RemoveCall(int nIndex);
};

