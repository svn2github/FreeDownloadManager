/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>

#define MAX_KEY_LEN  260

extern int g_cTemplates;
extern CFactoryTemplate g_Templates[];

STDAPI
EliminateSubKey( HKEY hkey, LPTSTR strSubKey )
{
  HKEY hk;
  if (0 == lstrlen(strSubKey) ) {
      
      return E_FAIL;
  }

  LONG lreturn = RegOpenKeyEx( hkey
                             , strSubKey
                             , 0
                             , MAXIMUM_ALLOWED
                             , &hk );

  ASSERT(    lreturn == ERROR_SUCCESS
          || lreturn == ERROR_FILE_NOT_FOUND
          || lreturn == ERROR_INVALID_HANDLE );

  if( ERROR_SUCCESS == lreturn )
  {
    
    

    for( ; ; )
    {
      TCHAR Buffer[MAX_KEY_LEN];
      DWORD dw = MAX_KEY_LEN;
      FILETIME ft;

      lreturn = RegEnumKeyEx( hk
                            , 0
                            , Buffer
                            , &dw
                            , NULL
                            , NULL
                            , NULL
                            , &ft);

      ASSERT(    lreturn == ERROR_SUCCESS
              || lreturn == ERROR_NO_MORE_ITEMS );

      if( ERROR_SUCCESS == lreturn )
      {
        EliminateSubKey(hk, Buffer);
      }
      else
      {
        break;
      }
    }

    RegCloseKey(hk);
    RegDeleteKey(hkey, strSubKey);
  }

  return NOERROR;
}

STDAPI
AMovieSetupRegisterServer( CLSID   clsServer
                         , LPCWSTR szDescription
                         , LPCWSTR szFileName
                         , LPCWSTR szThreadingModel = L"Both"
                         , LPCWSTR szServerType     = L"InprocServer32" )
{
  
  
  TCHAR achTemp[MAX_PATH];

  
  
  
  OLECHAR szCLSID[CHARS_IN_GUID];
  HRESULT hr = StringFromGUID2( clsServer
                              , szCLSID
                              , CHARS_IN_GUID );
  ASSERT( SUCCEEDED(hr) );

  
  
  HKEY hkey;
  wsprintf( achTemp, TEXT("CLSID\\%ls"), szCLSID );
  LONG lreturn = RegCreateKey( HKEY_CLASSES_ROOT
                             , (LPCTSTR)achTemp
                             , &hkey              );
  if( ERROR_SUCCESS != lreturn )
  {
    return AmHresultFromWin32(lreturn);
  }

  
  

  wsprintf( achTemp, TEXT("%ls"), szDescription );
  lreturn = RegSetValue( hkey
                       , (LPCTSTR)NULL
                       , REG_SZ
                       , achTemp
                       , sizeof(achTemp) );
  if( ERROR_SUCCESS != lreturn )
  {
    RegCloseKey( hkey );
    return AmHresultFromWin32(lreturn);
  }

  
  
  
  
  HKEY hsubkey;

  wsprintf( achTemp, TEXT("%ls"), szServerType );
  lreturn = RegCreateKey( hkey
                        , achTemp
                        , &hsubkey     );
  if( ERROR_SUCCESS != lreturn )
  {
    RegCloseKey( hkey );
    return AmHresultFromWin32(lreturn);
  }

  
  
  wsprintf( achTemp, TEXT("%ls"), szFileName );
  lreturn = RegSetValue( hsubkey
                       , (LPCTSTR)NULL
                       , REG_SZ
                       , (LPCTSTR)achTemp
                       , sizeof(TCHAR) * (lstrlen(achTemp)+1) );
  if( ERROR_SUCCESS != lreturn )
  {
    RegCloseKey( hkey );
    RegCloseKey( hsubkey );
    return AmHresultFromWin32(lreturn);
  }

  wsprintf( achTemp, TEXT("%ls"), szThreadingModel );
  lreturn = RegSetValueEx( hsubkey
                         , TEXT("ThreadingModel")
                         , 0L
                         , REG_SZ
                         , (CONST BYTE *)achTemp
                         , sizeof(TCHAR) * (lstrlen(achTemp)+1) );

  
  
  RegCloseKey( hkey );
  RegCloseKey( hsubkey );

  
  
  return HRESULT_FROM_WIN32(lreturn);

}

