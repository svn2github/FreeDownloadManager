/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                        

#ifndef __WXLIST__
#define __WXLIST__

   
#ifndef __AFX_H__
struct __POSITION { int unused; };
typedef __POSITION* POSITION;
#endif

const int DEFAULTCACHE = 10;      

class CBaseList 
#ifdef DEBUG
    : public CBaseObject
#endif
{
    

public:

#ifdef DEBUG
    class CNode : public CBaseObject {
#else
    class CNode {
#endif

        CNode *m_pPrev;         
        CNode *m_pNext;         
        void *m_pObject;      

    public:

        
        CNode()
#ifdef DEBUG
            : CBaseObject(NAME("List node"))
#endif
        {
        };  

        
        CNode *Prev() const { return m_pPrev; };  

        
        CNode *Next() const { return m_pNext; };  

        
        void SetPrev(CNode *p) { m_pPrev = p; };  

        
        void SetNext(CNode *p) { m_pNext = p; };  

        
        void *GetData() const { return m_pObject; };  

        
        void SetData(void *p) { m_pObject = p; };
    };

    class CNodeCache
    {
    public:
        CNodeCache(INT iCacheSize) : m_iCacheSize(iCacheSize),
                                     m_pHead(NULL),
                                     m_iUsed(0)
                                     {};
        ~CNodeCache() {
            CNode *pNode = m_pHead;
            while (pNode) {
                CNode *pCurrent = pNode;
                pNode = pNode->Next();
                delete pCurrent;
            }
        };
        void AddToCache(CNode *pNode)
        {
            if (m_iUsed < m_iCacheSize) {
                pNode->SetNext(m_pHead);
                m_pHead = pNode;
                m_iUsed++;
            } else {
                delete pNode;
            }
        };
        CNode *RemoveFromCache()
        {
            CNode *pNode = m_pHead;
            if (pNode != NULL) {
                m_pHead = pNode->Next();
                m_iUsed--;
                ASSERT(m_iUsed >= 0);
            } else {
                ASSERT(m_iUsed == 0);
            }
            return pNode;
        };
    private:
        INT m_iCacheSize;
        INT m_iUsed;
        CNode *m_pHead;
    };

protected:

    CNode* m_pFirst;    
    CNode* m_pLast;     
    LONG m_Count;       

private:

    CNodeCache m_Cache; 

private:

    
    CBaseList(const CBaseList &refList);
    CBaseList &operator=(const CBaseList &refList);

public:

    CBaseList(TCHAR *pName,
              INT iItems);

    CBaseList(TCHAR *pName);
#ifdef UNICODE
    CBaseList(CHAR *pName,
              INT iItems);

    CBaseList(CHAR *pName);
#endif
    ~CBaseList();

    
    void RemoveAll();  

    
    POSITION GetHeadPositionI() const;  

    
    POSITION GetTailPositionI() const;  

    
    int GetCountI() const;

protected:
    
    void *GetNextI(POSITION& rp) const;  

    
    void *GetI(POSITION p) const;

public:
    
    POSITION Next(POSITION pos) const
    {
        if (pos == NULL) {
            return (POSITION) m_pFirst;
        }
        CNode *pn = (CNode *) pos;
        return (POSITION) pn->Next();
    } 

    
    POSITION Prev(POSITION pos) const
    {
        if (pos == NULL) {
            return (POSITION) m_pLast;
        }
        CNode *pn = (CNode *) pos;
        return (POSITION) pn->Prev();
    }   

    
protected:
    POSITION FindI( void * pObj) const;

    
    
    
      

    
    void *RemoveHeadI();  

    
    void *RemoveTailI();  

    
    void *RemoveI(POSITION p);

    
    POSITION AddTailI(void * pObj);
public:  

    
    BOOL AddTail(CBaseList *pList);  

    

    
protected:
    POSITION AddHeadI(void * pObj);
public:

    
    BOOL AddHead(CBaseList *pList);  

    
protected:
    POSITION AddAfterI(POSITION p, void * pObj);
public:

    
    BOOL AddAfter(POSITION p, CBaseList *pList);  

    
    protected:
    POSITION AddBeforeI(POSITION p, void * pObj);
    public:

    
    BOOL AddBefore(POSITION p, CBaseList *pList);  

        

    

    
    BOOL MoveToTail(POSITION pos, CBaseList *pList);  

    
    BOOL MoveToHead(POSITION pos, CBaseList *pList);  

    
    void Reverse();  

    
    #define TRAVERSELIST(list, cursor)               \
    for ( cursor = (list).GetHeadPosition()           \
        ; cursor!=NULL                               \
        ; cursor = (list).Next(cursor)                \
        )  

    
    #define REVERSETRAVERSELIST(list, cursor)        \
    for ( cursor = (list).GetTailPosition()           \
        ; cursor!=NULL                               \
        ; cursor = (list).Prev(cursor)                \
        )

}; 

template<class OBJECT> class CGenericList : public CBaseList
{
public:
    CGenericList(TCHAR *pName,
                 INT iItems,
                 BOOL bLock = TRUE,
                 BOOL bAlert = FALSE) :
                     CBaseList(pName, iItems) {
        UNREFERENCED_PARAMETER(bAlert);
        UNREFERENCED_PARAMETER(bLock);
    };
    CGenericList(TCHAR *pName) :
                     CBaseList(pName) {
    };

    POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    POSITION GetTailPosition() const { return (POSITION)m_pLast; }
    int GetCount() const { return m_Count; }

    OBJECT *GetNext(POSITION& rp) const { return (OBJECT *) GetNextI(rp); }

    OBJECT *Get(POSITION p) const { return (OBJECT *) GetI(p); }
    OBJECT *GetHead() const  { return Get(GetHeadPosition()); }

    OBJECT *RemoveHead() { return (OBJECT *) RemoveHeadI(); }

    OBJECT *RemoveTail() { return (OBJECT *) RemoveTailI(); }

    OBJECT *Remove(POSITION p) { return (OBJECT *) RemoveI(p); }
    POSITION AddBefore(POSITION p, OBJECT * pObj) { return AddBeforeI(p, pObj); }
    POSITION AddAfter(POSITION p, OBJECT * pObj)  { return AddAfterI(p, pObj); }
    POSITION AddHead(OBJECT * pObj) { return AddHeadI(pObj); }
    POSITION AddTail(OBJECT * pObj)  { return AddTailI(pObj); }
    BOOL AddTail(CGenericList<OBJECT> *pList)
            { return CBaseList::AddTail((CBaseList *) pList); }
    BOOL AddHead(CGenericList<OBJECT> *pList)
            { return CBaseList::AddHead((CBaseList *) pList); }
    BOOL AddAfter(POSITION p, CGenericList<OBJECT> *pList)
            { return CBaseList::AddAfter(p, (CBaseList *) pList); };
    BOOL AddBefore(POSITION p, CGenericList<OBJECT> *pList)
            { return CBaseList::AddBefore(p, (CBaseList *) pList); };
    POSITION Find( OBJECT * pObj) const { return FindI(pObj); }
};         

typedef CGenericList<CBaseObject> CBaseObjectList;
typedef CGenericList<IUnknown> CBaseInterfaceList;

#endif 

