/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "nsIGenericFactory.h"
#include "FDMUrl.h"
#include "FDMUrlListReceiver.h"
#include "FDMUrlReceiver.h"
#include "FDMForFirefox.h"
#include "FDMFirefoxExtension.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(CFDMUrl)
NS_GENERIC_FACTORY_CONSTRUCTOR(CFDMUrlListReceiver)
NS_GENERIC_FACTORY_CONSTRUCTOR(CFDMUrlReceiver)
NS_GENERIC_FACTORY_CONSTRUCTOR(CFDMForFirefox)
NS_GENERIC_FACTORY_CONSTRUCTOR(CFDMFirefoxExtension)

static nsModuleComponentInfo components[] =
{
    {
       FDMURL_CLASSNAME, 
       FDMURL_CID,
       FDMURL_CONTRACTID,
       CFDMUrlConstructor,
    },

	{
       FDMURLLISTRECEIVER_CLASSNAME, 
       FDMURLLISTRECEIVER_CID,
       FDMURLLISTRECEIVER_CONTRACTID,
       CFDMUrlListReceiverConstructor,
    },

	{
       FDMURLRECEIVER_CLASSNAME, 
       FDMURLRECEIVER_CID,
       FDMURLRECEIVER_CONTRACTID,
       CFDMUrlReceiverConstructor,
    },

	{
       FDMFORFIREFOX_CLASSNAME, 
       FDMFORFIREFOX_CID,
       FDMFORFIREFOX_CONTRACTID,
       CFDMForFirefoxConstructor,
    },

	{
	   FDMFIREFOXEXTENSION_CLASSNAME,
	   FDMFIREFOXEXTENSION_CID,
	   FDMFIREFOXEXTENSION_CONTRACTID,
	   CFDMFirefoxExtensionConstructor,
	},
};

NS_IMPL_NSGETMODULE("FDMFFsModule", components)
