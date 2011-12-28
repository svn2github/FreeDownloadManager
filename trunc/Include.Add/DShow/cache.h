/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CACHE__
#define __CACHE__

class CCache : CBaseObject {

    

    CCache(const CCache &refCache);
    CCache &operator=(const CCache &refCache);

private:

    

    CBaseObject **m_ppObjects;
    const INT m_iCacheSize;
    INT m_iUsed;

public:

    CCache(TCHAR *pName,INT iItems);
    virtual ~CCache();

    
    CBaseObject *AddToCache(CBaseObject *pObject);

    
    CBaseObject *RemoveFromCache();

    
    void RemoveAll(void);

    
    INT GetCacheSize(void) const {return m_iCacheSize;};
};

#endif 

