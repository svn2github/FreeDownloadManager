/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>

#define INTERNALTRAVERSELIST(list, cursor)               \
for ( cursor = (list).GetHeadPositionI()           \
    ; cursor!=NULL                               \
    ; cursor = (list).Next(cursor)                \
    )

#define INTERNALREVERSETRAVERSELIST(list, cursor)        \
for ( cursor = (list).GetTailPositionI()           \
    ; cursor!=NULL                               \
    ; cursor = (list).Prev(cursor)                \
    )

CBaseList::CBaseList(TCHAR *pName,    
                     INT iItems) :    
#ifdef DEBUG
    CBaseObject(pName),
#endif
    m_pFirst(NULL),
    m_pLast(NULL),
    m_Count(0),
    m_Cache(iItems)
{
} 

CBaseList::CBaseList(TCHAR *pName) :  
#ifdef DEBUG
    CBaseObject(pName),
#endif
    m_pFirst(NULL),
    m_pLast(NULL),
    m_Count(0),
    m_Cache(DEFAULTCACHE)
{
} 

#ifdef UNICODE
CBaseList::CBaseList(CHAR *pName,    
                     INT iItems) :    
#ifdef DEBUG
    CBaseObject(pName),
#endif
    m_pFirst(NULL),
    m_pLast(NULL),
    m_Count(0),
    m_Cache(iItems)
{
} 

CBaseList::CBaseList(CHAR *pName) :  
#ifdef DEBUG
    CBaseObject(pName),
#endif
    m_pFirst(NULL),
    m_pLast(NULL),
    m_Count(0),
    m_Cache(DEFAULTCACHE)
{
} 

#endif

CBaseList::~CBaseList()
{
    

    RemoveAll();

} 

void CBaseList::RemoveAll()
{
    

    CNode *pn = m_pFirst;
    while (pn) {
        CNode *op = pn;
        pn = pn->Next();
        delete op;
    }

    

    m_Count = 0;
    m_pFirst = m_pLast = NULL;

} 

POSITION CBaseList::GetHeadPositionI() const
{
    return (POSITION) m_pFirst;
} 

POSITION CBaseList::GetTailPositionI() const
{
    return (POSITION) m_pLast;
} 

int CBaseList::GetCountI() const
{
    return m_Count;
} 

void *CBaseList::GetNextI(POSITION& rp) const
{
    

    if (rp == NULL) {
        return NULL;
    }

    

    void *pObject;

    

    CNode *pn = (CNode *) rp;
    ASSERT(pn != NULL);
    rp = (POSITION) pn->Next();

    

    pObject = pn->GetData();
    
    return pObject;
} 

void *CBaseList::GetI(POSITION p) const
{
    if (p == NULL) {
        return NULL;
    }

    CNode * pn = (CNode *) p;
    void *pObject = pn->GetData();
    
    return pObject;
} 

POSITION CBaseList::FindI( void * pObj) const
{
    POSITION pn;
    INTERNALTRAVERSELIST(*this, pn){
        if (GetI(pn)==pObj) {
            return pn;
        }
    }
    return NULL;
} 

void *CBaseList::RemoveHeadI()
{
    

    return RemoveI((POSITION)m_pFirst);
} 

void *CBaseList::RemoveTailI()
{
    

    return RemoveI((POSITION)m_pLast);
} 

void *CBaseList::RemoveI(POSITION pos)
{
    

    
    if (pos==NULL) return NULL;

    CNode *pCurrent = (CNode *) pos;
    ASSERT(pCurrent != NULL);

    

    CNode *pNode = pCurrent->Prev();
    if (pNode == NULL) {
        m_pFirst = pCurrent->Next();
    } else {
        pNode->SetNext(pCurrent->Next());
    }

    

    pNode = pCurrent->Next();
    if (pNode == NULL) {
        m_pLast = pCurrent->Prev();
    } else {
        pNode->SetPrev(pCurrent->Prev());
    }

    

    void *pObject = pCurrent->GetData();

    

    

    m_Cache.AddToCache(pCurrent);

    

    --m_Count;
    ASSERT(m_Count >= 0);
    return pObject;
} 

POSITION CBaseList::AddTailI(void *pObject)
{
    

    CNode *pNode;
    

    

    pNode = (CNode *) m_Cache.RemoveFromCache();
    if (pNode == NULL) {
        pNode = new CNode;
    }

    

    if (pNode == NULL) {
        return NULL;
    }

    

    pNode->SetData(pObject);
    pNode->SetNext(NULL);
    pNode->SetPrev(m_pLast);

    if (m_pLast == NULL) {
        m_pFirst = pNode;
    } else {
        m_pLast->SetNext(pNode);
    }

    

    m_pLast = pNode;
    ++m_Count;

    return (POSITION) pNode;
} 

POSITION CBaseList::AddHeadI(void *pObject)
{
    CNode *pNode;
    

    

    pNode = (CNode *) m_Cache.RemoveFromCache();
    if (pNode == NULL) {
        pNode = new CNode;
    }

    

    if (pNode == NULL) {
        return NULL;
    }

    

    pNode->SetData(pObject);

    
    pNode->SetPrev(NULL);
    pNode->SetNext(m_pFirst);

    if (m_pFirst == NULL) {
        m_pLast = pNode;
    } else {
        m_pFirst->SetPrev(pNode);
    }
    m_pFirst = pNode;

    ++m_Count;

    return (POSITION) pNode;
} 

BOOL CBaseList::AddTail(CBaseList *pList)
{
    
    POSITION pos = pList->GetHeadPositionI();

    while (pos) {
       if (NULL == AddTailI(pList->GetNextI(pos))) {
           return FALSE;
       }
    }
    return TRUE;
} 

