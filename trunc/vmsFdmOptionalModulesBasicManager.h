/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsOptionalModulesManager.h"
#include "vmsOptionalModulesBasicDownloader.h"
#include "vmsFdmOptionalModulesBasicInstaller.h"
#include "vmsWwwResourceDownloader.h"
class vmsFdmOptionalModulesBasicManager : 
	public vmsOptionalModulesManager
{
public:
	vmsFdmOptionalModulesBasicManager(std::shared_ptr <vmsOptionalModulesList> modules) :
	  vmsOptionalModulesManager (modules, 
		  std::make_shared <vmsOptionalModulesBasicDownloader> (std::make_shared <vmsWwwResourceDownloaderFactory> (), vmsGetTempPath ()),
		  std::make_shared <vmsFdmOptionalModulesBasicInstaller> (modules, vmsGetModulePath ()))
	{
	}

	~vmsFdmOptionalModulesBasicManager(void)
	{
	}
};

