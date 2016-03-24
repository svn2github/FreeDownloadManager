/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef VMSFDMBROWSERPLUGINMONITORINGH
#define VMSFDMBROWSERPLUGINMONITORINGH

class vmsFdmBrowserPluginMonitoring{
public:
	static bool MonitorFF( BOOL bMonitor, BOOL &bNeedRestartBrowser );
	static bool MonitorChrome( bool* value, bool *needRestartBrowser );

	
	
	
	static void CheckFirefoxExtension(LPDWORD pdwResult);
	static void SuggestBrowserExtensions();

	vmsFdmBrowserPluginMonitoring();
	virtual ~vmsFdmBrowserPluginMonitoring();
protected:
	static void runChrome(void);
};

#endif