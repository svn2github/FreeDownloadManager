/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __PSTREAM__
#define __PSTREAM__

class CPersistStream : public IPersistStream {
    private:

        

    protected:
        DWORD     mPS_dwFileVersion;         
        BOOL      mPS_fDirty;

    public:

        

        STDMETHODIMP IsDirty()
            {return (mPS_fDirty ? S_OK : S_FALSE);}  
        STDMETHODIMP Load(LPSTREAM pStm);
        STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty);
        STDMETHODIMP GetSizeMax(ULARGE_INTEGER * pcbSize)
                         
                         { pcbSize->QuadPart = 12*sizeof(WCHAR)+SizeMax(); return NOERROR; }

        

        CPersistStream(IUnknown *punk, HRESULT *phr);
        ~CPersistStream();

        HRESULT SetDirty(BOOL fDirty)
            { mPS_fDirty = fDirty; return NOERROR;}

        
        

        

        
        STDMETHODIMP GetClassID(CLSID *pClsid) PURE;

        
        
        virtual DWORD GetSoftwareVersion(void) { return 0; }

        
        
        
        

        virtual int SizeMax() {return 0;}
        virtual HRESULT WriteToStream(IStream *pStream);
        virtual HRESULT ReadFromStream(IStream *pStream);
        

    private:

};

STDAPI WriteInt(IStream *pIStream, int n);

STDAPI_(int) ReadInt(IStream *pIStream, HRESULT &hr);

#endif 
