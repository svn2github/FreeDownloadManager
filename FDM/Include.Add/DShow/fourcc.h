/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                                        

#ifndef __FOURCC__
#define __FOURCC__              

class FOURCCMap : public GUID
{

public:
    FOURCCMap();
    FOURCCMap(DWORD Fourcc);
    FOURCCMap(const GUID *);  

    DWORD GetFOURCC(void);
    void SetFOURCC(DWORD fourcc);
    void SetFOURCC(const GUID *);

private:
    void InitGUID();
};

#define GUID_Data2      0
#define GUID_Data3     0x10
#define GUID_Data4_1   0xaa000080
#define GUID_Data4_2   0x719b3800

inline void
FOURCCMap::InitGUID() {
    Data2 = GUID_Data2;
    Data3 = GUID_Data3;
    ((DWORD *)Data4)[0] = GUID_Data4_1;
    ((DWORD *)Data4)[1] = GUID_Data4_2;
}

inline
FOURCCMap::FOURCCMap() {
    InitGUID();
    SetFOURCC( DWORD(0));
}

inline
FOURCCMap::FOURCCMap(DWORD fourcc)
{
    InitGUID();
    SetFOURCC(fourcc);
}

inline
FOURCCMap::FOURCCMap(const GUID * pGuid)
{
    InitGUID();
    SetFOURCC(pGuid);
}

inline void
FOURCCMap::SetFOURCC(const GUID * pGuid)
{
    FOURCCMap * p = (FOURCCMap*) pGuid;
    SetFOURCC(p->GetFOURCC());
}

inline void
FOURCCMap::SetFOURCC(DWORD fourcc)
{
    Data1 = fourcc;
}

inline DWORD
FOURCCMap::GetFOURCC(void)
{
    return Data1;
}

#endif 