BOOL CBaseList::AddHead(CBaseList *pList)
{
    

    POSITION pos;

    INTERNALREVERSETRAVERSELIST(*pList, pos) {
        if (NULL== AddHeadI(pList->GetI(pos))){
            return FALSE;
        }
    }
    return TRUE;
} 

POSITION  CBaseList::AddAfterI(POSITION pos, void * pObj)
{
    if (pos==NULL)
        return AddHeadI(pObj);

    
    CNode *pAfter = (CNode *) pos;
    ASSERT(pAfter != NULL);
    if (pAfter==m_pLast)
        return AddTailI(pObj);

    

    CNode *pNode = (CNode *) m_Cache.RemoveFromCache();
    if (pNode == NULL) {
        pNode = new CNode;
    }

    

    if (pNode == NULL) {
        return NULL;
    }

    

    pNode->SetData(pObj);

    
    CNode * pBefore = pAfter->Next();
    ASSERT(pBefore != NULL);

    
    pNode->SetPrev(pAfter);
    pNode->SetNext(pBefore);
    pBefore->SetPrev(pNode);
    pAfter->SetNext(pNode);

    ++m_Count;

    return (POSITION) pNode;

} 

BOOL CBaseList::AddAfter(POSITION p, CBaseList *pList)
{
    POSITION pos;
    INTERNALTRAVERSELIST(*pList, pos) {
        
        p = AddAfterI(p, pList->GetI(pos));
        if (p==NULL) return FALSE;
    }
    return TRUE;
} 

POSITION CBaseList::AddBeforeI(POSITION pos, void * pObj)
{
    if (pos==NULL)
        return AddTailI(pObj);

    

    CNode *pBefore = (CNode *) pos;
    ASSERT(pBefore != NULL);
    if (pBefore==m_pFirst)
        return AddHeadI(pObj);

    CNode * pNode = (CNode *) m_Cache.RemoveFromCache();
    if (pNode == NULL) {
        pNode = new CNode;
    }

    

    if (pNode == NULL) {
        return NULL;
    }

    

    pNode->SetData(pObj);

    

    CNode * pAfter = pBefore->Prev();
    ASSERT(pAfter != NULL);

    
    pNode->SetPrev(pAfter);
    pNode->SetNext(pBefore);
    pBefore->SetPrev(pNode);
    pAfter->SetNext(pNode);

    ++m_Count;

    return (POSITION) pNode;

} 

BOOL CBaseList::AddBefore(POSITION p, CBaseList *pList)
{
    POSITION pos;
    INTERNALREVERSETRAVERSELIST(*pList, pos) {
        
        p = AddBeforeI(p, pList->GetI(pos));
        if (p==NULL) return FALSE;
    }
    return TRUE;
} 

BOOL CBaseList::MoveToTail
        (POSITION pos, CBaseList *pList)
{
    

    if (pos==NULL) return TRUE;  

    
    CNode * p = (CNode *)pos;
    int cMove = 0;            
    while(p!=NULL) {
       p = p->Prev();
       ++cMove;
    }

    
    if (pList->m_pLast!=NULL)
        pList->m_pLast->SetNext(m_pFirst);
    if (m_pFirst!=NULL)
        m_pFirst->SetPrev(pList->m_pLast);

    
    p = (CNode *)pos;

    if (pList->m_pFirst==NULL)
        pList->m_pFirst = m_pFirst;
    m_pFirst = p->Next();
    if (m_pFirst==NULL)
        m_pLast = NULL;
    pList->m_pLast = p;

    
    if (m_pFirst!=NULL)
        m_pFirst->SetPrev(NULL);
    p->SetNext(NULL);

    
    m_Count -= cMove;
    pList->m_Count += cMove;

    return TRUE;

} 

BOOL CBaseList::MoveToHead
        (POSITION pos, CBaseList *pList)
{

    

    if (pos==NULL) return TRUE;  

    
    CNode * p = (CNode *)pos;
    int cMove = 0;            
    while(p!=NULL) {
       p = p->Next();
       ++cMove;
    }

    
    if (pList->m_pFirst!=NULL)
        pList->m_pFirst->SetPrev(m_pLast);
    if (m_pLast!=NULL)
        m_pLast->SetNext(pList->m_pFirst);

    
    p = (CNode *)pos;

    if (pList->m_pLast==NULL)
        pList->m_pLast = m_pLast;

    m_pLast = p->Prev();
    if (m_pLast==NULL)
        m_pFirst = NULL;
    pList->m_pFirst = p;

    
    if (m_pLast!=NULL)
        m_pLast->SetNext(NULL);
    p->SetPrev(NULL);

    
    m_Count -= cMove;
    pList->m_Count += cMove;

    return TRUE;

} 

void CBaseList::Reverse()
{
    
    CNode * p;

    
    p = m_pFirst;
    while (p!=NULL) {
        CNode * q;
        q = p->Next();
        p->SetNext(p->Prev());
        p->SetPrev(q);
        p = q;
    }

    p = m_pFirst;
    m_pFirst = m_pLast;
    m_pLast = p;

#if 0     

    if (m_pFirst==NULL) return;          
    if (m_pFirst->Next()==NULL) return;  

    
    for ( p = m_pFirst
        ; p!=NULL
        ; p = p->Next()
        ){
        p->SetPrev(p->Next());
    }

    
    m_pFirst->SetNext(NULL);     

    
    for ( p = m_pFirst           
        ; p->Prev()!=NULL        
        ; p = p->Prev()          
        ){
        p->Prev()->SetNext(p);
    }

    
    p = m_pFirst;
    m_pFirst = m_pLast;
    m_pLast = p;
#endif

} 
