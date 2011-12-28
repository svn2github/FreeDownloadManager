/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

STDAPI AMovieDllRegisterServer2( BOOL );
STDAPI AMovieDllRegisterServer();
STDAPI AMovieDllUnregisterServer();

STDAPI EliminateSubKey( HKEY, LPTSTR );

STDAPI
AMovieSetupRegisterFilter2( const AMOVIESETUP_FILTER * const psetupdata
                          , IFilterMapper2 *                 pIFM2
                          , BOOL                             bRegister  );