STDAPI
AMovieSetupUnregisterServer( CLSID clsServer )
{
  
  
  
  OLECHAR szCLSID[CHARS_IN_GUID];
  HRESULT hr = StringFromGUID2( clsServer
                              , szCLSID
                              , CHARS_IN_GUID );
  ASSERT( SUCCEEDED(hr) );

  TCHAR achBuffer[MAX_KEY_LEN];
  wsprintf( achBuffer, TEXT("CLSID\\%ls"), szCLSID );

  
  

  hr = EliminateSubKey( HKEY_CLASSES_ROOT, achBuffer );
  ASSERT( SUCCEEDED(hr) );

  
  
  return NOERROR;
}

STDAPI
AMovieSetupRegisterFilter2( const AMOVIESETUP_FILTER * const psetupdata
                          , IFilterMapper2 *                 pIFM2
                          , BOOL                             bRegister  )
{
  DbgLog((LOG_TRACE, 3, TEXT("= AMovieSetupRegisterFilter")));

  
  
  if( NULL == psetupdata ) return S_FALSE;

  
  
  
  
  DbgLog((LOG_TRACE, 3, TEXT("= = unregister filter")));
  HRESULT hr = pIFM2->UnregisterFilter(
      0,                        
      0,                        
      *psetupdata->clsID );

  if( bRegister )
  {
    REGFILTER2 rf2;
    rf2.dwVersion = 1;
    rf2.dwMerit = psetupdata->dwMerit;
    rf2.cPins = psetupdata->nPins;
    rf2.rgPins = psetupdata->lpPin;
    
    
    
    DbgLog((LOG_TRACE, 3, TEXT("= = register filter")));
    hr = pIFM2->RegisterFilter(*psetupdata->clsID
                             , psetupdata->strName
                             , 0 
                             , 0 
                             , NULL 
                             , &rf2);
  }

  
  
  
  
  
  if( 0x80070002 == hr)
    return NOERROR;
  else
    return hr;
}

STDAPI
RegisterAllServers( LPCWSTR szFileName, BOOL bRegister )
{
  HRESULT hr = NOERROR;

  for( int i = 0; i < g_cTemplates; i++ )
  {
    
    
    const CFactoryTemplate *pT = &g_Templates[i];

    DbgLog((LOG_TRACE, 2, TEXT("- - register %ls"),
           (LPCWSTR)pT->m_Name ));

    
    
    if( bRegister )
    {
      hr = AMovieSetupRegisterServer( *(pT->m_ClsID)
                                    , (LPCWSTR)pT->m_Name
                                    , szFileName );
    }
    else
    {
      hr = AMovieSetupUnregisterServer( *(pT->m_ClsID) );
    }

    
    
    
    if( FAILED(hr) )
      break;
  }

  return hr;
}

STDAPI
AMovieDllRegisterServer2( BOOL bRegister )
{
  HRESULT hr = NOERROR;

  DbgLog((LOG_TRACE, 2, TEXT("AMovieDllRegisterServer2()")));

  
  
  
  WCHAR achFileName[MAX_PATH];

  
  
  {
    char achTemp[MAX_PATH];

    DbgLog((LOG_TRACE, 2, TEXT("- get module file name")));

    
    
    ASSERT(g_hInst != 0);

    if( 0 == GetModuleFileNameA( g_hInst
                              , achTemp
                              , sizeof(achTemp) ) )
    {
      
      DWORD dwerr = GetLastError();
      return AmHresultFromWin32(dwerr);
    }

    MultiByteToWideChar( CP_ACP
                       , 0L
                       , achTemp
                       , lstrlenA(achTemp) + 1
                       , achFileName
                       , sizeof(achFileName) );
  }

  
  
  
  if( bRegister )
  {
    DbgLog((LOG_TRACE, 2, TEXT("- register OLE Servers")));
    hr = RegisterAllServers( achFileName, TRUE );
  }

  
  
  

  if( SUCCEEDED(hr) )
  {
    
    
    
    DbgLog((LOG_TRACE, 2, TEXT("- CoInitialize")));
    hr = CoInitialize( (LPVOID)NULL );
    ASSERT( SUCCEEDED(hr) );

    
    
    DbgLog((LOG_TRACE, 2, TEXT("- obtain IFilterMapper2")));
    IFilterMapper2 *pIFM2 = 0;
    IFilterMapper *pIFM = 0;
    hr = CoCreateInstance( CLSID_FilterMapper2
                         , NULL
                         , CLSCTX_INPROC_SERVER
                         , IID_IFilterMapper2
                         , (void **)&pIFM2       );
    if(FAILED(hr))
    {
        DbgLog((LOG_TRACE, 2, TEXT("- trying IFilterMapper instead")));

        hr = CoCreateInstance(
            CLSID_FilterMapper,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IFilterMapper,
            (void **)&pIFM);
    }
    if( SUCCEEDED(hr) )
    {
      
      
      
      DbgLog((LOG_TRACE, 2, TEXT("- register Filters")));
      for( int i = 0; i < g_cTemplates; i++ )
      {
        
        
        const CFactoryTemplate *pT = &g_Templates[i];

        if( NULL != pT->m_pAMovieSetup_Filter )
        {
          DbgLog((LOG_TRACE, 2, TEXT("- - register %ls"), (LPCWSTR)pT->m_Name ));

          if(pIFM2)
          {
              hr = AMovieSetupRegisterFilter2( pT->m_pAMovieSetup_Filter, pIFM2, bRegister );
          }
          else
          {
              hr = AMovieSetupRegisterFilter( pT->m_pAMovieSetup_Filter, pIFM, bRegister );
          }
        }

        
        
        
        if( FAILED(hr) )
          break;
      }

      
      
      if(pIFM2)
          pIFM2->Release();
      else
          pIFM->Release();

    }

    
    
    CoFreeUnusedLibraries();
    CoUninitialize();
  }

  
  
  
  if( SUCCEEDED(hr) && !bRegister )
  {
    DbgLog((LOG_TRACE, 2, TEXT("- register OLE Servers")));
    hr = RegisterAllServers( achFileName, FALSE );
  }

  DbgLog((LOG_TRACE, 2, TEXT("- return %0x"), hr));
  return hr;
}

