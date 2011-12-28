/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __MEASURE__
#define __MEASURE__

#ifdef PERF
#define MSR_INIT() Msr_Init()
#define MSR_TERMINATE() Msr_Terminate()
#define MSR_REGISTER(a) Msr_Register(a)
#define MSR_RESET(a) Msr_Reset(a)
#define MSR_CONTROL(a) Msr_Control(a)
#define MSR_START(a) Msr_Start(a)
#define MSR_STOP(a) Msr_Stop(a)
#define MSR_NOTE(a) Msr_Note(a)
#define MSR_INTEGER(a,b) Msr_Integer(a,b)
#define MSR_DUMP(a) Msr_Dump(a)
#define MSR_DUMPSTATS(a) Msr_DumpStats(a)
#else
#define MSR_INIT() ((void)0)
#define MSR_TERMINATE() ((void)0)
#define MSR_REGISTER(a) 0
#define MSR_RESET(a) ((void)0)
#define MSR_CONTROL(a) ((void)0)
#define MSR_START(a) ((void)0)
#define MSR_STOP(a) ((void)0)
#define MSR_NOTE(a) ((void)0)
#define MSR_INTEGER(a,b) ((void)0)
#define MSR_DUMP(a) ((void)0)
#define MSR_DUMPSTATS(a) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void WINAPI Msr_Init(void);

void WINAPI Msr_Terminate(void);

int  WINAPI Msr_Register(LPTSTR Incident);

void WINAPI Msr_Reset(int Id);

#define MSR_RESET_ALL 0
#define MSR_PAUSE 1
#define MSR_RUN 2

void WINAPI Msr_Control(int iAction);

void WINAPI Msr_Start(int Id);

void WINAPI Msr_Stop(int Id);

void WINAPI Msr_Note(int Id);

void WINAPI Msr_Integer(int Id, int n);

void WINAPI Msr_Dump(HANDLE hFile);

void WINAPI Msr_DumpStats(HANDLE hFile);

typedef void WINAPI MSR_DUMPPROC(HANDLE hFile);
typedef void WINAPI MSR_CONTROLPROC(int iAction);

#ifdef __cplusplus
}
#endif

#endif 