STDAPI
AMovieDllRegisterServer( void )
{
  HRESULT hr = NOERROR;

  
  
  
  WCHAR achFileName[MAX_PATH];

  {
    
    
    char achTemp[MAX_PATH];

    if( 0 == GetModuleFileNameA( g_hInst
                              , achTemp
                              , sizeof(achTemp) ) )
    {
      
      DWORD dwerr = GetLastError();
      return AmHresultFromWin32(dwerr);
    }

    MultiByteToWideChar( CP_ACP
                       , 0L
                       , achTemp
                       , lstrlenA(achTemp) + 1
                       , achFileName
                       , sizeof(achFileName) );
  }

  
  
  
  for( int i = 0; i < g_cTemplates; i++ )
  {
    
    
    const CFactoryTemplate *pT = &g_Templates[i];

    
    
    hr = AMovieSetupRegisterServer( *(pT->m_ClsID)
                                  , (LPCWSTR)pT->m_Name
                                  , achFileName );

    
    
    
    
    if( SUCCEEDED(hr) && (NULL != pT->m_lpfnNew) )
    {
      
      
      PAMOVIESETUP psetup;
      hr = CoCreateInstance( *(pT->m_ClsID)
                           , 0
                           , CLSCTX_INPROC_SERVER
                           , IID_IAMovieSetup
                           , reinterpret_cast<void**>(&psetup) );
      if( SUCCEEDED(hr) )
      {
        hr = psetup->Unregister();
        if( SUCCEEDED(hr) )
          hr = psetup->Register();
        psetup->Release();
      }
      else
      {
        if(    (E_NOINTERFACE      == hr )
            || (VFW_E_NEED_OWNER == hr ) )
          hr = NOERROR;
      }
    }

    
    
    
    if( FAILED(hr) )
      break;

  } 

  return hr;
}

STDAPI
AMovieDllUnregisterServer()
{
  
  
  HRESULT hr = NOERROR;

  
  
  
  for( int i = g_cTemplates; i--; )
  {
    
    
    const CFactoryTemplate *pT = &g_Templates[i];

    
    
    if( NULL != pT->m_lpfnNew )
    {
      
      
      PAMOVIESETUP psetup;
      hr = CoCreateInstance( *(pT->m_ClsID)
                           , 0
                           , CLSCTX_INPROC_SERVER
                           , IID_IAMovieSetup
                           , reinterpret_cast<void**>(&psetup) );
      if( SUCCEEDED(hr) )
      {
        hr = psetup->Unregister();
        psetup->Release();
      }
      else
      {
        if(    (E_NOINTERFACE      == hr )
            || (VFW_E_NEED_OWNER == hr ) )
           hr = NOERROR;
      }
    }

    
    
    if( SUCCEEDED(hr) )
    {
      hr = AMovieSetupUnregisterServer( *(pT->m_ClsID) );
    }

    
    
    
    if( FAILED(hr) )
      break;
  }

  return hr;
}
